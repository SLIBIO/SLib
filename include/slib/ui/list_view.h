/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_LIST_VIEW
#define CHECKHEADER_SLIB_UI_LIST_VIEW

#include "definition.h"

#include "scroll_view.h"

namespace slib
{
	class ListView;
	
	class SLIB_EXPORT IListViewAdapter
	{
	public:
		IListViewAdapter();

		virtual ~IListViewAdapter();

	public:
		virtual sl_uint64 getItemsCount(ListView* lv) = 0;
		
		virtual Ref<View> getView(ListView* lv, sl_uint64 index, View* original) = 0;
		
		virtual sl_ui_len getAverageItemHeight(ListView* lv);
		
	};
	
	template <class T>
	class SLIB_EXPORT IListViewAdapterT : public IListViewAdapter
	{
	public:
		virtual void onBindView(ListView* lv, sl_uint64 index, T* view) = 0;
		
	public:
		Ref<View> getView(ListView* lv, sl_uint64 index, View* original) override
		{
			Ref<T> view;
			if (original) {
				view = (T*)original;
			} else {
				view = new T;
			}
			onBindView(lv, index, view.get());
			return view;
		}
		
	};
	
	
	
	class _ListContentView;
	
	class SLIB_EXPORT ListView : public VerticalScrollView
	{
		SLIB_DECLARE_OBJECT
		
	public:
		ListView();
		
		~ListView();
		
	public:
		void setAdapter(const Ptr<IListViewAdapter>& adapter);
		
		void refreshItems();
		
	protected:
		void onScroll(sl_scroll_pos x, sl_scroll_pos y) override;
		
		void onResize(sl_ui_len x, sl_ui_len y) override;
		
	protected:
		void _checkUpdateContent(sl_bool fromDraw);
		
		void _initStatus();
		
		void _resetAdapter();
		
		void _layoutItemViews(sl_bool flagFromDraw, sl_bool flagFromScroll, sl_bool flagRefresh);
		
		sl_ui_len _measureItemWidth(const Ref<View>& itemView, sl_ui_len heightList);
		
		sl_ui_len _measureItemHeight(const Ref<View>& itemView, sl_ui_len heightList);
		
	protected:
		AtomicPtr<IListViewAdapter> m_adapter;
		
		Ref<_ListContentView> m_contentView;
		sl_bool m_flagResetAdapter;
		sl_bool m_flagRefreshItems;
		
		sl_uint64 m_countTotalItems;
		sl_uint64 m_indexFirstItem;
		sl_uint32 m_countVisibleItems;
		sl_ui_pos m_yFirstItem;
		Ref<View>* m_viewsVisibleItems;
		Ref<View>* m_viewsGoDownItems;
		Ref<View>* m_viewsGoUpItems;
		Ref<View>* m_viewsFreeItems;
		sl_ui_len* m_heightsVisibleItems;
		sl_ui_len* m_heightsTopItems;
		sl_ui_len* m_heightsBottomItems;
		sl_ui_len* m_heightsGoDownItems;
		sl_ui_len* m_heightsGoUpItems;
		sl_ui_len m_averageItemHeight;
		double m_averageMidItemHeight;
		sl_ui_len m_heightTotalItems;
		sl_ui_pos m_lastScrollY;
		
		sl_int32 m_lockCountLayouting;
		
		friend class _ListContentView;
		
	};

}

#endif
