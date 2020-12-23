#pragma once
#include "ofMain.h"
#include <iostream>
#include "GUIComponent.h"
#include "AppConstants.h"

extern const int ANIMATION_WIDTH, ANIMATION_HEIGHT, OFXGUI_DEF_WIDTH, RIGHT_CONTROLBAR;

/*
* Class that manages GUI component parameters for the OF application, 
* and houses global controls for background color and window aspect ratio
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

    void setupParameterListeners() override {
        addParameterListener(backgroundColorSelector);
    }
private:
    ofParameter<ofColor> backgroundColorSelector;
    ofParameterGroup animationTogglesGroup;

    std::map<std::string, ofParameter<bool>> toggles;

    // TODO create collapsible parameter group for the resize window params
    //ofParameterGroup aspectRatioGroup;
    ofxButton resizeWindowButton;
    ofParameter<int> animationWidth, animationHeight;

    // private methods
    void resizeWindow();
};