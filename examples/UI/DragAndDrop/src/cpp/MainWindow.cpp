#include "MainWindow.h"

void MainWindow::onCreate()
{
	DragItem drag;
	drag.setText("add-button");
	drag.setDraggingSize(100, 50);
	drag.setDraggingImage(drawable::button::getImage());
	btnDrag->setDragItem(drag);
	linearDrop->setDroppable();
	linearDrop->setOnDropEvent([this](View*, UIEvent* ev) {
		if (ev->getDragItem().getText() == "add-button") {
			ev->setDragOperation(DragOperations::Copy);
			switch (ev->getAction()) {
				case UIAction::DropEnter:
					{
						Ref<Button> button = new Button;
						button->setWidthWrapping();
						button->setHeightWrapping();
						button->setCenterHorizontal();
						button->setPadding(5);
						button->setMargin(5);
						button->setBackgroundColor(Color(0, 128, 0, 255));
						button->setBoundRadius(4);
						button->setText("Button");
						button->setTextColor(Color::White);
						button->setId(String::format("btn%d", ev->getDragId()));
						linearDrop->addChild(button);
					}
					break;
				case UIAction::DropLeave:
					{
						Ref<View> button = linearDrop->getChildById(String::format("btn%d", ev->getDragId()));
						if (button.isNotNull()) {
							linearDrop->removeChild(button);
						}
					}
					break;
				case UIAction::Drop:
					{
						Ref<Button> button = Ref<Button>::from(linearDrop->getChildById(String::format("btn%d", ev->getDragId())));
						if (button.isNotNull()) {
							static int n = 0;
							n++;
							button->setText(String::format("Button%d", n));
						}
					}
					break;
				default:
					break;
			}
		}
	});
}
