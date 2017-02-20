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

#define TWOPI (6.28318530717958647693)

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

// Sine Table
const int sineTableSize = 1024;
float sineTable[sineTableSize] = {0};
float stripToSineTableSize = 1.0 / (float) ledsPerStrip * (float) sineTableSize;

// Colors
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 128);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);

// Morse Code
int stripOffsets[nStrips] = {0};
uint32_t palette[] = {orange, magenta, black};
// uint32_t palette[] = {orange, magenta};
int paletteSize = sizeof(palette) / sizeof(uint32_t);
int lastColors[nStrips] = {0};
int currentColors[nStrips] = {1};

int targetStrip = 0;
int targetDirection = 1;

int framesPerTransition = ledsPerStrip / 4;
float framesPerTransitionInv = 1.0 / (float) framesPerTransition;
int framesLeft = framesPerTransition;

// Phasors
float phaseBank[3] = {0};
float phaseIncBank[3] = {1.0 / 120.0, 1.0 / 125.0, 1.0 / 115.0};

void setup() {
  createSineTable();
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

  // Display each Strip
  for (int i = 0; i < nStrips; i++ ) {
    int stripOffset = stripOffsets[i];

    // LFO Phase
    float phase = phaseBank[i];
    float phaseOffset = phase * (float) sineTableSize;

    // Color
    uint32_t c0 = palette[lastColors[i]];
    uint32_t c1 = palette[currentColors[i]];
    uint32_t c = lerpColor(c0, c1, (float) (framesPerTransition - 1 - framesLeft) * framesPerTransitionInv);

    // Update LEDs
    for (int j = 0; j < ledsPerStrip; j++) {
      // Get morse from buffer
      int index = (j + stripOffset) % encodedLength;
      uint8_t v = getEncoded(index);

      // Display
      if (v) {
        int lfo = sineTable[((int) (phaseOffset + (float) j * stripToSineTableSize) % sineTableSize)];
        leds.setPixel(j + i * ledsPerStrip, lerpColor(c, black, lfo));
      }
    }
  }

  // Choose next
  if (framesLeft == 0) {
    // Reset frame counter
    framesLeft = framesPerTransition;

    // Move current into last
    lastColors[targetStrip] = currentColors[targetStrip];

    // Select new target and rotational direction
    int lastTargetStrip = targetStrip;
    targetStrip = random(0, nStrips);

    if (targetStrip == lastTargetStrip) {
      targetDirection *= -1;
    } else {
      targetDirection = 1 - random(2) * 2;  // 1 or -1
    }

    // Select from colors
    int newColor = random(0, paletteSize);
    while (newColor == currentColors[targetStrip]) {
      newColor = random(0, paletteSize);
    }
    currentColors[targetStrip] = newColor;
  }

  // Update phasor bank
  for (int i = 0; i < 3; i++) {
    float phase = phaseBank[i];
    phase += phaseIncBank[i];
    phase -= (int) phase;
    phaseBank[i] = phase;
  }

  displayLEDs();
}

ulong showTime = millis() + frameDelay;
void displayLEDs() {
  while(millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}

void createSineTable() {
  for (int i = 0; i < sineTableSize; i++) {
    float v = (sin((float) i / (float) sineTableSize * TWOPI) * 0.5 + 0.5) * 255.0;
    sineTable[i] = (int) v;
  }
}
