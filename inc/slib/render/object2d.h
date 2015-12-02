#ifndef CHECKHEADER_SLIB_RENDER_OBJECT2D
#define CHECKHEADER_SLIB_RENDER_OBJECT2D

#include "definition.h"
#include "object.h"

#include "../core/list.h"
#include "../core/map.h"

SLIB_RENDER_NAMESPACE_BEGIN

class SLIB_EXPORT RenderObject2D : public RenderObject
{
public:
	SLIB_INLINE RenderObject2D()
	{
		m_flagUpdateTransformLocal = sl_false;
		m_flagUpdateTransformLocalInverse = sl_false;
		m_flagUpdateTransformParentInverse = sl_false;
		m_flagUpdateTransformWorld = sl_false;
		m_flagUpdateTransformWorldInverse = sl_false;
		m_flagUpdateBoundsParent = sl_false;
		m_flagUpdateBoundsWorld = sl_false;
		m_position = Point(0, 0);
		m_size = Size(1, 1);
		m_anchorPosition = Point(0, 0);
		m_rotationAngle = 0;
		m_transformLocal = Matrix3::identity();
		m_transformLocalInverse = Matrix3::identity();
		m_transformParent = Matrix3::identity();
		m_transformParentInverse = Matrix3::identity();
		m_transformWorld = Matrix3::identity();
		m_transformWorldInverse = Matrix3::identity();
		m_bounds = Rectangle(-0.5f, -0.5f, 0.5f, 0.5f);
		m_boundsParent = m_bounds;
		m_boundsWorld = m_bounds;
	}

private:
	Point m_position;
	Size m_size;
	Point m_anchorPosition;
	sl_real m_rotationAngle;

	Matrix3 m_transformLocal;
	Matrix3 m_transformLocalInverse;
	Matrix3 m_transformParent;
	Matrix3 m_transformParentInverse;
	Matrix3 m_transformWorld;
	Matrix3 m_transformWorldInverse;
	Rectangle m_bounds;
	Rectangle m_boundsParent;
	Rectangle m_boundsWorld;
	sl_bool m_flagUpdateTransformLocal;
	sl_bool m_flagUpdateTransformLocalInverse;
	sl_bool m_flagUpdateTransformParentInverse;
	sl_bool m_flagUpdateTransformWorld;
	sl_bool m_flagUpdateTransformWorldInverse;
	sl_bool m_flagUpdateBoundsParent;
	sl_bool m_flagUpdateBoundsWorld;

public:
	SLIB_INLINE const Point& getPosition() const
	{
		return m_position;
	}
	SLIB_INLINE void setPosition(const Point& position)
	{
		m_position = position;
		m_flagUpdateTransformLocal = sl_true;
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}

	SLIB_INLINE const Size& getSize() const
	{
		return m_size;
	}
	SLIB_INLINE void setSize(const Size& size)
	{
		m_size = size;
		m_flagUpdateTransformLocal = sl_true;
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}

	SLIB_INLINE Rectangle getRectangle() const
	{
		const Point& pt = getPosition();
		const Size& size = getSize();
		return Rectangle(pt.x - size.x / 2, pt.y - size.y / 2, size.x, size.y);
	}
	SLIB_INLINE void setRectangle(const Rectangle& rc)
	{
		m_position = rc.getCenter();
		m_size = rc.getSize();
		m_flagUpdateTransformLocal = sl_true;
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}
	SLIB_INLINE void setRectangle(sl_real left, sl_real top, sl_real width, sl_real height)
	{
		setRectangle(Rectangle(left, top, left + width, top + height));
	}

	SLIB_INLINE const Point& getAnchorPosition() const
	{
		return m_anchorPosition;
	}
	SLIB_INLINE void setAnchorPosition(const Point& position)
	{
		m_anchorPosition = position;
		m_flagUpdateTransformLocal = sl_true;
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}

	SLIB_INLINE sl_real getRotationAngle() const
	{
		return m_rotationAngle;
	}
	SLIB_INLINE void setRotationAngle(sl_real angle)
	{
		m_rotationAngle = angle;
		m_flagUpdateTransformLocal = sl_true;
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}

	SLIB_INLINE const Rectangle& getBounds() const
	{
		return m_bounds;
	}
	SLIB_INLINE void setBounds(const Rectangle& bounds)
	{
		m_bounds = bounds;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
	}

	SLIB_INLINE const Matrix3& getTransform()
	{
		if (m_flagUpdateTransformLocal) {
			_updateLocalTransform();
		}
		return m_transformLocal;
	}
	SLIB_INLINE void setTransform(const Matrix3& m)
	{
		_setTransform(m);
		m_flagUpdateTransformLocalInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsParent = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
		onUpdateTransform();
	}

	SLIB_INLINE const Matrix3& getTransformInverse()
	{
		if (m_flagUpdateTransformLocalInverse) {
			_updateLocalTransformInverse();
		}
		return m_transformLocalInverse;
	}
	SLIB_INLINE void setTransformInverse(const Matrix3& m)
	{
		m_transformLocalInverse = m;
		m_flagUpdateTransformLocalInverse = sl_false;
		m_flagUpdateTransformWorldInverse = sl_true;
	}

	SLIB_INLINE const Matrix3& getParentTransform() const
	{
		return m_transformParent;
	}
	SLIB_INLINE void setParentTransform(const Matrix3& m)
	{
		m_transformParent = m;
		m_flagUpdateTransformParentInverse = sl_true;
		m_flagUpdateTransformWorld = sl_true;
		m_flagUpdateTransformWorldInverse = sl_true;
		m_flagUpdateBoundsWorld = sl_true;
		onUpdateTransform();
	}
	
	SLIB_INLINE const Matrix3& getParentTransformInverse()
	{
		if (m_flagUpdateTransformParentInverse) {
			_updateParentTransformInverse();
		}
		return m_transformParentInverse;
	}
	SLIB_INLINE void setParentTransformInverse(const Matrix3& m)
	{
		m_transformParentInverse = m;
		m_flagUpdateTransformParentInverse = sl_false;
		m_flagUpdateTransformWorldInverse = sl_true;
	}
	
	SLIB_INLINE const Matrix3& getWorldTransform()
	{
		if (m_flagUpdateTransformWorld) {
			_updateWorldTransform();
		}
		return m_transformWorld;
	}
	SLIB_INLINE void setWorldTransform(const Matrix3& m)
	{
		_setWorldTransform(m);
		onUpdateTransform();
	}
	
	SLIB_INLINE const Matrix3& getWorldTransformInverse()
	{
		if (m_flagUpdateTransformWorldInverse) {
			_updateWorldTransformInverse();
		}
		return m_transformWorldInverse;
	}
	SLIB_INLINE void setWorldTransformInverse(const Matrix3& m)
	{
		m_transformWorldInverse = m;
		m_flagUpdateTransformWorldInverse = sl_false;
	}
	
	SLIB_INLINE const Rectangle& getFrame()
	{
		if (m_flagUpdateBoundsParent) {
			_updateFrame();
		}
		return m_boundsParent;
	}
	SLIB_INLINE void setFrame(const Rectangle& bound)
	{
		m_boundsParent = bound;
		m_flagUpdateBoundsParent = sl_false;
	}
	
	SLIB_INLINE const Rectangle& getFrameInWorld()
	{
		if (m_flagUpdateBoundsWorld) {
			_updateFrameInWorld();
		}
		return m_boundsWorld;
	}
	SLIB_INLINE void setFrameInWorld(const Rectangle& bound)
	{
		m_boundsWorld = bound;
		m_flagUpdateBoundsWorld = sl_false;
	}

	sl_bool hitTest(const Point& ptWorld);

protected:
	void _updateLocalTransform();
	void _updateLocalTransformInverse();
	void _updateParentTransformInverse();
	void _updateWorldTransform();
	void _updateWorldTransformInverse();
	void _updateFrame();
	void _updateFrameInWorld();

	SLIB_INLINE void _setWorldTransform(const Matrix3& m)
	{
		m_transformWorld = m;
		m_flagUpdateTransformWorld = sl_false;
	}

	SLIB_INLINE void _setTransform(const Matrix3& m)
	{
		m_transformLocal = m;
		m_flagUpdateTransformLocal = sl_false;
	}
};

class SLIB_EXPORT RenderObjectGroup2D : public RenderObject2D
{
public:
	SLIB_INLINE RenderObjectGroup2D()
	{
	}

public:
	void render(RenderEngine* engine);

	void dispatchUIEvent(UIEvent* event);

protected:
	void onUpdateTransform();

	void onUIEvent(UIEvent* event)
	{
	}
	
public:
	void updateBoundsFromChildren();

private:
	List< Ref<RenderObject2D> > m_children;
	Map< String, Ref<RenderObject2D> > m_mapChildren;
	WeakRef<RenderObject2D> m_childMouseCapture;

public:
	SLIB_INLINE List< Ref<RenderObject2D> > getChildren()
	{
		return m_children.duplicate();
	}
	SLIB_INLINE Ref<RenderObject2D> getChild(String id)
	{
		return m_mapChildren.getValue(id, Ref<RenderObject2D>::null());
	}

	void addChild(RenderObject2D* object);
	SLIB_INLINE void addChild(Ref<RenderObject2D> object)
	{
		addChild(object.get());
	}
	
	void removeChild(RenderObject2D* object);
	SLIB_INLINE void removeChild(Ref<RenderObject2D> object)
	{
		removeChild(object.get());
	}

	void removeChild(String id);

	void removeAllObjects();
};

class SLIB_EXPORT RenderRectangle2D : public RenderObject2D
{
public:
	SLIB_INLINE RenderRectangle2D()
	{
		setColorForNormalState(Color::white());
		setColorForDownState(Color::white());
		setColorForHoverState(Color::white());
		setColorForDisabledState(Color::white());
	}

	SLIB_PROPERTY_INLINE(Color4f, ColorForNormalState)
	SLIB_PROPERTY_INLINE(Color4f, ColorForDownState)
	SLIB_PROPERTY_INLINE(Color4f, ColorForHoverState)
	SLIB_PROPERTY_INLINE(Color4f, ColorForDisabledState)
	SLIB_PROPERTY_INLINE(Ref<RenderProgram2D>, Program)
	SLIB_PROPERTY_INLINE(Ref<RenderProgram2D>, ProgramForDownState)
	SLIB_PROPERTY_INLINE(Ref<RenderProgram2D>, ProgramForHoverState)
	SLIB_PROPERTY_INLINE(Ref<RenderProgram2D>, ProgramForDisabledState)

	SLIB_INLINE const Color4f& getColor()
	{
		return getColorForNormalState();
	}
	void setColor(const Color4f& color);

public:
	void render(RenderEngine* engine);

protected:
	Ref<VertexBuffer> _getDefaultVertexBuffer();
	Ref<RenderProgram2D> _getDefaultRenderProgram();

	const Color4f& getColorForState();
	Ref<RenderProgram2D> getProgramForState();
};

class SLIB_EXPORT RenderImage2D : public RenderRectangle2D
{
public:
	SLIB_INLINE RenderImage2D()
	{
		setTextureRectangle(Rectangle(0, 0, 1, 1));
		setTextureRectangleForDownState(Rectangle(0, 0, 1, 1));
		setTextureRectangleForHoverState(Rectangle(0, 0, 1, 1));
		setTextureRectangleForDisabledState(Rectangle(0, 0, 1, 1));
	}

	SLIB_PROPERTY_INLINE(Ref<Texture>, Texture)
	SLIB_PROPERTY_INLINE(Rectangle, TextureRectangle)
	SLIB_PROPERTY_INLINE(Ref<Texture>, TextureForDownState)
	SLIB_PROPERTY_INLINE(Rectangle, TextureRectangleForDownState)
	SLIB_PROPERTY_INLINE(Ref<Texture>, TextureForHoverState)
	SLIB_PROPERTY_INLINE(Rectangle, TextureRectangleForHoverState)
	SLIB_PROPERTY_INLINE(Ref<Texture>, TextureForDisabledState)
	SLIB_PROPERTY_INLINE(Rectangle, TextureRectangleForDisabledState)

public:
	SLIB_INLINE void setTextureRectangle(sl_real left, sl_real top, sl_real width, sl_real height)
	{
		setTextureRectangle(Rectangle(left, top, left + width, top + height));
	}

public:
	void render(RenderEngine* engine);

protected:
	Ref<VertexBuffer> _getDefaultVertexBuffer();
	Ref<RenderProgram2D> _getDefaultRenderProgram();

	Ref<Texture> getTextureForState(Rectangle& rect);
};


SLIB_RENDER_NAMESPACE_END

#endif

