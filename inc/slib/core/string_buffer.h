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
	 * @brief Queue containing string items.
	 */
	class SLIB_EXPORT StringBuffer : public Object
	{
	public:
		StringBuffer();

	public:

		/**
		 * Returns total length of all string items.
		 */
		sl_size getLength() const;
		
		/**
		 * Add string object to the queue. This function is not thread-safe.
		 */
		sl_bool add_NoLock(const String& str);

		/**
		 * Add string object to the queue. This function is thread-safe.
		 */
		sl_bool add(const String& str);
		
		/**
		 * Add string represented by StringData struct to the queue. This function is not thread-safe.
		 */
		sl_bool add_NoLock(const StringData& str);

		/**
		 * Add string represented by StringData struct to the queue. This function is thread-safe.
		 */
		sl_bool add(const StringData& str);
		
		/**
		 * Add string pointed by buf to the queue. This function is not thread-safe.
		 */
		sl_bool addStatic_NoLock(const sl_char8* buf, sl_size length);

		/**
		 * Add string pointed by buf to the queue. This function is thread-safe.
		 */
		sl_bool addStatic(const sl_char8* buf, sl_size length);
		
		/**
		 * Add all string items in buf to the current object and then clear buf. This function is not thread-safe.
		 */
		void link_NoLock(StringBuffer& buf);
		
		/**
		 * Add all string items in buf to the current object and then clear buf. This function is thread-safe.
		 */
		void link(StringBuffer& buf);
		
		/**
		 * Clears all items. This function is not thread-safe.
		 */
		void clear_NoLock();

		/**
		 * Clears all items. This function is thread-safe.
		 */
		void clear();
		
		/**
		 * Merge all items and returns the merged string. This function is not thread-safe.
		 */
		String merge_NoLock() const;

		/**
		 * Merge all items and returns the merged string. This function is thread-safe.
		 */
		String merge() const;

		/**
		 * Merge all items and returns it as memory. This function is not thread-safe.
		 */
		Memory mergeToMemory_NoLock() const;
		
		/**
		 * Merge all items and returns it as memory. This function is thread-safe.
		 */
		Memory mergeToMemory() const;
		
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
	 * @brief Queue containing string items.
	 */
	class SLIB_EXPORT StringBuffer16 : public Object
	{
	public:
		StringBuffer16();

	public:
		/**
		 * Returns total length of all string items.
		 */
		sl_size getLength() const;
		
		/**
		 * Add string object to the queue. This function is not thread-safe.
		 */
		sl_bool add_NoLock(const String16& str);

		/**
		 * Add string object to the queue. This function is thread-safe.
		 */
		sl_bool add(const String16& str);
		
		/**
		 * Add string represented by StringData struct to the queue. This function is not thread-safe.
		 */
		sl_bool add_NoLock(const StringData& str);

		/**
		 * Add string represented by StringData struct to the queue. This function is thread-safe.
		 */
		sl_bool add(const StringData& str);
		
		/**
		 * Add string pointed by buf to the queue. This function is not thread-safe.
		 */
		sl_bool addStatic_NoLock(const sl_char16* buf, sl_size length);
		
		/**
		 * Add string pointed by buf to the queue. This function is thread-safe.
		 */
		sl_bool addStatic(const sl_char16* buf, sl_size length);

		/**
		 * Add all string items in buf to the current object and then clear buf. This function is not thread-safe.
		 */
		void link_NoLock(StringBuffer16& buf);

		/**
		 * Add all string items in buf to the current object and then clear buf. This function is thread-safe.
		 */
		void link(StringBuffer16& buf);
		
		/**
		 * Clears all items. This function is not thread-safe.
		 */
		void clear_NoLock();
		
		/**
		 * Clears all items. This function is thread-safe.
		 */
		void clear();
		
		/**
		 * Merge all items and returns the merged string. This function is not thread-safe.
		 */
		String16 merge_NoLock() const;

		/**
		 * Merge all items and returns the merged string. This function is thread-safe.
		 */
		String16 merge() const;
		
		/**
		 * Merge all items and returns it as memory. This function is not thread-safe.
		 */
		Memory mergeToMemory_NoLock() const;

		/**
		 * Merge all items and returns it as memory. This function is thread-safe.
		 */
		Memory mergeToMemory() const;
		
	private:
		LinkedQueue<StringData> m_queue;
		sl_size m_len;
		
	};
	
}

/// @}

#endif
