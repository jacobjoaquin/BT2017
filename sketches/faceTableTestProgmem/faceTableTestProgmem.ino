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
*/

#include <OctoWS2811.h>
#include "facetable.h"

// Combines red, green, blue into a single value
#define rgb(R, G, B)       ( ( ( R ) << 16 ) | ( ( G ) << 8 ) | ( B ) )

// Set a Triangle Face's LED
//   F: Index of face. [0-7]  (use -1 for DEBUG)
//   I: index of LED in face. [0-ledsPerFace)
//   C: Color  
#define setFaceLED(F, I, C) ( leds.setPixel(  pgm_read_byte( &faceTable[ ( F ) * ledsPerFace + ( I ) ] ), ( C ) ) )

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = ledsPerStrip / 4;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 60;

// Setup OctoWS2811 library
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User defined variables
int currentFace = 0;

void setup() {
  leds.begin();
}

void loop() {
  // Cycle through each LED in each face
  for (int i = 0; i < ledsPerFace; i++) {
    clear();
    setFaceLED(currentFace, i, rgb(255, 0, 128));
    leds.show();
    delay(frameDelay);
  }
  currentFace = (currentFace + 1) % nFaces;
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

