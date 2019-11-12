#include "MainPage.h"

void MainPage::onOpen()
{
	UI::dispatchToUiThread([this]{
		Application::grantPermissions(AppPermissions::ReadContacts, [this]() {
			StringBuffer sb;
			auto contacts = Device::getAllContacts();
			for (auto& contact : contacts) {
				sb.add(contact.toJson().toJsonString());
				sb.add("\n");
			}
			lblReport->setText(sb.merge());
		});
	}, 1000);
}
