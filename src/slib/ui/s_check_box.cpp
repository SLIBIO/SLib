#include "../../../inc/slib/ui/s_check_box.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_UI_NAMESPACE_BEGIN

class _SCheckBox_Icon : public Drawable
{
public:
	Ref<Pen> m_penBorder;
	Ref<Brush> m_brush;
	Ref<Pen> m_penCheck;
	
	_SCheckBox_Icon(const Ref<Pen>& penBorder, const Color& backColor, const Ref<Pen>& penCheck)
	{
		m_penBorder = penBorder;
		if (backColor.getAlpha() > 0) {
			m_brush = Brush::createSolidBrush(backColor);
		}
		m_penCheck = penCheck;
	}
	
	sl_real getDrawableWidth()
	{
		return 1;
	}
	
	sl_real getDrawableHeight()
	{
		return 1;
	}
	
	void onDraw(Canvas* canvas, const Rectangle& _rectDst, const Rectangle& rectSrc)
	{
		Rectangle rectDst = GraphicsUtil::transformRectangle(_rectDst, rectSrc, Rectangle(0, 0, 1, 1));
		canvas->setAntiAlias(sl_false);
		canvas->drawRectangle(rectDst, m_penBorder, m_brush);
		canvas->setAntiAlias(sl_true);

		if (m_penCheck.isNotNull()) {
			Point pts[3];
			pts[0] = Point(0.2f, 0.6f);
			pts[1] = Point(0.4f, 0.8f);
			pts[2] = Point(0.8f, 0.3f);
			for (int i = 0; i < 3; i++) {
				pts[i].x = rectDst.left + rectDst.getWidth() * pts[i].x;
				pts[i].y = rectDst.top + rectDst.getHeight() * pts[i].y;
			}
			canvas->drawLines(pts, 3, m_penCheck);
		}
	}

};

class _SCheckBox_Categories
{
public:
	SButtonCategory categories[2];
	
	_SCheckBox_Categories()
	{
		Color colorBackNormal = Color::white();
		Color colorBackHover = Color::white();
		Color colorBackDown(220, 230, 255);
		Color colorBackDisabled = Color::gray(220);
		Ref<Pen> penNormal = Pen::createSolidPen(1, Color::black());
		Ref<Pen> penHover = Pen::createSolidPen(1, Color(0, 80, 200));
		Ref<Pen> penDown = penHover;
		Ref<Pen> penDisabled = Pen::createSolidPen(1, Color::gray(90));
		Ref<Pen> penCheckNormal = Pen::createSolidPen(2, Color::black());
		Ref<Pen> penCheckHover = Pen::createSolidPen(2, Color(0, 80, 200));
		Ref<Pen> penCheckDown = penCheckHover;
		Ref<Pen> penCheckDisabled = Pen::createSolidPen(2, Color::gray(90));
		categories[0].properties[buttonStateNormal].textColor = Color::black();
		categories[0].properties[buttonStateNormal].icon = new _SCheckBox_Icon(penNormal, colorBackNormal, Ref<Pen>::null());
		categories[0].properties[buttonStateDisabled].textColor = Color::gray(90);
		categories[0].properties[buttonStateDisabled].icon = new _SCheckBox_Icon(penDisabled, colorBackDisabled, Ref<Pen>::null());
		categories[0].properties[buttonStateHover].icon = new _SCheckBox_Icon(penHover, colorBackHover, Ref<Pen>::null());
		categories[0].properties[buttonStateDown].icon = new _SCheckBox_Icon(penDown, colorBackDown, Ref<Pen>::null());
		
		categories[1] = categories[0];
		categories[1].properties[buttonStateNormal].icon = new _SCheckBox_Icon(penNormal, colorBackNormal, penCheckNormal);
		categories[1].properties[buttonStateDisabled].icon = new _SCheckBox_Icon(penDisabled, colorBackDisabled, penCheckDisabled);
		categories[1].properties[buttonStateHover].icon = new _SCheckBox_Icon(penHover, colorBackHover, penCheckHover);
		categories[1].properties[buttonStateDown].icon = new _SCheckBox_Icon(penDown, colorBackDown, penCheckDown);
	}
	
	static _SCheckBox_Categories* get()
	{
		SLIB_SAFE_STATIC(_SCheckBox_Categories, ret);
		return &ret;
	}
};

SCheckBox::SCheckBox() : SButton(2, _SCheckBox_Categories::get()->categories)
{
	setIconSize(Font::getDefaultFontSize());
	setIconMargin(2);
	setTextMargin(2, 0, 2, 2);
	setContentAlignment(alignMiddleLeft);
	setIconAlignment(alignMiddleLeft);
	setTextAlignment(alignMiddleLeft);
}

SCheckBox::SCheckBox(sl_uint32 nCategories, SButtonCategory* categories) : SButton(nCategories, categories)
{
	setIconSize(Font::getDefaultFontSize());
	setIconMargin(2);
	setTextMargin(2, 0, 2, 2);
	setContentAlignment(alignMiddleLeft);
	setIconAlignment(alignMiddleLeft);
	setTextAlignment(alignMiddleLeft);
}

sl_bool SCheckBox::isChecked()
{
	return m_flagChecked;
}

void SCheckBox::setChecked(sl_bool flag)
{
	m_flagChecked = flag;
	setCurrentCategory(flag ? 1 : 0);
}

void SCheckBox::dispatchClick()
{
	setChecked(!m_flagChecked);
	SButton::dispatchClick();
}
SLIB_UI_NAMESPACE_END
