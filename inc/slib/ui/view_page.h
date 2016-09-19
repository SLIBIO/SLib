#ifndef CHECKHEADER_SLIB_VIEW_PAGE
#define CHECKHEADER_SLIB_VIEW_PAGE

#include "definition.h"

#include "../ui/view.h"

SLIB_UI_NAMESPACE_BEGIN

class ViewPage;
class ViewPager;

class SLIB_EXPORT IViewPagerListener
{
public:
	virtual void onPushPage(ViewPager* pager, View* pageIn);
	
	virtual void onPopPage(ViewPager* pager, View* pageOut);
	
	virtual void onResumePage(ViewPager* pager, View* page);
	
	virtual void onPausePage(ViewPager* pager, View* page);
	
	virtual void onReadyPage(ViewPager* pager, View* page);
	
};

class SLIB_EXPORT ViewPageMoveParam
{
public:
	
	
public:
	ViewPageMoveParam();
};

class SLIB_EXPORT ViewPagePushParam : public ViewPageMoveParam
{
public:
	
	
public:
	ViewPagePushParam();
	
};

class SLIB_EXPORT ViewPagePopParam : public ViewPageMoveParam
{
public:
	
	
public:
	ViewPagePopParam();
	
};

class SLIB_EXPORT ViewPager : public ViewGroup
{
	SLIB_DECLARE_OBJECT
	
public:
	ViewPager();
	
public:
	sl_size getPagesCount();
	
	sl_size getCurrentPageIndex();
	
	Ref<View> getCurrentPage();
	
	void push(const Ref<View>& page, const ViewPagePushParam& param);
	
	void push(const Ref<View>& page);
	
	void pop(const ViewPagePopParam& param);
	
	void pop();
	
	void moveNext(const ViewPageMoveParam& param);
	
	void moveNext();
	
	void movePrevious(const ViewPageMoveParam& param);
	
	void movePrevious();
	
	void moveTo(sl_size index);
	
	
protected:
	virtual void onPushPage(View* pageIn);
	
	virtual void onPopPage(View* pageOut);
	
	virtual void onResumePage(View* page);
	
	virtual void onPausePage(View* page);
	
	virtual void onReadyPage(View* page);
	
public:
	virtual void dispatchPushPage(View* pageIn);
	
	virtual void dispatchPopPage(View* pageOut);
	
	virtual void dispatchResumePage(View* page);
	
	virtual void dispatchPausePage(View* page);
	
	virtual void dispatchReadyPage(View* page);

public:
	SLIB_PTR_PROPERTY(IViewPagerListener, Listener)
	
private:
	CList< Ref<View> > m_pages;
	sl_size m_indexCurrent;
	
};

class SLIB_EXPORT ViewPage : public ViewGroup
{
	SLIB_DECLARE_OBJECT
	
public:
	ViewPage();
	
protected:
	virtual void onPush(ViewPager* pager);
	
	virtual void onPop(ViewPager* pager);
	
	virtual void onResume(ViewPager* pager);
	
	virtual void onPause(ViewPager* pager);
	
	virtual void onReady(ViewPager* pager);
	
public:
	virtual void dispatchPush(ViewPager* pager);
	
	virtual void dispatchPop(ViewPager* pager);
	
	virtual void dispatchResume(ViewPager* pager);
	
	virtual void dispatchPause(ViewPager* pager);
	
	virtual void dispatchReady(ViewPager* pager);
	
	
};


SLIB_UI_NAMESPACE_END

#endif
