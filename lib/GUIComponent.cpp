#include "GUIComponent.h"

GUIComponent::GUIComponent(std::string uid) : UID(uid) {
	gui.setup();
}

void GUIComponent::setGUIName(std::string name) {
	gui.setName(name);
}

std::string GUIComponent::getGUIName() {
	return gui.getName();
}

void GUIComponent::drawGUI() {
	gui.draw();
}

void GUIComponent::setMenuXY(float x, float y) {
	menuX = x, menuY = y;
	gui.setPosition(x, y);
	std::cout << "set menu position to " << menuX << "," << y << '\n';

}

float GUIComponent::getMenuHeight() {
	return gui.getHeight();
}
float GUIComponent::getMenuWidth() {
	return gui.getWidth();
}

void GUIComponent::setDefaultMenuWidth(float width) {
	gui.setDefaultWidth(width);
}

//void GUIComponent::setMenuXY(int x, int y) {
//	menuX = x, menuY = y;
//}