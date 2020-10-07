// WS2812

#include "FastLED.h"


#define NUM_LEDS 60           // Number of RGB LEDs in the strand
#define COLOUR_ORDER GRB      // Colour order within each led 

#define LED_PIN 6             // Arduino pin used for Data


// Define the array of leds
CRGB leds[NUM_LEDS];


void setup()
{
  FastLED.addLeds<WS2812, LED_PIN, COLOUR_ORDER>(leds, NUM_LEDS);
 
}

void loop() {
  // one at a time
  for(int j = 0; j < 3; j++) {
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      switch(j) {
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastLED.show();
      delay(10);
    }
  }

  // growing/receeding bars
  for(int j = 0; j < 3; j++) {
    memset(leds, 0, NUM_LEDS * 3);
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      switch(j) {
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastLED.show();
      delay(10);
    }
    for(int i = NUM_LEDS-1 ; i >= 0; i-- ) {
      switch(j) {
        case 0: leds[i].r = 0; break;
        case 1: leds[i].g = 0; break;
        case 2: leds[i].b = 0; break;
      }
      FastSPI_LED.show();
      delay(1);
    }
  }

  // Fade in/fade out
  for(int j = 0; j < 3; j++ ) {
    memset(leds, 0, NUM_LEDS * 3);
    for(int k = 0; k < 256; k++) {
      for(int i = 0; i < NUM_LEDS; i++ ) {
        switch(j) {
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastLED.show();
      delay(3);
    }
    for(int k = 255; k >= 0; k--) {
      for(int i = 0; i < NUM_LEDS; i++ ) {
        switch(j) {
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastLED.show();
      delay(3);
    }
  }
}
