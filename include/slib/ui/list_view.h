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
	
	
	
	class _priv_ListContentView;
	
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
		
		Ref<_priv_ListContentView> m_contentView;
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
		
		friend class _priv_ListContentView;
		
	};

}

#endif
