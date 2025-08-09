#ifndef _SpaceChase
    #define _SpaceChase

    #if ARDUINO >= 100
        #include "Arduino.h"
        #include "Print.h"
    #else
        #include "WProgram.h"
    #endif

    class SpaceChase
    {
        public:
            //Constructor
            SpaceChase();
            static unsigned char PROGMEM titleBg[];
            static unsigned char PROGMEM failBg[];
            static unsigned char PROGMEM winBg[];
            static unsigned char PROGMEM bossShell0[];
            static unsigned char PROGMEM bossShell1[];
            static unsigned char PROGMEM bossShell2[];
            static unsigned char PROGMEM bossShellHit[];
            static unsigned char PROGMEM bossPod0[];
            static unsigned char PROGMEM bossPod1[];
            static unsigned char PROGMEM bossPod2[];
            static unsigned char PROGMEM bossPodHit[];
            static unsigned char PROGMEM explode0[];
            static unsigned char PROGMEM explode1[];
            //Player ships
            static unsigned char PROGMEM shipOp0[];
            static unsigned char PROGMEM shipOp1[];
            static unsigned char PROGMEM shipOp2[];
            static unsigned char PROGMEM shipOp3[];
            //Flames
            static unsigned char PROGMEM smallFlame0[];
            static unsigned char PROGMEM smallFlame1[];
            static unsigned char PROGMEM largeFlame0[];
            static unsigned char PROGMEM largeFlame1[];
        private:
    };

#endif