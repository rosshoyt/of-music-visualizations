//
//  ADSR.h
//  midiVisualizer
//
//  Created by Ross Hoyt on 9/22/20.
//

#ifndef ADSR_h
#define ADSR_h


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

// Class which can be shared between NoteADSRState
class ADSR {
public:
    ADSR(int a = 15, int d = 4000, int s = 0, int r = 15) : a(a), d(d), s(s), r(r), aL(1.f), dL(0.f), sL(0.0f) {
        total = this->a + this->d + this->r + this->s;
        std::cout << "ADSR total = " << total << '\n';
    }
    float getLevel(long elapsed){
        long startLevel, endLevel;
        
        // segment of the ADSR curve that the elapsed time falls into
        long portionTimeLength(a);
        
        long portionCompleted;
        
        if(elapsed <= portionTimeLength){
            startLevel = 0.f;
            endLevel = aL;
            portionCompleted = elapsed;
            //lerpAmt = float(elapsed.count()) / float(a.count());
        }
        // TODO update portion time length to store lengths used in below if() conditions
        else if(elapsed <= (portionTimeLength + d)){
            startLevel = dL;
            endLevel = sL;
            portionCompleted = elapsed - a;
        } else if(elapsed <= (portionTimeLength + d + s)){
            startLevel = sL;
            endLevel = 0.f;
            portionCompleted = elapsed - a - d;
        }
        std::cout << "Before Linear Interpolation, startLevel = " << startLevel << ", endLevel = " << endLevel << ", portionCompleted = " << elapsed << ", portionTimeLength = "<< portionCompleted<<'\n';
        return lerp(startLevel, endLevel, float(portionCompleted) / float(portionTimeLength));
        
    }
    
private:
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
    long a, d, s, r, total;
    float aL, dL, sL;
    
};

class NoteADSRState{
public:
       
    NoteADSRState(ADSR adsr) : adsr(adsr), active(), startTime(), endTime() {}
    
    void start(){
        startTime = Time::getCurrentTime();
        active.store(true);
        
    }
    void stop(){
        endTime = Time::getCurrentTime();
        //level = 0.f;
        active.store(false);
    }
    
    float getLevel(){
//        auto level(0.f);
        if(active){
            //std::cout << "Getting time elapsed since start time = " << startTime << '\n';
            auto timePassed = Time::elapsedTimeSince(startTime);
            //console.lo
            std::cout<< timePassed << " milliseconds elapsed\n";
            auto level = adsr.getLevel(timePassed);
            std::cout<<"Note ADSR Level = " << level << '\n';
            return level;
        }
        return 0.f;//level;
    }
    
    void setADSR(ADSR adsr){
        this->adsr = adsr;
    }
    
private:
//    void update(){
//
//    }
//    Time time;
    std::atomic<bool> active;
    ADSR adsr;
    //float level;
    long startTime, endTime;
};

#endif /* ADSR_h */
