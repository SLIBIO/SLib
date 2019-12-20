#pragma once

#include "app.h"

class PayPage : public ui::PayPage
{
public:
	void onOpen() override;
	
private:
	WeChatPaymentOrder m_order;
	
};
