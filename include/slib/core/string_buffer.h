/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_CORE_STRING_BUFFER
#define CHECKHEADER_SLIB_CORE_STRING_BUFFER

#include "string.h"
#include "queue.h"


/**
 * @addtogroup core
 *  @{
 */
namespace slib
{

	/** @class StringBuffer
	 * @brief Queue containing string items. StringBuffer is not thread-safe.
	 */
	class SLIB_EXPORT StringBuffer
	{
	public:
		StringBuffer() noexcept;
		
		~StringBuffer() noexcept;

	public:

		/**
		 * Returns total length of all string items.
		 */
		sl_size getLength() const noexcept;

		/**
		 * Add string object to the queue.
		 */
		sl_bool add(const String& str) noexcept;
		
		/**
		 * Add string represented by StringData struct to the queue.
		 */
		sl_bool add(const StringData& str) noexcept;
		
		/**
		 * Add string pointed by buf to the queue.
		 */
		sl_bool addStatic(const sl_char8* buf, sl_size length) noexcept;
	
		/**
		 * Add all string items in buf to the current object and then clear buf.
		 */
		void link(StringBuffer& buf) noexcept;
		
		/**
		 * Clears all items.
		 */
		void clear() noexcept;
		
		/**
		 * Merge all items and returns the merged string.
		 */
		String merge() const noexcept;

		/**
		 * Merge all items and returns it as memory.
		 */
		Memory mergeToMemory() const noexcept;
		
	private:
		LinkedQueue<StringData> m_queue;
		sl_size m_len;
		
	};

}
/// @}


/**
 * @addtogroup core
 *  @{
 */
namespace slib
{

	/** @class StringBuffer16
	 * @brief Queue containing String16 items. StringBuffer16 is not thread-safe.
	 */
	class SLIB_EXPORT StringBuffer16
	{
	public:
		StringBuffer16() noexcept;
		
		~StringBuffer16() noexcept;

	public:
		/**
		 * Returns total length of all string items.
		 */
		sl_size getLength() const noexcept;
		
		/**
		 * Add string object to the queue.
		 */
		sl_bool add(const String16& str) noexcept;
		
		/**
		 * Add string represented by StringData struct to the queue.
		 */
		sl_bool add(const StringData& str) noexcept;
		
		/**
		 * Add string pointed by buf to the queue.
		 */
		sl_bool addStatic(const sl_char16* buf, sl_size length) noexcept;

		/**
		 * Add all string items in buf to the current object and then clear buf.
		 */
		void link(StringBuffer16& buf) noexcept;
		
		/**
		 * Clears all items.
		 */
		void clear() noexcept;

		/**
		 * Merge all items and returns the merged string.
		 */
		String16 merge() const noexcept;

		/**
		 * Merge all items and returns it as memory.
		 */
		Memory mergeToMemory() const noexcept;
		
	private:
		LinkedQueue<StringData> m_queue;
		sl_size m_len;
		
	};
	
}

/// @}

#endif
