#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/select_view.h"

#include "view_ios.h"

@interface _Slib_iOS_SelectView : UITextField<UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate>
{
	@public UIPickerView* m_picker;
	
	@public sl_uint32 m_selectionBefore;
	
	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;
}
@end

SLIB_UI_NAMESPACE_BEGIN

class _SelectView : public SelectView
{
public:
	sl_uint32 __getItemsCount()
	{
		return (sl_uint32)(m_titles.getCount());
	}
	
	NSString* __getItemTitle(sl_uint32 row)
	{
		String s = m_titles.getValueAt(row);
		return Apple::getNSStringFromString(s);
	}
	
	void __onSelectItem(_Slib_iOS_SelectView* v, sl_uint32 row)
	{
		m_indexSelected = row;
		v.text = __getItemTitle(row);
		onSelectItem(row);
	}
	
	void __onStartSelection(_Slib_iOS_SelectView* v)
	{
		sl_uint32 n = m_indexSelected;
		v->m_selectionBefore = n;
		dispatch_async(dispatch_get_main_queue(), ^{
			[v->m_picker selectRow:n inComponent:0 animated:NO];
		});
	}
	
	void __onCancelSelection(_Slib_iOS_SelectView* v)
	{
		__onSelectItem(v, v->m_selectionBefore);
	}
	
	void __selectItem(_Slib_iOS_SelectView* v, sl_uint32 row)
	{
		v.text = __getItemTitle(row);
		[v->m_picker selectRow:row inComponent:0 animated:NO];
	}
};

Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	_Slib_iOS_SelectView* handle = [[_Slib_iOS_SelectView alloc] initWithFrame:frame];
	if (handle != nil) {
		((_SelectView*)this)->__selectItem(handle, m_indexSelected);
		
		Ref<Font> font = getFont();
		UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[handle setFont:hFont];
		}
	}
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

void SelectView::_getSelectedIndex_NW()
{
}

void SelectView::_select_NW(sl_uint32 index)
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_select_NW(index);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_SelectView class]]) {
		_Slib_iOS_SelectView* v = (_Slib_iOS_SelectView*)handle;
		((_SelectView*)this)->__selectItem(v, index);
	}
}

void SelectView::_refreshItemsCount_NW()
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_refreshItemsCount_NW();
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_SelectView class]]) {
		_Slib_iOS_SelectView* v = (_Slib_iOS_SelectView*)handle;
		[v->m_picker reloadAllComponents];
	}
}

void SelectView::_refreshItemsContent_NW()
{
	if (![NSThread isMainThread]) {
		dispatch_async(dispatch_get_main_queue(), ^{
			_refreshItemsContent_NW();
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_SelectView class]]) {
		_Slib_iOS_SelectView* v = (_Slib_iOS_SelectView*)handle;
		[v->m_picker reloadAllComponents];
	}
}

void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
{
	if (![NSThread isMainThread]) {
		String _title = title;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setItemTitle_NW(index, _title);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_SelectView class]]) {
		_Slib_iOS_SelectView* v = (_Slib_iOS_SelectView*)handle;
		[v->m_picker reloadAllComponents];
	}
}

void SelectView::_setFont_NW(const Ref<Font>& font)
{
	if (![NSThread isMainThread]) {
		Ref<Font> _font = font;
		dispatch_async(dispatch_get_main_queue(), ^{
			_setFont_NW(_font);
		});
		return;
	}
	
	UIView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil && [handle isKindOfClass:[_Slib_iOS_SelectView class]]) {
		_Slib_iOS_SelectView* v = (_Slib_iOS_SelectView*)handle;
		UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
		if (hFont != nil) {
			[v setFont:hFont];
		}
	}
}

SLIB_UI_NAMESPACE_END

#define DROP_ICON_WIDTH 20
#define DROP_ICON_HEIGHT 12

@interface _Slib_iOS_SelectView_DropIcon : UIView
{
	@public __weak _Slib_iOS_SelectView* parent;
}
@end

@implementation _Slib_iOS_SelectView_DropIcon
- (id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self setClipsToBounds:YES];
		[self setOpaque:FALSE];
	}
	return self;
}

- (void)drawRect:(CGRect)dirtyRect
{
	CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextBeginPath(context);
	float pl = DROP_ICON_WIDTH * 0.2f;
	float pr = DROP_ICON_WIDTH * 0.3f;
	float ph = DROP_ICON_HEIGHT * 0.2f;
	CGContextSetAllowsAntialiasing(context, YES);
	CGContextSetShouldAntialias(context, YES);
	CGContextMoveToPoint(context, pl, ph);
	CGContextAddLineToPoint(context, pl + (DROP_ICON_WIDTH - pl - pr) / 2, DROP_ICON_HEIGHT - ph);
	CGContextAddLineToPoint(context, DROP_ICON_WIDTH - pr, ph);
	CGContextSetLineWidth(context, 2);
	CGContextSetLineCap(context, kCGLineCapRound);
	CGContextSetLineJoin(context, kCGLineJoinRound);
	CGContextSetRGBStrokeColor(context, 0, 0, 0, 1);
	CGContextStrokePath(context);
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)theEvent
{
	[super touchesBegan:touches withEvent:theEvent];
	[parent becomeFirstResponder];
}
@end

@implementation _Slib_iOS_SelectView
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	
	if (self != nil) {
	
		self->m_selectionBefore = 0;
		
		[self setDelegate:self];
		[self setBackgroundColor:[UIColor whiteColor]];
		[self setBorderStyle:UITextBorderStyleRoundedRect];
		
		// hide the caret and its blinking
		[[self valueForKey:@"textInputTraits"] setValue:[UIColor clearColor] forKey:@"insertionPointColor"];
		
		// add icon
		_Slib_iOS_SelectView_DropIcon* icon = [[_Slib_iOS_SelectView_DropIcon alloc] initWithFrame:(CGRectMake(0, 0, DROP_ICON_WIDTH, DROP_ICON_HEIGHT))];
		icon->parent = self;
		self.rightView =  icon;
		self.rightViewMode = UITextFieldViewModeAlways;
		
		// picker
		UIPickerView* picker = [[UIPickerView alloc] init];
		picker.showsSelectionIndicator = YES;
		picker.dataSource = self;
		picker.delegate = self;
		self->m_picker = picker;
	
		// toolbar
		UIToolbar* toolbar = [[UIToolbar alloc] init];
		toolbar.barStyle = UIBarStyleDefault;
		[toolbar sizeToFit];
		UIBarButtonItem *flexibleSpace = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
		UIBarButtonItem* doneButton = [[UIBarButtonItem alloc] initWithTitle:@"Done" style:UIBarButtonItemStyleDone target:self action:@selector(doneClicked:)];
		UIBarButtonItem* cancelButton = [[UIBarButtonItem alloc] initWithTitle:@"Cancel" style:UIBarButtonItemStylePlain target:self action:@selector(cancelClicked:)];
		[toolbar setItems:[NSArray arrayWithObjects:cancelButton, flexibleSpace, doneButton, nil]];

		self.inputView = picker;
		self.inputAccessoryView = toolbar;
	}
	return self;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView;
{
	return 1;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			_view->__onSelectItem(self, (sl_uint32)row);
		}
	}
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			return (NSInteger)(_view->__getItemsCount());
		}
	}
	return 0;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component;
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			return _view->__getItemTitle((sl_uint32)row);
		}
	}
	return @"";
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)aTextField
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			if ((NSInteger)(_view->__getItemsCount()) > 0) {
				return YES;
			}
		}
	}
	return NO;
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	[self sendActionsForControlEvents:UIControlEventEditingDidBegin];
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			_view->__onStartSelection(self);
		}
	}
}

- (void)textFieldDidEndEditing:(UITextField *)aTextField {
	aTextField.userInteractionEnabled = YES;
	[self sendActionsForControlEvents:UIControlEventEditingDidEnd];
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	return NO;
}

-(void)doneClicked:(id) sender
{
	[self resignFirstResponder];
	[self sendActionsForControlEvents:UIControlEventValueChanged];
}

-(void)cancelClicked:(id)sender
{
	[self resignFirstResponder];
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (slib::_SelectView* _view = slib::CastInstance<slib::_SelectView>(view.get())) {
			_view->__onCancelSelection(self);
		}
	}
}
@end

#endif
