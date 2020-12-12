#include "Envelope.h"
/** 
* Data class used by Envelope to generate an output level for a given time.
* Implements operator chaining to allow changing default values in the same line as construction
*/
class EnvelopeSettings {
public:
    EnvelopeSettings() = default;

    inline EnvelopeSettings& setAttackLength(double len) {
        lengthA = len;
        return *this;
    }

    inline EnvelopeSettings& setDecayLength(double len) {
        lengthD = len;
        return *this;
    }

    inline EnvelopeSettings& setReleaseLength(double len) {
        lengthR = len;
        return *this;
    }

    inline EnvelopeSettings& setAttackLevel(double lev) {
        levelA = lev;
        return *this;
    }

    inline EnvelopeSettings& setDecayLevel(double lev) {
        levelD = lev;
        return *this;
    }

    inline EnvelopeSettings& setAttackSpline(double lev) {
        splineA = lev;
        return *this;
    }

    inline EnvelopeSettings& setDecaySpline(double lev) {
        splineD = lev;
        return *this;
    }

    inline EnvelopeSettings& setReleaseSpline(double lev) {
        splineR = lev;
        return *this;
    }

    inline EnvelopeSettings& setSustain(bool val = true) {
        sustain = val;
        return *this;
    }

    inline long getLength() {
        return lengthA + lengthD + lengthR;
    }

private:
    // TODO implement generation of this settings class 
    class EnvelopeSegmentSettings {
    public:
        double lengthSeg, levelSegStart, levelSegEnd, levelSpline;
        //double lengthToThisStart; // TODO could track for reference 
    };

    friend class Envelope;


    double lengthA = 250, lengthD = 1500, lengthR = 250;
    double levelA = 1, levelD = .5;
    double splineA = .1, splineD = -.1, splineR = -.1;
    bool sustain = false;

};
