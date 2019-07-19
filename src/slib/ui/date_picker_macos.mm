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

#include "slib/ui/date_picker.h"

#include "view_macos.h"

@interface SLIBDatePickerHandle : NSDatePicker<NSDatePickerCellDelegate>
{
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
}
@end

namespace slib
{

	Ref<ViewInstance> DatePicker::createNativeWidget(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		SLIBDatePickerHandle* handle = [[SLIBDatePickerHandle alloc] initWithFrame:frame];
		if (handle != nil) {
			handle.dateValue = Apple::getNSDateFromTime(m_date);
			handle.datePickerElements = NSDatePickerElementFlagYearMonthDay;
		}
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

	void DatePicker::_getDate_NW()
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSDatePicker class]]) {
			NSDatePicker* v = (NSDatePicker*)handle;
			m_date = Apple::getTimeFromNSDate(v.dateValue);
		}
	}

	void DatePicker::_setDate_NW(const Time& date)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil && [handle isKindOfClass:[NSDatePicker class]]) {
			NSDatePicker* v = (NSDatePicker*)handle;
			v.dateValue = Apple::getNSDateFromTime(date);
		}
	}

	sl_bool DatePicker::_measureSize_NW(UISize& _out)
	{
		return UIPlatform::measureNativeWidgetFittingSize(this, _out);
	}

	void DatePicker::_setFont_NW(const Ref<Font>& font)
	{
	}

}

using namespace slib;

@implementation SLIBDatePickerHandle

MACOS_VIEW_DEFINE_ON_FOCUS

-(id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		self.delegate = self;
	}
	return self;
}

- (void)datePickerCell:(NSDatePickerCell *)datePickerCell
validateProposedDateValue:(NSDate * _Nonnull *)proposedDateValue
		  timeInterval:(NSTimeInterval *)proposedTimeInterval
{
	Ref<DatePicker> view = GetViewFromInstance<DatePicker, macOS_ViewInstance>(m_viewInstance);
	if (view.isNotNull()) {
		NSDate* date = *proposedDateValue;
		Time time = Apple::getTimeFromNSDate(date);
		Time old = time;
		view->dispatchChange(time);
		if (old != time) {
			*proposedDateValue = Apple::getNSDateFromTime(time);
		}
	}
}

@end

#endif
