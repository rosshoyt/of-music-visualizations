#include "MainGUI.h"

MainGUI::MainGUI() : GUIComponent("Main Controls") {}

void MainGUI::setupGUI() {
    gui.add(backgroundColorSelector.set("Background Color", ofColor::darkGray, ofColor(0, 0), ofColor(255, 255)));
    //backgroundColorSelector.maximize(); // starts in un-collapsed state
    animationTogglesGroup.setName("Animation Toggles");
    
    // TODO fix bug where default value doesn't set correctly
    gui.add(animationWidth.set("Animation Width:", ANIMATION_WIDTH, 300, 3000));
    gui.add(animationHeight.set("Animation Height:", ANIMATION_HEIGHT, 300, 3000));
    gui.add(resizeWindowButton.setup("Reset Animation Size"));
    resizeWindowButton.addListener(this, &MainGUI::resizeWindow);
}

void MainGUI::drawGUI() {
    GUIComponent::drawGUI();
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

void MainGUI::setToggleState(std::string uid, bool toggled){
    toggles.at(uid).set(toggled);
}

bool MainGUI::isToggled(std::string uid) {
    return toggles.at(uid);
}

ofColor MainGUI::getBackgroundColor() {
    return backgroundColorSelector;
}

void MainGUI::resizeWindow(){
    ofSetWindowShape(animationWidth + RIGHT_CONTROLBAR, animationHeight);
}
