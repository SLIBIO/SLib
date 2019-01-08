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

#ifndef CHECKHEADER_SLIB_UI_RESOURCE
#define CHECKHEADER_SLIB_UI_RESOURCE

#include "definition.h"

#include "../core/resource.h"

#include "menu.h"
#include "event.h"
#include "view.h"
#include "window.h"
#include "mobile_app.h"

namespace slib
{

	class UIResource
	{
	public:
		static void updateDefaultScreenSize();
		
		static sl_ui_len getScreenWidth();
		
		static void setScreenWidth(sl_ui_len width);
		
		static sl_ui_len getScreenHeight();
		
		static void setScreenHeight(sl_ui_len height);
		
		static sl_ui_len getScreenMinimum();
		
		static sl_ui_len getScreenMaximum();
		
		static double getScreenPPI();
		
		static void setScreenPPI(double ppi);
		
		static sl_real pixelToInch(sl_real px);
		
		static sl_real inchToPixel(sl_real inch);
		
		static sl_real pixelToMeter(sl_real px);
		
		static sl_real meterToPixel(sl_real meters);
		
		static sl_real pixelToCentimeter(sl_real px);
		
		static sl_real centimeterToPixel(sl_real cm);
		
		static sl_real pixelToMillimeter(sl_real px);
		
		static sl_real millimeterToPixel(sl_real mm);
		
		static sl_real pixelToPoint(sl_real px);
		
		static sl_real pointToPixel(sl_real dp);
		
		static sl_real pixelToDp(sl_real px);
		
		static sl_real dpToPixel(sl_real dp);
		
	};
	
#define SLIB_DECLARE_MENU_BEGIN(NAME) \
	class NAME : public slib::Referable { \
	public: \
		static const NAME* get(); \
		NAME(); \
		slib::Ref<slib::Menu> root; \
		slib::Ref<slib::Menu> root_menu;
	
#define SLIB_DECLARE_SUBMENU(NAME) \
	slib::Ref<slib::Menu> NAME##_menu; \
	slib::Ref<slib::MenuItem> NAME;
	
#define SLIB_DECLARE_MENU_ITEM(NAME) \
		slib::Ref<slib::MenuItem> NAME;
	
#define SLIB_DECLARE_MENU_SEPARATOR(NAME) \
		slib::Ref<slib::MenuItem> NAME;
	
#define SLIB_DECLARE_MENU_END };
	
	
#define SLIB_DEFINE_MENU_BEGIN(NAME) \
	const NAME* NAME::get() { \
		SLIB_SAFE_STATIC(NAME, ret); \
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) { \
			return sl_null; \
		} \
		return &ret; \
	} \
	NAME::NAME() { \
		root = root_menu = slib::Menu::create(); \
		if (root.isNull()) return;
	
#define SLIB_DEFINE_SUBMENU(PARENT, NAME, ...) \
	NAME##_menu = slib::Menu::create(); \
	if (NAME##_menu.isNull()) return; \
	NAME = PARENT##_menu->addSubmenu(NAME##_menu, __VA_ARGS__);
	
#define SLIB_DEFINE_MENU_ITEM(PARENT, NAME, ...) \
	NAME = PARENT##_menu->addMenuItem(__VA_ARGS__);
	
#define SLIB_DEFINE_MENU_SEPARATOR(PARENT, NAME) \
	NAME = PARENT##_menu->addSeparator();
	
#define SLIB_DEFINE_MENU_SEPARATOR_NONAME(PARENT) \
	PARENT##_menu->addSeparator();
	
#define SLIB_DEFINE_MENU_END }
	
#define SLIB_DECLARE_UILAYOUT_BEGIN(NAME, BASE_CLASS) \
	class NAME : public BASE_CLASS \
	{ \
		SLIB_DECLARE_OBJECT \
	public: \
		NAME(sl_real sp = 1); \
	protected: \
		void initialize(); \
		void layoutViews(sl_ui_len width, sl_ui_len height) override; \
	public:
	
#define SLIB_DECLARE_UILAYOUT_END \
	};
	
#define SLIB_DEFINE_UILAYOUT(NAME, BASE_CLASS) \
	SLIB_DEFINE_OBJECT(NAME, BASE_CLASS) \
	NAME::NAME(sl_real sp): BASE_CLASS(sp) \
	{ \
		SLIB_REFERABLE_CONSTRUCTOR \
		initialize(); \
		setInitialized(); \
		slib::UISize size = getContentSize(); \
		if (size.x > 0 && size.y > 0) { \
			layoutViews(size.x, size.y); \
		} \
	}
	
	class UILayoutResource
	{
	public:
		UILayoutResource(sl_real sp = 1);
		
		virtual ~UILayoutResource();

	public:
		Ref<View> getContent();
		
		sl_real getScaledPixel();
		
		void setScaledPixel(sl_real sp);
		
	protected:
		virtual void layoutViews(sl_ui_len width, sl_ui_len height) = 0;
		
	protected:
		void _layoutViews_safe(sl_ui_len width, sl_ui_len height);
		
		sl_bool isInitialized();
		
		void setInitialized();
		
	protected:
		View* m_contentView;
		Ref<View> m_contentViewRef;
		
		sl_real m_sp;
		
		sl_int32 m_countRecursiveLayout;
		sl_bool m_flagInitialized;
		
	};
	
#define SLIB_DECLARE_WINDOW_LAYOUT_BEGIN(NAME) \
	SLIB_DECLARE_UILAYOUT_BEGIN(NAME, slib::WindowLayoutResource)
	
#define SLIB_DECLARE_WINDOW_LAYOUT_END \
	SLIB_DECLARE_UILAYOUT_END
	
#define SLIB_DEFINE_WINDOW_LAYOUT(NAME) \
	SLIB_DEFINE_UILAYOUT(NAME, slib::WindowLayoutResource)
	
	
	class WindowLayoutResource : public Window, public UILayoutResource
	{
		SLIB_DECLARE_OBJECT

	public:
		WindowLayoutResource(sl_real sp = 1);
		
		~WindowLayoutResource();

	public:
		UISize getContentSize();
		
	public:
		void dispatchResize(sl_ui_len width, sl_ui_len height) override;
		
	protected:
		void _layoutViews_safe();
		
	};
	
#define SLIB_DECLARE_VIEW_LAYOUT_BEGIN(NAME) \
	SLIB_DECLARE_UILAYOUT_BEGIN(NAME, slib::ViewLayoutResource)
	
#define SLIB_DECLARE_VIEW_LAYOUT_END \
	SLIB_DECLARE_UILAYOUT_END
	
#define SLIB_DEFINE_VIEW_LAYOUT(NAME) \
	SLIB_DEFINE_UILAYOUT(NAME, slib::ViewLayoutResource)
	
	class ViewLayoutResource : public ViewGroup, public UILayoutResource
	{
		SLIB_DECLARE_OBJECT
	public:
		ViewLayoutResource(sl_real sp = 1);
		
		~ViewLayoutResource();

	public:
		UISize getContentSize();
		
	public:
		void dispatchResize(sl_ui_len width, sl_ui_len height) override;
		
	};
	
	
#define SLIB_DECLARE_PAGE_LAYOUT_BEGIN(NAME) \
	SLIB_DECLARE_UILAYOUT_BEGIN(NAME, slib::PageLayoutResource)
	
#define SLIB_DECLARE_PAGE_LAYOUT_END \
	SLIB_DECLARE_UILAYOUT_END
	
#define SLIB_DEFINE_PAGE_LAYOUT(NAME) \
	SLIB_DEFINE_UILAYOUT(NAME, slib::PageLayoutResource)
	
	class PageLayoutResource : public ViewPage, public UILayoutResource
	{
		SLIB_DECLARE_OBJECT

	public:
		PageLayoutResource(sl_real sp = 1);
		
		~PageLayoutResource();

	public:
		UISize getContentSize();
		
	public:
		void dispatchResize(sl_ui_len width, sl_ui_len height) override;
		
	};
	
	
	class Button;
	class LabelView;
	class CheckBox;
	class RadioButton;
	class RadioGroup;
	class EditView;
	class PasswordView;
	class TextArea;
	class ImageView;
	class SelectView;
	class ScrollView;
	class LinearView;
	class ListView;
	class ListReportView;
	class RenderView;
	class TabView;
	class TreeView;
	class WebView;
	class SplitView;
	class ProgressBar;
	class Slider;
	class PickerView;
	class ViewPager;
	class VideoView;
	class CameraView;
	class QRCodeScanner;
	class Drawer;

}

#endif
