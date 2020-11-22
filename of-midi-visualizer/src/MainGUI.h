#pragma once
#include "ofMain.h"
#include <iostream>
#include "GUIComponent.h"
/*
* Class that manages GUI component parameters for the OF application
* Always visible to the user during the program runtime.
*/
class MainGUI : public GUIComponent {
public:
    MainGUI();

    void setupGUI();

    void drawGUI() override;

    void addToggle(std::string uid, bool startToggled = false);

    void initToggles();

    void setToggleState(std::string uid, bool toggled);
    
    bool isToggled(std::string uid);

    ofColor getBackgroundColor();

private:

    ofxColorSlider backgroundColorSelector;
    ofParameterGroup animationTogglesGroup;

    std::map<std::string, ofParameter<bool>> toggles;
};