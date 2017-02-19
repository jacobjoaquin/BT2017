/*
MorseCode.ino - Morse Code Pattern
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

Required Connections
--------------------
pin 2:  LED Strip #0    OctoWS2811 drives 8 LED Strips.
pin 14: LED strip #1    All 8 are the same length.
pin 7:  LED strip #2
*/

#include <OctoWS2811.h>
#include "encoded.h"

// Combines red, green, blue into a single value
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Get encoded
#define getEncoded(I) (pgm_read_byte(&encoded[(I)]))

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = ledsPerStrip / 4;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 120;

// Setup OctoWS2811 library
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Colors
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 192);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
// uint32_t dotColor = magenta;
// uint32_t dahColor = orange;
uint32_t dotColor = orange;
uint32_t dahColor = orange;

// Morse Code
int framesLeft = ledsPerBeam;
int stripOffsets[nStrips] = {0};
int stripColors[nStrips] = {orange};


int targetStrip = 0;
int targetDirection = 1;
void setup() {
  leds.begin();
}

void loop() {
  clear();

  framesLeft--;

  int offset = stripOffsets[targetStrip];
  offset += targetDirection;
  if (offset == ledsPerStrip) {
    offset -= ledsPerStrip;
  } else if (offset < 0) {
    offset += ledsPerStrip;
  }
  stripOffsets[targetStrip] = offset;

  // Display LEDs
  for (int i = 0; i < nStrips; i++ ) {
    int stripOffset = stripOffsets[i];
    for (int j = 0; j < ledsPerStrip; j++) {
      int index = (j + stripOffset) % encodedLength;
      uint8_t v = getEncoded(index);
      if (v == 1) {
        leds.setPixel(j + i * ledsPerStrip, dotColor);
      } else if (v == 2) {
        leds.setPixel(j + i * ledsPerStrip, dahColor);
      }
    }
  }

  // Choose next
  if (framesLeft == 0) {
    framesLeft = ledsPerStrip / 4;

    int lastTargetStrip = targetStrip;
    targetStrip = random(0, nStrips);

    if (targetStrip == lastTargetStrip) {
      targetDirection *= -1;
    } else {
      targetDirection = random(2) ? -1 : 1;
    }
  }




  // for (int i = 0; i < nLeds; i++) {
  //   int index = (i + counter) % encodedLength;
  //   uint8_t v = getEncoded(index);
  //   if (v == 1) {
  //     leds.setPixel(i, dotColor);
  //   } else if (v == 2) {
  //     leds.setPixel(i, dahColor);
  //   }
  // }
  // counter = (counter + 1) % encodedLength;

  leds.show();
  delay(frameDelay);
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}
