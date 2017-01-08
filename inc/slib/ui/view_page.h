#ifndef CHECKHEADER_SLIB_VIEW_PAGE
#define CHECKHEADER_SLIB_VIEW_PAGE

#include "definition.h"

#include "view.h"
#include "transition.h"

#include "../core/event.h"

SLIB_UI_NAMESPACE_BEGIN

class ViewPage;
class ViewPager;

class SLIB_EXPORT IViewPagerListener
{
public:
	virtual void onPageAction(ViewPager* pager, View* page, UIPageAction action);
	
	virtual void onFinishPageAnimation(ViewPager* pager, View* page, UIPageAction action);
	
};

class SLIB_EXPORT ViewPager : public View
{
	SLIB_DECLARE_OBJECT
	
public:
	ViewPager();
	
public:
	sl_size getPagesCount();
	
	sl_size getCurrentPageIndex();
	
	Ref<View> getCurrentPage();
	
	void addPage(const Ref<View>& page, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void removePageAt(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void selectPage(sl_size index, UIUpdateMode mode = UIUpdateMode::Redraw);
	
	void goToPageAt(sl_size index, const Transition& transition);
	
	void goToPageAt(sl_size index);
	
	void goPrev(const Transition& transition);
	
	void goPrev();
	
	void goNext(const Transition& transition);
	
	void goNext();
	
	void push(const Ref<View>& page, const Transition& transition, sl_bool flagRemoveAllBackPages = sl_false);

	void push(const Ref<View>& page, sl_bool flagRemoveAllBackPages = sl_false);
	
	void pop(const Ref<View>& page, const Transition& transition);
	
	void pop(const Ref<View>& page);
	
	void pop(const Transition& transition);
	
	void pop();
	
	void setSwipeNavigation(sl_bool flag);
	
	TransitionType getPushTransitionType();
	
	void setPushTransitionType(TransitionType type);
	
	TransitionType getPopTransitionType();
	
	void setPopTransitionType(TransitionType type);
	
	void setTransitionType(TransitionType type);
	
	TransitionDirection getPushTransitionDirection();
	
	void setPushTransitionDirection(TransitionDirection direction);
	
	TransitionDirection getPopTransitionDirection();
	
	void setPopTransitionDirection(TransitionDirection direction);
	
	void setTransitionDirection(TransitionDirection direction);
	
	float getPushTransitionDuration();
	
	void setPushTransitionDuration(float duration);

	float getPopTransitionDuration();
	
	void setPopTransitionDuration(float duration);
	
	void setTransitionDuration(float duration);
	
	AnimationCurve getPushTransitionCurve();
	
	void setPushTransitionCurve(AnimationCurve curve);
	
	AnimationCurve getPopTransitionCurve();
	
	void setPopTransitionCurve(AnimationCurve curve);
	
	void setTransitionCurve(AnimationCurve curve);
	
protected:
	void _goTo(sl_size index, const Transition& transition);
	
	void _push(const Ref<View>& page, const Transition& transition, sl_bool flagRemoveAllBackPages);
	
	void _pop(const Ref<View>& page, const Transition& transition);
	
	void _applyDefaultPushTransition(Transition& transition);
	
	void _applyDefaultPopTransition(Transition& transition);
	
	
protected:
	virtual void onPageAction(View* page, UIPageAction action);
	
	virtual void onFinishPageAnimation(View* page, UIPageAction action);
	
public:
	virtual void dispatchPageAction(View* page, UIPageAction action);
	
	virtual void dispatchFinishPageAnimation(View* page, UIPageAction action);
	
public:
	// override
	void onResize(sl_ui_len width, sl_ui_len height);
	
	// override
	void onChangePadding();
	
	// override
	void onSwipe(GestureType type);
	
public:
	SLIB_PROPERTY(AtomicPtr<IViewPagerListener>, Listener)
	
protected:
	CList< Ref<View> > m_pages;
	sl_size m_indexCurrent;

	sl_bool m_flagSwipeNavigation;

	TransitionType m_pushTransitionType;
	TransitionType m_popTransitionType;
	TransitionDirection m_pushTransitionDirection;
	TransitionDirection m_popTransitionDirection;
	float m_pushTransitionDuration; // seconds
	float m_popTransitionDuration; // seconds
	AnimationCurve m_pushTransitionCurve;
	AnimationCurve m_popTransitionCurve;
	
};

class SLIB_EXPORT ViewPage : public ViewGroup
{
	SLIB_DECLARE_OBJECT
	
public:
	ViewPage();
	
public:
	Ref<ViewPager> getPager();
	
	void setPager(const Ref<ViewPager>& pager);
	
	
	void open(const Ref<ViewPager>& pager, const Transition& transition);
	
	void open(const Ref<ViewPager>& pager);
	
	void openHome(const Ref<ViewPager>& pager, const Transition& transition);
	
	void openHome(const Ref<ViewPager>& pager);
	
	void close(const Transition& transition);
	
	void close();
	
	void openPage(const Ref<View>& pageOther, const Transition& transition);
	
	void openPage(const Ref<View>& pageOther);
	
	void openHomePage(const Ref<View>& pageOther, const Transition& transition);
	
	void openHomePage(const Ref<View>& pageOther);
	
	
	TransitionType getGlobalOpeningTransitionType();
	
	void setGlobalOpeningTransitionType(TransitionType type);
	
	TransitionType getGlobalClosingTransitionType();
	
	void setGlobalClosingTransitionType(TransitionType type);
	
	void setGlobalTransitionType(TransitionType type);
	
	TransitionDirection getGlobalOpeningTransitionDirection();
	
	void setGlobalOpeningTransitionDirection(TransitionDirection direction);
	
	TransitionDirection getGlobalClosingTransitionDirection();
	
	void setGlobalClosingTransitionDirection(TransitionDirection direction);
	
	void setGlobalTransitionDirection(TransitionDirection direction);
	
	float getGlobalOpeningTransitionDuration();
	
	void setGlobalOpeningTransitionDuration(float duration);
	
	float getGlobalClosingTransitionDuration();
	
	void setGlobalClosingTransitionDuration(float duration);
	
	void setGlobalTransitionDuration(float duration);
	
	AnimationCurve getGlobalOpeningTransitionCurve();
	
	void setGlobalOpeningTransitionCurve(AnimationCurve curve);
	
	AnimationCurve getGlobalClosingTransitionCurve();
	
	void setGlobalClosingTransitionCurve(AnimationCurve curve);
	
	void setGlobalTransitionCurve(AnimationCurve curve);
	
	
	void popup(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
	
	void popup(const Ref<View>& parent, sl_bool flagFillParentBackground = sl_true);
	
	void popupPage(const Ref<ViewPage>& pageOther, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
	
	void popupPage(const Ref<ViewPage>& pageOther, sl_bool flagFillParentBackground = sl_true);
	
	sl_bool isPopup();
	
	Color getPopupBackgroundColor();
	
	void setPopupBackgroundColor(const Color& color);
	
	TransitionType getGlobalOpeningPopupTransitionType();
	
	void setGlobalOpeningPopupTransitionType(TransitionType type);
	
	TransitionType getGlobalClosingPopupTransitionType();
	
	void setGlobalClosingPopupTransitionType(TransitionType type);
	
	void setGlobalPopupTransitionType(TransitionType type);
	
	TransitionDirection getGlobalOpeningPopupTransitionDirection();
	
	void setGlobalOpeningPopupTransitionDirection(TransitionDirection direction);
	
	TransitionDirection getGlobalClosingPopupTransitionDirection();
	
	void setGlobalClosingPopupTransitionDirection(TransitionDirection direction);
	
	void setGlobalPopupTransitionDirection(TransitionDirection direction);
	
	float getGlobalOpeningPopupTransitionDuration();
	
	void setGlobalOpeningPopupTransitionDuration(float duration);
	
	float getGlobalClosingPopupTransitionDuration();
	
	void setGlobalClosingPopupTransitionDuration(float duration);
	
	void setGlobalPopupTransitionDuration(float duration);
	
	AnimationCurve getGlobalOpeningPopupTransitionCurve();
	
	void setGlobalOpeningPopupTransitionCurve(AnimationCurve curve);
	
	AnimationCurve getGlobalClosingPopupTransitionCurve();
	
	void setGlobalClosingPopupTransitionCurve(AnimationCurve curve);
	
	void setGlobalPopupTransitionCurve(AnimationCurve curve);
	
	Color getGlobalPopupBackgroundColor();
	
	void setGlobalPopupBackgroundColor(const Color& color);

	
	void modal(const Ref<View>& parent, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
	
	void modal(const Ref<View>& parent, sl_bool flagFillParentBackground = sl_true);
	
	void modalPage(const Ref<ViewPage>& pageOther, const Transition& transition, sl_bool flagFillParentBackground = sl_true);
	
	void modalPage(const Ref<ViewPage>& pageOther, sl_bool flagFillParentBackground = sl_true);
	
protected:
	virtual void onOpen();
	
	virtual void onClose();
	
	virtual void onResume();
	
	virtual void onPause();
	
	virtual void onPageAction(UIPageAction action);
	
	virtual void onFinishPageAnimation(UIPageAction action);
	
	// For mobile platforms
	virtual void onBackPressed(UIEvent* ev);
	
public:
	virtual void dispatchPageAction(ViewPager* pager, UIPageAction action);
	
	virtual void dispatchFinishPageAnimation(ViewPager* pager, UIPageAction action);
	
	virtual void dispatchOpen();
	
	virtual void dispatchClose();
	
	virtual void dispatchResume();
	
	virtual void dispatchPause();
	
	virtual void dispatchBackPressed(UIEvent* ev);
	
protected:
	void _openPopup(const Ref<View>& parent, Transition transition, sl_bool flagFillParentBackground);
	
	void _closePopup(Transition transition);
	
	void _finishPopupAnimation(UIPageAction action);
	
	void _applyDefaultOpeningPopupTransition(Transition& transition);
	
	void _applyDefaultClosingPopupTransition(Transition& transition);
	
public:
	SLIB_PROPERTY(AtomicFunction<void()>, OnClose);
	
protected:
	AtomicWeakRef<ViewPager> m_pager;
	
	sl_bool m_flagDidPopup;
	sl_bool m_flagClosingPopup;
	Color m_popupBackgroundColor;
	sl_bool m_flagDidModalOnUIThread;
	AtomicRef<Event> m_eventClosePopup;
	
};


SLIB_UI_NAMESPACE_END

#endif
