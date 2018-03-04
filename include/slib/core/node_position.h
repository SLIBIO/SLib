/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		SLIB_INLINE NODE& operator++() noexcept
		{
			node = node->getNext();
			return node;
		}
		
	public:
		NODE* node;
		Ref<Referable> ref;
		
	};
	
}

#endif
