#include "MainGUI.h"

MainGUI::MainGUI() : GUIComponent("Main Controls") {}

void MainGUI::setupGUI() {

    midiPortStateGUI.setupGUI();
    gui.add(backgroundColorSelector.setup("Background Color", ofColor::darkGray, ofColor(0, 0), ofColor(255, 255)));
    backgroundColorSelector.maximize(); // starts in un-collapsed state
    animationTogglesGroup.setName("Animation Toggles");

    gui.loadFromFile("settings.xml");

}

void MainGUI::drawGUI() {
    midiPortStateGUI.drawGUI();
    GUIComponent::drawGUI();
}

void MainGUI::setMenuXY(float x, float y) {
    // TODO refactor - move gui position management to GUIManager class
    menuX = x, menuY = y;

    midiPortStateGUI.setMenuXY(x, y);

    gui.setPosition(x, y + midiPortStateGUI.getMenuHeight());

}

float MainGUI::getMenuHeight() {
    return midiPortStateGUI.getMenuHeight() + GUIComponent::getMenuHeight();
}

void MainGUI::addToggle(std::string uid, bool startToggled) {
    ofParameter<bool> toggle = startToggled;
    toggles.insert({ uid, toggle });
}

void MainGUI::initToggles() {
    for (auto& uidParamPair : toggles) {
        auto uid = uidParamPair.first;
        auto param = uidParamPair.second;
        animationTogglesGroup.add(param.set("Show " + uid, param));
    }
    gui.add(animationTogglesGroup);
}

bool MainGUI::isToggled(std::string uid) {
    if (toggles.count(uid) > 0)
        return toggles[uid];
    return false; // TODO could if assert map contains provided uid
}

ofColor MainGUI::getBackgroundColor() {
    return backgroundColorSelector;
}
