#ifndef CHECKHEADER_SLIB_UI_VIEW_GROUP
#define CHECKHEADER_SLIB_UI_VIEW_GROUP

#include "definition.h"

#include "generic_view.h"

SLIB_UI_NAMESPACE_BEGIN
class SLIB_EXPORT ViewGroup : public GenericViewWithDrawing
{
	SLIB_DECLARE_OBJECT(ViewGroup, GenericViewWithDrawing)
public:
	ViewGroup();
	
public:
	void addChild(const Ref<View>& view);
	void removeChild(const Ref<View>& view);
	void removeAllChildren();
	const List< Ref<View> >& getChildren();
	
protected:
	// override
	void onAttach();
	
protected:
	List< Ref<View> > m_children;
};
SLIB_UI_NAMESPACE_END

#endif
