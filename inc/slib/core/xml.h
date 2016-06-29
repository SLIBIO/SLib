#ifndef CHECKHEADER_SLIB_CORE_XML
#define CHECKHEADER_SLIB_CORE_XML

/************************************************************
 
			XML (Extensible Markup Language)
 
 XML 1.0 => https://www.w3.org/TR/REC-xml/
			https://www.w3.org/TR/2008/REC-xml-20081126/
 XML 1.1 => http://www.w3.org/TR/2006/REC-xml11-20060816/
 
 
 Supports DOM & SAX parsers
 
************************************************************/

#include "definition.h"

#include "variant.h"

SLIB_NAMESPACE_BEGIN

class XmlNodeGroup;
class XmlDocument;
class XmlElement;
class XmlText;
class XmlProcessingInstruction;
class XmlComment;
class XmlParseControl;

enum class XmlNodeType
{
	Document = 1,
	Element = 2,
	Text = 3,
	ProcessingInstruction = 4,
	Comment = 5,
	WhiteSpace = 6
};

class SLIB_EXPORT IXmlParseListener
{
public:
	virtual void onStartDocument(XmlParseControl* control, XmlDocument* document);
	
	virtual void onEndDocument(XmlParseControl* control, XmlDocument* document);
	
	virtual void onStartElement(XmlParseControl* control, XmlElement* element);
	
	virtual void onEndElement(XmlParseControl* control, XmlElement* element);
	
	virtual void onText(XmlParseControl* control, const String& text);
	
	virtual void onCDATA(XmlParseControl* control, const String& text);
	
	virtual void onProcessingInstruction(XmlParseControl* control, const String& target, const String& content);
	
	virtual void onComment(XmlParseControl* control, const String& content);
	
	virtual void onStartPrefixMapping(XmlParseControl* control, const String& prefix, const String& uri);
	
	virtual void onEndPrefixMapping(XmlParseControl* control, const String& prefix);
	
};

class SLIB_EXPORT XmlNode : public Referable
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlNode(XmlNodeType type);
	
public:
	XmlNodeType getType() const;
	
	virtual sl_bool buildText(StringBuffer& output) const = 0;
	
	virtual sl_bool buildXml(StringBuffer& output) const = 0;
	
	virtual String getText() const;
	
	String toString() const;
	
	sl_bool isDocumentNode() const;
	
	Ref<XmlDocument> toDocumentNode() const;
	
	sl_bool isElementNode() const;
	
	Ref<XmlElement> toElementNode() const;
	
	sl_bool isTextNode() const;
	
	Ref<XmlText> toTextNode() const;
	
	sl_bool isProcessingInstructionNode() const;
	
	Ref<XmlProcessingInstruction> toProcessingInstructionNode() const;
	
	sl_bool isCommentNode() const;
	
	Ref<XmlComment> toCommentNode() const;
	
	Ref<XmlDocument> getDocument() const;
	
	Ref<XmlElement> getRoot() const;
	
	Ref<XmlNodeGroup> getParent() const;
	
	Ref<XmlElement> getParentElement() const;
	
	sl_size getStartPositionInSource();
	
	void setStartPositionInSource(sl_size pos);
	
	sl_size getEndPositionInSource();
	
	void setEndPositionInSource(sl_size pos);
	
	sl_size getLineNumberInSource();
	
	void setLineNumberInSource(sl_size line);
	
	sl_size getColumnNumberInSource();
	
	void setColumnNumberInSource(sl_size line);

protected:
	XmlNodeType m_type;
	WeakRef<XmlNodeGroup> m_parent;
	WeakRef<XmlDocument> m_document;
	sl_size m_positionStartInSource;
	sl_size m_positionEndInSource;
	sl_size m_lineInSource;
	sl_size m_columnInSource;

protected:
	void _setDocument(const Ref<XmlDocument>& documentNew);
	
	friend class XmlNodeGroup;
	
};

class SLIB_EXPORT XmlNodeGroup : public XmlNode
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlNodeGroup(XmlNodeType type);
	
public:
	// override
	sl_bool buildText(StringBuffer& output) const;
	
	sl_bool buildInnerXml(StringBuffer& output) const;
	
	String getInnerXml() const;

	sl_size getChildrenCount() const;
	
	Ref<XmlNode> getChild(sl_size index) const;
	
	sl_bool addChild(const Ref<XmlNode>& node);

	sl_bool removeChild(const Ref<XmlNode>& node, sl_bool flagUnregisterDocument = sl_true);
	
	void removeAllChildren(sl_bool flagUnregisterDocument = sl_true);
	
	String getChildText(sl_size index) const;
	
	Ref<XmlElement> getChildElement(sl_size index) const;
	
	List< Ref<XmlElement> > getChildElements() const;
	
	sl_size getChildElementsCount() const;
	
	List< Ref<XmlElement> > getChildElements(const String& tagName) const;
	
	List< Ref<XmlElement> > getChildElements(const String& uri, const String& localName) const;
	
	Ref<XmlElement> getFirstChildElement() const;
	
	Ref<XmlElement> getFirstChildElement(const String& tagName) const;
	
	Ref<XmlElement> getFirstChildElement(const String& uri, const String& localName) const;
	
	String getFirstChildElementText() const;
	
	String getFirstChildElementText(const String& tagName) const;
	
	String getFirstChildElementText(const String& uri, const String& localName) const;
	
	List< Ref<XmlElement> > getDescendantElements(const String& tagName) const;
	
	void getDescendantElements(const String& tagName, List< Ref<XmlElement> >& list) const;
	
	List< Ref<XmlElement> > getDescendantElements(const String& uri, const String& localName) const;
	
	void getDescendantElements(const String& uri, const String& localName, List< Ref<XmlElement> >& list) const;
	
	Ref<XmlElement> getFirstDescendantElement(const String& tagName) const;
	
	Ref<XmlElement> getFirstDescendantElement(const String& uri, const String& localName) const;
	
	String getFirstDescendantElementText(const String& tagName) const;
	
	String getFirstDescendantElementText(const String& uri, const String& localName) const;
	
	Ref<XmlElement> findChildElementByAttribute(const String& name, const String& value) const;
	
	Ref<XmlElement> findChildElementById(const String& _id) const;

protected:
	CList< Ref<XmlNode> > m_children;

	friend class XmlDocument;
	
};

struct SLIB_EXPORT XmlAttribute
{
	String name;
	String uri;
	String localName;
	String value;
	String whiteSpacesBeforeName;
};

class SLIB_EXPORT XmlElement : public XmlNodeGroup
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlElement();
	
public:
	static Ref<XmlElement> create(const String& name);
	
	static Ref<XmlElement> create(const String& name, const String& uri, const String& localName);
	
	// override
	sl_bool buildXml(StringBuffer& output) const;

	String getName() const;
	
	String getUri() const;
	
	String getLocalName() const;

	sl_bool setName(const String& name);
	
	sl_bool setName(const String& name, const String& uri, const String& localName);
	
	sl_size getAttributesCount() const;
	
	sl_bool getAttribute(sl_size index, XmlAttribute* _out = sl_null) const;
	
	String getAttribute(const String& name) const;
	
	String getAttribute(const String& uri, const String& localName) const;
	
	sl_bool containsAttribute(const String& name) const;
	
	sl_bool setAttribute(sl_size index, const String& value);
	
	sl_bool setAttribute(sl_size index, const String& uri, const String& localName, const String& value);
	
	sl_bool setAttribute(const String& name, const String& value);
	
	sl_bool setAttribute(const XmlAttribute& attr);
	
	sl_bool setAttribute(const String& uri, const String& localName, const String& value);
	
	sl_bool removeAttribute(sl_size index);
	
	sl_bool removeAttribute(const String& name);
	
	void removeAllAttributes();
	
protected:
	SafeString m_name;
	SafeString m_uri;
	SafeString m_localName;
	CList<XmlAttribute> m_attributes;
	HashMap<String, String> m_mapAttributes;
	Mutex m_lockAttributes;
	
protected:
	friend class XmlNode;
	friend class XmlNodeGroup;
	
};

class SLIB_EXPORT XmlDocument : public XmlNodeGroup
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlDocument();
	
public:
	static Ref<XmlDocument> create();
	
	// override
	sl_bool buildXml(StringBuffer& output) const;

	Ref<XmlElement> getElementById(const String& _id) const;
	
	void registerElementsById(const String& idAttributeName);
	
	void registerElementsById(const Ref<XmlElement>& element, const String& idAttributeName);
	
	sl_bool checkWellFormed() const;
	
protected:
	HashMap< String, WeakRef<XmlElement> > m_elementsById;
	
};

class SLIB_EXPORT XmlText : public XmlNode
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlText();
	
public:
	static Ref<XmlText> create(const String& text, sl_bool flagCDATA = sl_false);

	static Ref<XmlText> createCDATA(const String& text);
	
	// override
	sl_bool buildText(StringBuffer& output) const;
	
	// override
	sl_bool buildXml(StringBuffer& output) const;

	// override
	String getText() const;
	
	void setText(const String& text);
	
	sl_bool isCDATA() const;
	
	void setCDATA(sl_bool flag);
	
protected:
	SafeString m_text;
	sl_bool m_flagCDATA;

};

class SLIB_EXPORT XmlProcessingInstruction : public XmlNode
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlProcessingInstruction();
	
public:
	static Ref<XmlProcessingInstruction> create(const String& target, const String& content);
	
	// override
	sl_bool buildText(StringBuffer& output) const;
	
	// override
	sl_bool buildXml(StringBuffer& output) const;
	
	String getTarget() const;
	
	sl_bool setTarget(const String& target);
	
	String getContent() const;
	
	void setContent(const String& content);

protected:
	SafeString m_target;
	SafeString m_content;

};

class SLIB_EXPORT XmlComment : public XmlNode
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlComment();
	
public:
	static Ref<XmlComment> create(const String& comment);
	
	// override
	sl_bool buildText(StringBuffer& output) const;
	
	// override
	sl_bool buildXml(StringBuffer& output) const;
	
	String getComment() const;
	
	void setComment(const String& comment);
	
protected:
	SafeString m_comment;
	
};

class SLIB_EXPORT XmlWhiteSpace : public XmlNode
{
	SLIB_DECLARE_OBJECT
	
public:
	XmlWhiteSpace();
	
public:
	static Ref<XmlWhiteSpace> create(const String& content);
	
	// override
	sl_bool buildText(StringBuffer& output) const;
	
	// override
	sl_bool buildXml(StringBuffer& output) const;
	
	String getContent() const;
	
	void setContent(const String& comment);
	
protected:
	SafeString m_content;
	
};

class SLIB_EXPORT XmlParseControl
{
public:
	// read & write
	StringData source;
	
	sl_uint32 characterSize;
	
	// write only
	sl_bool flagChangeSource;

	// read & write
	sl_size parsingPosition;
	
	// write only
	sl_bool flagStopParsing;
	
	// read only
	XmlNode* currentNode;
	
public:
	XmlParseControl();
	
};

class SLIB_EXPORT XmlParseParam
{
public:
	// in
	sl_bool flagCreateDocument;
	// in
	sl_bool flagCreateCommentNodes;
	// in
	sl_bool flagCreateProcessingInstructionNodes;
	// in
	sl_bool flagCreateTextNodes;
	// in
	sl_bool flagCreateWhiteSpaces;
	
	// in
	sl_bool flagProcessNamespaces;
	// in
	sl_bool flagCheckWellFormed;
	
	// in
	Ptr<IXmlParseListener> listener;
	
	// out
	sl_bool flagError;
	// out
	sl_size errorPosition;
	// out
	sl_size errorLine;
	// out
	sl_size errorColumn;
	// out
	String errorMessage;
	
public:
	XmlParseParam();
	
public:
	String getErrorText();
	
	void setCreatingAll();
	
	void setCreatingOnlyElements();
	
	void setCreatingOnlyElementsAndTexts();
	
};

class SLIB_EXPORT Xml
{
public:
	static Ref<XmlDocument> parseXml(const char* xml, sl_size length, XmlParseParam& param);
	
	static Ref<XmlDocument> parseXml(const char* xml, sl_size length);

	static Ref<XmlDocument> parseXml(const String& xml, XmlParseParam& param);
	
	static Ref<XmlDocument> parseXml(const String& xml);
	
	
	static Ref<XmlDocument> parseXml16(const sl_char16* xml, sl_size length, XmlParseParam& param);
	
	static Ref<XmlDocument> parseXml16(const sl_char16* xml, sl_size length);
	
	static Ref<XmlDocument> parseXml16(const String16& xml, XmlParseParam& param);
	
	static Ref<XmlDocument> parseXml16(const String16& xml);
	
	
	static Ref<XmlDocument> parseXmlFromTextFile(const String& filePath, XmlParseParam& param);
	
	static Ref<XmlDocument> parseXmlFromTextFile(const String& filePath);
	
	
	static String makeEscapedText(const String& text);
	
	static sl_bool buildEscapedText(const String& text, StringBuffer& output);
	
	static sl_bool checkName(const sl_char8* sz, sl_size len);
	
	static sl_bool checkName(const sl_char16* sz, sl_size len);
	
	static sl_bool checkName(const String& tagName);
	
};

SLIB_NAMESPACE_END

#endif