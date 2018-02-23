#include <FastLED.h>
#include <SoftwareSerial.h>
#define NUM_LEDS 20 //97
#define DATA_PIN 3
#define COLOR_ORDER BRG

SoftwareSerial BT(10, 11); // RX, TX
String BTMsg;

byte red, green, blue, bright1 = 255, bright2 = 255;
bool stripes, spots;

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,       //black
  80 ,   255,  0,  0,     //red
  170,   255, 80, 20,     //orange
  220,   255, 200, 50,    //bright yellow
  255,   255, 255, 220 }; //full white

CRGBPalette256 sunrisePal = heatmap_gp;

CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(9600);
  BT.begin(115200);
  delay(500);
  BT.setTimeout(500);
  FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {

  if (BT.available()) {
    readMsg();
  }

  Sonnenaufgang();
  //full();
  
  FastLED.show();
}

void full() {
  fill_solid(leds, NUM_LEDS, ColorFromPalette(sunrisePal, 255));


}

void readMsg() {
  char incomingByte = 0;
  byte buff[1];
//  while (BT.available() > 0) {
//    incomingByte = BT.read();    
//    BTMsg = BTMsg + incomingByte;
//  }
      incomingByte = BT.read();
      BTMsg += incomingByte;
      switch (incomingByte) {
      case 'R': BT.readBytes(buff,1);
                red = buff[0];
                fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
                //BTMsg += buff[0];
      case 'G': BT.readBytes(buff,1);
                green = buff[0];
                fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
                //BTMsg += buff[0];
      case 'B': BT.readBytes(buff,1);
                blue = buff[0];
                fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
                //BTMsg += buff[0];
      case 'a': spots != spots;
                BT.readBytes(buff,1);
                if (spots) {
                  bright1 = buff[0];
                }
                else {bright1 = 0;}
      case 'b': stripes != stripes;
                BT.readBytes(buff,1);
                //BTMsg += buff[0];
                if (stripes) {
                  bright2 = buff[0];
                }
                else {bright2 = 0;}    
      case 'c': BT.readBytes(buff,1);
                bright1 = buff[0];
                //BTMsg += buff[0];
      case 'd': BT.readBytes(buff,1);
                bright2 = buff[0];
                //FastLED.setBrightness(bright2);
                //BTMsg += buff[0];
      case 'P': BT.readBytes(buff,1);
                switch (buff[0]) {
                    case '1': 

                    case '2':

                    case '3':

                    case '4': Sonnenaufgang();

                    case '5':
                  break;
                }
        break;
    }
  
  //Serial.println(BTMsg);
  //BTMsg = "";
}


void Sonnenaufgang() {

  static const uint8_t sunriseLength = 5;
  static const uint8_t interval = (sunriseLength * 60) / 256;
  static uint8_t sunIndex = 0;
  static uint8_t locIndex = 7;
  int diffIndex = 0;
  static uint8_t grad = 4;

  CRGB clr1 = ColorFromPalette(sunrisePal, sunIndex);

  leds[locIndex] = clr1;
  diffIndex = sunIndex;
  for (uint8_t i = locIndex + 1; i < NUM_LEDS; i++) {
    diffIndex = diffIndex - (i - locIndex) * grad;
    if (diffIndex > 0) {
      leds[i] = ColorFromPalette(sunrisePal, diffIndex);
    }
  }
  diffIndex = sunIndex;
  for (uint8_t i = locIndex; i > 0;) {
    i--;
    diffIndex = diffIndex - (locIndex - i) * grad;
    if (diffIndex > 0) {
      leds[i] = ColorFromPalette(sunrisePal, diffIndex);
    }
  }


  EVERY_N_SECONDS(sunriseLength * 60 / 2) {
    if (grad > 0) {
      //grad --;
    }
  }
  EVERY_N_SECONDS(20) {
    if (locIndex < NUM_LEDS - 1) {
      locIndex ++;
    }
  }

  EVERY_N_SECONDS(interval) {

    if (sunIndex < 255) {
      sunIndex++;
    }
  }
}

