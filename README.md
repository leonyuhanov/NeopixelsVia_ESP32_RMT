# NeopixelsVia_ESP32_RMT
Simple Neopixel driver for the ESP32 utilizing the built in RMT Interface

Create a strand/data output as an object:
You will need to pass the numebr of pixels and the pin you wish to output to
```C++
int numLeds = 144;
int dataPin = 18;
NeoViaRMT strandOne = NeoViaRMT(numLeds, dataPin);
```

In setup() initiate the strand with the RMT device and set up its tick rate to 100
```C++
strandOne.rmt_send = rmtInit(strandOne._dataPin, true, RMT_MEM_64  )
rmtSetTick(strandOne.rmt_send, 100);
```

Once in the main code or anywhere else you can set a pixel an RGB value like so:
```C++
byte tempCol[3] = {255,0,0}; //red
int pIndex=0;
strandOne.setPixel(pIndex, tempCol);
```

You get also GET the colour of a pixel like so:
```C++
byte tempCol[3];
int pIndex=0;
strandOne.getPixel(pIndex, tempCol);
```

Once you ar eready to update the LEDS in teh real world you need to call 2 functions in this order:
```C++
strandOne.encode(); //encodes the bitstream into the RMT data object
rmtWrite(strandOne.rmt_send, strandOne.neoBits, strandOne._NeoBitsframeLength); //Writes data to the leds 
```
