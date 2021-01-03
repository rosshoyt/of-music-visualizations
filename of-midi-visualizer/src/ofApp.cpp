#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() : abletonController() {}

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetWindowTitle("OpenFrameworks MIDI Visualizer - Ross Hoyt Music");
    ofEnableAntiAliasing();

   
    // setup the mainGUI animation (TODO - create a list of AnimationComponents to setup at same time)
    mainGUI.setup();

     // add MIDI Port State GUI to the permanent gui components list
    permanentGUIComponentsList.push_back(&midiPortState);
    permanentGUIComponentsList.push_back(&mainGUI);

    
    // setup all GUI Animation components which are not animations
    for (auto& guiComponent : permanentGUIComponentsList) {
        guiComponent->setupGUI();
        // also add them to the list for all GUI components for updating positions on resize
        guiComponentsList.push_back(guiComponent);
    }

    // add Main GUI to the permanent GUI Animation Components list
    permanentGUIAnimationComponentsList.push_back(&mainGUI);

    // track animation components
    midiAnimationGUIComponentsList.push_back(&lfoVisualizer);
    midiAnimationGUIComponentsList.push_back(&adsrVisualizer);
    midiAnimationGUIComponentsList.push_back(&noteGridAnimation);
    midiAnimationGUIComponentsList.push_back(&animated3DMesh);
    midiAnimationGUIComponentsList.push_back(&circleOfFifths);
    //midiAnimationGUIComponentsList.push_back(&meshFromImage);
   
    
    // setup animation components
    for (auto& component : midiAnimationGUIComponentsList) {
        component->setMIDIPortState(&midiPortState);
        component->setup();
        component->setupGUI();
        // add a toggle to turn the animation on and off from the main gui
        mainGUI.addToggle(component->getUID(), false);
        // add the component to the GUI Component list (TODO separate MIDIAnimationGUIComponent from GUIComponent parent class)
        guiComponentsList.push_back(component);
    }

    // initialize the animation toggles in the main gui
    mainGUI.initToggles();
    
    // set an animation to start with
    mainGUI.setToggleState(circleOfFifths.getUID(), true);
    
    // load all GUI panel settings 
    for (auto& guiComponent : guiComponentsList) {
        guiComponent->loadSavedSettings();
        guiComponent->setupParameterListeners();
    }
    
    windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    for (auto& permanentAnimation : permanentGUIAnimationComponentsList) {
        permanentAnimation->update();
    }
    for (auto& animation : midiAnimationGUIComponentsList) {
        if(mainGUI.isToggled(animation->getUID()))
            animation->update();
    }    
}

//--------------------------------------------------------------
void ofApp::draw(){
    // draw all permanent animations (backround image/color, etc)
    for (auto& permanentAnimation : permanentGUIAnimationComponentsList) {
        permanentAnimation->draw();
    }
    
    for (auto& animation: midiAnimationGUIComponentsList) {
        if (mainGUI.isToggled(animation->getUID())) {
            animation->draw();
            animation->drawGUI();
        }
    }
    for (auto& guiComponent : permanentGUIComponentsList) {
        guiComponent->drawGUI();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
    case ' ':
        //std::cout << "Pressed Space\n";
        abletonController.processSpacebar();
    }
    animated3DMesh.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    std::cout << "Resizing: current width/height = " << w << "/" << h << "\n";
    //AnimationComponent::setAnimationDimensions(;
    AnimationComponent::setAnimationDimensions(w - RIGHT_CONTROLBAR, h);
    // update all animation posisions
    for (auto& animation : midiAnimationGUIComponentsList) {
        // TODO could set Animation dimensions statically
        //animation->setAnimationDimensions(animationDisplayWidth, h);
        animation->resized(w, h);
    }

    repositionGUIComponents();
    
//    for (auto& component : guiComponentsList) {
//        auto bottomOfNewGUIComponent = y + component->getMenuHeight();
//        // check if we are going to position GUI off-screen
//        if (bottomOfNewGUIComponent > ANIMATION_HEIGHT) {
//            // TODO refactor the positioning of the GUI components
//            y = 0; // reset Y
//            x += OFXGUI_DEF_WIDTH; // shift right to the next column
//            // TODO ensure doesn't paint gui menu in off-screen columns
//        }
//        component->setMenuXY(x, y);
//        y += component->getMenuHeight() + guiYSpacer;
//
//        // Check if we just drew the midi port state, move over a column for the rest of components.
//        // TODO create functions to abstract the column position management, or create
//        // a separate class.
//        if (component->getUID() == midiPortState.getUID()) {
//            y = 0; // reset Y
//            x += OFXGUI_DEF_WIDTH;
//        }
//        std::cout << "Placing " << component->getUID() << " at " << x << "/" << y << " (x/y) \n";
//    }
}

void ofApp::repositionGUIComponents(){
    // put all gui panels into a map based on size
    std::map<float, std::queue<GUIComponent*>> sizeGuiMap;
    for(auto& guiComponent : guiComponentsList){
        //float menuHeight = guiComponent->getMenuHeight();
        sizeGuiMap[guiComponent->getMenuHeight()].push(guiComponent);
    }
    
    
    
    // or
    // update all GUI panel positions
    //float guiYSpacer = 10; // TODO move to global variable, create WindowManager
    auto startingX = AnimationComponent::getAnimationWidth();
    //float x = , y = 0;
    
    
    int numCols = 3; // TODO refactor where number of columns for GUI panels to display is determined
    
    std::vector<float> columnSizes(numCols);
    
    
    
    // reverse iterate through the gui components map by the size of each
    for (auto it = sizeGuiMap.rbegin(); it != sizeGuiMap.rend(); it++) {
        
        // find smallest column
        int column = 0;
        for(int j = column + 1; j < columnSizes.size(); ++j){
            if(columnSizes[j] < columnSizes[column])
                column = j;
        }
        
        // get the height of the component(s)' menu
        auto height = it->first;
        auto compQueue = it->second;
        
        // get the first component (probably the only component in queue)
        auto guiComp = compQueue.front();
        compQueue.pop();  // remove from queue
        
        auto x = startingX + column * OFXGUI_DEF_WIDTH;
        auto y = columnSizes[column];
        guiComp->setMenuXY(x, y);
        
        columnSizes[column] += height;
        
        std::cout << "Placing " << guiComp->getUID() << " at " << x << "/" << y << " (x/y), gui column # = " << column << " \n";
    }
    
}
