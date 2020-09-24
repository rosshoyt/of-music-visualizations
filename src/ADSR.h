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
   
    const long getCurrentTime(){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        std::cout << "Getting current time of " << millis << '\n';
        return millis;
    }
    
   
    const long elapsedTimeSince(long oldTime){
        long elapsed = getCurrentTime() - oldTime;
        //std::cout<< elapsed << " milliseconds elapsed\n";
        return elapsed;
    }
private:

    
};

// Class which can be shared between NoteADSRState
class ADSR {
public:
    ADSR(int a = 15, int d = 4000, int s = 0, int r = 15) : a(a), d(d), s(s), r(r), aL(1.f), dL(0.f), sL(0.f) {
        total = this->a + this->d + this->r + this->s;
        std::cout << "ADSR total = " << total << '\n';
    }
    float getLevel(long elapsed){
        float level(0.f);
        
        long start, end;
        long divisor;
        std::cout<< "Getting ADSR level for elapsed ms = " << elapsed << '\n';
        if(elapsed < (divisor += a)){
            start = 0.f;
            end = aL;
            //lerpAmt = float(elapsed.count()) / float(a.count());
        } else if(elapsed < (divisor += d)){
            start = dL;
            end = sL;
            
        } else if(elapsed < (divisor += s)){
            start = sL;
            end = 0.f;
        }
        std::cout << "After if/elses, start = " << start << " end = " << end << " divisor(ms)= " << elapsed << '\n';
        return lerp(start, end, float(elapsed) / float(divisor));
        
    }
    
private:
    static float lerp(float a, float b, float f){
        return (a * (1.0 - f)) + (b * f);
    }
    
    long a, d, s, r, total;
    float aL, dL, sL;
    
};

class NoteADSRState{
public:
       
    NoteADSRState(ADSR adsr) : time(), adsr(adsr), level(0.f), active(), startTime(), endTime() {
        
    }
    
    
    void start(){
        startTime = time.getCurrentTime();
        active.store(true);
        
    }
    void stop(){
        endTime = time.getCurrentTime();
        level = 0.f;
        active.store(false);
    }
    
    float getLevel(){
        update();
        return level;
    }
    
    void setADSR(ADSR adsr){
        this->adsr = adsr;
    }
    
private:
    void update(){
        if(active){
            //TimeMS elapsed(startTime - Time::getCurrentTime());
            //auto timeElapsed =
            auto timePassed = time.elapsedTimeSince(startTime);
            //console.lo
            std::cout<< timePassed << " milliseconds elapsed\n";
            level = adsr.getLevel(timePassed);
            //std::cout<<"Note ADSR Level = " << level << '\n';
            
        }
    }
    Time time;
    std::atomic<bool> active;
    ADSR adsr;
    float level;
    long startTime, endTime;
};

#endif /* ADSR_h */
