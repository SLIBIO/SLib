#include "../../../inc/slib/ui/radio_button.h"

#include "../../../inc/slib/core/safe_static.h"

SLIB_UI_NAMESPACE_BEGIN

class _RadioButton_Icon : public Drawable
{
public:
	Ref<Pen> m_penBorder;
	Ref<Brush> m_brushBack;
	Ref<Brush> m_brushCheck;
	
public:
	_RadioButton_Icon(const Ref<Pen>& penBorder, const Color& backColor, const Color& checkColor)
	{
		m_penBorder = penBorder;
		if (backColor.a > 0) {
			m_brushBack = Brush::createSolidBrush(backColor);
		}
		if (checkColor.a > 0) {
			m_brushCheck = Brush::createSolidBrush(checkColor);
		}
	}
	
public:
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		canvas->drawEllipse(rectDst, m_penBorder, m_brushBack);
		if (m_brushCheck.isNotNull()) {
			Rectangle rcCheck;
			rcCheck.setLeftTop(rectDst.getCenter());
			sl_real w = rectDst.getWidth() / 2;
			sl_real h = rectDst.getHeight() / 2;
			rcCheck.left -= w / 2;
			rcCheck.top -= h / 2;
			rcCheck.right = rcCheck.left + w;
			rcCheck.bottom = rcCheck.top + h;
			canvas->fillEllipse(rcCheck, m_brushCheck);
		}
	}
	
};

class _RadioButton_Categories
{
public:
	ButtonCategory categories[2];
	
	_RadioButton_Categories()
	{
		Color colorBackNormal = Color::White;
		Color colorBackHover = Color::White;
		Color colorBackDown(220, 230, 255);
		Color colorBackDisabled = Color(220, 220, 220);
		Ref<Pen> penNormal = Pen::createSolidPen(1, Color::Black);
		Ref<Pen> penHover = Pen::createSolidPen(1, Color(0, 80, 200));
		Ref<Pen> penDown = penHover;
		Ref<Pen> penDisabled = Pen::createSolidPen(1, Color(90, 90, 90));
		Color colorCheckNormal = Color::Black;
		Color colorCheckDisabled = Color(90, 90, 90);
		Color colorCheckHover = Color(0, 80, 200);
		Color colorCheckDown = colorCheckHover;
		categories[0].properties[(int)ButtonState::Normal].icon = new _RadioButton_Icon(penNormal, colorBackNormal, Color::zero());
		categories[0].properties[(int)ButtonState::Disabled].icon = new _RadioButton_Icon(penDisabled, colorBackDisabled, Color::zero());
		categories[0].properties[(int)ButtonState::Hover].icon = new _RadioButton_Icon(penHover, colorBackHover, Color::zero());
		categories[0].properties[(int)ButtonState::Pressed].icon = new _RadioButton_Icon(penDown, colorBackDown, Color::zero());
		
		categories[1] = categories[0];
		categories[1].properties[(int)ButtonState::Normal].icon = new _RadioButton_Icon(penNormal, colorBackNormal, colorCheckNormal);
		categories[1].properties[(int)ButtonState::Disabled].icon = new _RadioButton_Icon(penDisabled, colorBackDisabled, colorCheckDisabled);
		categories[1].properties[(int)ButtonState::Hover].icon = new _RadioButton_Icon(penHover, colorBackHover, colorCheckHover);
		categories[1].properties[(int)ButtonState::Pressed].icon = new _RadioButton_Icon(penDown, colorBackDown, colorCheckDown);
	}
	
	static ButtonCategory* getCategories()
	{
		SLIB_SAFE_STATIC(_RadioButton_Categories, ret)
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		return ret.categories;
	}

};


SLIB_DEFINE_OBJECT(RadioButton, CheckBox)

RadioButton::RadioButton() : CheckBox(2, _RadioButton_Categories::getCategories())
{
	setCreatingNativeWidget(sl_true);
}

RadioButton::RadioButton(sl_uint32 nCategories, ButtonCategory* categories) : CheckBox(nCategories, categories)
{
}

void RadioButton::dispatchClick(UIEvent* ev)
{
	CheckBox::dispatchClick(ev);
	Ref<RadioGroup> group = getRadioGroup();
	if (group.isNotNull()) {
		group->select(this);
	}
}


SLIB_DEFINE_OBJECT(RadioGroup, Object)

RadioGroup::RadioGroup()
{
}

void RadioGroup::add(const Ref<RadioButton>& view)
{
	if (view.isNotNull()) {
		view->setRadioGroup(this);
		m_views.addIfNotExist(view);
		select(m_viewSelected);
	}
}

void RadioGroup::remove(const Ref<RadioButton>& view)
{
	if (view.isNotNull()) {
		if (view->getRadioGroup() == this) {
			view->setRadioGroup(Ref<RadioGroup>::null());
		}
		m_views.removeValue(view);
	}
}

void RadioGroup::select(const Ref<RadioButton>& sel)
{
	m_viewSelected = sel;
	ListLocker< Ref<RadioButton> > views(m_views);
	for (sl_size i = 0; i < views.count; i++) {
		Ref<RadioButton> view = views[i];
		if (view.isNotNull() && view != sel) {
			view->setChecked(sl_false);
		}
	}
	if (sel.isNotNull()) {
		sel->setChecked(sl_true);
	}
}

Ref<RadioButton> RadioGroup::getSelected()
{
	return m_viewSelected;
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_WIN32))
Ref<ViewInstance> RadioButton::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}
#endif

SLIB_UI_NAMESPACE_END

