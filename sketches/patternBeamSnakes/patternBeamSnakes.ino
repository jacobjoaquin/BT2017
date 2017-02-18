/*
patternBeamSnakes.ino - Beam Snakes Pattern
Coded by Jacob Joaquin.

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
//#include "gamma8.h"

// Combine rbg values to color
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Constansts
const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = ledsPerStrip / 4;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 120;
const int ledsPerHalfBeam = ledsPerBeam / 2;
const int nBeams = nStrips * 4;

// Octows2811 Setup
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User defined variables
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 192);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
int pos = 0;

// Set tail properties
uint32_t tailColor = orange;
uint32_t headColor = magenta;
int tail = ledsPerBeam / 4;
int head = ledsPerBeam / 12;

void setup() {
  leds.begin();
}

void loop() {
  clear();

  for (int i = 0; i < tail; i++) {
    uint32_t thisColor;

    // Head
    if (i > tail - head) {
      thisColor = lerpColor(tailColor, headColor, (float) (i - (tail - head)) / (float) head);
    }
    // Tail
    else {
      thisColor = lerpColor(black, tailColor, (float) i / (float) (tail - head));
    }

    int forwardIndex = (i + pos) % ledsPerHalfBeam;
    int reverseIndex = ledsPerBeam - 1 - forwardIndex;

    for (int j = 0; j < nBeams; j++) {
      int beamOffset = j * ledsPerBeam;
      leds.setPixel(forwardIndex + beamOffset, thisColor);
      leds.setPixel(reverseIndex + beamOffset, thisColor);
    }
  }
  pos = (pos + 1) % ledsPerHalfBeam;
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
  int i = (int) (amt * 256.0) + 1;
  int di = 256 - i;
  uint32_t r1 = (c1 & 0xff0000) >> 16;
  uint32_t g1 = (c1 & 0x00ff00) >> 8;
  uint32_t b1 = (c1 & 0x0000ff);
  uint32_t r2 = (c2 & 0xff0000) >> 16;
  uint32_t g2 = (c2 & 0x00ff00) >> 8;
  uint32_t b2 = (c2 & 0x0000ff);
  r1 = ((r2 * i) + (r1 * di)) >> 8;
  g1 = ((g2 * i) + (g1 * di)) >> 8;
  b1 = ((b2 * i) + (b1 * di)) >> 8;
  return (r1 << 16) | (g1 << 8) | b1;
}
