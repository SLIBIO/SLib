#pragma once

#include <slib.h>

using namespace slib;

class QRCodeExampleApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(QRCodeExampleApp)
public:
	QRCodeExampleApp();
	
protected:
	void onStart() override;
	
private:
	Ref<ImageView> m_imgEncoded;
	Ref<EditView> m_editEncoding;
	Ref<QRCodeScanner> m_QRCodeScanner;
	Ref<EditView> m_editDecoded;

};
