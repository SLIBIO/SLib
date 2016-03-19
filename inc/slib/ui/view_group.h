#ifndef CHECKHEADER_SLIB_UI_VIEW_GROUP
#define CHECKHEADER_SLIB_UI_VIEW_GROUP

#include "definition.h"

#include "generic_view.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT ViewGroup : public GenericViewWithDrawing
{
	SLIB_DECLARE_OBJECT
	
public:
	ViewGroup();
	
public:
	void addChild(const Ref<View>& view);
	
	void removeChild(const Ref<View>& view);
	
	void removeAllChildren();
	
	List< Ref<View> > getChildren();
	
protected:
	// override
	void onAttach();
	
private:
	void _removeChild(const Ref<View>& view);
	
protected:
	CList< Ref<View> > m_children;
	
};

SLIB_UI_NAMESPACE_END

#endif
