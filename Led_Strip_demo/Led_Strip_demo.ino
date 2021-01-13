// WS2812

#include <stdio.h>
#include "FastLED.h"


#define TRANSITION_DELAY 20
#define FULL_CYCLE_DELAY 10000
#define N_CHAMBERS 7
#define NUM_LEDS 136          // Number of RGB LEDs in the strand
#define COLOUR_ORDER GRB      // Colour order within each led 
#define LED_PIN 6             // Arduino pin used for Data

// Define the array of leds
CRGB leds[NUM_LEDS];

struct color {
    char r; char g; char b;
};

struct color R = {r: 200, g: 0,   b: 0};
struct color B = {r: 0,   g: 0,   b: 200};
struct color Y = {r: 200, g: 80,  b: 0};
struct color W = {r: 200, g: 130, b: 100};
struct color I = {r: 255, g: 255, b: 255};
struct color G = {r: 0, g: 255, b: 0};
struct color colors[N_CHAMBERS] = {W, R, W, W, B, Y, W};


struct chamber {
    int first;
    int last;
};

int LED_SEGMENT_COUNT[N_CHAMBERS] = {
    27,   // chamber D
    72,   // chamber A
    10,   // chamber G
    12,   // chamber F
     9,   // chamber B
     3,   // chamber C
     3    // chamber E
};
struct chamber chambers[N_CHAMBERS];


void apply_rgb(int chamber_index, struct color c) {
    for(int i = chambers[chamber_index].first; i < chambers[chamber_index].last; i++){
        leds[i].r = c.r;
        leds[i].g = c.g;
        leds[i].b = c.b;
    }
}

void transition_to_white(CRGB *leds_copy) {
    for(int k = 0; k < 256; k++) {
        for(int i = 0; i < NUM_LEDS; i++) {
            if ((k >= leds_copy[i].r) && (k <= 200)) {leds[i].r = k;}
            if ((k >= leds_copy[i].g) && (k <= 180)) {leds[i].g = k;}
            if ((k >= leds_copy[i].b) && (k <= 150)) {leds[i].b = k;}
        }
        FastLED.show();
        delay(TRANSITION_DELAY);
    }  
}

void transition_from_white(CRGB *leds_copy) {
    for(int k = 255; k >= 0; k--) {
        for(int i = 0; i < NUM_LEDS; i++) {
            if ((k >= leds_copy[i].r) && (k <= 200)) {leds[i].r = k;}
            if ((k >= leds_copy[i].g) && (k <= 180)) {leds[i].g = k;}
            if ((k >= leds_copy[i].b) && (k <= 150)) {leds[i].b = k;}
        }
        FastLED.show();
        delay(TRANSITION_DELAY);
    }
}


void initialize_chambers() {
    chambers[0].first = 0;
    chambers[0].last = LED_SEGMENT_COUNT[0];
    for (int i = 1; i < N_CHAMBERS; i++) {   
        chambers[i].first = chambers[i - 1].last;
        chambers[i].last  = chambers[i].first + LED_SEGMENT_COUNT[i];    
    }
}

void apply_color_sequence(struct color *c) {
    for(int i = 0; i < N_CHAMBERS; i++) {
        apply_rgb(i, c[i]);
    }
    FastLED.show();
}


void shift_color_sequence(struct color *c) {
    static int turn = 0;
    c[0], c[2], c[3], c[6] = W;
    switch(turn) {
        case 0:
          c[1] = R;
          c[4] = B;
          c[5] = Y;
          break;
        case 1:
          c[1] = B;
          c[4] = Y;
          c[5] = R;
          break;
        case 2:
          c[1] = Y;
          c[4] = R;
          c[5] = B;
          break;
    }
    turn = (turn + 1) % 3;
}

void color_to_leds(CRGB *leds_copy, struct chamber *chambers, struct color *c) {
    for(int chamber_index = 0; chamber_index < N_CHAMBERS; chamber_index++) {
        for(int i = chambers[chamber_index].first; i < chambers[chamber_index].last; i++){
            leds_copy[i].r = c[chamber_index].r;
            leds_copy[i].g = c[chamber_index].g;
            leds_copy[i].b = c[chamber_index].b;
        }
    }
}


void setup(){
    FastLED.addLeds<WS2812, LED_PIN, COLOUR_ORDER>(leds, NUM_LEDS);
    initialize_chambers();    
    apply_color_sequence(colors);
}


CRGB leds_copy[NUM_LEDS];    

void loop(){
    apply_color_sequence(colors);
    FastLED.show();
    
    for(int i = 0; i < NUM_LEDS; i++) {
        leds_copy[i] = leds[i];
    }

    transition_to_white(leds_copy);
    shift_color_sequence(colors);
    color_to_leds(leds_copy, chambers, colors);
    transition_from_white(leds_copy);
    
    
    apply_color_sequence(colors);
    FastLED.show();

    delay(FULL_CYCLE_DELAY);
}

/*
void loop() {
  // one at a time
  for(int j = 0; j < 3; j++) {
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      // memset(leds, 0, NUM_LEDS * 3);
      switch(j) {
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastLED.show();
      delay(50);
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
      FastLED.show();
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
*/
