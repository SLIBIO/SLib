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

#include "ListViewPage.h"

struct MyListItem
{
	int id;
	Ref<Drawable> icon;
	String name;
};

void ListViewPage::onOpen()
{
	List<MyListItem> list;
	for (int i = 0; i < 100; i++) {
		MyListItem item;
		item.id = i;
		item.icon = i % 2 ? example::drawable::lion::get() : example::drawable::tiger::get();
		item.name = String::format("List Row %d", i);
		list.add(item);
	}
	listView->setAdapter(ListViewAdapter<MyListItem, example::ui::ListViewItem>::create(list, [](MyListItem& data, example::ui::ListViewItem* row, View*) {
		row->icon->setSource(data.icon);
		row->text->setText(data.name);
		int id = data.id;
		row->btnDetail->setOnClick([id](View*) {
			UI::showAlert(String::format("Row %d is clicked on", id));
		});
	}));
}
