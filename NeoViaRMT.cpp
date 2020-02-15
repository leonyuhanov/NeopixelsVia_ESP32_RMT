#include "NeoViaRMT.h"

NeoViaRMT::NeoViaRMT(unsigned short int numLEDs, byte dataPin)
{  
  byte blackOutCol[3] = {0,0,0};
  _dataPin = dataPin;
  _numLEDs = numLEDs;
  _LEDframeLength = _numLEDs*3;
  _NeoBitsframeLength = 24*_numLEDs;
  
  LEDs = new byte[_LEDframeLength];
  neoBits = new rmt_data_t[_NeoBitsframeLength];

  //Black Out
  for(_ledCounter=0; _ledCounter<_numLEDs; _ledCounter++)
  {
    setPixel(_ledCounter, blackOutCol);
  }
  encode();
}

void NeoViaRMT::setPixel(short int pixelIndex, byte *pixelColour)
{
  _ledCounter = 3*pixelIndex;
  LEDs[ _ledCounter ] = pixelColour[1];
  LEDs[ _ledCounter + 1 ] = pixelColour[0];
  LEDs[ _ledCounter + 2 ] = pixelColour[2];
}

void NeoViaRMT::getPixel(short int pixelIndex, byte *pixelColour)
{
   _ledCounter = 3*pixelIndex;
   pixelColour[0] = LEDs[ _ledCounter + 1 ];
   pixelColour[1] = LEDs[ _ledCounter ];
   pixelColour[2] = LEDs[ _ledCounter + 2 ];
}

void NeoViaRMT::encode()
{
  _neoBitCounter=0;
  for(_ledCounter=0; _ledCounter<_LEDframeLength; _ledCounter++)
  {
    for (_bitCounter=0; _bitCounter<8; _bitCounter++)
    {
        if ( (LEDs[ _ledCounter] & (1<<(7-_bitCounter))) )
        {
          neoBits[_neoBitCounter].level0 = 1;
          neoBits[_neoBitCounter].duration0 = 8;
          neoBits[_neoBitCounter].level1 = 0;
          neoBits[_neoBitCounter].duration1 = 4;
        }
        else
        {
          neoBits[_neoBitCounter].level0 = 1;
          neoBits[_neoBitCounter].duration0 = 4;
          neoBits[_neoBitCounter].level1 = 0;
          neoBits[_neoBitCounter].duration1 = 8;
        }
        _neoBitCounter++;
    }
  }
}
