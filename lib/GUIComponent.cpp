#include "GUIComponent.h"

GUIComponent::GUIComponent() {
	gui.setup();
}

//void GUIComponent::setupGUI() {
//	setMenuXY(x, y);
//}

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

//void GUIComponent::setMenuXY(int x, int y) {
//	menuX = x, menuY = y;
//}