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

namespace slib
{
	namespace priv
	{
		namespace date_picker
		{
			class DatePickerInstance;
		}
	}
}

@interface SLIBDatePickerHandle : NSDatePicker<NSDatePickerCellDelegate>
{
	@public slib::WeakRef<slib::priv::date_picker::DatePickerInstance> m_viewInstance;
}
@end

namespace slib
{
	
	namespace priv
	{
		namespace date_picker
		{
			
			class DatePickerInstance : public macOS_ViewInstance, public IDatePickerInstance
			{
				SLIB_DECLARE_OBJECT
				
			public:
				NSDatePicker* getHandle()
				{
					return (NSDatePicker*)m_handle;
				}
				
				sl_bool getDate(DatePicker* view, Time& _out) override
				{
					NSDatePicker* handle = getHandle();
					if (handle != nil) {
						_out = Apple::getTimeFromNSDate(handle.dateValue);
						return sl_true;
					}
					return sl_false;
				}
				
				void setDate(DatePicker* view, const Time& date) override
				{
					NSDatePicker* handle = getHandle();
					if (handle != nil) {
						handle.dateValue = Apple::getNSDateFromTime(date);
					}
				}
				
				sl_bool measureSize(DatePicker* view, UISize& _out) override
				{
					return UIPlatform::measureNativeWidgetFittingSize(this, _out);
				}
				
				void onChange(SLIBDatePickerHandle* handle, NSDate** pValue)
				{
					Ref<DatePicker> view = CastRef<DatePicker>(getView());
					if (view.isNotNull()) {
						NSDate* date = *pValue;
						Time time = Apple::getTimeFromNSDate(date);
						Time old = time;
						view->dispatchChange(time);
						if (old != time) {
							*pValue = Apple::getNSDateFromTime(time);
						}
					}
				}
				
			};
			
			SLIB_DEFINE_OBJECT(DatePickerInstance, macOS_ViewInstance)
			
		}
	}
	
	using namespace priv::date_picker;

	Ref<ViewInstance> DatePicker::createNativeWidget(ViewInstance* parent)
	{
		Ref<DatePickerInstance> ret = macOS_ViewInstance::create<DatePickerInstance, SLIBDatePickerHandle>(this, parent);
		if (ret.isNotNull()) {
			NSDatePicker* handle = ret->getHandle();
			handle.dateValue = Apple::getNSDateFromTime(m_date);
			handle.datePickerElements = NSDatePickerElementFlagYearMonthDay;
			return ret;
		}
		return sl_null;
	}
	
	Ptr<IDatePickerInstance> DatePicker::getDatePickerInstance()
	{
		return CastRef<DatePickerInstance>(getViewInstance());
	}
	
}

using namespace slib;
using namespace slib::priv::date_picker;

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
	Ref<DatePickerInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onChange(self, proposedDateValue);
	}
}

@end

#endif
