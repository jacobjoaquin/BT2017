/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
  http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
  Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  Required Connections
  --------------------
  pin 2:  LED Strip #0    OctoWS2811 drives 8 LED Strips.
  pin 14: LED strip #1    All 8 are the same length.
  pin 7:  LED strip #2
  pin 8:  LED strip #3    A 100 ohm resistor should used
  pin 6:  LED strip #4    between each Teensy pin and the
  pin 20: LED strip #5    wire to the LED strip, to minimize
  pin 21: LED strip #6    high frequency ringining & noise.
  pin 5:  LED strip #7
  pin 15 & 16 - Connect together, but do not use
  pin 4 - Do not use
  pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/

#include <OctoWS2811.h>
#include "facetable.h"

// Combines red, green, blue into a single value
#define rgb(R, G, B)       ( ( R << 16 ) | ( G << 8 ) | B )
//#define getFaceIndex(F, I)  ( faceTable[ F * ledsPerFace + I ] )
#define setFaceLED(F, I, C) ( leds.setPixel( faceTable[ F * ledsPerFace + I ], C ) )

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = 114;
const int ledsPerFace = 3 * ledsPerBeam;

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User define variables
int currentFace = 0;

void setup() {
  leds.begin();
}

void loop() {
  // Cycle through each LED in each face
  for (int i = 0; i < ledsPerFace; i++) {
    clear();
    //    leds.setPixel(getFaceIndex(currentFace, i), rgb(255, 0, 128));
    setFaceLED(currentFace, i, rgb(255, 0, 128));
    leds.show();
    delay(1000 / 120);  // Attempting 120 frames per second
  }
  currentFace = (currentFace + 1) % nFaces;
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

