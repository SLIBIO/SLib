/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/list_report_view.h"

#include "view_macos.h"

@interface SLIBListReportViewHandle_TableView : NSTableView
{	
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
}
@end

@interface SLIBListReportViewHandle : NSScrollView<NSTableViewDataSource, NSTableViewDelegate>
{
	@public SLIBListReportViewHandle_TableView* table;
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	@public slib::CList<NSTableColumn*> m_columns;
	@public NSFont* m_font;
}
@end

namespace slib
{

	namespace priv
	{
		namespace list_report_view
		{
			
			class ListReportViewHelper : public ListReportView
			{
			public:
				void _applyColumnsCount(SLIBListReportViewHandle* tv)
				{
					ObjectLocker lock(this);
					CList<NSTableColumn*>& _columns = tv->m_columns;
					sl_uint32 nOrig = (sl_uint32)(_columns.getCount());
					sl_uint32 nNew = (sl_uint32)(m_columns.getCount());
					if (nOrig == nNew) {
						return;
					}
					if (nOrig > nNew) {
						ListLocker<NSTableColumn*> columns(_columns);
						for (sl_uint32 i = nNew; i < columns.count; i++) {
							[tv->table removeTableColumn:(columns[i])];
						}
						_columns.setCount(nNew);
					} else {
						_columns.setCount(nNew);
						ListLocker<NSTableColumn*> columns(_columns);
						for (sl_uint32 i = nOrig; i < columns.count; i++) {
							NSTableColumn* column = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"%d",i]];
							[tv->table addTableColumn:column];
							columns[i] = column;
						}
					}
				}
				
				void _copyColumns(SLIBListReportViewHandle* tv)
				{
					ObjectLocker lock(this);
					ListLocker<ListReportViewColumn> columns(m_columns);
					_applyColumnsCount(tv);
					for (sl_uint32 i = 0; i < columns.count; i++) {
						NSTableColumn* tc = tv->m_columns.getValueAt(i, nil);
						if (tc != nil) {
							[tc setTitle:(Apple::getNSStringFromString(columns[i].title))];
							[tc setWidth:(CGFloat)(columns[i].width)];
							NSTableHeaderCell* headerCell = [tc headerCell];
							NSCell* dataCell = [tc dataCell];
							[headerCell setAlignment:translateAlignment(columns[i].headerAlign)];
							[dataCell setAlignment:translateAlignment(columns[i].align)];
							[dataCell setEditable:FALSE];
						}
					}
				}
				
				void _applyFont(SLIBListReportViewHandle* tv)
				{
					NSFont* font = tv->m_font;
					if (font != nil) {
						ListLocker<ListReportViewColumn> columns(m_columns);
						for (sl_uint32 i = 0; i < columns.count; i++) {
							NSTableColumn* tc = tv->m_columns.getValueAt(i, nil);
							if (tc != nil) {
								NSCell* dataCell = [tc dataCell];
								[dataCell setFont:font];
							}
						}
					}
					[tv->table setRowHeight:([font pointSize] - [font descender])];
				}
				
				static NSTextAlignment translateAlignment(Alignment _align)
				{
					Alignment align = _align & Alignment::HorizontalMask;
					if (align == Alignment::Center) {
						return NSCenterTextAlignment;
					} else if (align == Alignment::Right) {
						return NSRightTextAlignment;
					}
					return NSLeftTextAlignment;
				}
				
			};

		}
	}

	using namespace priv::list_report_view;

	Ref<ViewInstance> ListReportView::createNativeWidget(ViewInstance* _parent)
	{
		SLIBListReportViewHandle_TableView* table = nil;
		ObjectLocker lock(this);
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBListReportViewHandle* handle = nil;
		table = [[SLIBListReportViewHandle_TableView alloc] init];
		if (table != nil) {
			handle = [[SLIBListReportViewHandle alloc] initWithFrame:frame];
			if (handle != nil) {
				handle->table = table;
				[table setDelegate:handle];
				[table setDataSource:handle];
				
				Ref<Font> font = getFont();
				NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
				handle->m_font = hFont;
				((ListReportViewHelper*)this)->_copyColumns(handle);
				[table setRowSizeStyle:NSTableViewRowSizeStyleCustom];
				((ListReportViewHelper*)this)->_applyFont(handle);
				
				[handle setDocumentView:table];
				[handle setHasVerticalScroller:TRUE];
				[handle setHasHorizontalScroller:TRUE];
				[handle setBorderType:NSBezelBorder];
				[table setGridStyleMask:NSTableViewSolidVerticalGridLineMask | NSTableViewSolidHorizontalGridLineMask];
			}
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		if (ret.isNotNull()) {
			ret->setView(this);
			if (table != nil) {
				[table reloadData];
				table->m_viewInstance = ret;
			}
		}
		return ret;
	}

	void ListReportView::_refreshColumnsCount_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			((ListReportViewHelper*)this)->_applyColumnsCount(tv);
			[tv->table reloadData];
		}
	}

	void ListReportView::_refreshRowsCount_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			[tv->table reloadData];
		}
	}

	void ListReportView::_setHeaderText_NW(sl_uint32 iCol, const String& text)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			NSTableColumn* tc = tv->m_columns.getValueAt(iCol, nil);
			if (tc != nil) {
				[tc setTitle:Apple::getNSStringFromString(text)];
			}
		}
	}

	void ListReportView::_setColumnWidth_NW(sl_uint32 iCol, sl_ui_len width)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			NSTableColumn* tc = tv->m_columns.getValueAt(iCol, nil);
			if (tc != nil) {
				[tc setWidth:(CGFloat)width];
			}
		}
	}

	void ListReportView::_setHeaderAlignment_NW(sl_uint32 iCol, Alignment align)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			NSTableColumn* tc = tv->m_columns.getValueAt(iCol, nil);
			if (tc != nil) {
				[[tc headerCell] setAlignment:ListReportViewHelper::translateAlignment(align)];
			}
		}
	}

	void ListReportView::_setColumnAlignment_NW(sl_uint32 iCol, Alignment align)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			NSTableColumn* tc = tv->m_columns.getValueAt(iCol, nil);
			if (tc != nil) {
				[[tc dataCell] setAlignment:ListReportViewHelper::translateAlignment(align)];
			}
		}
	}

	void ListReportView::_getSelectedRow_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			m_selectedRow = (sl_int32)([tv->table selectedRow]);
		}
	}

	void ListReportView::_setFont_NW(const Ref<Font>& font)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[SLIBListReportViewHandle class]]) {
			SLIBListReportViewHandle* tv = (SLIBListReportViewHandle*)handle;
			NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
			tv->m_font = hFont;
			((ListReportViewHelper*)this)->_applyFont(tv);
			[tv->table reloadData];
		}
	}

}

using namespace slib;
using namespace slib::priv::list_report_view;

@implementation SLIBListReportViewHandle

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		self->m_columns.setCount(0);
	}
	return self;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (ListReportView* _view = CastInstance<ListReportView>(view.get())) {
			return _view->getRowsCount();
		}
	}
	return 0;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (ListReportView* _view = CastInstance<ListReportView>(view.get())) {
			NSString* _id = tableColumn.identifier;
			if (_id != nil) {
				sl_uint32 iRow = (sl_uint32)(row);
				sl_uint32 iCol = (sl_uint32)(_id.intValue);
				return Apple::getNSStringFromString(_view->getItemText(iRow, iCol));
			}
		}
	}
	return @"";
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (ListReportViewHelper* _view = CastInstance<ListReportViewHelper>(view.get())) {
			sl_int32 n = (sl_int32)([table selectedRow]);
			if (n >= 0) {
				_view->dispatchSelectRow(n);
			}
		}
	}
}

@end

@implementation SLIBListReportViewHandle_TableView

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

- (void)mouseDown:(NSEvent *)theEvent
{
	NSInteger indexRowBefore = [self selectedRow];
	[super mouseDown:theEvent];
	NSPoint ptWindow = [theEvent locationInWindow];
	NSPoint ptView = [self convertPoint:ptWindow fromView:nil];
	NSInteger indexRow = [self rowAtPoint:ptView];
	if (indexRow >= 0) {
		Ref<macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			Ref<View> view = instance->getView();
			if (ListReportViewHelper* _view = CastInstance<ListReportViewHelper>(view.get())) {
				if (indexRow == indexRowBefore) {
					// don't call event callback when it is new selection because it is already called by default
					_view->dispatchSelectRow((sl_uint32)(indexRow));
				}
				sl_ui_posf x = (sl_ui_posf)(ptView.x);
				sl_ui_posf y = (sl_ui_posf)(ptView.y);
				NSInteger clicks = [theEvent clickCount];
				if (clicks == 1) {
					_view->dispatchClickRow((sl_uint32)(indexRow), UIPointf(x, y));
				} else if (clicks == 2) {
					_view->dispatchDoubleClickRow((sl_uint32)(indexRow), UIPointf(x, y));
				}
			}
		}
	}
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	[super rightMouseDown:theEvent];
	NSPoint ptWindow = [theEvent locationInWindow];
	NSPoint ptView = [self convertPoint:ptWindow fromView:nil];
	NSInteger indexRow = [self rowAtPoint:ptView];
	if (indexRow >= 0) {
		Ref<macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			Ref<View> view = instance->getView();
			if (ListReportViewHelper* _view = CastInstance<ListReportViewHelper>(view.get())) {
				sl_ui_posf x = (sl_ui_posf)(ptView.x);
				sl_ui_posf y = (sl_ui_posf)(ptView.y);
				_view->dispatchRightButtonClickRow((sl_uint32)(indexRow), UIPointf(x, y));
			}
		}
	}
}

@end

#endif
