/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

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
#include "function.h"

namespace slib
{
	
	class XmlNodeGroup;
	class XmlDocument;
	class XmlElement;
	class XmlText;
	class XmlProcessingInstruction;
	class XmlComment;
	class XmlParseControl;
	class StringBuffer;
	class StringBuffer16;

	enum class XmlNodeType
	{
		Document = 1,
		Element = 2,
		Text = 3,
		ProcessingInstruction = 4,
		Comment = 5,
		WhiteSpace = 6
	};
	
	typedef String16 XmlString;
	typedef StringBuffer16 XmlStringBuffer;
	
	class SLIB_EXPORT XmlNode : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlNode(XmlNodeType type);

	public:
		XmlNodeType getType() const;

		virtual sl_bool buildText(XmlStringBuffer& output) const = 0;

		virtual sl_bool buildXml(XmlStringBuffer& output) const = 0;

		virtual XmlString getText() const;

		XmlString toString() const;

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

		String getSourceFilePath() const;

		void setSourceFilePath(const String& path);

		sl_size getStartPositionInSource() const;

		void setStartPositionInSource(sl_size pos);

		sl_size getEndPositionInSource() const;

		void setEndPositionInSource(sl_size pos);

		sl_size getLineNumberInSource() const;

		void setLineNumberInSource(sl_size line);

		sl_size getColumnNumberInSource() const;

		void setColumnNumberInSource(sl_size line);
	
	protected:
		XmlNodeType m_type;
		WeakRef<XmlNodeGroup> m_parent;
		WeakRef<XmlDocument> m_document;
		AtomicString m_sourceFilePath;
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
		sl_bool buildText(XmlStringBuffer& output) const override;

		sl_bool buildInnerXml(XmlStringBuffer& output) const;

		XmlString getInnerXml() const;
	
		sl_size getChildrenCount() const;

		Ref<XmlNode> getChild(sl_size index) const;

		sl_bool addChild(const Ref<XmlNode>& node);
	
		sl_bool removeChild(const Ref<XmlNode>& node, sl_bool flagUnregisterDocument = sl_true);

		void removeAllChildren(sl_bool flagUnregisterDocument = sl_true);

		XmlString getChildText(sl_size index) const;

		Ref<XmlElement> getChildElement(sl_size index) const;

		List< Ref<XmlElement> > getChildElements() const;

		sl_size getChildElementsCount() const;

		List< Ref<XmlElement> > getChildElements(const XmlString& tagName) const;

		List< Ref<XmlElement> > getChildElements(const XmlString& uri, const XmlString& localName) const;

		Ref<XmlElement> getFirstChildElement() const;

		Ref<XmlElement> getFirstChildElement(const XmlString& tagName) const;

		Ref<XmlElement> getFirstChildElement(const XmlString& uri, const XmlString& localName) const;

		XmlString getFirstChildElementText() const;

		XmlString getFirstChildElementText(const XmlString& tagName) const;

		XmlString getFirstChildElementText(const XmlString& uri, const XmlString& localName) const;

		List< Ref<XmlElement> > getDescendantElements(const XmlString& tagName) const;

		void getDescendantElements(const XmlString& tagName, List< Ref<XmlElement> >& list) const;

		List< Ref<XmlElement> > getDescendantElements(const XmlString& uri, const XmlString& localName) const;

		void getDescendantElements(const XmlString& uri, const XmlString& localName, List< Ref<XmlElement> >& list) const;

		Ref<XmlElement> getFirstDescendantElement(const XmlString& tagName) const;

		Ref<XmlElement> getFirstDescendantElement(const XmlString& uri, const XmlString& localName) const;

		XmlString getFirstDescendantElementText(const XmlString& tagName) const;

		XmlString getFirstDescendantElementText(const XmlString& uri, const XmlString& localName) const;

		Ref<XmlElement> findChildElementByAttribute(const XmlString& name, const XmlString& value) const;

		Ref<XmlElement> findChildElementById(const XmlString& _id) const;
	
	protected:
		CList< Ref<XmlNode> > m_children;
	
		friend class XmlDocument;

	};
	
	struct SLIB_EXPORT XmlAttribute
	{
		XmlString name;
		XmlString uri;
		XmlString localName;
		XmlString value;
		XmlString whiteSpacesBeforeName;
	};
	
	class SLIB_EXPORT XmlElement : public XmlNodeGroup
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlElement();

	public:
		static Ref<XmlElement> create(const XmlString& name);

		static Ref<XmlElement> create(const XmlString& name, const XmlString& uri, const XmlString& localName);

		sl_bool buildXml(XmlStringBuffer& output) const override;
	
		XmlString getName() const;

		XmlString getUri() const;

		XmlString getLocalName() const;
	
		sl_bool setName(const XmlString& name);

		sl_bool setName(const XmlString& name, const XmlString& uri, const XmlString& localName);

		sl_size getAttributesCount() const;

		sl_bool getAttribute(sl_size index, XmlAttribute* _out = sl_null) const;

		XmlString getAttribute(const XmlString& name) const;

		XmlString getAttribute(const XmlString& uri, const XmlString& localName) const;

		sl_bool containsAttribute(const XmlString& name) const;

		sl_bool setAttribute(sl_size index, const XmlString& value);

		sl_bool setAttribute(sl_size index, const XmlString& uri, const XmlString& localName, const XmlString& value);

		sl_bool setAttribute(const XmlString& name, const XmlString& value);

		sl_bool setAttribute(const XmlAttribute& attr);

		sl_bool setAttribute(const XmlString& uri, const XmlString& localName, const XmlString& value);

		sl_bool removeAttribute(sl_size index);

		sl_bool removeAttribute(const XmlString& name);

		void removeAllAttributes();
		
		sl_size getStartContentPositionInSource() const;
		
		void setStartContentPositionInSource(sl_size pos);

		sl_size getEndContentPositionInSource() const;
		
		void setEndContentPositionInSource(sl_size pos);

	protected:
		Atomic<XmlString> m_name;
		Atomic<XmlString> m_uri;
		Atomic<XmlString> m_localName;
		List<XmlAttribute> m_attributes;
		HashMap<XmlString, XmlString> m_mapAttributes;
		Mutex m_lockAttributes;
		sl_size m_positionStartContentInSource;
		sl_size m_positionEndContentInSource;

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

		sl_bool buildXml(XmlStringBuffer& output) const override;
	
		Ref<XmlElement> getElementById(const XmlString& _id) const;

		void registerElementsById(const XmlString& idAttributeName);

		void registerElementsById(const Ref<XmlElement>& element, const XmlString& idAttributeName);

		sl_bool checkWellFormed() const;

	protected:
		HashMap< XmlString, WeakRef<XmlElement> > m_elementsById;

	};
	
	class SLIB_EXPORT XmlText : public XmlNode
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlText();

	public:
		static Ref<XmlText> create(const XmlString& text, sl_bool flagCDATA = sl_false);
	
		static Ref<XmlText> createCDATA(const XmlString& text);

		sl_bool buildText(XmlStringBuffer& output) const override;

		sl_bool buildXml(XmlStringBuffer& output) const override;
	
		XmlString getText() const override;

		void setText(const XmlString& text);

		sl_bool isCDATA() const;

		void setCDATA(sl_bool flag);

	protected:
		Atomic<XmlString> m_text;
		sl_bool m_flagCDATA;
	
	};
	
	class SLIB_EXPORT XmlProcessingInstruction : public XmlNode
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlProcessingInstruction();

	public:
		static Ref<XmlProcessingInstruction> create(const XmlString& target, const XmlString& content);

		sl_bool buildText(XmlStringBuffer& output) const override;

		sl_bool buildXml(XmlStringBuffer& output) const override;

		XmlString getTarget() const;

		sl_bool setTarget(const XmlString& target);

		XmlString getContent() const;

		void setContent(const XmlString& content);
	
	protected:
		Atomic<XmlString> m_target;
		Atomic<XmlString> m_content;
	
	};
	
	class SLIB_EXPORT XmlComment : public XmlNode
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlComment();

	public:
		static Ref<XmlComment> create(const XmlString& comment);

		sl_bool buildText(XmlStringBuffer& output) const override;

		sl_bool buildXml(XmlStringBuffer& output) const override;

		XmlString getComment() const;

		void setComment(const XmlString& comment);

	protected:
		Atomic<XmlString> m_comment;

	};
	
	class SLIB_EXPORT XmlWhiteSpace : public XmlNode
	{
		SLIB_DECLARE_OBJECT

	public:
		XmlWhiteSpace();

	public:
		static Ref<XmlWhiteSpace> create(const XmlString& content);

		sl_bool buildText(XmlStringBuffer& output) const override;

		sl_bool buildXml(XmlStringBuffer& output) const override;

		XmlString getContent() const;

		void setContent(const XmlString& comment);

	protected:
		Atomic<XmlString> m_content;

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

		// in, callbacks
		Function<void(XmlParseControl*, XmlDocument*)> onStartDocument;
		Function<void(XmlParseControl*, XmlDocument*)> onEndDocument;
		Function<void(XmlParseControl*, XmlElement*)> onStartElement;
		Function<void(XmlParseControl*, XmlElement*)> onEndElement;
		Function<void(XmlParseControl*, const XmlString& text)> onText;
		Function<void(XmlParseControl*, const XmlString& text)> onCDATA;
		Function<void(XmlParseControl*, const XmlString& target, const XmlString& content)> onProcessingInstruction;
		Function<void(XmlParseControl*, const XmlString& content)> onComment;
		Function<void(XmlParseControl*, const XmlString& prefix, const XmlString& uri)> onStartPrefixMapping;
		Function<void(XmlParseControl*, const XmlString& prefix)> onEndPrefixMapping;

		// in
		sl_bool flagLogError;

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
	
	/**
	 * @class Xml
	 * @brief provides utilities for parsing and build XML.
	 */
	class SLIB_EXPORT Xml
	{
	public:
		/**
		 * parses XML text (UTF-8 encoding) contained in `xml`
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] length length of the XML text
		 * @param[in] param options for XML parsing
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml(const char* xml, sl_size length, XmlParseParam& param);

		/**
		 * parses XML text (UTF-8 encoding) contained in `xml`.
		 * XML parser uses default option for parsing.
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] length length of the XML text
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml(const char* xml, sl_size length);
	
		/**
		 * parses XML text (UTF-8 encoding) contained in `xml`
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] param options for XML parsing
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml8(const String& xml, XmlParseParam& param);

		/**
		 * parses XML text (UTF-8 encoding) contained in `xml`.
		 * XML parser uses default option for parsing.
		 *
		 * @param[in] xml String value containing XML text
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml8(const String& xml);


		/**
		 * parses XML text (Unicode) contained in `xml`
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] length length of the XML text
		 * @param[in] param options for XML parsing
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml(const sl_char16* xml, sl_size length, XmlParseParam& param);

		/**
		 * parses XML text (Unicode) contained in `xml`.
		 * XML parser uses default option for parsing.
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] length length of the XML text
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml(const sl_char16* xml, sl_size length);
		
		/**
		 * parses XML text (Unicode) contained in `xml`
		 *
		 * @param[in] xml String value containing XML text
		 * @param[in] param options for XML parsing
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml16(const String16& xml, XmlParseParam& param);
		static Ref<XmlDocument> parseXml(const XmlString& xml, XmlParseParam& param);

		/**
		 * parses XML text (Unicode) contained in `xml`.
		 * XML parser uses default option for parsing.
		 *
		 * @param[in] xml String value containing XML text
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXml16(const String16& xml);
		static Ref<XmlDocument> parseXml(const XmlString& xml);

		/**
		 * parses XML text file located in `filePath`.
		 * The character positions are calculated by UTF-16 encoding.
		 *
		 * @param[in] filePath XML text file (UTF-8, UTF-16BE, UTF-16LE)
		 * @param[in] param options for XML parsing
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXmlFromTextFile(const String& filePath, XmlParseParam& param);

		/**
		 * parses XML text file located in `filePath`.
		 * The character positions are calculated by UTF-16 encoding.
		 * XML parser uses default option for parsing.
		 *
		 * @param[in] filePath XML text file (UTF-8, UTF-16BE, UTF-16LE)
		 *
		 * @return XmlDocument object on success
		 * @return nullptr on failure
		 */
		static Ref<XmlDocument> parseXmlFromTextFile(const String& filePath);


		/**
		 * Encodes speical characters (&lt; &gt; &amp; &quot; &apos;) to XML entities.
		 *
		 * @param[in] text String value containing the original text
		 *
		 * @return Encoded result text with XML entities for special characters
		 */
		static XmlString encodeTextToEntities(const XmlString& text);

		/**
		 * Encodes speical characters (&lt; &gt; &amp; &quot; &apos;) to XML entities.
		 * Encoded result text will be stored in `output` buffer.
		 *
		 * @param[in] text String value containing the original text
		 * @param[out] output StringBuffer that receives the encoded result text
		 *
		 * @return `true` on success
		 */
		static sl_bool encodeTextToEntities(const XmlString& text, XmlStringBuffer& output);
		
		/**
		 * Decodes XML entities (&amp;lt; &amp;gt; &amp;amp; ...) contained in `text`.
		 *
		 * @param[in] text String value that may contain XML entities.
		 *
		 * @return decoded text
		 */
		static XmlString decodeTextFromEntities(const XmlString& text);

		/**
		 * Check the `name` can be used as XML tag name
		 *
		 * @param[in] name XML tag name
		 * @param[in] len length of the tag name
		 *
		 * @return `true` if the `name` is valid for XML tag name
		 */
		static sl_bool checkName(const sl_char8* name, sl_size len);

		/**
		 * Check the `name` can be used as XML tag name
		 *
		 * @param[in] name XML tag name
		 * @param[in] len length of the tag name
		 *
		 * @return `true` if the `name` is valid for XML tag name
		 */
		static sl_bool checkName(const sl_char16* name, sl_size len);

		/**
		 * Check the `name` can be used as XML tag name
		 *
		 * @param[in] name XML tag name
		 *
		 * @return `true` if the `name` is valid for XML tag name
		 */
		static sl_bool checkName(const XmlString& name);

	};

}

#endif
