#include <FastLED.h>
#define NUM_LEDS 97
#define DATA_PIN 3
#define COLOR_ORDER BRG

CRGB leds[NUM_LEDS];

void setup() { 
   FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {
Sonnenaufgang();

FastLED.show();
}

void Sonnenaufgang() {


static const uint8_t sunriseLength = 30;
static const uint8_t interval = (sunriseLength * 60) / 256;
static uint8_t heatIndex = 0;
CRGB color = ColorFromPalette(HeatColors_p, heatIndex);
fill_solid(leds, NUM_LEDS, color);
EVERY_N_SECONDS(interval) {

if (heatIndex < 255) {
heatIndex++;
}
}
}
