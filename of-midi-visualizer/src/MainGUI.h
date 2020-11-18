#pragma once
#include "ofMain.h"
#include <iostream>
#include "GUIComponent.h"
#include "MIDIPortStateGUI.h"
/*
* Class that represents all the GUI components for the application
* which should be visible to the user during the entire program runtime.
* Manages the display location of its GUI sub-components.
* Sub-components are available publically to allow access to their contents
* Initializes the underlying MIDIPortState upon construction
*/
class MainGUI : public GUIComponent {
public:
    MainGUI();

    void setupGUI();

    void drawGUI() override;

    void setMenuXY(float x, float y) override;

    float getMenuHeight();

    void addToggle(std::string uid, bool startToggled = false);

    void initToggles();

    void setToggleState(std::string uid, bool toggled);
    
    bool isToggled(std::string uid);

    ofColor getBackgroundColor();

    MIDIPortStateGUI midiPortStateGUI;
private:

    ofxColorSlider backgroundColorSelector;
    ofParameterGroup animationTogglesGroup;

    std::map<std::string, ofParameter<bool>> toggles;

};