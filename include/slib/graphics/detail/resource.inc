/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{
	
	class _private_ImageResourceEntry
	{
	public:
		sl_bool flagValid;

		sl_uint32 width;
		sl_uint32 height;

		const sl_uint8* source_bytes;
		sl_uint32 source_size;

		sl_int32 lock;
		void* image;
		sl_bool flag_load;

	public:
		Ref<Image> getImage();

		Ref<Image> getMatchingImage(sl_uint32 width, sl_uint32 height);

	};
	
	class _private_ImageResourceFreeStatic
	{
	public:
		_private_ImageResourceFreeStatic(_private_ImageResourceEntry* entries);

		~_private_ImageResourceFreeStatic();

	private:
		_private_ImageResourceEntry* m_entries;

	};
	
	Ref<Image> _private_ImageResource_getImage(_private_ImageResourceEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight);
	
	List< Ref<Image> > _private_ImageResource_getImages(_private_ImageResourceEntry* entries);
	
	Ref<Drawable> _private_ImageResource_get(_private_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height);

}
