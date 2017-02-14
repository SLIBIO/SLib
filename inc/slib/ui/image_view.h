#ifndef CHECKHEADER_SLIB_UI_IMAGE_VIEW
#define CHECKHEADER_SLIB_UI_IMAGE_VIEW

#include "definition.h"

#include "view.h"

namespace slib
{

	class SLIB_EXPORT ImageView : public View
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ImageView();
		
		~ImageView();

	public:
		static Ref<ImageView> create(sl_ui_len width, sl_ui_len height, const Ref<Drawable>& drawable);
		
	public:
		Ref<Drawable> getSource();
		
		virtual void setSource(const Ref<Drawable>& drawable, UIUpdateMode mode = UIUpdateMode::Redraw);
		
		
		ScaleMode getScaleMode();
		
		virtual void setScaleMode(ScaleMode scaleMode, UIUpdateMode updateMode = UIUpdateMode::Redraw);
		
		
		Alignment getGravity();
		
		virtual void setGravity(Alignment align, UIUpdateMode mode = UIUpdateMode::Redraw);
		
	public:
		// override
		void onDraw(Canvas* canvas);
		
		// override
		void onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical);
		
	protected:
		AtomicRef<Drawable> m_source;
		ScaleMode m_scaleMode;
		Alignment m_gravity;
		
	};

}

#endif
