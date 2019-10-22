#pragma once

#include <slib.h>

using namespace slib;

class ExampleSensorApp : public MobileApp
{
	SLIB_DECLARE_APPLICATION(ExampleSensorApp)
public:
	ExampleSensorApp();
	
protected:
	void onStart() override;
	
private:
	Ref<Sensor> m_sensor;
	
};
