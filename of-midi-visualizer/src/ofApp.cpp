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
        // Set ofxGUI menu position for note grid animation
        component->setMenuXY(WIDTH, HEIGHT / 2);
        component->setMIDIPortState(&midiPortState);
        component->setup();
        
       

        // track UIDS of animations to add to the animationSelectorDropdown
        animationUIDS.push_back(pair.first);
    }

    // instantiate the animation selector dropdown and set position
    animationSelectorDropdown = new ofxDatGuiDropdown("SELECT AN ANIMATION", animationUIDS);
    animationSelectorDropdown->setPosition(WIDTH, 0);
    //// register to listen for change events
    animationSelectorDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    animationSelectorDropdown->expand();

    gui.setup();
    gui.setPosition(WIDTH, animationSelectorDropdown->getHeight()); // // HEIGHT / 3 * 2);//
    gui.add(drawAllAnimationsToggle.setup("Draw All Animations", false));
    


    // set animation ID to first animation entry in map (TODO refactor, currently must be at least 1 animation 
    currentAnimationUID = animationComponents.begin()->first;
      

    
    
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
        }
    }
    else {
        // draw selected animation
        animationComponents[currentAnimationUID]->draw();
    }

    animationSelectorDropdown->draw();
    // display main GUI
    gui.draw();
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
    noteGridAnimation.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    currentAnimationUID = animationUIDS[e.child];
    animationSelectorDropdown->expand();
}

