#include "MainGUI.h"

MainGUI::MainGUI() : GUIAnimationComponent("Main Controls") {}

void MainGUI::setup(){}

void MainGUI::update(){
    ofBackground(backgroundColorSelector);
}

void MainGUI::draw() {
    ofSetColor(255);
    for (auto& entry : images) {
        auto toggleImagePair = entry.second;
        if (*toggleImagePair.first) {
            //std::cout << "Drawing image " << entry.first <<"\n";
            toggleImagePair.second->draw(0, 0);
        }
    }
   
}

void MainGUI::setupGUI() {
    // read in background images for background
    ofDirectory dir(IMAGE_FOLDER_RELATIVE);

    for (auto& file : dir.getFiles()) {
        auto extension = file.getExtension();
        std::cout << "Looking at file " << file.getFileName() << " with extension " << extension << "\n";
        if (IMAGE_FILE_EXTENSIONS.count(extension)) {
            auto* image = new ofImage(file.getAbsolutePath());
            std::string imageID = file.getFileName();
            auto* imageToggle = new ofParameter<bool>();
            backgroundImageGroup.add(imageToggle->set(imageID, false));
            //imageToggles.insert({ imageNameImagePair.first, imageToggle });
            images.insert({ imageID, { imageToggle, image } });
            std::cout << "Inserted the image\n";
        }
    }

    // setup background image param group
    backgroundImageGroup.setName("Background Images");
  
    gui.add(backgroundImageGroup);


    // setup background color
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
    animationToggles.insert({ uid, toggle });
}

void MainGUI::initToggles() {
    for (auto& uidParamPair : animationToggles) {
        auto uid = uidParamPair.first;
        auto param = uidParamPair.second;
        animationTogglesGroup.add(param.set("Show " + uid, param));
    }
    gui.add(animationTogglesGroup);
}

void MainGUI::setToggleState(std::string uid, bool toggled){
    animationToggles.at(uid).set(toggled);
}

bool MainGUI::isToggled(std::string uid) {
    return animationToggles.at(uid);
}

ofColor MainGUI::getBackgroundColor() {
    return backgroundColorSelector;
}

void MainGUI::setupParameterListeners() {
    addParameterListener(backgroundColorSelector);
    // TODO implement ability to control a parameter group with a single MIDI CC#, i.e.
    //addParameterListener<>(animationTogglesGroup)
}

void MainGUI::resizeWindow(){
    ofSetWindowShape(animationWidth + RIGHT_CONTROLBAR, AnimationComponent::getAnimationHeight());
}


