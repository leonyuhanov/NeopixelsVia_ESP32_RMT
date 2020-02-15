#ifndef NeoViaRMT_h
#define NeoViaRMT_h
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"
#include "esp32-hal.h"

class NeoViaRMT
{
  public:
    NeoViaRMT(unsigned short int numLEDs, byte dataPin);
    void setPixel(short int pixelIndex, byte *pixelColour);
    void getPixel(short int pixelIndex, byte *pixelColour);
    void encode();
    
    byte* LEDs;
    byte _dataPin;
    rmt_data_t* neoBits;
    rmt_obj_t* rmt_send = NULL;
     
    unsigned short int _LEDframeLength;
    unsigned short int _NeoBitsframeLength;
    unsigned short int _numLEDs;
    
  private:
    unsigned short int _ledCounter;
    unsigned short int _colCounter;
    unsigned short int _neoBitCounter;
    byte _bitCounter;

};

#endif
