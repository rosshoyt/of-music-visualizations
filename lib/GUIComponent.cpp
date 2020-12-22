#include "GUIComponent.h"

GUIComponent::GUIComponent(std::string uid) : UID(uid) {
	gui.setup();
	setGUIName(uid);
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

void GUIComponent::loadSavedSettings(){
	gui.loadFromFile(DEFAULT_SETTINGS_FILE_NAME);
}

void GUIComponent::setMenuXY(float x, float y) {
	menuX = x, menuY = y;
	gui.setPosition(x, y);
	//std::cout << "set menu position of " << getUID() << " to " << menuX << "," << y << '\n';
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

// empty method definition, can be implemented by inheriting classes
void GUIComponent::setupParameterListeners() {}

GUIParameterNode* GUIComponent::lastClickedParam = new GUIParameterNode();

