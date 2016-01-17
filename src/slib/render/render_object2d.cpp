#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/object2d.h"
#include "../../../inc/slib/math/transform2d.h"

SLIB_RENDER_NAMESPACE_BEGIN
sl_bool RenderObject2D::hitTest(const Point& ptWorld)
{
	Point pt = getWorldTransformInverse().transformPosition(ptWorld);
	return getBounds().containsPoint(pt);
}

void RenderObject2D::_updateLocalTransform()
{
	const Point& anchor = getAnchorPosition();
	const Point& pos = getPosition();
	const Size& size = getSize();
	Matrix3 r, m;
	Transform2::setTranslation(m, -anchor.x, -anchor.y);
	r = m;
	Transform2::setScaling(m, size.x, size.y);
	r *= m;
	Transform2::setRotation(m, getRotationAngle());
	r *= m;
	Transform2::setTranslation(m, anchor.x + pos.x, anchor.y + pos.y);
	r *= m;
	_setTransform(r);
}

void RenderObject2D::_updateLocalTransformInverse()
{
	const Matrix3& m = getTransform();
	setTransformInverse(m.inverse());
}

void RenderObject2D::_updateParentTransformInverse()
{
	const Matrix3& m = getParentTransform();
	setParentTransformInverse(m.inverse());
}

void RenderObject2D::_updateWorldTransform()
{
	_setWorldTransform(getTransform() * getParentTransform());
}

void RenderObject2D::_updateWorldTransformInverse()
{
	Matrix3 m = getParentTransformInverse() * getTransformInverse();
	setWorldTransformInverse(m);
}

void RenderObject2D::_updateFrame()
{
	const Matrix3& m = getTransform();
	Point pts[4];
	getBounds().getCornerPoints(pts);
	Rectangle b;
	b.setFromPoint(m.transformPosition(pts[0]));
	b.mergePoint(m.transformPosition(pts[1]));
	b.mergePoint(m.transformPosition(pts[2]));
	b.mergePoint(m.transformPosition(pts[3]));
	setFrame(b);
}

void RenderObject2D::_updateFrameInWorld()
{
	const Matrix3& m = getWorldTransform();
	Point pts[4];
	getBounds().getCornerPoints(pts);
	Rectangle b;
	b.setFromPoint(m.transformPosition(pts[0]));
	b.mergePoint(m.transformPosition(pts[1]));
	b.mergePoint(m.transformPosition(pts[2]));
	b.mergePoint(m.transformPosition(pts[3]));
	setFrameInWorld(b);
}

void RenderObjectGroup2D::addChild(RenderObject2D* object)
{
	if (object) {
		m_children.add(object);
		String name = object->getId();
		if (name.isNotEmpty()) {
			m_mapChildren.put(name, object);
		}
	}
}

void RenderObjectGroup2D::removeChild(RenderObject2D* object)
{
	if (object) {
		m_children.removeValue(object);
		String name = object->getId();
		if (name.isNotEmpty()) {
			m_mapChildren.remove(name);
		}
	}
}

void RenderObjectGroup2D::removeChild(String id)
{
	Ref<RenderObject2D> object = getChild(id);
	if (object.isNotNull()) {
		removeChild(object.get());
	}
}

void RenderObjectGroup2D::removeAllObjects()
{
	m_children.removeAll();
	m_mapChildren.removeAll();
}

void RenderObjectGroup2D::render(RenderEngine* engine)
{
	ListLocker< Ref<RenderObject2D> > list(m_children.duplicate());
	for (sl_size i = 0; i < list.count(); i++) {
		const Ref<RenderObject2D>& r = list[i];
		if (r.isNotNull() && r->isVisible()) {
			r->render(engine);
		}
	}
}

void RenderObjectGroup2D::onUpdateTransform()
{
	const Matrix3& m = getWorldTransform();
	const Matrix3& mi = getWorldTransformInverse();
	ListLocker< Ref<RenderObject2D> > list(m_children.duplicate());
	for (sl_size i = 0; i < list.count(); i++) {
		const Ref<RenderObject2D>& r = list[i];
		if (r.isNotNull()) {
			r->setParentTransform(m);
			r->setParentTransformInverse(mi);
		}
	}
}

void RenderObjectGroup2D::dispatchUIEvent(UIEvent* event)
{
	sl_bool flagAbort = sl_false;

	if (getButtonState() == buttonState_Disabled) {
		return;
	}

	Ref<RenderObject2D> c = m_childMouseCapture;
	if (c.isNotNull()) {
		ButtonState s = c->getButtonState();
		if (s == buttonState_Down || s == buttonState_Hover) {
			c->dispatchUIEvent(event);
			if (event->isStoppedPropagation()) {
				flagAbort = sl_true;
			}
			s = c->getButtonState();
			if (flagAbort || (s != buttonState_Down && s != buttonState_Hover)) {
				setButtonState(buttonState_Normal);
				m_childMouseCapture.setNull();
			}
			return;
		}
	}
	setButtonState(buttonState_Normal);
	m_childMouseCapture.setNull();
	
	Point ptWorld = event->getPoint();
	Point ptGroup = getWorldTransformInverse().transformPosition(ptWorld);
	
	ListLocker< Ref<RenderObject2D> > list(m_children.duplicate());
	for (sl_size i = list.count(); i > 0; i--) {
		const Ref<RenderObject2D>& r = list[i - 1];
		if (r.isNotNull() && r->isVisible() && r->isHitTestable()) {
			if (r->getFrame().containsPoint(ptGroup)) {
				if (r->hitTest(ptWorld)) {
					r->dispatchUIEvent(event);
					if (event->isStoppedPropagation()) {
						flagAbort = sl_true;
					}
					sl_bool flagCapture = sl_false;
					ButtonState s = r->getButtonState();
					if (flagAbort || (s != buttonState_Down && s != buttonState_Hover)) {
						s = buttonState_Normal;
					} else {
						m_childMouseCapture = r;
						flagCapture = sl_true;
					}
					setButtonState(s);
					if (flagAbort) {
						return;
					}
					if (flagCapture) {
						break;
					}
				}
			}
		}
	}
	if (event->isStoppedPropagation()) {
		return;
	}
	RenderObject::dispatchUIEvent(event);
}

void RenderObjectGroup2D::updateBoundsFromChildren()
{
	Rectangle bounds;
	ListLocker< Ref<RenderObject2D> > list(m_children.duplicate());
	if (list.count() > 0) {
		for (sl_size i = 0; i < list.count(); i++) {
			const Ref<RenderObject2D>& r = list[i];
			if (r.isNotNull() && r->isVisible()) {
				if (i == 0) {
					bounds = r->getFrame();
				} else {
					bounds.mergeRectangle(r->getFrame());
				}
			}
		}
	} else {
		bounds.setZero();
	}
	setBounds(bounds);
}

void RenderRectangle2D::setColor(const Color4f& color)
{
	setColorForNormalState(color);
	setColorForDownState(color);
	setColorForHoverState(color);
	setColorForDisabledState(color);
}

const Color4f& RenderRectangle2D::getColorForState()
{
	ButtonState state = getButtonState();
	if (state == buttonState_Normal) {
		return getColorForNormalState();
	} else if (state == buttonState_Down) {
		return getColorForDownState();
	} else if (state == buttonState_Hover) {
		return getColorForHoverState();
	} else if (state == buttonState_Disabled) {
		return getColorForDisabledState();
	} else {
		return getColorForNormalState();
	}
}

Ref<RenderProgram2D> RenderRectangle2D::getProgramForState()
{
	Ref<RenderProgram2D> ret;
	ButtonState state = getButtonState();
	if (state == buttonState_Normal) {
		ret = getProgram();
		return ret;
	} else if (state == buttonState_Down) {
		ret = getProgramForDownState();
	} else if (state == buttonState_Hover) {
		ret = getProgramForHoverState();
	} else if (state == buttonState_Disabled) {
		ret = getProgramForDisabledState();
	}
	if (ret.isNull()) {
		ret = getProgram();
	}
	return ret;
}

void RenderRectangle2D::render(RenderEngine* engine)
{
	Ref<RenderProgram2D> program = getProgramForState();
	if (program.isNull()) {
		program = _getDefaultRenderProgram();
		if (program.isNull()) {
			return;
		}
	}
	const Matrix3& m = getWorldTransform();
	const Color4f& c = getColorForState();
	program->setColor(c.xyz());
	program->setAlpha(c.w);
	program->setTransform(m);
	engine->draw(program.get(), 4, _getDefaultVertexBuffer(), primitiveType_TriangleStrip);
}

Ref<VertexBuffer> RenderRectangle2D::_getDefaultVertexBuffer()
{
	static RenderProgram2D_Position::VertexData v[] = {
		{ { -0.5f, -0.5f } }
		, { { 0.5f, -0.5f } }
		, { { -0.5f, 0.5f } }
		, { { 0.5f, 0.5f } }
	};
	SLIB_SAFE_STATIC(Ref<VertexBuffer>, ret, VertexBuffer::create(v, sizeof(v)));
	return ret;
}

Ref<RenderProgram2D> RenderRectangle2D::_getDefaultRenderProgram()
{
	SLIB_SAFE_STATIC(Ref<RenderProgram2D>, ret, new RenderProgram2D_Position);
	return ret;
}

Ref<Texture> RenderImage2D::getTextureForState(Rectangle& rect)
{
	Ref<Texture> ret;
	ButtonState state = getButtonState();
	if (state == buttonState_Normal) {
		ret = getTexture();
		if (ret.isNotNull()) {
			rect = getTextureRectangle();
		}
		return ret;
	} else if (state == buttonState_Down) {
		ret = getTextureForDownState();
		if (ret.isNotNull()) {
			rect = getTextureRectangleForDownState();
		}
	} else if (state == buttonState_Hover) {
		ret = getTextureForHoverState();
		if (ret.isNotNull()) {
			rect = getTextureRectangleForHoverState();
		}
	} else if (state == buttonState_Disabled) {
		ret = getTextureForDisabledState();
		if (ret.isNotNull()) {
			rect = getTextureRectangleForDisabledState();
		}
	}
	if (ret.isNull()) {
		ret = getTexture();
		rect = getTextureRectangle();
	}
	return ret;
}


static void _RenderImage2D_makeTextureTransform2D(Matrix3& mat, const Rectangle& rt)
{
	sl_real x = rt.left;
	sl_real y = rt.top;
	sl_real w = rt.right - rt.left;
	sl_real h = rt.bottom - rt.top;
	mat.m00 = w; mat.m01 = 0; mat.m02 = 0;
	mat.m10 = 0; mat.m11 = h; mat.m12 = 0;
	mat.m20 = x; mat.m21 = y; mat.m22 = 1;
}

void RenderImage2D::render(RenderEngine* engine)
{
	Rectangle rectangleTexture;
	const Ref<Texture>& texture = getTextureForState(rectangleTexture);
	if (texture.isNotNull()) {
		Ref<RenderProgram2D> program = getProgramForState();
		if (program.isNull()) {
			program = _getDefaultRenderProgram();
			if (program.isNull()) {
				return;
			}
		}
		const Matrix3& m = getWorldTransform();
		Matrix3 mt;
		_RenderImage2D_makeTextureTransform2D(mt, rectangleTexture);
		const Color4f& c = getColorForState();
		program->setColor(c.xyz());
		program->setAlpha(c.w);
		program->setTexture(texture);
		program->setTextureTransform(mt);
		program->setTransform(m);
		engine->draw(program.get(), 4, _getDefaultVertexBuffer(), primitiveType_TriangleStrip);
	}
}

Ref<VertexBuffer> RenderImage2D::_getDefaultVertexBuffer()
{
	static RenderProgram2D_PositionTexture::VertexData v[] = {
		{ { -0.5f, -0.5f }, { 0, 0 } }
		, { { 0.5f, -0.5f }, { 1, 0 } }
		, { { -0.5f, 0.5f }, { 0, 1 } }
		, { { 0.5f, 0.5f }, { 1, 1 } }
	};
	SLIB_SAFE_STATIC(Ref<VertexBuffer>, ret, VertexBuffer::create(v, sizeof(v)));
	return ret;
}

Ref<RenderProgram2D> RenderImage2D::_getDefaultRenderProgram()
{
	SLIB_SAFE_STATIC(Ref<RenderProgram2D>, ret, new RenderProgram2D_PositionTexture);
	return ret;
}

SLIB_RENDER_NAMESPACE_END
