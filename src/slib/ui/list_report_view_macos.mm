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


namespace slib
{
	namespace priv
	{
		namespace list_report_view
		{
			class ListReportViewInstance;
		}
	}
}

@interface SLIBListReportViewHandle_TableView : NSTableView
{	
	@public slib::WeakRef<slib::priv::list_report_view::ListReportViewInstance> m_viewInstance;
}
@end

@interface SLIBListReportViewHandle : NSScrollView<NSTableViewDataSource, NSTableViewDelegate>
{
	@public SLIBListReportViewHandle_TableView* table;
	@public slib::WeakRef<slib::priv::list_report_view::ListReportViewInstance> m_viewInstance;
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
			
			static NSTextAlignment TranslateAlignment(Alignment _align)
			{
				Alignment align = _align & Alignment::HorizontalMask;
				if (align == Alignment::Center) {
					return NSCenterTextAlignment;
				} else if (align == Alignment::Right) {
					return NSRightTextAlignment;
				}
				return NSLeftTextAlignment;
			}
			
			class ListReportViewHelper : public ListReportView
			{
			public:
				void applyColumnsCount(SLIBListReportViewHandle* tv)
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
				
				void copyColumns(SLIBListReportViewHandle* tv)
				{
					ObjectLocker lock(this);
					ListLocker<ListReportViewColumn> columns(m_columns);
					applyColumnsCount(tv);
					for (sl_uint32 i = 0; i < columns.count; i++) {
						NSTableColumn* tc = tv->m_columns.getValueAt(i, nil);
						if (tc != nil) {
							[tc setTitle:(Apple::getNSStringFromString(columns[i].title))];
							[tc setWidth:(CGFloat)(columns[i].width)];
							NSTableHeaderCell* headerCell = [tc headerCell];
							NSCell* dataCell = [tc dataCell];
							[headerCell setAlignment:TranslateAlignment(columns[i].headerAlign)];
							[dataCell setAlignment:TranslateAlignment(columns[i].align)];
							[dataCell setEditable:FALSE];
						}
					}
				}
				
				void applyFont(SLIBListReportViewHandle* tv, const Ref<Font>& font)
				{
					if (font.isNull()) {
						return;
					}
					NSFont* hFont = GraphicsPlatform::getNSFont(font.get());
					if (hFont == nil) {
						return;
					}
					tv->m_font = hFont;
					ListLocker<ListReportViewColumn> columns(m_columns);
					for (sl_uint32 i = 0; i < columns.count; i++) {
						NSTableColumn* tc = tv->m_columns.getValueAt(i, nil);
						if (tc != nil) {
							NSCell* dataCell = [tc dataCell];
							[dataCell setFont:hFont];
						}
					}
					[tv->table setRowHeight:([hFont pointSize] - [hFont descender])];
				}
				
			};

			class ListReportViewInstance : public macOS_ViewInstance, public IListReportViewInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				SLIBListReportViewHandle* getHandle()
				{
					return (SLIBListReportViewHandle*)m_handle;
				}
				
				Ref<ListReportViewHelper> getHelper()
				{
					return CastRef<ListReportViewHelper>(getView());
				}
				
				void refreshColumnsCount(ListReportView* view) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						static_cast<ListReportViewHelper*>(view)->applyColumnsCount(handle);
						[handle->table reloadData];
					}
				}
				
				void refreshRowsCount(ListReportView* view) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						[handle->table reloadData];
					}
				}
				
				void setHeaderText(ListReportView* view, sl_uint32 iCol, const String& text) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						NSTableColumn* tc = handle->m_columns.getValueAt(iCol, nil);
						if (tc != nil) {
							[tc setTitle:Apple::getNSStringFromString(text)];
						}
					}
				}
				
				void setColumnWidth(ListReportView* view, sl_uint32 iCol, sl_ui_len width) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						NSTableColumn* tc = handle->m_columns.getValueAt(iCol, nil);
						if (tc != nil) {
							[tc setWidth:(CGFloat)width];
						}
					}
				}
				
				void setHeaderAlignment(ListReportView* view, sl_uint32 iCol, const Alignment& align) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						NSTableColumn* tc = handle->m_columns.getValueAt(iCol, nil);
						if (tc != nil) {
							[[tc headerCell] setAlignment:TranslateAlignment(align)];
						}
					}
				}
				
				void setColumnAlignment(ListReportView* view, sl_uint32 iCol, const Alignment& align) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						NSTableColumn* tc = handle->m_columns.getValueAt(iCol, nil);
						if (tc != nil) {
							[[tc dataCell] setAlignment:TranslateAlignment(align)];
						}
					}
				}
				
				sl_bool getSelectedRow(ListReportView* view, sl_int32& _out) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						_out = (sl_int32)([handle->table selectedRow]);
						return sl_true;
					}
					return sl_false;
				}
				
				void setFont(View* view, const Ref<Font>& font) override
				{
					SLIBListReportViewHandle* handle = getHandle();
					if (handle != nil) {
						static_cast<ListReportViewHelper*>(view)->applyFont(handle, font);
						[handle->table reloadData];
					}
				}
				
				void onMouseDown(SLIBListReportViewHandle_TableView* tv, NSEvent* ev)
				{
					Ref<ListReportViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						NSInteger indexRowBefore = [tv selectedRow];
						NSPoint ptWindow = [ev locationInWindow];
						NSPoint ptView = [tv convertPoint:ptWindow fromView:nil];
						NSInteger indexRow = [tv rowAtPoint:ptView];
						if (indexRow >= 0) {
							if (indexRow == indexRowBefore) {
								// don't call event callback when it is new selection because it is already called by default
								helper->dispatchSelectRow((sl_uint32)(indexRow));
							}
							sl_ui_posf x = (sl_ui_posf)(ptView.x);
							sl_ui_posf y = (sl_ui_posf)(ptView.y);
							NSInteger clicks = [ev clickCount];
							if (clicks == 1) {
								helper->dispatchClickRow((sl_uint32)(indexRow), UIPointf(x, y));
							} else if (clicks == 2) {
								helper->dispatchDoubleClickRow((sl_uint32)(indexRow), UIPointf(x, y));
							}
						}
					}
				}
				
				void onRightMouseDown(SLIBListReportViewHandle_TableView* tv, NSEvent* ev)
				{
					Ref<ListReportViewHelper> helper = getHelper();
					if (helper.isNotNull()) {
						NSPoint ptWindow = [ev locationInWindow];
						NSPoint ptView = [tv convertPoint:ptWindow fromView:nil];
						NSInteger indexRow = [tv rowAtPoint:ptView];
						if (indexRow >= 0) {
							sl_ui_posf x = (sl_ui_posf)(ptView.x);
							sl_ui_posf y = (sl_ui_posf)(ptView.y);
							helper->dispatchRightButtonClickRow((sl_uint32)(indexRow), UIPointf(x, y));
						}
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(ListReportViewInstance, macOS_ViewInstance)
			
		}
	}

	using namespace priv::list_report_view;

	Ref<ViewInstance> ListReportView::createNativeWidget(ViewInstance* parent)
	{
		Ref<ListReportViewInstance> ret = macOS_ViewInstance::create<ListReportViewInstance, SLIBListReportViewHandle>(this, parent);
		if (ret.isNotNull()) {
			SLIBListReportViewHandle* handle = ret->getHandle();
			static_cast<ListReportViewHelper*>(this)->copyColumns(handle);
			static_cast<ListReportViewHelper*>(this)->applyFont(handle, getFont());
			[handle setHasVerticalScroller:TRUE];
			[handle setHasHorizontalScroller:TRUE];
			[handle setBorderType:NSBezelBorder];
			handle->table->m_viewInstance = ret;
			[handle->table reloadData];
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IListReportViewInstance> ListReportView::getListReportViewInstance()
	{
		return CastRef<ListReportViewInstance>(getViewInstance());
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
		self->table = [[SLIBListReportViewHandle_TableView alloc] init];
		[self->table setDelegate:self];
		[self->table setDataSource:self];
		[self->table setRowSizeStyle:NSTableViewRowSizeStyleCustom];
		[self->table setGridStyleMask:NSTableViewSolidVerticalGridLineMask | NSTableViewSolidHorizontalGridLineMask];
		[self setDocumentView:table];
	}
	return self;
}

- (Ref<ListReportViewHelper>)getHelper
{
	Ref<ListReportViewInstance> instance = self->m_viewInstance;
	if (instance.isNotNull()) {
		return instance->getHelper();
	}
	return sl_null;
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView{
	Ref<ListReportViewHelper> helper = [self getHelper];
	if (helper.isNotNull()) {
		return helper->getRowsCount();
	}
	return 0;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row{
	Ref<ListReportViewHelper> helper = [self getHelper];
	if (helper.isNotNull()) {
		NSString* _id = tableColumn.identifier;
		if (_id != nil) {
			sl_uint32 iRow = (sl_uint32)(row);
			sl_uint32 iCol = (sl_uint32)(_id.intValue);
			return Apple::getNSStringFromString(helper->getItemText(iRow, iCol));
		}
	}
	return @"";
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
	Ref<ListReportViewHelper> helper = [self getHelper];
	if (helper.isNotNull()) {
		sl_int32 n = (sl_int32)([table selectedRow]);
		if (n >= 0) {
			helper->dispatchSelectRow(n);
		}
	}
}

@end

@implementation SLIBListReportViewHandle_TableView

MACOS_VIEW_DEFINE_ON_FOCUS
MACOS_VIEW_DEFINE_ON_KEY

- (void)mouseDown:(NSEvent *)theEvent
{
	[super mouseDown:theEvent];
	Ref<ListReportViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onMouseDown(self, theEvent);
	}
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	[super rightMouseDown:theEvent];
	Ref<ListReportViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onRightMouseDown(self, theEvent);
	}
}

@end

#endif
