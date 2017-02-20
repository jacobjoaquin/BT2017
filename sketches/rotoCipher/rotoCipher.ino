/*
rotoCipher.ino
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
#include "facetable.h"

#define TWOPI (6.28318530717958647693)

// Combines red, green, blue into a single value
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Set a Triangle Face's LED
//   F: Index of face. [0-7]  (use -1 for DEBUG)
//   I: index of LED in face. [0-ledsPerFace)
//   C: Color
#define setFaceLED(F, I, C) (leds.setPixel(pgm_read_word(&faceTable[(F) * ledsPerFace + (I)]), (C)))
#define getFaceLED(F, I) (pgm_read_word(&faceTable[(F) * ledsPerFace + (I)]))


struct RotoStep {
  int face;
  int frames;
  bool direction;
};

const int nSteps = 32;
RotoStep rotoSteps[nSteps];
int currentRotoStep = 0;
const int ENCODING_MODE = 0;
const int DECODING_MODE = 1;
int mode = ENCODING_MODE;

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nBeams = nStrips * 4;
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

// Buffer
uint32_t beamBuffer[nLeds] = {0};

// Colors
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 128);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);

//
int framesLeft = 64;
int currentFace = 0;
bool currentDirection = true;

int tempDelay = 100;

void setup() {
  createSineTable();
  createBeamBuffer();
  leds.begin();

  // Show at beginning
  beamBufferToLEDs();
  leds.show();
  delay(500);
  encode();

  // Debugging
  // Serial.begin(9600);
  // debugPrintBeamBufferForwardReverse();
  // delay(5000);
}

void loop() {
  clear();

  framesLeft--;

  rotateFace(currentFace, currentDirection);
  rotateFace((currentFace + nFaces / 2) % nFaces, currentDirection);

  if (currentRotoStep == 0 && mode == ENCODING_MODE && framesLeft == rotoSteps[0].frames - 1) {
    delay(tempDelay);
    tempDelay = 3000;
  }


  if (framesLeft == 0) {
    if (mode == ENCODING_MODE) {
      currentRotoStep++;

      if (currentRotoStep < nSteps) {
        encode();
      } else {
        mode = DECODING_MODE;
        currentRotoStep--;
        decode();
      }

    } else if (mode == DECODING_MODE) {
      currentRotoStep--;

      if (currentRotoStep >= 0) {
        decode();
      } else {
        mode = ENCODING_MODE;
        currentRotoStep++;
        encode();
      }
    }
  }

  // Display
  beamBufferToLEDs();
  displayLEDs();
}

void encode() {
  currentFace = random(nFaces);
  // currentFace = 3;
  framesLeft = 2 << random(1, 6);
  // framesLeft = 32;
  currentDirection = random(2) ? true : false;
  // currentDirection = false;

  rotoSteps[currentRotoStep].face = currentFace;
  rotoSteps[currentRotoStep].frames = framesLeft;
  rotoSteps[currentRotoStep].direction = currentDirection;
}

void decode() {
  currentFace = rotoSteps[currentRotoStep].face;
  framesLeft = rotoSteps[currentRotoStep].frames;
  currentDirection = !rotoSteps[currentRotoStep].direction;
}

ulong showTime = millis() + frameDelay;
void displayLEDs() {
  while(millis() < showTime) {}
  leds.show();
  showTime = millis() + frameDelay;
}

void rotateFace(int face, bool isForward) {
  if (isForward) {
    int beamBufferTemp = beamBuffer[getFaceLED(face, 0)];

    for (int i = 0; i < ledsPerFace - 1; i++) {
      beamBuffer[getFaceLED(face, i)] = beamBuffer[getFaceLED(face, i + 1)];
    }
    beamBuffer[getFaceLED(face, ledsPerFace - 1)] = beamBufferTemp;
  } else {
    int beamBufferTemp = beamBuffer[getFaceLED(face, ledsPerFace - 1)];

    for (int i = ledsPerFace - 2; i >= 0; i--) {
      beamBuffer[getFaceLED(face, i + 1)] = beamBuffer[getFaceLED(face, i)];
    }
    beamBuffer[getFaceLED(face, 0)] = beamBufferTemp;
  }
}
