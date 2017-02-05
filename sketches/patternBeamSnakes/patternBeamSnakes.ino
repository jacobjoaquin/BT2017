/*
  patternBeamSnakes.ino
  Jacob Joquin


  Original OctoWS2811 Copyright Notice:

  OctoWS2811 BasicTest.ino - Basic RGB LED Test
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

  pin 2:  LED Strip #0
  pin 14: LED strip #1
  pin 7:  LED strip #2
*/

#include <OctoWS2811.h>

// Macro Functions
// Convert rbg values to single value
#define rgb(R, G, B)       ( ( R << 16 ) | ( G << 8 ) | B )
// Get the display index from face and face index
#define getFaceIndex(F, I)  ( faceTable[ F * ledsPerFace + I ] )
// Set a face LED
#define setFaceLED(F, I, C) ( leds.setPixel( faceTable[ F * ledsPerFace + I ], C ) )

// Constansts
const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = 114;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 120;

// Octows2811 Setup
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User defined variables

// Colors
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 128);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);

// Set head and tail colors
uint32_t tailColor = orange;
uint32_t headColor = magenta;

int pos = 0;
int tail = ledsPerBeam / 4;
int head = ledsPerBeam / 8;

void setup() {
  leds.begin();
}

void loop() {
  clear();

  for (int i = 0; i < tail; i++) {
    uint32_t thisColor = 0;

    // Head
    if (i > tail - head) {
      thisColor = lerpColor(tailColor, headColor, (float) (i - (tail - head)) / (float) head);
    }
    // Tail
    else {
      thisColor = lerpColor(black, tailColor, (float) i / (float) (tail - head));
    }

    int thisPos = i + pos;
    int forwardIndex = thisPos % (ledsPerBeam / 2);
    int reverseIndex = ledsPerBeam - 1 - (thisPos % (ledsPerBeam / 2));

    for (int j = 0; j < nStrips * 4; j++) {
      int beamOffset = j * ledsPerBeam;
      leds.setPixel(forwardIndex + beamOffset, thisColor);
      leds.setPixel(reverseIndex + beamOffset, thisColor);
    }
  }
  pos = (pos + 1) % (ledsPerBeam / 2);
  leds.show();
  delay(frameDelay);
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

// Interpolate between two colors.
uint32_t lerpColor(uint32_t c1, uint32_t c2, float amt) {
  uint32_t r1 = (c1 & 0xff0000) >> 16;
  uint32_t g1 = (c1 & 0xff00) >> 8;
  uint32_t b1 = (c1 & 0xff);
  uint32_t r2 = (c2 & 0xff0000) >> 16;
  uint32_t g2 = (c2 & 0xff00) >> 8;
  uint32_t b2 = (c2 & 0xff);

  float r = (float(r1) * (1.0 - amt) + float(r2) * amt) / 2.0;
  float g = (float(g1) * (1.0 - amt) + float(g2) * amt) / 2.0;
  float b = (float(b1) * (1.0 - amt) + float(b2) * amt) / 2.0;

  return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
}
