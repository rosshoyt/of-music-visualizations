#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : GUIComponent("Main Control Panel"), abletonController() {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();

    // Track Animation Components 
    animationComponentsMap.insert({ noteGridAnimation.getUID(), &noteGridAnimation });
    animationComponentsMap.insert({ animated3DMesh.getUID(), &animated3DMesh });
    //animationComponentsMap.insert({ meshFromImage.getUID(), &meshFromImage });
    animationComponentsMap.insert({ adsrVisualizer.getUID(), &adsrVisualizer });
    animationComponentsMap.insert({ circleOfFifths.getUID(), &circleOfFifths });
    
   
       
    // setup animation components
    for (auto& uidComponentPair : animationComponentsMap) {
        auto component = uidComponentPair.second;
        component->setMIDIPortState(&midiPortStateGUI.getMIDIPortState());
        component->setup();
        
        
        // track UIDS of animations to add to the animationSelectorDropdown
        animationUIDS.push_back(uidComponentPair.first);

        // Track the component in the GUIComponents list as well
        guiComponentsList.push_back(component);

    }

    // Also Track GUI-Only Components 
    guiComponentsList.push_back(this);
    guiComponentsList.push_back(&midiPortStateGUI);

    for (auto& component : guiComponentsList) {
        component->setDefaultMenuWidth(RIGHT_CONTROLBAR);
        component->setGUIName(component->getUID());
        component->setupGUI();
    }

    //setupGUI(); // TODO move before any tracking or setup of other components


    // set animation ID to first animation entry in map (TODO refactor, currently must be at least 1 animation 
    currentAnimationUID = animationComponentsMap.begin()->first;
    
    // force window resize to 
    windowResized(ofGetWidth(), ofGetHeight());
    
}

//--------------------------------------------------------------
void ofApp::setupGUI() {
    // instantiate the animation selector dropdown and set position
    animationSelectorDropdown = new ofxDatGuiDropdown("SELECT AN ANIMATION", animationUIDS);
   
    //// register to listen for change events
    animationSelectorDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    animationSelectorDropdown->expand();

    gui.add(drawAllAnimationsToggle.setup("Draw All Animations", false));
}

//--------------------------------------------------------------
void ofApp::update(){
    if (drawAllAnimationsToggle) {
        // update all animations 
        for (auto animation : animationComponentsMap) {
            animation.second->update();
        }
    } else {
        // draw selected animation
        animationComponentsMap[currentAnimationUID]->update();
    }

    // update dropdown menu
    animationSelectorDropdown->update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (drawAllAnimationsToggle) {
        // draw all animations
        for (auto animation : animationComponentsMap) {
            animation.second->draw();
            //animation.second->drawGUI();
        }
    }
    else {
        // draw selected animation
        auto animation = animationComponentsMap[currentAnimationUID];
        animation->draw();
        //animation->drawGUI();
    }

    animationSelectorDropdown->draw();
    midiPortStateGUI.drawGUI();
    // display the main GUI
    drawGUI();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        std::cout << "Pressed Space\n";
        abletonController.processSpacebar();
    }
    animated3DMesh.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //gui.setPosition(w - RIGHT_CONTROLBAR, animationSelectorDropdown->getHeight());// // HEIGHT / 3 * 2);//
    animationSelectorDropdown->setPosition(w - RIGHT_CONTROLBAR, 0);
    
    // update all animations
    for (auto& uidComp : animationComponentsMap) {
        std::cout << "Resizing " << uidComp.first << "\n";
        auto animationComponent = uidComp.second;
        animationComponent->setAnimationDimensions(w - RIGHT_CONTROLBAR, h);
        
        animationComponent->resized(w, h);
    }

    // update all guis
    float currX, currY = animationSelectorDropdown->getHeight();
    for (auto& guiComponent : guiComponentsList) {
        guiComponent->setDefaultMenuWidth(RIGHT_CONTROLBAR);
        guiComponent->setMenuXY(w - RIGHT_CONTROLBAR, currY);
        currY += guiComponent->getMenuHeight();
    }

}
    
//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    currentAnimationUID = animationUIDS[e.child];
    animationSelectorDropdown->expand();
}

