#include "app.h"

SLIB_DEFINE_APPLICATION(ExampleSensorApp, MobileApp)

ExampleSensorApp::ExampleSensorApp()
{
}

void ExampleSensorApp::onStart()
{
	getContentView()->setBackgroundColor(Color::White);
	
	auto label = New<LabelView>();
	label->setWidthWrapping(UIUpdateMode::Init);
	label->setHeightWrapping(UIUpdateMode::Init);
	label->setCenterInParent(UIUpdateMode::Init);
	label->setFont(Font::create("Arial", UI::dpToPixel(20)), UIUpdateMode::Init);
	label->setMultiLine(MultiLineMode::Multiple);
	addViewToContent(label);
	
	Device::grantPermissions(DevicePermissions::AccessFineLocation, [this, label]() {
		SensorParam param;
		param.flagUseLocation = sl_true;
		param.onLocationChanged = [label](Sensor*, const GeoLocation& location) {
			label->setText(String::format("Latitude = %.4f\nLongitude = %.4f\nAltitude = %.4f", location.latitude, location.longitude, location.altitude));
		};
		m_sensor = Sensor::create(param);
	});
	
}
