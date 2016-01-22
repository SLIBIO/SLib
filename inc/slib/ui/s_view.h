#ifndef CHECKHEADER_SLIB_UI_S_VIEW
#define CHECKHEADER_SLIB_UI_S_VIEW

#include "definition.h"

#include "generic_view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT SView : public GenericView
{
	SLIB_DECLARE_OBJECT(SView, GenericView)
public:
	SView();
	
public:
	sl_bool isNativeParent();
	
	sl_bool isNativeGroup();
	
	void setNativeParent(sl_bool flag);
	
	
	virtual void addChild(const Ref<View>& view);
	
	virtual void removeChild(const Ref<View>& view);
	
	void removeAllChildren();
	
	List< Ref<View> > getChildren();
	
protected:
	// override
	void onAttach();
	
private:
	void _removeChild(const Ref<View>& view);
	
protected:
	CList< Ref<View> > m_children;
	
	sl_bool m_flagNativeParent;
	
};

SLIB_UI_NAMESPACE_END

#endif
