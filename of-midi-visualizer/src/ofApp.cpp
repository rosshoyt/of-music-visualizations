#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController() {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();
    
    // setup the MIDI port
    midiPortSettings.numChannels = 12;
    midiPortState.setupMIDIPortState(midiPortSettings);

    animationComponents.insert({ noteGridAnimation.getUID(), &noteGridAnimation });
    animationComponents.insert({ animated3DMesh.getUID(), &animated3DMesh });
    //animationComponents.insert({ meshFromImage.getUID(), &meshFromImage });
    animationComponents.insert({ adsrVisualizer.getUID(), &adsrVisualizer });
    animationComponents.insert({ circleOfFifths.getUID(), &circleOfFifths });
    
    // setup animation components
    for (auto& pair : animationComponents) {
        
        auto component = animationComponents[pair.first];
        component->setMIDIPortState(&midiPortState);
        component->setup();
        component->setupGUI();
        //component->setMenuXY();

        // track UIDS of animations to add to the animationSelectorDropdown
        animationUIDS.push_back(pair.first);
    }
   
    // set animation ID to first animation entry in map (TODO refactor, currently must be at least 1 animation 
    currentAnimationUID = animationComponents.begin()->first;
     
    setupGUI();
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
        for (auto animation : animationComponents) {
            animation.second->update();
        }
    } else {
        // draw selected animation
        animationComponents[currentAnimationUID]->update();
    }

    // update dropdown menu
    animationSelectorDropdown->update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (drawAllAnimationsToggle) {
        // draw all animations
        for (auto animation : animationComponents) {
            animation.second->draw();
            animation.second->drawGUI();
        }
    }
    else {
        // draw selected animation
        auto animation = animationComponents[currentAnimationUID];
        animation->draw();
        animation->drawGUI();
    }

    animationSelectorDropdown->draw();
    // display main GUI
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
    gui.setPosition(w - RIGHT_CONTROLBAR, animationSelectorDropdown->getHeight()); // // HEIGHT / 3 * 2);//
    animationSelectorDropdown->setPosition(w - RIGHT_CONTROLBAR, 0);
    
    for (auto& uidComp : animationComponents) {
        std::cout << "Resizing " << uidComp.first << "\n";
        auto component = uidComp.second;
        component->setAnimationDimensions(w - RIGHT_CONTROLBAR, h);
        component->setMenuXY(w - RIGHT_CONTROLBAR, h / 2);
        component->resized(w, h);
    }

}
    
//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    currentAnimationUID = animationUIDS[e.child];
    animationSelectorDropdown->expand();
}

