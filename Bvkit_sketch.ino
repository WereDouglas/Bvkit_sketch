#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial BTserial(10, 11); // RX | TX

// Pick analog outputs, for the UNO these three work well
// use ~560  ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 3
#define greenpin 5
#define bluepin 6
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground

// set to false if using a common cathode LED
#define commonAnode true

// our RGB -> eye-recognized gamma color
byte gammatable[256];
int servoPin = 3;
Servo Servo1;
int angle = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {

  Servo1.attach(servoPin);
  Servo1.write(0);
  //BTserial.begin(9600);

 
  // Serial.println("Color View Test!");

  if (tcs.begin()) {
    // Serial.println("Found sensor");
  } else {
    //   Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  // use these three pins to drive an LED
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);

  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    //Serial.println(gammatable[i]);
  }
   Serial.begin(9600);
}

int timer = 5000;           // The higher the number, the slower the timing.
String strips[] = {"START","LEU", "NIT", "URO", "PRO", "pH", "BLO", "SG", "KET", "BIL", "GLU", "ASC"};
int stripCount = 10;

void loop() {

  // turn on LED

  delay(1000);
  Servo1.write(0);
  angle = 0;

  for (int strip = 0; strip <= stripCount; strip++) {


    //delay(timer);
    // Make servo go to 90 degrees


    uint16_t clear, red, green, blue;

    // takes 50ms to read

    tcs.setInterrupt(true);
    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED

    //Serial.print("C:\t"); Serial.print(clear);
    //Serial.print("\tR:\t"); Serial.print(red);
    //Serial.print("\tG:\t"); Serial.print(green);
    //Serial.print("\tB:\t"); Serial.print(blue);

    // Figure out some basic hex code for visualization
    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;
   //  Serial.print(r);
    if (r > 0) {
      
     Serial.print(strip); Serial.print("-"); Serial.print(strips[strip]); Serial.print("-"); Serial.print("#"); Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX); Serial.print("~");

      angle = angle + 10;
      Servo1.write(angle);
   }

    //  BTserial.print("#");  BTserial.print((int)r, HEX);  BTserial.print((int)g, HEX);  BTserial.print((int)b, HEX);


    // Serial.print((int)r ); Serial.print(" "); Serial.print((int)g);Serial.print(" ");  Serial.println((int)b );

    // analogWrite(redpin, gammatable[(int)r]);
    //  analogWrite(greenpin, gammatable[(int)g]);



    // turn the pin off:
    delay(timer);

  }
  //Serial.print("STOP");
  //Serial.println();
  //delay(1000);


}

