#ifndef _MicroGame
    #define _MicroGame

    #if ARDUINO >= 100
        #include "Arduino.h"
        //#include "Print.h"
        #include "Adafruit_SSD1306.h"
    #else
        #include "WProgram.h"
    #endif

    class MicroGame
    {
        public:
            //Constructor
            MicroGame();  
            static Adafruit_SSD1306 display;  
            static unsigned char PROGMEM splashScreen[];        
            /*Button and buzzer constants - DONT CHANGE THESE!!!*/
            
            static int btnLUp;     //left up
            static int btnLDn;     //left down
            static int btnLLt;     //left left
            static int btnLRt;     //left right
            
            static int btnRUp;     //right up
            static int btnRDn;     //right down
            static int btnRLt;     //right left
            static int btnRRt;     //right right
            static int buzPin;     //buzzer
            
            void Begin();
            void Beep(int note, int duration);
            void RenderSplashScreen();            
            bool PressLUp();
            bool PressLDn();
            bool PressLLt();
            bool PressLRt();
            bool PressRUp();
            bool PressRDn();
            bool PressRLt();
            bool PressRRt();
        private:
    };

#endif