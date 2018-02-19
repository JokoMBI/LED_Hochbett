#include <FastLED.h>
#define NUM_LEDS 6 //97
#define DATA_PIN 3
#define COLOR_ORDER BRG

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,200, 50,   //bright yellow
255,   255,255,255 }; //full white

CRGBPalette16 sunrisePal = heatmap_gp;

CRGB leds[NUM_LEDS];



CRGB clr1;
CRGB clr2;
uint8_t speed;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;



void setup() {  
  FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {

  //blendwave();
  Sonnenaufgang();

  FastLED.show();
}

void Sonnenaufgang() {

  static const uint8_t sunriseLength = 5;
  static const uint8_t interval = (sunriseLength * 60) / 256;
  static uint8_t heatIndex = 50;
  static uint8_t locIndex = 5;
  CRGB color1 = ColorFromPalette(sunrisePal, heatIndex);
  CRGB color2 = ColorFromPalette(sunrisePal, heatIndex-50);
  //fill_solid(leds, NUM_LEDS, color);

  fill_gradient_RGB(leds, locIndex-5, color2, locIndex, color1);
  fill_gradient_RGB(leds, locIndex, color1, locIndex+5, color2);

  EVERY_N_SECONDS(sunriseLength*60/97) {
    if (locIndex <97) {
      locIndex++;
    }
    else {locIndex = 0;}
  }
  
  EVERY_N_SECONDS(interval) {

    if (heatIndex < 255) {
      heatIndex++;
    }
    else {heatIndex = 0;}
  }
}

void blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()



