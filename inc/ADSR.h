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
#include <map>
#include <atomic>


typedef std::chrono::system_clock SystemClock;

class Time{
public:
   
    static inline const long getCurrentTime(){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        //std::cout << "In getCurrentTime, current time (MS) = " << millis << '\n';
        return millis;
    }
    static inline const long elapsedTimeSince(long oldTime){
        long elapsed = getCurrentTime() - oldTime;
        return elapsed;
    }
};
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



class NoteADSRState{
public:
       
    NoteADSRState(ADSR adsr, bool splineCurve = true, double splineControlY = .25f);
    
    void start();
    void stop();
    
    float getLevel();
    
    void setADSR(ADSR adsr);
    
private:
    
    
    void initSplines();
    void updateState();
    
    float getLevel(long elapsed, bool spline);
    
    bool splineCurve;
    //tk::spline spline;
    double splineControlY;
    
    tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment
    
    
    
    
    std::atomic<bool> active;
    ADSR adsr;
    ADSRState adsrState;
    
    long startTime, endTime;
    
    
    //    static float lerp(float a, float b, float f){
    //        return (a * (1.0 - f)) + (b * f);
    //    }
    
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0811r2.html
    const float lerp(float a, float b, float t);
};

#endif /* ADSR_h */
