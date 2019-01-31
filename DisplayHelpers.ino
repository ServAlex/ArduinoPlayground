#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// light sensor lib
#include <BH1750FVI.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create the Lightsensor instance
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);


void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

	LightSensor.begin();

	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
}

//  display.drawPixel(x, y, WHITE);
//  display.drawLine(x1, y1, x2, y2, WHITE);
//  delay(1);
//  display.clearDisplay();
//  display.display();
//  display.fillRect(x1, y1, x2, y2, WHITE);
//  display.drawRect(x1, y1, x2, y2, WHITE);
//  display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
//  display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
//  display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
//  display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, INVERSE);
//  display.setTextColor(WHITE);
//  display.setTextColor(BLACK, WHITE);
//  display.println(F("Hello, world!"));
//  display.println(3.141592);
//  display.print(F("0x"));
//  display.println(0xDEADBEEF, HEX);
//
//  display.drawBitmap(
//    (display.width()  - LOGO_WIDTH ) / 2,
//    (display.height() - LOGO_HEIGHT) / 2,
//    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
//  static const unsigned char PROGMEM logo_bmp[] =
//  { B00000000, B11000000,
//    B00000001, B11000000,
//    B00000001, B11000000,
//    B00000011, B11100000,
//    B11110011, B11100000,
//    B11111110, B11111000,
//    B01111110, B11111111,
//    B00110011, B10011111,
//    B00011111, B11111100,
//    B00001101, B01110000,
//    B00011011, B10100000,
//    B00111111, B11100000,
//    B00111111, B11110000,
//    B01111100, B11110000,
//    B01110000, B01110000,
//    B00000000, B00110000 };

int intensity_min = 8;
int intensity_max = 60;

void loop(){
/*
  display.setCursor(0, 0);
  display.print("ASDF");
  display.display();
*/ 
	uint16_t lux = LightSensor.GetLightIntensity();
	int normalizer = 5;
	float unscaled = log(lux+normalizer)/log(1.2);

	int x = (int)((unscaled-intensity_min)/(intensity_max - intensity_min)*SCREEN_WIDTH);
//	int x = (int)((float)lux/intensity_max*SCREEN_WIDTH);

/*
	Serial.print("Lux: \t");
	Serial.print(lux);
	Serial.print("\tx: \t");
	Serial.println(x);
*/
	display.clearDisplay();
	display.drawLine(x, 0, x, 30, WHITE);

	display.setCursor(10, SCREEN_HEIGHT - 10);
	display.print(lux);

	display.setCursor(60, SCREEN_HEIGHT - 10);
	display.print(unscaled);

	display.setCursor(SCREEN_WIDTH - 25, SCREEN_HEIGHT - 10);
	display.print(x);

	display.display();

	delay((int)(1000.f/60));
}
