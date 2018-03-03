#include <FastLED.h>
#include <SoftwareSerial.h>
#define NUM_LEDS 20 //97
#define DATA_PIN 3
#define COLOR_ORDER BRG

SoftwareSerial BT(10, 11); // RX, TX

byte red, green, blue, bright1 = 255, bright2 = 255, prog = 1;
bool stripes, spots;

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,       //black
  80 ,   255,  0,  0,     //red
  170,   255, 80, 20,     //orange
  220,   255, 200, 50,    //bright yellow
  255,   255, 255, 220    //full white
};

CRGBPalette256 sunrisePal = heatmap_gp;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  BT.begin(115200);
  delay(500);
  BT.setTimeout(500);
  FastLED.addLeds<WS2811, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
}

void loop() {

  while (BT.available()) {
    readMsg();
  }

  switch (prog) {
    case 1: fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
      FastLED.setBrightness(bright2);
      break;
    case 2: break;
    case 3: break;
    case 4: Sonnenaufgang(); break;
    case 5: break;
  }

  FastLED.show();
}


void readMsg() {
  String command;
  String dev;
  char* var;
  byte ivar;
  char BTMsg[15];
  int i = 0;

  while (BT.available() > 0 && i < 15) {
    BTMsg[i] = BT.read();
    i ++;
  }

  //  Serial.println(BTMsg);

  //  BT.readBytesUntil(',', command, 5);
  //  BT.readBytesUntil(',', dev, 5);
  //  BT.readBytesUntil('.', var, 5);

  command = strtok(BTMsg, ",.");
  dev = strtok(NULL, ",.");
  var = strtok(NULL, ",.");

  ivar = atoi(var);

  if (command == "set") {
    switch (dev[0]) {
      case 'a': bright1 = ivar; break;
      case 'b': bright2 = ivar; break;
      case 'R': red = ivar; break;
      case 'G': green = ivar; break;
      case 'B': blue = ivar; break;
      case 'P': prog = ivar; break;
    }
  }
  if (command == "sw") {
    switch (dev[0]) {
      case 'a': spots != spots; break;
      case 'b': stripes != stripes; break;
    }
  }

  //  Serial.print(command);
  //  Serial.print(dev);
  //  Serial.print(ivar);
  //  Serial.print('\n');

}


void Sonnenaufgang() {

  static const byte sunriseLength = 5;
  static const unsigned int interval = (sunriseLength * 60000) / 256; //milliseconds!!
  static byte sunIndex = 0;
  static byte maxIndex = NUM_LEDS / 4;
  static byte clr = 0;
  static unsigned int width = 1;

  for (byte i = 0; i < NUM_LEDS; i++) {
    clr = sunIndex * exp((-1 * pow((i - maxIndex), 2)) / (width));
    leds[i] = ColorFromPalette(sunrisePal, clr);
  }

  EVERY_N_MILLISECONDS((sunriseLength * 60000) / (NUM_LEDS / 1 / 2)) {
    if (maxIndex < NUM_LEDS - 1) {
      maxIndex ++;
    }
  }
  EVERY_N_MILLISECONDS(interval) {
    if (width < 65535) {
      width ++;
    }
  }
  EVERY_N_MILLISECONDS(interval) {

    if (sunIndex < 255) {
      sunIndex++;
    }
  }
}


