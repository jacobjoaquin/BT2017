/*
zap.ino
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
uint32_t magenta = rgb(255, 0, 92);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);

// LED Buffer
uint32_t buffer[nLeds] = {0};

// Zap
struct Zap {
  int position;
  int length;
  int direction;
  uint32_t color;
  int framesLeft;
};

const int nZaps = 800;
Zap zapList[nZaps];
int zapLength = 5;
int zapFrames = 20;
int zapIndex = 0;

void initZapList() {
  struct Zap * zPtr = zapList;
  for (int i = 0; i < nZaps; i++) {
    zPtr->position = 0;
    zPtr->length = zapLength;
    zPtr->direction = 0;
    zPtr->color = white;
    zPtr->framesLeft = 0;
    zPtr++;
  }
}

void newZap() {
  Zap * zPtr = &zapList[zapIndex];
  zPtr->position = random(nLeds);
  zPtr->direction = random(2) ? 1 : -1;
  zPtr->framesLeft = zapFrames;
  zPtr->length = random(3, 9);
  zPtr->color = white;
  // zPtr->color = random(2) ? rgb(0, 255, 0) : rgb(255, 255, 0);
  // zPtr->color = random(2) ? rgb(0, 255, 255) : rgb(255, 255, 255);
  zapIndex = (zapIndex + 1) % nZaps;
}

void updateZaps() {
  struct Zap * zPtr = zapList;
  for (int i = 0; i < nZaps; i++) {
    if (zPtr->framesLeft > 0) {
      zPtr->framesLeft--;
      zPtr->position += zPtr->direction;
      // uint32_t c = lerpColor(zPtr->color, 0, float(zPtr->framesLeft) / (float) zapFrames);
      uint32_t c = lerpColor(0, zPtr->color, float(zPtr->framesLeft) / (float) zapFrames);
      int length = zPtr->length;
      int position = zPtr->position;
      int direction = zPtr->direction;

      for (int j = 0; j < length; j++) {
        int index = position + j;
        index += nLeds * (index < 0);
        index -= nLeds * (index >= nLeds);
        buffer[index] = c;
      }
    }
    zPtr++;
  }
}

void bufferToLEDs() {
  // for (int i = 0; i < nLeds; i++) {
  //   leds.setPixel(i, buffer[i]);
  // }

  uint32_t * bufferPtr = buffer;
  for (int i = 0; i < nLeds; i++) {
    uint32_t c = *bufferPtr;
    int amt = random(256);
    c = lerpColor(0, c, amt);
    leds.setPixel(i, c);
    bufferPtr++;
  }
}

void setup() {
  initZapList();
  leds.begin();
}

void loop() {
  memset(&buffer[0], 0, sizeof(buffer));
  clear();
  newZap();
  newZap();
  newZap();
  updateZaps();
  bufferToLEDs();
  displayLEDs();
}

ulong showTime = millis() + frameDelay;
void displayLEDs() {
  while(millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}
