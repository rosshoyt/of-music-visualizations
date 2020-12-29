#pragma once
#include "ofMain.h"
#include <iostream>
//#include "GUIComponent.h"
#include "GUIAnimationComponent.h"
#include "AppConstants.h"

extern const int ANIMATION_WIDTH, ANIMATION_HEIGHT, OFXGUI_DEF_WIDTH, RIGHT_CONTROLBAR;
extern const std::string IMAGE_FOLDER_RELATIVE;
extern const std::set<std::string> IMAGE_FILE_EXTENSIONS;

typedef std::pair<ofParameter<bool>*, ofImage*> ImageToggleImagePair;
/*
* Class that manages GUI component parameters for the OF application, 
* and houses global controls for background color and window aspect ratio
* Always visible to the user during the program runtime.
*/
class MainGUI : public GUIAnimationComponent {
public:
    MainGUI();

    // Inherited via GUIAnimationComponent
    virtual void setup() override;
    virtual void update() override;
    virtual void draw() override;
    
    void setupGUI();

    // TODO remove?
    void drawGUI() override;

    void addToggle(std::string uid, bool startToggled = false);

    void initToggles();

    void setToggleState(std::string uid, bool toggled);
    
    // TODO rename to be clearer that this method doesn't refer to the MainGUI being toggled
    bool isToggled(std::string uid);

    ofColor getBackgroundColor();

    void setupParameterListeners() override;

private:
    ofParameterGroup backgroundImageGroup;
    ofParameter<ofColor> backgroundColorSelector;

    ofParameterGroup animationTogglesGroup;
    std::map<std::string, ofParameter<bool>> animationToggles;

    //ofParameter<bool> drawBackgroundImageToggle;
    
    std::map<std::string,ImageToggleImagePair> images;
    //std::map<std::string, ofParameter<bool>> imageToggles;

    // TODO create collapsible parameter group for the resize window params
    //ofParameterGroup aspectRatioGroup;
    ofxButton resizeWindowButton;
    ofParameter<int> animationWidth, animationHeight;

    // private methods
    void resizeWindow();

  
};