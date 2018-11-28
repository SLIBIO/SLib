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

#ifndef CHECKHEADER_SLIB_CORE_NODE_POSITION
#define CHECKHEADER_SLIB_CORE_NODE_POSITION

#include "definition.h"

namespace slib
{
	
	template <class NODE>
	class SLIB_EXPORT NodePosition
	{
	public:
		SLIB_INLINE constexpr NodePosition() noexcept
		 : node(sl_null)
		 {}
		
		SLIB_INLINE constexpr NodePosition(sl_null_t) noexcept
		 : node(sl_null)
		 {}
		
		SLIB_INLINE constexpr NodePosition(NODE* other) noexcept
		 : node(other)
		 {}
		
		SLIB_INLINE constexpr NodePosition(const NodePosition& other) noexcept
		 : node(other.node)
		 {}
		
	public:
		SLIB_INLINE NodePosition& operator=(const NodePosition& other) noexcept
		{
			node = other.node;
			return *this;
		}
		
		SLIB_INLINE NodePosition& operator=(NODE* other) noexcept
		{
			node = other;
			return *this;
		}
		
		SLIB_INLINE NODE& operator*() const noexcept
		{
			return *node;
		}
		
		SLIB_INLINE constexpr sl_bool operator==(const NodePosition& other) const noexcept
		{
			return node == other.node;
		}
		
		SLIB_INLINE constexpr sl_bool operator==(const NODE* other) const noexcept
		{
			return node == other;
		}
		
		SLIB_INLINE constexpr sl_bool operator!=(const NodePosition& other) const noexcept
		{
			return node != other.node;
		}
		
		SLIB_INLINE constexpr sl_bool operator!=(const NODE* other) const noexcept
		{
			return node != other;
		}
		
		SLIB_INLINE operator NODE*() const noexcept
		{
			return node;
		}
		
		SLIB_INLINE NodePosition& operator++() noexcept
		{
			node = node->getNext();
			return *this;
		}
		
	public:
		NODE* node;
		
	};

	template <class NODE>
	class SLIB_EXPORT NodePositionWithRef
	{
	public:
		SLIB_INLINE constexpr NodePositionWithRef() noexcept
		 : node(sl_null)
		 {}
		
		SLIB_INLINE constexpr NodePositionWithRef(sl_null_t) noexcept
		 : node(sl_null)
		 {}
		
		SLIB_INLINE constexpr NodePositionWithRef(NODE* _node) noexcept
		 : node(_node)
		 {}
		
		SLIB_INLINE constexpr NodePositionWithRef(NODE* _node, Referable* _ref) noexcept
		 : node(_node), ref(_ref)
		 {}
		
		SLIB_INLINE constexpr NodePositionWithRef(const NodePositionWithRef& other) noexcept
		 : node(other.node), ref(other.ref)
		 {}
		
	public:
		SLIB_INLINE NodePositionWithRef& operator=(const NodePositionWithRef& other) noexcept
		{
			node = other.node;
			ref = other.ref;
			return *this;
		}
				
		SLIB_INLINE NodePositionWithRef& operator=(NODE* other) noexcept
		{
			node = other;
			return *this;
		}
		
		SLIB_INLINE NODE& operator*() const noexcept
		{
			return *node;
		}
		
		SLIB_INLINE constexpr sl_bool operator==(const NodePositionWithRef& other) const noexcept
		{
			return node == other.node;
		}
		
		SLIB_INLINE constexpr sl_bool operator==(const NODE* other) const noexcept
		{
			return node == other;
		}
		
		SLIB_INLINE constexpr sl_bool operator!=(const NodePositionWithRef& other) const noexcept
		{
			return node != other.node;
		}
		
		SLIB_INLINE constexpr sl_bool operator!=(const NODE* other) const noexcept
		{
			return node != other;
		}
		
		SLIB_INLINE operator NODE*() const noexcept
		{
			return node;
		}
		
		SLIB_INLINE NodePositionWithRef& operator++() noexcept
		{
			node = node->getNext();
			return *this;
		}
		
	public:
		NODE* node;
		Ref<Referable> ref;
		
	};
	
}

#endif
