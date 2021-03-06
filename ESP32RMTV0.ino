//All the includes for RMT to work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp32-hal.h"
#include "Arduino.h"
//My custom COlour generator
#include "colourObject.h"
//The RMT CLass
#include "NeoViaRMT.h"

//Set up vars for coulour generator
byte maxValue=64;
char colourList[8*3]={maxValue,0,0, maxValue,maxValue,0, 0,maxValue,0, 0,maxValue,maxValue, 0,0,maxValue, maxValue,0,maxValue, maxValue,maxValue,maxValue, maxValue,0,0};
colourObject dynColObject(maxValue, 8, colourList);
//Set up 2 strands with 72*4 pixels on each (choose any GPIO pins you like)
unsigned short int numLeds=72*4;
NeoViaRMT strandOne = NeoViaRMT(numLeds, 18); //Strand one on pin 18
NeoViaRMT strandTwo = NeoViaRMT(numLeds, 23); //Strand two on pin 23

unsigned short int cIndex=0;    //used to track the colour you generate
short int pIndex=0;             //used as a pixel counter
byte tempCol[3] = {0,0,0};      //used to store colours generated by the colour generator
float realTick;                 //used to return the rmt tick rate, but only at setup

void setup() 
{
    Serial.begin(115200);
    
    //INIT the 1st strand
    if ((strandOne.rmt_send = rmtInit(strandOne._dataPin, true, RMT_MEM_64  )) == NULL)
    {
        Serial.println("init/t%d/tsender failed\n", strandOne._dataPin);
    }
    //INIT the 2nd strand
    if ((strandTwo.rmt_send = rmtInit(strandTwo._dataPin, true, RMT_MEM_64  )) == NULL)
    {
        Serial.println("init/t%d/tsender failed\n", strandTwo._dataPin);
    }
    //Set the tick rate for the 1st STrand
    realTick = rmtSetTick(strandOne.rmt_send, 100);
    //Set the tick rate for the 2nd STrand
    realTick = rmtSetTick(strandTwo.rmt_send, 100);
    //Set all pixels on both strans to BLACK
    for(pIndex=0; pIndex<numLeds; pIndex++)
    {
      strandOne.setPixel(pIndex, tempCol);
      strandTwo.setPixel(pIndex, tempCol);
    }
    //Render data to pixels (this clears them)
    renderLEDs();
    
}
void loop() 
{
    //Grab a colour based on the cIndex var and place it into the tempCol var
    dynColObject.getColour(cIndex%dynColObject._bandWidth, tempCol);
    //set pixel 0 of strand one to colour in tempCol
    strandOne.setPixel(0, tempCol);
    //Grab a colour based on the cIndex var + 128(just to see a major diference in) and place it into the tempCol var
    dynColObject.getColour((cIndex+128)%dynColObject._bandWidth, tempCol);
    //set pixel 0 of strand two to colour in tempCol
    strandTwo.setPixel(0, tempCol);
    //shift the pixels from end to simuate a flowing rainbow
    for(pIndex=numLeds-1; pIndex>0; pIndex--)
    {
      strandOne.getPixel(pIndex-1, tempCol);
      strandOne.setPixel(pIndex, tempCol);
      strandTwo.getPixel(pIndex-1, tempCol);
      strandTwo.setPixel(pIndex, tempCol);
    }
    //Render to the LEDs
    renderLEDs();
    delay(5);
    //Increment the cIndex var so the next olour is diferent
    cIndex+=4;
}

void renderLEDs()
{
    //Encode the bitstream for strand one
    strandOne.encode();
    //Encode the bitstream for strand two
    strandTwo.encode();
    //Dump data to strand one
    rmtWrite(strandOne.rmt_send, strandOne.neoBits, strandOne._NeoBitsframeLength);
    //Dump data to strand two
    rmtWrite(strandTwo.rmt_send, strandTwo.neoBits, strandTwo._NeoBitsframeLength);
}
