//
//  ADSR.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 9/22/20.
//

#ifndef ADSR_h
#define ADSR_h

#include "spline.h"


typedef std::chrono::system_clock SystemClock;

class Time{
public:
   
    static const long getCurrentTime(){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        //std::cout << "In getCurrentTime, current time (MS) = " << millis << '\n';
        return millis;
    }
    static const long elapsedTimeSince(long oldTime){
        long elapsed = getCurrentTime() - oldTime;
        //std::cout<< elapsed << " milliseconds elapsed\n";
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
    ADSR(int a = 30, int d = 4020, int s = 5980, int r = 15, bool sustainLoop = false) : a(a), d(d), s(s), r(r), aL(1.f), dL(.3f), sL(.0f), sustainLoop(sustainLoop) {
        init();
        //std::cout << "ADSR total = " << total << '\n';
    }
    bool sustainLoop;
    long a, d, s, r, total;
    float aL, dL, sL;
    long aTot, dTot, sTot, rTot;
    
    long
private:
    void init(){
        aTot = a;
        dTot = a + d;
        sTot = a + d + s;
        rTot = total = a + d + s + r;
    }
    //long getLength()
};

class NoteADSRState{
public:
       
    NoteADSRState(ADSR adsr) : adsr(adsr), adsrState(OFF), active(), startTime(), endTime() {
        //std::vector<double>
        spline.set_points(
            { double(0), double(adsr.aTot), double(adsr.dTot), double(adsr.sTot), double(adsr.rTot)}, // x
            { double(0),double(adsr.aL), double(adsr.dL), double(adsr.sL),double(0)} // y
        );
    }
    
    void start(){
        startTime = Time::getCurrentTime();
        //active.store(true);
        adsrState = ATTACKING;
        
    }
    void stop(){
        endTime = Time::getCurrentTime();
        //level = 0.f;
        //active.store(false);
        adsrState = OFF;
    }
    
    float getLevel(){
//        auto level(0.f);
        if(adsrState != OFF){
            //std::cout << "Getting time elapsed since start time = " << startTime << '\n';
            auto timePassed = Time::elapsedTimeSince(startTime);
            //console.lo
            std::cout<< timePassed << " milliseconds elapsed\n";
            auto level = getLevel(timePassed);
            std::cout<<"Note ADSR Level = " << level << '\n';
            return level;
        }
        return 0.f;//level;
    }
    
    void setADSR(ADSR adsr){
        this->adsr = adsr;
    }
    
private:
    //std::map<ADSRState,long> state;
    //std::vector<std::pair<long,ADSRState>> states;
    
    void updateState(){
        
    }
    
    float getLevel(long elapsed){
        // segment of the ADSR curve that the elapsed time falls into
        //long currentSegmentTimeLength(adsr.a),  segmentTimeLength = currentSegmentTimeLength;
        long segmentTimeLength;
        // how much of the current ADSR segment has been completed (in time)
        long segmentCompleted;
        // start/end levels of current segment to be determined
        long startLevel, endLevel;
        
        // Attacking
        if(elapsed <= adsr.aTot){
            startLevel = 0.f;
            endLevel = adsr.aL;
            segmentTimeLength = adsr.a;
            segmentCompleted = elapsed;
        }
        // Decaying
        else if(elapsed <= adsr.dTot){
            startLevel = adsr.aL;
            endLevel = adsr.dL;
            segmentTimeLength = adsr.d;
            segmentCompleted = elapsed - adsr.aTot;
        }
        // Sustaining
        else if(elapsed <= adsr.sTot){
            startLevel = adsr.dL;
            endLevel = adsr.sL;
            segmentTimeLength = adsr.s;
            segmentCompleted = elapsed - adsr.dTot;
        }
        // Releasing
        else if(elapsed <= adsr.rTot){
            startLevel = adsr.sL;
            endLevel = 0.f;
            segmentTimeLength = adsr.r;
            segmentCompleted = elapsed - adsr.sTot;
        }
        // Note has been completed
        else {
            return 0.f;
        }
        //endLevel = " << endLevel << ", elapsed = " << elapsed << ", segmentCompleted = "<< segmentCompleted<< "/segmentTimeLength = " << segmentTimeLength << '\n';
        
        return spline(double(elapsed));
        //return lerp(startLevel, endLevel, float(segmentCompleted) / float(segmentTimeLength));
        
    }
    
    tk::spline spline;
    
    // tk::spline splineAttack, splineDecay, splineRelease; // TODO unique spline for each ADSR segment
    
    std::atomic<bool> active;
    ADSR adsr;
    ADSRState adsrState;
    
    long startTime, endTime;
    
    
    //    static float lerp(float a, float b, float f){
    //        return (a * (1.0 - f)) + (b * f);
    //    }
    
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0811r2.html
    const float lerp(float a, float b, float t) {
        // Exact, monotonic, bounded, determinate, and (for a=b=0) consistent:
        if(a<=0 && b>=0 || a>=0 && b<=0) return t*b + (1-t)*a;
        
        if(t==1) return b;                        // exact
        // Exact at t=0, monotonic except near t=1,
        // bounded, determinate, and consistent:
        const float x = a + t*(b-a);
        return t>1 == b>a ? std::max(b,x) : std::min(b,x);  // monotonic near t=1
    }
};

#endif /* ADSR_h */
