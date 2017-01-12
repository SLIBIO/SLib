#include "../../../inc/slib/core/xml.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"

SLIB_NAMESPACE_BEGIN

void IXmlParseListener::onStartDocument(XmlParseControl* control, XmlDocument* document)
{
}

void IXmlParseListener::onEndDocument(XmlParseControl* control, XmlDocument* document)
{
}

void IXmlParseListener::onStartElement(XmlParseControl* control, XmlElement* element)
{
}

void IXmlParseListener::onEndElement(XmlParseControl* control, XmlElement* element)
{
}

void IXmlParseListener::onText(XmlParseControl* control, const String& text)
{
}

void IXmlParseListener::onCDATA(XmlParseControl* control, const String& text)
{
}

void IXmlParseListener::onProcessingInstruction(XmlParseControl* control, const String& target, const String& content)
{
}

void IXmlParseListener::onComment(XmlParseControl* control, const String& content)
{
}

void IXmlParseListener::onStartPrefixMapping(XmlParseControl* control, const String& prefix, const String& uri)
{
}

void IXmlParseListener::onEndPrefixMapping(XmlParseControl* control, const String& prefix)
{
}


/************************************************
			XmlNode
************************************************/

SLIB_DEFINE_ROOT_OBJECT(XmlNode)

XmlNode::XmlNode(XmlNodeType type) : m_type(type), m_positionStartInSource(0), m_positionEndInSource(0), m_lineInSource(1), m_columnInSource(1)
{
}

XmlNodeType XmlNode::getType() const
{
	return m_type;
}

String XmlNode::getText() const
{
	StringBuffer buf;
	if (buildText(buf)) {
		return buf.merge();
	}
	return sl_null;
}

String XmlNode::toString() const
{
	StringBuffer buf;
	if (buildXml(buf)) {
		return buf.merge();
	}
	return sl_null;
}

sl_bool XmlNode::isDocumentNode() const
{
	return m_type == XmlNodeType::Document;
}

Ref<XmlDocument> XmlNode::toDocumentNode() const
{
	if (m_type == XmlNodeType::Document) {
		return (XmlDocument*)this;
	}
	return sl_null;
}

sl_bool XmlNode::isElementNode() const
{
	return m_type == XmlNodeType::Element;
}

Ref<XmlElement> XmlNode::toElementNode() const
{
	if (m_type == XmlNodeType::Element) {
		return (XmlElement*)this;
	}
	return sl_null;
}

sl_bool XmlNode::isTextNode() const
{
	return m_type == XmlNodeType::Text;
}

Ref<XmlText> XmlNode::toTextNode() const
{
	if (m_type == XmlNodeType::Text) {
		return (XmlText*)this;
	}
	return sl_null;
}

sl_bool XmlNode::isProcessingInstructionNode() const
{
	return m_type == XmlNodeType::ProcessingInstruction;
}

Ref<XmlProcessingInstruction> XmlNode::toProcessingInstructionNode() const
{
	if (m_type == XmlNodeType::ProcessingInstruction) {
		return (XmlProcessingInstruction*)this;
	}
	return sl_null;
}

sl_bool XmlNode::isCommentNode() const
{
	return m_type == XmlNodeType::Comment;
}

Ref<XmlComment> XmlNode::toCommentNode() const
{
	if (m_type == XmlNodeType::ProcessingInstruction) {
		return (XmlComment*)this;
	}
	return sl_null;
}

Ref<XmlDocument> XmlNode::getDocument() const
{
	if (m_type == XmlNodeType::Document) {
		return (XmlDocument*)(this);
	} else {
		return m_document;
	}
	return sl_null;
}

Ref<XmlElement> XmlNode::getRoot() const
{
	if (m_type == XmlNodeType::Document) {
		XmlDocument* doc = (XmlDocument*)this;
		return doc->getFirstChildElement();
	} else {
		Ref<XmlDocument> doc = m_document;
		if (doc.isNotNull()) {
			return doc->getFirstChildElement();
		}
	}
	return sl_null;
}

Ref<XmlNodeGroup> XmlNode::getParent() const
{
	return m_parent;
}

Ref<XmlElement> XmlNode::getParentElement() const
{
	Ref<XmlNodeGroup> parent = m_parent;
	return parent->toElementNode();
}

String XmlNode::getSourceFilePath() const
{
	return m_sourceFilePath;
}

void XmlNode::setSourceFilePath(const String& path)
{
	m_sourceFilePath = path;
}

sl_size XmlNode::getStartPositionInSource() const
{
	return m_positionStartInSource;
}

void XmlNode::setStartPositionInSource(sl_size pos)
{
	m_positionStartInSource = pos;
}

sl_size XmlNode::getEndPositionInSource() const
{
	return m_positionEndInSource;
}

void XmlNode::setEndPositionInSource(sl_size pos)
{
	m_positionEndInSource = pos;
}

sl_size XmlNode::getLineNumberInSource() const
{
	return m_lineInSource;
}

void XmlNode::setLineNumberInSource(sl_size line)
{
	m_lineInSource = line;
}

sl_size XmlNode::getColumnNumberInSource() const
{
	return m_columnInSource;
}

void XmlNode::setColumnNumberInSource(sl_size line)
{
	m_columnInSource = line;
}

void XmlNode::_setDocument(const Ref<XmlDocument>& documentNew)
{
	if (m_type == XmlNodeType::Document) {
		return;
	}
	if (m_type == XmlNodeType::Element) {
		XmlElement* e = (XmlElement*)this;
		m_document = documentNew;
		ListLocker< Ref<XmlNode> > nodes(e->m_children);
		for (sl_size i = 0; i < nodes.count; i++) {
			nodes[i]->_setDocument(documentNew);
		}
	} else {
		m_document = documentNew;
	}
}

/************************************************
				XmlNodeGroup
************************************************/

SLIB_DEFINE_OBJECT(XmlNodeGroup, XmlNode)

XmlNodeGroup::XmlNodeGroup(XmlNodeType type) : XmlNode(type)
{
}

sl_bool XmlNodeGroup::buildText(StringBuffer& output) const
{
	ListLocker< Ref<XmlNode> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		if (children[i].isNotNull()) {
			if (!(children[i]->buildText(output))) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

sl_bool XmlNodeGroup::buildInnerXml(StringBuffer& output) const
{
	ListLocker< Ref<XmlNode> > children(m_children);
	for (sl_size i = 0; i < children.count; i++) {
		if (children[i].isNotNull()) {
			if (!(children[i]->buildXml(output))) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

String XmlNodeGroup::getInnerXml() const
{
	StringBuffer buf;
	if (buildInnerXml(buf)) {
		return buf.merge();
	}
	return sl_null;
}

sl_size XmlNodeGroup::getChildrenCount() const
{
	return m_children.getCount();
}

Ref<XmlNode> XmlNodeGroup::getChild(sl_size index) const
{
	return m_children.getValueAt(index);
}

sl_bool XmlNodeGroup::addChild(const Ref<XmlNode>& node)
{
	if (node.isNotNull() && node->getType() != XmlNodeType::Document) {
		node->m_parent = this;
		node->_setDocument(getDocument());
		return m_children.add(node);
	}
	return sl_false;
}

sl_bool XmlNodeGroup::removeChild(const Ref<XmlNode>& node, sl_bool flagUnregisterDocument)
{
	if (node.isNotNull()) {
		ObjectLocker lock(&m_children);
		sl_reg index = m_children.indexOf_NoLock(node);
		if (index >= 0) {
			if (flagUnregisterDocument) {
				node->_setDocument(Ref<XmlDocument>::null());
			}
			node->m_parent.setNull();
			return m_children.removeAt_NoLock(index);
		}
	}
	return sl_false;
}

void XmlNodeGroup::removeAllChildren(sl_bool flagUnregisterDocument)
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		if (flagUnregisterDocument) {
			nodes[i]->_setDocument(Ref<XmlDocument>::null());
		}
		nodes[i]->m_parent.setNull();
	}
	m_children.removeAll_NoLock();
}

String XmlNodeGroup::getChildText(sl_size index) const
{
	Ref<XmlNode> node = m_children.getValueAt(index);
	if (node.isNotNull()) {
		return node->getText();
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::getChildElement(sl_size index) const
{
	Ref<XmlNode> node = m_children.getValueAt(index);
	if (node.isNotNull()) {
		return node->toElementNode();
	}
	return sl_null;
}

List< Ref<XmlElement> > XmlNodeGroup::getChildElements() const
{
	List< Ref<XmlElement> > ret;
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			ret.add(e);
		}
	}
	return ret;
}

sl_size XmlNodeGroup::getChildElementsCount() const
{
	sl_size n = 0;
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			n++;
		}
	}
	return n;
}

List< Ref<XmlElement> > XmlNodeGroup::getChildElements(const String& tagName) const
{
	if (tagName.isEmpty()) {
		return getChildElements();
	}
	List< Ref<XmlElement> > ret;
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_name == tagName) {
				ret.add(e);
			}
		}
	}
	return ret;
}

List< Ref<XmlElement> > XmlNodeGroup::getChildElements(const String& uri, const String& localName) const
{
	List< Ref<XmlElement> > ret;
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_uri == uri && e->m_localName == localName) {
				ret.add(e);
			}
		}
	}
	return ret;
}

Ref<XmlElement> XmlNodeGroup::getFirstChildElement() const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			return e;
		}
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::getFirstChildElement(const String& tagName) const
{
	if (tagName.isEmpty()) {
		return getFirstChildElement();
	}
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_name == tagName) {
				return e;
			}
		}
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::getFirstChildElement(const String& uri, const String& localName) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_uri == uri && e->m_localName == localName) {
				return e;
			}
		}
	}
	return sl_null;
}

String XmlNodeGroup::getFirstChildElementText() const
{
	Ref<XmlElement> e = getFirstChildElement();
	if (e.isNotNull()) {
		return e->getText();
	}
	return sl_null;
}

String XmlNodeGroup::getFirstChildElementText(const String& tagName) const
{
	if (tagName.isEmpty()) {
		return getFirstChildElementText();
	}
	Ref<XmlElement> e = getFirstChildElement(tagName);
	if (e.isNotNull()) {
		return e->getText();
	}
	return sl_null;
}

String XmlNodeGroup::getFirstChildElementText(const String& uri, const String& localName) const
{
	Ref<XmlElement> e = getFirstChildElement(uri, localName);
	if (e.isNotNull()) {
		return e->getText();
	}
	return sl_null;
}

List< Ref<XmlElement> > XmlNodeGroup::getDescendantElements(const String& tagName) const
{
	List< Ref<XmlElement> > ret;
	getDescendantElements(tagName, ret);
	return ret;
}

void XmlNodeGroup::getDescendantElements(const String& tagName, List< Ref<XmlElement> >& list) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_name == tagName) {
				list.add(e);
			}
			e->getDescendantElements(tagName, list);
		}
	}
}

List< Ref<XmlElement> > XmlNodeGroup::getDescendantElements(const String& uri, const String& localName) const
{
	List< Ref<XmlElement> > ret;
	getDescendantElements(uri, localName, ret);
	return ret;
}

void XmlNodeGroup::getDescendantElements(const String& uri, const String& localName, List< Ref<XmlElement> >& list) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_uri == uri && e->m_localName == localName) {
				list.add(e);
			}
			e->getDescendantElements(uri, localName, list);
		}
	}
}

Ref<XmlElement> XmlNodeGroup::getFirstDescendantElement(const String& tagName) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_name == tagName) {
				return e;
			} else {
				e = e->getFirstDescendantElement(tagName);
				if (e.isNotNull()) {
					return e;
				}
			}
		}
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::getFirstDescendantElement(const String& uri, const String& localName) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			if (e->m_uri == uri && e->m_localName == localName) {
				return e;
			} else {
				e = e->getFirstDescendantElement(uri, localName);
				if (e.isNotNull()) {
					return e;
				}
			}
		}
	}
	return sl_null;
}

String XmlNodeGroup::getFirstDescendantElementText(const String& tagName) const
{
	Ref<XmlElement> e = getFirstDescendantElement(tagName);
	if (e.isNotNull()) {
		return e->getText();
	}
	return sl_null;
}

String XmlNodeGroup::getFirstDescendantElementText(const String& uri, const String& localName) const
{
	Ref<XmlElement> e = getFirstDescendantElement(uri, localName);
	if (e.isNotNull()) {
		return e->getText();
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::findChildElementByAttribute(const String& attrName, const String& attrValue) const
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			String value = e->getAttribute(attrName);
			if (value == attrValue) {
				return e;
			}
			Ref<XmlElement> found = e->findChildElementByAttribute(attrName, attrValue);
			if (found.isNotNull()) {
				return found;
			}
		}
	}
	return sl_null;
}

Ref<XmlElement> XmlNodeGroup::findChildElementById(const String& _id) const
{
	SLIB_STATIC_STRING_BY_ARRAY(name, 'i', 'd');
	return findChildElementByAttribute(name, _id);
}

/************************************************
				XmlElement
************************************************/

SLIB_DEFINE_OBJECT(XmlElement, XmlNodeGroup)

XmlElement::XmlElement() : XmlNodeGroup(XmlNodeType::Element)
{
}

Ref<XmlElement> XmlElement::create(const String& name)
{
	if (Xml::checkName(name)) {
		Ref<XmlElement> ret = new XmlElement;
		if (ret.isNotNull()) {
			ret->m_name = name;
			return ret;
		}
	}
	return sl_null;
}

Ref<XmlElement> XmlElement::create(const String& name, const String& uri, const String& localName)
{
	if (Xml::checkName(name)) {
		Ref<XmlElement> ret = new XmlElement;
		if (ret.isNotNull()) {
			ret->m_name = name;
			ret->m_uri = uri;
			ret->m_localName = localName;
			return ret;
		}
	}
	return sl_null;
}

sl_bool XmlElement::buildXml(StringBuffer& output) const
{
	String name = m_name;
	if (name.isEmpty()) {
		return sl_false;
	}
	if (!(output.addStatic("<", 1))) {
		return sl_false;
	}
	if (!(output.add(name))) {
		return sl_false;
	}
	// attributes
	{
		MutexLocker lock(&m_lockAttributes);
		ListElements<XmlAttribute> attrs(m_attributes);
		for (sl_size i = 0; i < attrs.count; i++) {
			if (attrs[i].whiteSpacesBeforeName.isEmpty()) {
				if (!(output.addStatic(" ", 1))) {
					return sl_false;
				}
			} else {
				if (!(output.add(attrs[i].whiteSpacesBeforeName))) {
					return sl_false;
				}
			}
			if (!(output.add(attrs[i].name))) {
				return sl_false;
			}
			if (!(output.addStatic("=\"", 2))) {
				return sl_false;
			}
			if (!(Xml::buildEscapedText(attrs[i].value, output))) {
				return sl_false;
			}
			if (!(output.addStatic("\"", 1))) {
				return sl_false;
			}
		}
	}
	// children
	{
		ObjectLocker lock(&m_children);
		if (m_children.getCount() == 0) {
			if (!(output.addStatic(" />", 3))) {
				return sl_false;
			}
		} else {
			if (!(output.addStatic(">", 1))) {
				return sl_false;
			}
			if (!(buildInnerXml(output))) {
				return sl_false;
			}
			if (!(output.addStatic("</", 2))) {
				return sl_false;
			}
			if (!(output.add(name))) {
				return sl_false;
			}
			if (!(output.addStatic(">", 1))) {
				return sl_false;
			}
		}
	}
	return sl_true;
}

String XmlElement::getName() const
{
	return m_name;
}

String XmlElement::getUri() const
{
	return m_uri;
}

String XmlElement::getLocalName() const
{
	return m_localName;
}

sl_bool XmlElement::setName(const String& name)
{
	if (Xml::checkName(name)) {
		m_name = name;
		return sl_true;
	}
	return sl_false;
}

sl_bool XmlElement::setName(const String& name, const String& uri, const String& localName)
{
	if (Xml::checkName(name)) {
		m_name = name;
		m_uri = uri;
		m_localName = localName;
		return sl_true;
	}
	return sl_false;
}

sl_size XmlElement::getAttributesCount() const
{
	return m_attributes.getCount();
}

sl_bool XmlElement::getAttribute(sl_size index, XmlAttribute* _out) const
{
	MutexLocker lock(&m_lockAttributes);
	return m_attributes.getAt_NoLock(index, _out);
}

String XmlElement::getAttribute(const String& name) const
{
	MutexLocker lock(&m_lockAttributes);
	return m_mapAttributes.getValue_NoLock(name, String::null());
}

String XmlElement::getAttribute(const String& uri, const String& localName) const
{
	MutexLocker lock(&m_lockAttributes);
	ListElements<XmlAttribute> attrs(m_attributes);
	for (sl_size i = 0; i < attrs.count; i++) {
		if (attrs[i].uri == uri && attrs[i].localName == localName) {
			return attrs[i].value;
		}
	}
	return sl_null;
}

sl_bool XmlElement::containsAttribute(const String& name) const
{
	MutexLocker lock(&m_lockAttributes);
	return m_mapAttributes.contains_NoLock(name);
}

sl_bool XmlElement::setAttribute(sl_size index, const String& value)
{
	MutexLocker lock(&m_lockAttributes);
	XmlAttribute attr;
	if (m_attributes.getAt_NoLock(index, &attr)) {
		m_mapAttributes.put(attr.name, value);
		attr.value = value;
		return m_attributes.setAt_NoLock(index, attr);
	}
	return sl_false;
}

sl_bool XmlElement::setAttribute(sl_size index, const String& uri, const String& localName, const String& value)
{
	MutexLocker lock(&m_lockAttributes);
	XmlAttribute attr;
	if (m_attributes.getAt_NoLock(index, &attr)) {
		m_mapAttributes.put(attr.name, value);
		attr.uri = uri;
		attr.localName = localName;
		attr.value = value;
		return m_attributes.setAt_NoLock(index, attr);
	}
	return sl_false;
}

sl_bool XmlElement::setAttribute(const String& name, const String& value)
{
	if (!(Xml::checkName(name))) {
		return sl_false;
	}
	MutexLocker lock(&m_lockAttributes);
	if (m_mapAttributes.contains_NoLock(name)) {
		m_mapAttributes.put_NoLock(name, value);
		ListElements<XmlAttribute> attrs(m_attributes);
		for (sl_size i = 0; i < attrs.count; i++) {
			if (attrs[i].name == name) {
				attrs[i].value = value;
				return sl_true;
			}
		}
	} else {
		m_mapAttributes.put_NoLock(name, value);
		XmlAttribute attr;
		attr.name = name;
		attr.value = value;
		return m_attributes.add_NoLock(attr);
	}
	return sl_false;
}

sl_bool XmlElement::setAttribute(const XmlAttribute& attr)
{
	if (!(Xml::checkName(attr.name))) {
		return sl_false;
	}
	MutexLocker lock(&m_lockAttributes);
	if (m_mapAttributes.contains_NoLock(attr.name)) {
		m_mapAttributes.put_NoLock(attr.name, attr.value);
		ListElements<XmlAttribute> attrs(m_attributes);
		for (sl_size i = 0; i < attrs.count; i++) {
			if (attrs[i].name == attr.name) {
				attrs[i] = attr;
				return sl_true;
			}
		}
	} else {
		m_mapAttributes.put_NoLock(attr.name, attr.value);
		return m_attributes.add_NoLock(attr);
	}
	return sl_false;
}

sl_bool XmlElement::setAttribute(const String& uri, const String& localName, const String& value)
{
	MutexLocker lock(&m_lockAttributes);
	ListElements<XmlAttribute> attrs(m_attributes);
	for (sl_size i = 0; i < attrs.count; i++) {
		if (attrs[i].uri == uri && attrs[i].localName == localName) {
			attrs[i].value = value;
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool XmlElement::removeAttribute(sl_size index)
{
	MutexLocker lock(&m_lockAttributes);
	XmlAttribute attr;
	if (m_attributes.getAt_NoLock(index, &attr)) {
		m_mapAttributes.remove_NoLock(attr.name);
		return m_attributes.removeAt_NoLock(index);
	}
	return sl_false;
}

sl_bool XmlElement::removeAttribute(const String& name)
{
	MutexLocker lock(&m_lockAttributes);
	if (m_mapAttributes.contains_NoLock(name)) {
		m_mapAttributes.remove_NoLock(name);
		ListElements<XmlAttribute> attrs(m_attributes);
		for (sl_size i = 0; i < attrs.count; i++) {
			if (attrs[i].name == name) {
				return m_attributes.removeAt_NoLock(i);
			}
		}
	}
	return sl_false;
}

void XmlElement::removeAllAttributes()
{
	MutexLocker lock(&m_lockAttributes);
	m_attributes.removeAll_NoLock();
	m_mapAttributes.removeAll_NoLock();
}

/************************************************
				XmlDocument
************************************************/

SLIB_DEFINE_OBJECT(XmlDocument, XmlNodeGroup)

XmlDocument::XmlDocument() : XmlNodeGroup(XmlNodeType::Document)
{
}

Ref<XmlDocument> XmlDocument::create()
{
	return new XmlDocument;
}

sl_bool XmlDocument::buildXml(StringBuffer& output) const
{
	return buildInnerXml(output);
}

Ref<XmlElement> XmlDocument::getElementById(const String& _id) const
{
	return m_elementsById.getValue(_id, Ref<XmlElement>::null());
}

void XmlDocument::registerElementsById(const String& idAttributeName)
{
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		Ref<XmlElement> e = nodes[i]->toElementNode();
		if (e.isNotNull()) {
			registerElementsById(e, idAttributeName);
		}
	}
}

void XmlDocument::registerElementsById(const Ref<XmlElement>& element, const String& idAttributeName)
{
	if (element.isNotNull()) {
		String value = element->getAttribute(idAttributeName);
		if (value.isNotEmpty()) {
			m_elementsById.put(value, element);
		}
		ListLocker< Ref<XmlNode> > nodes(element->m_children);
		for (sl_size i = 0; i < nodes.count; i++) {
			Ref<XmlElement> e = nodes[i]->toElementNode();
			if (e.isNotNull()) {
				registerElementsById(e, idAttributeName);
			}
		}
	}
}

sl_bool XmlDocument::checkWellFormed() const
{
	sl_bool flagFoundRoot = sl_false;
	ListLocker< Ref<XmlNode> > nodes(m_children);
	for (sl_size i = 0; i < nodes.count; i++) {
		XmlNodeType type = nodes[i]->getType();
		if (type == XmlNodeType::Element) {
			if (flagFoundRoot) {
				return sl_false;
			}
			flagFoundRoot = sl_true;
		} else if (type == XmlNodeType::Text) {
			return sl_false;
		}
	}
	return flagFoundRoot;
}

/************************************************
				XmlText
************************************************/

SLIB_DEFINE_OBJECT(XmlText, XmlNode)

XmlText::XmlText() : XmlNode(XmlNodeType::Text)
{
}

Ref<XmlText> XmlText::create(const String& text, sl_bool flagCDATA)
{
	Ref<XmlText> ret = new XmlText;
	if (ret.isNotNull()) {
		ret->m_text = text;
		ret->m_flagCDATA = flagCDATA;
	}
	return ret;
}

Ref<XmlText> XmlText::createCDATA(const String& text)
{
	return create(text, sl_true);
}

sl_bool XmlText::buildText(StringBuffer& output) const
{
	return output.add(m_text);
}

sl_bool XmlText::buildXml(StringBuffer& output) const
{
	String text = m_text;
	if (text.isEmpty()) {
		return sl_true;
	}
	if (m_flagCDATA) {
		if (!(output.addStatic("<![CDATA[", 9))) {
			return sl_false;
		}
		StringData data;
		data.str8 = text;
		sl_char8* sz = text.getData();
		sl_size len = text.getLength();
		sl_size start = 0;
		for (sl_size i = 0; i < len - 2; i++) {
			if (sz[i] == ']' && sz[i+1] == ']' && sz[i+2] == '>') {
				if (i > start) {
					data.sz8 = sz + start;
					data.len = i - start;
					if (!(output.add(data))) {
						return sl_false;
					}
					start = i + 3;
					i = start - 1;
				}
				if (!(output.addStatic("]]]]><![CDATA[>", 15))) {
					return sl_false;
				}
			}
		}
		if (len > start) {
			data.sz8 = sz + start;
			data.len = len - start;
			if (!(output.add(data))) {
				return sl_false;
			}
		}
		if (!(output.addStatic("]]>", 3))) {
			return sl_false;
		}
		return sl_true;
	} else {
		return Xml::buildEscapedText(text, output);
	}
}

String XmlText::getText() const
{
	return m_text;
}

void XmlText::setText(const String& text)
{
	m_text = text;
}

sl_bool XmlText::isCDATA() const
{
	return m_flagCDATA;
}

void XmlText::setCDATA(sl_bool flag)
{
	m_flagCDATA = flag;
}


/************************************************
			XmlProcessingInstruction
************************************************/

SLIB_DEFINE_OBJECT(XmlProcessingInstruction, XmlNode)

XmlProcessingInstruction::XmlProcessingInstruction() : XmlNode(XmlNodeType::ProcessingInstruction)
{
}

Ref<XmlProcessingInstruction> XmlProcessingInstruction::create(const String& target, const String& content)
{
	if (Xml::checkName(target)) {
		Ref<XmlProcessingInstruction> ret = new XmlProcessingInstruction;
		if (ret.isNotNull()) {
			ret->m_target = target;
			ret->m_content = content;
			return ret;
		}
	}
	return sl_null;
}

sl_bool XmlProcessingInstruction::buildText(StringBuffer& output) const
{
	return sl_true;
}

sl_bool XmlProcessingInstruction::buildXml(StringBuffer& output) const
{
	String target = m_target;
	if (target.isEmpty()) {
		return sl_false;
	}
	if (!(output.addStatic("<?", 2))) {
		return sl_false;
	}
	if (!(output.add(target))) {
		return sl_false;
	}
	if (!(output.addStatic(" ", 1))) {
		return sl_false;
	}
	// content
	{
		String content = m_content;
		StringData data;
		data.str8 = content;
		sl_char8* sz = content.getData();
		sl_size len = content.getLength();
		sl_size start = 0;
		for (sl_size i = 0; i < len - 1; i++) {
			if (sz[i] == '?' && sz[i+1] == '>') {
				if (i > start) {
					data.sz8 = sz + start;
					data.len = i - start;
					if (!(output.add(data))) {
						return sl_false;
					}
					start = i + 2;
					i = start - 1;
				}
			}
		}
		if (len > start) {
			data.sz8 = sz + start;
			data.len = len - start;
			if (!(output.add(data))) {
				return sl_false;
			}
		}
	}
	if (!(output.addStatic("?>", 2))) {
		return sl_false;
	}
	return sl_true;
}

String XmlProcessingInstruction::getTarget() const
{
	return m_target;
}

sl_bool XmlProcessingInstruction::setTarget(const String& target)
{
	if (Xml::checkName(target)) {
		m_target = target;
		return sl_true;
	}
	return sl_false;
}

String XmlProcessingInstruction::getContent() const
{
	return m_content;
}

void XmlProcessingInstruction::setContent(const String& content)
{
	m_content = content;
}


/************************************************
				XmlComment
************************************************/

SLIB_DEFINE_OBJECT(XmlComment, XmlNode)

XmlComment::XmlComment() : XmlNode(XmlNodeType::Comment)
{
}

Ref<XmlComment> XmlComment::create(const String& comment)
{
	Ref<XmlComment> ret = new XmlComment;
	if (ret.isNotNull()) {
		ret->m_comment = comment;
	}
	return ret;
}

sl_bool XmlComment::buildText(StringBuffer& output) const
{
	return sl_true;
}

sl_bool XmlComment::buildXml(StringBuffer& output) const
{
	String comment = m_comment;
	if (comment.isEmpty()) {
		return sl_true;
	}
	if (!(output.addStatic("<!--", 4))) {
		return sl_false;
	}
	// comment
	{
		StringData data;
		data.str8 = comment;
		sl_char8* sz = comment.getData();
		sl_size len = comment.getLength();
		sl_size start = 0;
		for (sl_size i = 0; i < len - 1; i++) {
			if (sz[i] == '-' && sz[i+1] == '-') {
				if (i > start) {
					data.sz8 = sz + start;
					data.len = i - start;
					if (!(output.add(data))) {
						return sl_false;
					}
					start = i + 2;
					i = start - 1;
				}
			}
		}
		if (len > start) {
			data.sz8 = sz + start;
			data.len = len - start;
			if (!(output.add(data))) {
				return sl_false;
			}
		}
	}
	if (!(output.addStatic("-->", 3))) {
		return sl_false;
	}
	return sl_true;
}

String XmlComment::getComment() const
{
	return m_comment;
}

void XmlComment::setComment(const String& comment)
{
	m_comment = comment;
}

/************************************************
				XmlWhiteSpace
************************************************/

SLIB_DEFINE_OBJECT(XmlWhiteSpace, XmlNode)

XmlWhiteSpace::XmlWhiteSpace() : XmlNode(XmlNodeType::WhiteSpace)
{
}

Ref<XmlWhiteSpace> XmlWhiteSpace::create(const String& content)
{
	Ref<XmlWhiteSpace> ret = new XmlWhiteSpace;
	if (ret.isNotNull()) {
		ret->m_content = content;
	}
	return ret;
}

sl_bool XmlWhiteSpace::buildText(StringBuffer& output) const
{
	return sl_true;
}

sl_bool XmlWhiteSpace::buildXml(StringBuffer& output) const
{
	if (!(output.add(m_content))) {
		return sl_false;
	}
	return sl_true;
}

String XmlWhiteSpace::getContent() const
{
	return m_content;
}

void XmlWhiteSpace::setContent(const String& content)
{
	m_content = content;
}

/************************************************
				Xml Parsing
************************************************/

XmlParseParam::XmlParseParam()
{
	flagCreateDocument = sl_true;
	flagCreateCommentNodes = sl_false;
	flagCreateProcessingInstructionNodes = sl_true;
	flagCreateTextNodes = sl_true;
	flagCreateWhiteSpaces = sl_false;
	
	flagProcessNamespaces = sl_true;
	flagCheckWellFormed = sl_true;
	
	flagLogError = sl_true;
	
	flagError = sl_false;
	errorLine = 0;
	errorColumn = 0;
	errorPosition = 0;
}

String XmlParseParam::getErrorText()
{
	if (flagError) {
		return "(" + String::fromSize(errorLine) + ":" + String::fromSize(errorColumn) + ") " + errorMessage;
	}
	return sl_null;
}

void XmlParseParam::setCreatingAll()
{
	flagCreateDocument = sl_true;
	flagCreateCommentNodes = sl_true;
	flagCreateProcessingInstructionNodes = sl_true;
	flagCreateTextNodes = sl_true;
	flagCreateWhiteSpaces = sl_true;
}

void XmlParseParam::setCreatingOnlyElements()
{
	flagCreateDocument = sl_true;
	flagCreateCommentNodes = sl_false;
	flagCreateProcessingInstructionNodes = sl_false;
	flagCreateTextNodes = sl_false;
	flagCreateWhiteSpaces = sl_false;
}

void XmlParseParam::setCreatingOnlyElementsAndTexts()
{
	flagCreateDocument = sl_true;
	flagCreateCommentNodes = sl_false;
	flagCreateProcessingInstructionNodes = sl_false;
	flagCreateTextNodes = sl_true;
	flagCreateWhiteSpaces = sl_false;
}

XmlParseControl::XmlParseControl()
{
	characterSize = 0;
	flagChangeSource = sl_false;
	parsingPosition = 0;
	flagStopParsing = sl_false;
}

template <class ST, class CT, class BT>
class _Xml_Parser
{
public:
	String sourceFilePath;
	const CT* buf;
	sl_size len;
	sl_size pos;
	sl_size lineNumber;
	sl_size columnNumber;
	sl_size posForLineColumn;
	
	Ref<XmlDocument> document;
	Ptr<IXmlParseListener> listener;
	XmlParseControl control;
	
	XmlParseParam param;
	
	sl_bool flagError;
	String errorMessage;
	
public:
	_Xml_Parser();
	
	void escapeWhiteSpaces();
	
	void calcLineNumber();
	
	void createWhiteSpace(XmlNodeGroup* parent, sl_size posStart, sl_size posEnd);

	void escapeEntity(BT* buf);
	
	void parseName(String& name);

	void parseComment(XmlNodeGroup* parent);

	void parseCDATA(XmlNodeGroup* parent);
	
	void parsePI(XmlNodeGroup* parent);
	
	void processPrefix(const String& name, const String& defNamespace, const Map<String, String>& namespaces, String& prefix, String& uri, String& localName);
	
	void parseAttribute(String& name, String& value);
	
	void parseElement(XmlNodeGroup* parent, const String& defNamespace, const Map<String, String>& namespaces);
	
	void parseText(XmlNodeGroup* parent);
	
	void parseNodes(XmlNodeGroup* parent, const String& defNamespace, const Map<String, String>& namespaces);
	
	void parseXml();
	
	static Ref<XmlDocument> parseXml(const String& sourceFilePath, const CT* buf, sl_size len, XmlParseParam& param);
	
};


// 1: valid, 2: invalid for starting, 0: invalid
const sl_uint8 _XML_check_name_pattern[128] = {
	/*		NUL		SOH		STX		ETX		EOT		ENQ		ACK		BEL		*/
	/*00*/	0,		0,		0,		0,		0,		0,		0,		0,
	/*		BS		HT		LF		VT		FF		CR		SO		SI		*/
	/*08*/	0,		0,		0,		0,		0,		0,		0,		0,
	/*		DLE		DC1		DC2		DC3		DC4		NAK		SYN		ETB		*/
	/*10*/	0,		0,		0,		0,		0,		0,		0,		0,
	/*		CAN		EM		SUB		ESC		FS		GS		RS		US		*/
	/*18*/	0,		0,		0,		0,		0,		0,		0,		0,
	/*		SP		!		"		#		$		%		&		'		*/
	/*20*/	0,		0,		0,		0,		0,		0,		0,		0,
	/*		(		)		*		+		,		-		.		/		*/
	/*28*/	0,		0,		0,		0,		0,		2,		2,		0,
	/*		0		1		2		3		4		5		6		7		*/
	/*30*/	2,		2,		2,		2,		2,		2,		2,		2,
	/*		8		9		:		;		<		=		>		?		*/
	/*38*/	2,		2,		1,		0,		0,		0,		0,		0,
	/*		@		A		B		C		D		E		F		G		*/
	/*40*/	0,		1,		1,		1,		1,		1,		1,		1,
	/*		H		I		J		K		L		M		N		O		*/
	/*48*/	1,		1,		1,		1,		1,		1,		1,		1,
	/*		P		Q		R		S		T		U		V		W		*/
	/*50*/	1,		1,		1,		1,		1,		1,		1,		1,
	/*		X		Y		Z		[		\		]		^		_		*/
	/*58*/	1,		1,		1,		0,		0,		0,		0,		1,
	/*		`		a		b		c		d		e		f		g		*/
	/*60*/	0,		1,		1,		1,		1,		1,		1,		1,
	/*		h		i		j		k		l		m		n		o		*/
	/*68*/	1,		1,		1,		1,		1,		1,		1,		1,
	/*		p		q		r		s		t		u		v		w		*/
	/*70*/	1,		1,		1,		1,		1,		1,		1,		1,
	/*		x		y		z		{		|		}		~		DEL		*/
	/*78*/	1,		1,		1,		0,		0,		0,		1,		0
};


SLIB_STATIC_STRING(_g_xml_error_msg_unknown, "Unknown Error")
SLIB_STATIC_STRING(_g_xml_error_msg_memory_lack, "Lack of Memory")
SLIB_STATIC_STRING(_g_xml_error_msg_user_stop, "User stopped parsing")
SLIB_STATIC_STRING(_g_xml_error_msg_invalid_escape, "Invalid escaping entity")
SLIB_STATIC_STRING(_g_xml_error_msg_escape_not_end, "Missing semi-colon(;) at the end of entity definition")
SLIB_STATIC_STRING(_g_xml_error_msg_invalid_markup, "Invalid Markup")
SLIB_STATIC_STRING(_g_xml_error_msg_comment_double_hyphen, "Double-hyphen(--) is not allowed in comment text")
SLIB_STATIC_STRING(_g_xml_error_msg_comment_not_end, "Comment Section must be ended with -->")
SLIB_STATIC_STRING(_g_xml_error_msg_CDATA_not_end, "CDATA Section must be ended with ]]>")
SLIB_STATIC_STRING(_g_xml_error_msg_name_missing, "Name definition is missing")
SLIB_STATIC_STRING(_g_xml_error_msg_name_invalid_start, "Name definition is starting with invalid character")
SLIB_STATIC_STRING(_g_xml_error_msg_name_invalid_char, "Name definition is containing invalid character")
SLIB_STATIC_STRING(_g_xml_error_msg_PI_not_end, "Processing Instruction Section must be ended with ?>")
SLIB_STATIC_STRING(_g_xml_error_msg_element_tag_not_end, "Element tag definition must be ended with > or />")
SLIB_STATIC_STRING(_g_xml_error_msg_element_tag_not_matching_end_tag, "Element must be terminated by the matching end-tag")
SLIB_STATIC_STRING(_g_xml_error_msg_element_attr_required_assign, "An assign(=) symbol is required for attribute definition")
SLIB_STATIC_STRING(_g_xml_error_msg_element_attr_required_quot, "Attribute value definition must be started with \" or ' symbol")
SLIB_STATIC_STRING(_g_xml_error_msg_element_attr_not_end, "Attribute value definition does not be ended")
SLIB_STATIC_STRING(_g_xml_error_msg_element_attr_end_with_invalid_char, "Attribute value definition must be followed by >, /, or whitespaces")
SLIB_STATIC_STRING(_g_xml_error_msg_element_attr_duplicate, "Attribute name is already specified")
SLIB_STATIC_STRING(_g_xml_error_msg_content_include_lt, "Content must not include less-than(<) character")
SLIB_STATIC_STRING(_g_xml_error_msg_document_not_wellformed, "Document must be well-formed")

#define CALL_LISTENER(NAME, NODE, ...) \
{ \
	PtrLocker<IXmlParseListener> _listener(listener); \
	if (_listener.isNotNull()) { \
		control.parsingPosition = pos; \
		control.flagChangeSource = sl_false; \
		control.currentNode = NODE; \
		_listener->NAME(&control, __VA_ARGS__); \
		if (control.flagStopParsing) { \
			flagError = sl_true; \
			errorMessage = _g_xml_error_msg_user_stop; \
			return; \
		} \
		if (control.flagChangeSource) { \
			buf = (CT*)(control.source.sz8); \
			len = control.source.len; \
		} \
		pos = control.parsingPosition; \
	} \
}

#define REPORT_ERROR(MSG) \
	flagError = sl_true; \
	errorMessage = MSG; \
	return;


template <class ST, class CT, class BT>
_Xml_Parser<ST, CT, BT>::_Xml_Parser()
{
	pos = 0;
	lineNumber = 1;
	columnNumber = 1;
	posForLineColumn = 0;
	
	flagError = sl_false;
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::escapeWhiteSpaces()
{
	while (pos < len) {
		CT ch = buf[pos];
		if (!(SLIB_CHAR_IS_WHITE_SPACE(ch))) {
			break;
		}
		pos++;
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::calcLineNumber()
{
	for (sl_size i = posForLineColumn; i < pos; i++) {
		CT ch = buf[i];
		if (ch == '\r') {
			lineNumber++;
			columnNumber = 1;
			if (i + 1 < len && buf[i+1] == '\n') {
				i++;
			}
		} else if (ch == '\n') {
			if (i == 0 || buf[i-1] != '\r') {
				lineNumber++;
				columnNumber = 1;
			}
		} else {
			columnNumber++;
		}
	}
	posForLineColumn = pos;
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::createWhiteSpace(XmlNodeGroup* parent, sl_size posStart, sl_size posEnd)
{
	if (posEnd <= posStart) {
		return;
	}
	if (param.flagCreateWhiteSpaces) {
		if (parent) {
			String content(buf + posStart, posEnd - posStart);
			if (content.isNull()) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
			Ref<XmlWhiteSpace> node = XmlWhiteSpace::create(content);
			if (node.isNull()) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
			calcLineNumber();
			node->setSourceFilePath(sourceFilePath);
			node->setStartPositionInSource(posStart);
			node->setEndPositionInSource(posEnd);
			node->setLineNumberInSource(lineNumber);
			node->setColumnNumberInSource(columnNumber);
			if (!(parent->addChild(node))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::escapeEntity(BT* sb)
{
	if (pos < len - 2 && buf[pos] == 'l' && buf[pos+1] == 't' && buf[pos+2] == ';') {
		static CT sc = '<';
		if (sb) {
			if (!(sb->addStatic(&sc, 1))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos += 3;
	} else if (pos < len - 2 && buf[pos] == 'g' && buf[pos+1] == 't' && buf[pos+2] == ';') {
		static CT sc = '>';
		if (sb) {
			if (!(sb->addStatic(&sc, 1))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos += 3;
	} else if (pos < len - 3 && buf[pos] == 'a' && buf[pos+1] == 'm' && buf[pos+2] == 'p' && buf[pos+3] == ';') {
		static CT sc = '&';
		if (sb) {
			if (!(sb->addStatic(&sc, 1))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos += 4;
	} else if (pos < len - 4 && buf[pos] == 'a' && buf[pos+1] == 'p' && buf[pos+2] == 'o' && buf[pos+3] == 's' && buf[pos+4] == ';') {
		static CT sc = '\'';
		if (sb) {
			if (!(sb->addStatic(&sc, 1))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos += 5;
	} else if (pos < len - 4 && buf[pos] == 'q' && buf[pos+1] == 'u' && buf[pos+2] == 'o' && buf[pos+3] == 't' && buf[pos+4] == ';') {
		static CT sc = '\"';
		if (sb) {
			if (!(sb->addStatic(&sc, 1))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos += 5;
	} else if (pos < len - 2 && buf[pos] == '#'){
		pos++;
		sl_uint32 n;
		sl_reg parseRes;
		if (buf[pos] == 'x') {
			pos++;
			parseRes = ST::parseUint32(16, &n, buf, pos, len - pos);
		} else {
			parseRes = ST::parseUint32(10, &n, buf, pos, len - pos);
		}
		if (parseRes == SLIB_PARSE_ERROR) {
			REPORT_ERROR(_g_xml_error_msg_invalid_escape)
		}
		pos = parseRes;
		if (pos >= len) {
			REPORT_ERROR(_g_xml_error_msg_escape_not_end)
		}
		if (buf[pos] != ';') {
			REPORT_ERROR(_g_xml_error_msg_escape_not_end)
		}
		String16 s(&n, 1);
		if (s.isNull()) {
			REPORT_ERROR(_g_xml_error_msg_memory_lack)
		}
		if (sb) {
			if (!(sb->add(s))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
		}
		pos++;
	} else {
		REPORT_ERROR(_g_xml_error_msg_invalid_escape)
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseName(String& name)
{
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_name_missing)
	}
	sl_uint32 ch = (sl_uint32)(buf[pos]);
	if (ch < 128 && _XML_check_name_pattern[ch] != 1) {
		REPORT_ERROR(_g_xml_error_msg_name_invalid_start)
	}
	sl_size start = pos;
	pos++;
	while (pos < len) {
		ch = (sl_uint32)(buf[pos]);
		if (ch < 128 && _XML_check_name_pattern[ch] == 0) {
			break;
		}
		pos++;
	}
	name = String(buf + start, pos - start);
	if (name.isNull()) {
		REPORT_ERROR(_g_xml_error_msg_memory_lack)
	}
}


template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseComment(XmlNodeGroup* parent)
{
	calcLineNumber();
	sl_size startLine = lineNumber;
	sl_size startColumn = columnNumber;
	sl_size startComment = pos;
	sl_bool flagEnded = sl_false;
	while (pos < len - 2) {
		if (buf[pos] == '-' && buf[pos + 1] == '-') {
			if (buf[pos + 2] == '>') {
				if (param.flagCreateCommentNodes) {
					String str(buf + startComment, pos - startComment);
					if (str.isNull()) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					if (parent) {
						Ref<XmlComment> comment = XmlComment::create(str);
						if (comment.isNull()) {
							REPORT_ERROR(_g_xml_error_msg_memory_lack)
						}
						comment->setSourceFilePath(sourceFilePath);
						comment->setStartPositionInSource(startComment);
						comment->setEndPositionInSource(pos + 3);
						comment->setLineNumberInSource(startLine);
						comment->setColumnNumberInSource(startColumn);
						if (!(parent->addChild(comment))) {
							REPORT_ERROR(_g_xml_error_msg_memory_lack)
						}
						CALL_LISTENER(onComment, comment.get(), str)
					} else {
						CALL_LISTENER(onComment, sl_null, str)
					}
				}
				pos += 3;
				flagEnded = sl_true;
				break;
			} else {
				REPORT_ERROR(_g_xml_error_msg_comment_double_hyphen)
			}
		} else {
			pos++;
		}
	}
	if (!flagEnded) {
		REPORT_ERROR(_g_xml_error_msg_comment_not_end)
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseCDATA(XmlNodeGroup* parent)
{
	calcLineNumber();
	sl_size startLine = lineNumber;
	sl_size startColumn = columnNumber;
	sl_size startCDATA = pos;
	sl_bool flagEnded = sl_false;
	while (pos < len - 2) {
		if (buf[pos] == ']' && buf[pos + 1] == ']' && buf[pos + 2] == '>') {
			if (param.flagCreateTextNodes) {
				String str(buf + startCDATA, pos - startCDATA);
				if (str.isNull()) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
				if (parent) {
					Ref<XmlText> text = XmlText::createCDATA(str);
					if (text.isNull()) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					text->setSourceFilePath(sourceFilePath);
					text->setStartPositionInSource(startCDATA);
					text->setEndPositionInSource(pos + 3);
					text->setLineNumberInSource(startLine);
					text->setColumnNumberInSource(startColumn);
					if (!(parent->addChild(text))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					CALL_LISTENER(onCDATA, text.get(), str)
				} else {
					CALL_LISTENER(onCDATA, sl_null, str)
				}
			}
			pos += 3;
			flagEnded = sl_true;
			break;
		} else {
			pos++;
		}
	}
	if (!flagEnded) {
		REPORT_ERROR(_g_xml_error_msg_CDATA_not_end)
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parsePI(XmlNodeGroup* parent)
{
	String target;
	parseName(target);
	if (flagError) {
		return;
	}
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_PI_not_end)
	}
	CT ch = buf[pos];
	if (ch != '?') {
		if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
			pos++;
			escapeWhiteSpaces();
		} else {
			REPORT_ERROR(_g_xml_error_msg_name_invalid_char)
		}
	}
	calcLineNumber();
	sl_size startLine = lineNumber;
	sl_size startColumn = columnNumber;
	sl_size startPI = pos;
	sl_bool flagEnded = sl_false;
	while (pos < len - 1) {
		if (buf[pos] == '?' && buf[pos + 1] == '>') {
			if (param.flagCreateProcessingInstructionNodes) {
				String str(buf + startPI, pos - startPI);
				if (str.isNull()) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
				if (parent) {
					Ref<XmlProcessingInstruction> PI = XmlProcessingInstruction::create(target, str);
					if (PI.isNull()) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					PI->setSourceFilePath(sourceFilePath);
					PI->setStartPositionInSource(startPI);
					PI->setEndPositionInSource(pos + 2);
					PI->setLineNumberInSource(startLine);
					PI->setColumnNumberInSource(startColumn);
					if (!(parent->addChild(PI))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					CALL_LISTENER(onProcessingInstruction, PI.get(), target, str)
				} else {
					CALL_LISTENER(onProcessingInstruction, sl_null, target, str)
				}
			}
			pos += 2;
			flagEnded = sl_true;
			break;
		} else {
			pos++;
		}
	}
	if (!flagEnded) {
		REPORT_ERROR(_g_xml_error_msg_PI_not_end)
	}
}

template <class ST, class CT, class BT>
SLIB_INLINE void _Xml_Parser<ST, CT, BT>::processPrefix(const String& name, const String& defNamespace, const Map<String, String>& namespaces, String& prefix, String& uri, String& localName)
{
	sl_reg index = name.indexOf(':');
	if (index >= 0) {
		prefix = name.substring(0, index);
		localName = name.substring(index+1);
		namespaces.get(prefix, &uri);
	} else {
		localName = name;
		uri = defNamespace;
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseAttribute(String& name, String& value)
{
	parseName(name);
	if (flagError) {
		return;
	}
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
	}
	CT ch = buf[pos];
	if (ch != '=') {
		if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
			pos++;
			escapeWhiteSpaces();
		} else {
			REPORT_ERROR(_g_xml_error_msg_name_invalid_char)
		}
	}
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_element_attr_required_assign)
	}
	if (buf[pos] != '=') {
		REPORT_ERROR(_g_xml_error_msg_element_attr_required_assign)
	}
	pos++;
	escapeWhiteSpaces();
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_element_attr_required_quot)
	}
	ch = buf[pos];
	if (ch == '\"' || ch == '\'') {
		pos++;
		sl_size startAttrValue = pos;
		sl_bool flagEnded = sl_false;
		BT sb;
		CT chQuot = ch;
		while (pos < len) {
			ch = buf[pos];
			if (ch == '&') {
				if (pos > startAttrValue) {
					if (!(sb.addStatic(buf + startAttrValue, pos - startAttrValue))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
				}
				pos++;
				escapeEntity(&sb);
				if (flagError) {
					return;
				}
				startAttrValue = pos;
			} else if (ch == '<') {
				REPORT_ERROR(_g_xml_error_msg_content_include_lt)
			} else if (ch == chQuot) {
				if (pos > startAttrValue) {
					if (!(sb.addStatic(buf + startAttrValue, pos - startAttrValue))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
				}
				pos++;
				flagEnded = sl_true;
				value = sb.merge();
				if (value.isNull()) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
				break;
			} else {
				pos++;
			}
		}
		if (!flagEnded) {
			REPORT_ERROR(_g_xml_error_msg_element_attr_not_end)
		}
	} else {
		REPORT_ERROR(_g_xml_error_msg_element_attr_required_quot)
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseElement(XmlNodeGroup* parent, const String& _defNamespace, const Map<String, String>& _namespaces)
{
	String defNamespace = _defNamespace;
	Map<String, String> namespaces = _namespaces;
	
	calcLineNumber();
	sl_size startLine = lineNumber;
	sl_size startColumn = columnNumber;
	sl_size posNameStart = pos;
	String name;
	parseName(name);
	if (flagError) {
		return;
	}
	sl_size lenName = pos - posNameStart;
	
	Ref<XmlElement> element = new XmlElement;
	if (element.isNull()) {
		REPORT_ERROR(_g_xml_error_msg_memory_lack)
	}
	
	CList<String> listPrefixMappings;
	sl_size indexAttr = 0;
	
	while (pos < len) {
		
		sl_size startWhiteSpace = pos;
		sl_size endWhiteSpace = pos;
		
		CT ch = buf[pos];
		
		if (ch != '>' && ch != '/') {
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				pos++;
				escapeWhiteSpaces();
				endWhiteSpace = pos;
			} else {
				if (indexAttr == 0) {
					REPORT_ERROR(_g_xml_error_msg_name_invalid_char)
				} else {
					REPORT_ERROR(_g_xml_error_msg_element_attr_end_with_invalid_char)
				}
			}
		}
		if (pos >= len) {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
		}
		
		ch = buf[pos];
		if (ch == '>' || ch == '/') {
			break;
		} else {
			XmlAttribute attr;
			parseAttribute(attr.name, attr.value);
			if (flagError) {
				return;
			}
			if (element->containsAttribute(attr.name)) {
				REPORT_ERROR(_g_xml_error_msg_element_attr_duplicate)
			}
			String prefix;
			processPrefix(attr.name, defNamespace, namespaces, prefix, attr.uri, attr.localName);
			if (param.flagCreateWhiteSpaces) {
				if (endWhiteSpace > startWhiteSpace) {
					String ws(buf + startWhiteSpace, endWhiteSpace - startWhiteSpace);
					if (ws.isNull()) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					attr.whiteSpacesBeforeName = ws;
				}
			}
			if (!(element->setAttribute(attr))) {
				REPORT_ERROR(_g_xml_error_msg_memory_lack)
			}
			if (param.flagProcessNamespaces) {
				if (attr.name == "xmlns") {
					defNamespace = attr.value;
					if (!(listPrefixMappings.add(String::null()))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					CALL_LISTENER(onStartPrefixMapping, element.get(), String::null(), defNamespace);
				} else if (prefix == "xmlns" && attr.localName.isNotEmpty() && attr.value.isNotEmpty()) {
					if (namespaces == _namespaces) {
						namespaces = _namespaces.duplicate();
					}
					if (!(namespaces.put(attr.localName, attr.value))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					if (!(listPrefixMappings.add(attr.localName))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
					CALL_LISTENER(onStartPrefixMapping, element.get(), attr.localName, attr.value)
				}
			}
		}
		
		indexAttr++;
		
	}
	
	if (pos >= len) {
		REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
	}
	sl_bool flagEmptyTag = sl_false;
	if (buf[pos] == '/') {
		if (pos < len - 1 && buf[pos+1] == '>') {
			flagEmptyTag = sl_true;
			pos += 2;
		} else {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
		}
	} else {
		pos++;
	}
	
	element->setSourceFilePath(sourceFilePath);
	element->setStartPositionInSource(posNameStart);
	element->setLineNumberInSource(startLine);
	element->setColumnNumberInSource(startColumn);
	element->setEndPositionInSource(pos);

	String prefix, uri, localName;
	processPrefix(name, defNamespace, namespaces, prefix, uri, localName);
	if (!(element->setName(name, uri, localName))) {
		REPORT_ERROR(_g_xml_error_msg_unknown)
	}

	if (parent) {
		if (!(parent->addChild(element))) {
			REPORT_ERROR(_g_xml_error_msg_memory_lack)
		}
	}
	CALL_LISTENER(onStartElement, element.get(), element.get())
	if (!flagEmptyTag) {
		parseNodes(parent ? element.get() : sl_null, defNamespace, namespaces);
		if (flagError) {
			return;
		}
		if (pos + 3 + lenName > len) {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_matching_end_tag)
		}
		if (buf[pos] != '<' || buf[pos+1] != '/') {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_matching_end_tag)
		}
		pos += 2;
		if (!(Base::equalsMemory(buf + posNameStart, buf + pos, lenName * sizeof(CT)))) {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_matching_end_tag)
		}
		pos += lenName;
		CT ch = buf[pos];
		if (ch != '>') {
			if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
				pos++;
				escapeWhiteSpaces();
			} else {
				REPORT_ERROR(_g_xml_error_msg_name_invalid_char)
			}
		}
		if (pos >= len) {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
		}
		if (buf[pos] != '>') {
			REPORT_ERROR(_g_xml_error_msg_element_tag_not_end)
		}
		pos++;
	}
	element->setEndPositionInSource(pos);
	CALL_LISTENER(onEndElement, element.get(), element.get());
	if (param.flagProcessNamespaces) {
		ListLocker<String> prefixes(listPrefixMappings);
		for (sl_size i = 0; i < prefixes.count; i++) {
			CALL_LISTENER(onEndPrefixMapping, element.get(), prefixes[i]);
		}
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseText(XmlNodeGroup* parent)
{
	calcLineNumber();
	sl_size startLine = lineNumber;
	sl_size startColumn = columnNumber;
	sl_size startWhiteSpace = pos;
	escapeWhiteSpaces();
	if (pos > startWhiteSpace) {
		createWhiteSpace(parent, startWhiteSpace, pos);
		if (flagError) {
			return;
		}
	}
	sl_size startText = pos;
	BT _sb;
	BT* sb = param.flagCreateTextNodes ? &_sb : sl_null;
	while (pos < len) {
		CT ch = buf[pos];
		if (ch == '&') {
			if (sb) {
				if (pos > startText) {
					if (!(sb->addStatic(buf + startText, pos - startText))) {
						REPORT_ERROR(_g_xml_error_msg_memory_lack)
					}
				}
			}
			pos++;
			escapeEntity(sb);
			if (flagError) {
				return;
			}
			startText = pos;
		} else if (ch == '<') {
			break;
		} else {
			pos++;
		}
	}
	if (sb) {
		if (pos > startText) {
			sl_size endText = pos - 1;
			while (endText >= startText) {
				CT ch = buf[endText];
				if (SLIB_CHAR_IS_WHITE_SPACE(ch)) {
					endText--;
				} else {
					break;
				}
			}
			if (endText >= startText) {
				if (!(sb->addStatic(buf + startText, endText - startText + 1))) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
			}
			startWhiteSpace = endText + 1;
		} else {
			startWhiteSpace = pos;
		}
		String text = sb->merge();
		if (text.isNull()) {
			REPORT_ERROR(_g_xml_error_msg_memory_lack)
		}
		if (text.isNotEmpty()) {
			if (parent) {
				Ref<XmlText> node = XmlText::create(text);
				if (node.isNull()) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
				node->setSourceFilePath(sourceFilePath);
				node->setStartPositionInSource(startText);
				node->setEndPositionInSource(pos);
				node->setLineNumberInSource(startLine);
				node->setColumnNumberInSource(startColumn);
				if (!(parent->addChild(node))) {
					REPORT_ERROR(_g_xml_error_msg_memory_lack)
				}
				CALL_LISTENER(onText, node.get(), text)
			} else {
				CALL_LISTENER(onText, sl_null, text)
			}
		}
		createWhiteSpace(parent, startWhiteSpace, pos);
		if (flagError) {
			return;
		}
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseNodes(XmlNodeGroup* parent, const String& defNamespace, const Map<String, String>& namespaces)
{
	while (pos < len) {
		if (buf[pos] == '<') { // Element, Comment, PI, CDATA
			pos++;
			CT ch = buf[pos];
			if (ch == '!') { // Comment, CDATA
				pos++;
				if (pos < len - 1 && buf[pos] == '-' && buf[pos+1] == '-') { // Comment
					pos += 2;
					parseComment(parent);
					if (flagError) {
						return;
					}
				} else if (pos < len - 6 && buf[pos] == '[' && buf[pos+1] == 'C' && buf[pos+2] == 'D' && buf[pos+3] == 'A' && buf[pos+4] == 'T' && buf[pos+5] == 'A' && buf[pos+6] == '[') { // CDATA
					pos += 7;
					parseCDATA(parent);
					if (flagError) {
						return;
					}
				} else {
					REPORT_ERROR(_g_xml_error_msg_invalid_markup)
				}
			} else if (ch == '?') { // PI
				pos++;
				parsePI(parent);
				if (flagError) {
					return;
				}
			} else if (ch == '/') { // Element End Tag
				pos--;
				return;
			} else { // Element
				parseElement(parent, defNamespace, namespaces);
				if (flagError) {
					return;
				}
			}
		} else {
			parseText(parent);
			if (flagError) {
				return;
			}
		}
	}
}

template <class ST, class CT, class BT>
void _Xml_Parser<ST, CT, BT>::parseXml()
{
	CALL_LISTENER(onStartDocument, document.get(), document.get())
	parseNodes(document.get(), String::null(), Map<String, String>::null());
	if (flagError) {
		return;
	}
	if (pos < len) {
		REPORT_ERROR(_g_xml_error_msg_document_not_wellformed);
	}
	if (document.isNotNull() && param.flagCheckWellFormed) {
		if (!(document->checkWellFormed())) {
			REPORT_ERROR(_g_xml_error_msg_document_not_wellformed);
		}
	}
	CALL_LISTENER(onEndDocument, document.get(), document.get())
}

template <class ST, class CT, class BT>
Ref<XmlDocument> _Xml_Parser<ST, CT, BT>::parseXml(const String& sourceFilePath, const CT* buf, sl_size len, XmlParseParam& param)
{
	param.flagError = sl_false;
	
	_Xml_Parser<ST, CT, BT> parser;
	parser.sourceFilePath = sourceFilePath;
	parser.buf = buf;
	parser.len = len;
	
	if (param.flagCreateDocument) {
		parser.document = XmlDocument::create();
		if (parser.document.isNull()) {
			parser.document->setStartPositionInSource(0);
			parser.document->setEndPositionInSource(len);
			param.flagError = sl_true;
			param.errorMessage = _g_xml_error_msg_memory_lack;
			return sl_null;
		}
	}
	parser.param = param;
	parser.listener = param.listener;
	parser.control.source.sz8 = (sl_char8*)(buf);
	parser.control.source.len = len;
	parser.control.characterSize = sizeof(CT);
	
	parser.parseXml();
	
	if (!(parser.flagError)) {
		return parser.document;
	}

	param.flagError = sl_true;
	param.errorPosition = parser.pos;
	param.errorMessage = parser.errorMessage;
	param.errorLine = ST::countLineNumber(buf, parser.pos, &(param.errorColumn));
	
	if (param.flagLogError) {
		LogError("Xml", param.getErrorText());
	}
	
	return sl_null;
	
}

Ref<XmlDocument> Xml::parseXml(const sl_char8* sz, sl_size len, XmlParseParam& param)
{
	return _Xml_Parser<String, sl_char8, StringBuffer>::parseXml(String::null(), sz, len, param);
}

Ref<XmlDocument> Xml::parseXml(const sl_char8* sz, sl_size len)
{
	XmlParseParam param;
	return parseXml(sz, len, param);
}

Ref<XmlDocument> Xml::parseXml(const String& xml, XmlParseParam& param)
{
	return _Xml_Parser<String, sl_char8, StringBuffer>::parseXml(String::null(), xml.getData(), xml.getLength(), param);
}

Ref<XmlDocument> Xml::parseXml(const String& xml)
{
	XmlParseParam param;
	return parseXml(xml, param);
}

Ref<XmlDocument> Xml::parseXml16(const sl_char16* sz, sl_size len, XmlParseParam& param)
{
	return _Xml_Parser<String16, sl_char16, StringBuffer16>::parseXml(String::null(), sz, len, param);
}

Ref<XmlDocument> Xml::parseXml16(const sl_char16* sz, sl_size len)
{
	XmlParseParam param;
	return Xml::parseXml16(sz, len, param);
}

Ref<XmlDocument> Xml::parseXml16(const String16& xml, XmlParseParam& param)
{
	return _Xml_Parser<String16, sl_char16, StringBuffer16>::parseXml(String::null(), xml.getData(), xml.getLength(), param);
}

Ref<XmlDocument> Xml::parseXml16(const String16& xml)
{
	XmlParseParam param;
	return Xml::parseXml16(xml, param);
}

Ref<XmlDocument> Xml::parseXmlFromTextFile(const String& filePath, XmlParseParam& param)
{
	String16 xml = File::readAllText16(filePath);
	return _Xml_Parser<String16, sl_char16, StringBuffer16>::parseXml(filePath, xml.getData(), xml.getLength(), param);
}

Ref<XmlDocument> Xml::parseXmlFromTextFile(const String& filePath)
{
	XmlParseParam param;
	String16 xml = File::readAllText16(filePath);
	return _Xml_Parser<String16, sl_char16, StringBuffer16>::parseXml(filePath, xml.getData(), xml.getLength(), param);
}

/************************************************
				Xml Utilities
************************************************/
String Xml::makeEscapedText(const String& text)
{
	StringBuffer buf;
	if (buildEscapedText(text, buf)) {
		return buf.merge();
	}
	return sl_null;
}

sl_bool Xml::buildEscapedText(const String& text, StringBuffer& output)
{
	StringData data;
	StringData dataEscape;
	data.str8 = text;
	sl_char8* sz = text.getData();
	sl_size len = text.getLength();
	sl_size start = 0;
	for (sl_size i = 0; i < len; i++) {
		sl_char8 ch = sz[i];
		sl_bool flagEscape = sl_false;
		if (ch == '<') {
			flagEscape = sl_true;
			dataEscape.sz8 = "&lt;";
			dataEscape.len = 4;
		} else if (ch == '>') {
			flagEscape = sl_true;
			dataEscape.sz8 = "&gt;";
			dataEscape.len = 4;
		} else if (ch == '&') {
			flagEscape = sl_true;
			dataEscape.sz8 = "&amp;";
			dataEscape.len = 5;
		} else if (ch == '\'') {
			flagEscape = sl_true;
			dataEscape.sz8 = "&apos;";
			dataEscape.len = 6;
		} else if (ch == '\"') {
			flagEscape = sl_true;
			dataEscape.sz8 = "&quot;";
			dataEscape.len = 6;
		}
		if (flagEscape) {
			if (i > start) {
				data.sz8 = sz + start;
				data.len = i - start;
				if (!(output.add(data))) {
					return sl_false;
				}
			}
			start = i + 1;
			if (!(output.add(dataEscape))) {
				return sl_false;
			}
		}
	}
	if (len > start) {
		data.sz8 = sz + start;
		data.len = len - start;
		if (!(output.add(data))) {
			return sl_false;
		}
	}
	return sl_true;
}

template <class CT>
SLIB_INLINE sl_bool _Xml_checkName(const CT* sz, sl_size len)
{
	if (len == 0) {
		return sl_false;
	}
	sl_uint32 ch = (sl_uint32)(sz[0]);
	if (ch < 128 && _XML_check_name_pattern[ch] != 1) {
		return sl_false;
	}
	for (sl_size i = 1; i < len; i++) {
		ch = (sl_uint32)(sz[i]);
		if (ch < 128 && _XML_check_name_pattern[ch] == 0) {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool Xml::checkName(const sl_char8* sz, sl_size len)
{
	return _Xml_checkName(sz, len);
}

sl_bool Xml::checkName(const sl_char16* sz, sl_size len)
{
	return _Xml_checkName(sz, len);
}

sl_bool Xml::checkName(const String& tagName)
{
	return checkName(tagName.getData(), tagName.getLength());
}

SLIB_NAMESPACE_END
