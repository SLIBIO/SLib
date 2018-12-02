#include <slib.h>

#include "../gen/resources.h"

using namespace slib;

class MainMenu : public example::ui::MainMenu
{
public:
	void onOpen() override;
	
	void onClickHelloWorld(View* view);
	
	void onClickViewPager(View* view);
	
};
