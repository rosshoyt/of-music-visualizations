//
//  ADSR.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 9/22/20.
//

#ifndef ADSR_h
#define ADSR_h

#include <chrono>
#include "spline.h"
#include "Utils.h"
#include <map>
#include <atomic>


enum ADSRState {
    ATTACKING,
    DECAYING,
    SUSTAINING,
    RELEASING,
    OFF
};


// Class which can be shared between NoteADSRState
// TODO add 'looping' ADSR curve
class ADSR {
public:
    ADSR(int a = 10, int d = 6000, int s = 0, int r = 1000, bool sustainLoop = false);
    bool sustainLoop;
    double a, d, s, r, total;
    float aL, dL, sL;
    double aTot, dTot, sTot, rTot;
    long splineControlY;
    //long aMidpointL, dMidpointL, rMidpointL;
    std::map<long, ADSRState> envelopeSections;
    
private:
    void init();
    //long getLength()
};



class NoteADSRState {
public:

    NoteADSRState(ADSR adsr = ADSR(), bool splineCurve = true, double splineControlY = .25f);

    void start();
    void stop();

    float getLevel();

    long getLength();

    void setADSR(ADSR adsr);

private:
    void initSplines();
    void updateState();

    float getLevel(long elapsed, bool spline);

    bool splineCurve;
    double splineControlY;

    tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment
   
    std::atomic<bool> active;
    ADSR adsr;
    ADSRState adsrState;

    long startTime, endTime;
};
#endif /* ADSR_h */
