#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"
#include "esp32-hal.h"
#include "colourObject.h"
#include "NeoViaRMT.h"

byte maxValue=64;
unsigned short int numLeds=72*4;
char colourList[8*3]={maxValue,0,0, maxValue,maxValue,0, 0,maxValue,0, 0,maxValue,maxValue, 0,0,maxValue, maxValue,0,maxValue, maxValue,maxValue,maxValue, maxValue,0,0};
colourObject dynColObject(maxValue, 8, colourList);
NeoViaRMT strandOne = NeoViaRMT(numLeds, 18);
NeoViaRMT strandTwo = NeoViaRMT(numLeds, 23);

unsigned short int cIndex=0;
short int pIndex=0;
byte tempCol[3] = {0,0,0};
float realTick;

void setup() 
{
    Serial.begin(115200);
    
    if ((strandOne.rmt_send = rmtInit(strandOne._dataPin, true, RMT_MEM_64  )) == NULL)
    {
        Serial.println("init/t%d/tsender failed\n", strandOne._dataPin);
    }
    if ((strandTwo.rmt_send = rmtInit(strandTwo._dataPin, true, RMT_MEM_64  )) == NULL)
    {
        Serial.println("init/t%d/tsender failed\n", strandTwo._dataPin);
    }
    realTick = rmtSetTick(strandOne.rmt_send, 100);
    realTick = rmtSetTick(strandTwo.rmt_send, 100);
    for(pIndex=0; pIndex<numLeds; pIndex++)
    {
      strandOne.setPixel(pIndex, tempCol);
      strandTwo.setPixel(pIndex, tempCol);
    }
    renderLEDs();
    
}
void loop() 
{
    
    // Init data with only one led ON
    dynColObject.getColour(cIndex%dynColObject._bandWidth, tempCol);
    strandOne.setPixel(0, tempCol);
    dynColObject.getColour((cIndex+128)%dynColObject._bandWidth, tempCol);
    strandTwo.setPixel(0, tempCol);
    for(pIndex=numLeds-1; pIndex>0; pIndex--)
    {
      strandOne.getPixel(pIndex-1, tempCol);
      strandOne.setPixel(pIndex, tempCol);
      strandTwo.getPixel(pIndex-1, tempCol);
      strandTwo.setPixel(pIndex, tempCol);
    }
    renderLEDs();
    delay(5);
    cIndex+=4;
}

void renderLEDs()
{
    strandOne.encode();
    strandTwo.encode();
    rmtWrite(strandOne.rmt_send, strandOne.neoBits, strandOne._NeoBitsframeLength);
    rmtWrite(strandTwo.rmt_send, strandTwo.neoBits, strandTwo._NeoBitsframeLength);
}
