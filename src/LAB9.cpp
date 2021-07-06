/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Vishal/Documents/CTDProjects/LAB9/src/LAB9.ino"
/*
 * Project LAB9
 * Description:
 * Author:
 * Date:
 */

// libraries
#include <blynk.h>
#include "oled-wing-adafruit.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"

void setup();
void loop();
#line 13 "c:/Users/Vishal/Documents/CTDProjects/LAB9/src/LAB9.ino"
SYSTEM_THREAD(ENABLED);

// variables
OledWingAdafruit display;
uint64_t reading;
double voltage;
double temperature;
VCNL4040 proximitySensor;
uint16_t low;
uint16_t high;
uint16_t prevCount = 1;
uint16_t count = 0;
bool lightLow;
bool lightMid;
bool lightHigh;

// setup() runs once, when the device is first turned on.
void setup()
{
  Serial.begin(9600);
  display.clearDisplay();
  // TMP
  pinMode(A5, INPUT);
  // Poten
  pinMode(A4, INPUT);
  // Button
  pinMode(D5, INPUT);
  // LEDs
  pinMode(D8, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  Wire.begin();
  display.setup();
  display.clearDisplay();
  Blynk.begin("V3TcOKgPtNu5RcwyR1xbY5bpxEJjarlU", IPAddress(167, 172, 234, 162), 9090);
  proximitySensor.begin();
  proximitySensor.powerOnProximity();
  proximitySensor.powerOnAmbient();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{

  // The core of your code will likely live here.
  display.loop();
  Blynk.run();

  // light level
  unsigned int ambientValue = proximitySensor.getAmbient();

  //blinks blue if below low
  if (ambientValue < low)
  {
    digitalWrite(D8, HIGH);
    digitalWrite(D7, LOW);
    digitalWrite(D6, LOW);

    if (lightLow == false)
    {
        Blynk.notify("Light level transitioned over a set point"); // sends message to phone
    }

    lightLow = true;
    lightMid = false;
    lightHigh = false;
  }

  // blinks green if above low but below high
  else if (ambientValue > low && ambientValue < high)
  {
    digitalWrite(D8, LOW);
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);

    if (lightMid == false)
    {
      Blynk.notify("Light level transitioned over a set point"); // sends message to phone
    }

    lightLow = false;
    lightMid = false;
    lightHigh = true;
  }

  // blinks yellow if above high
  else
  {
    digitalWrite(D8, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D6, HIGH);

    if (lightHigh == false)
    {
      Blynk.notify("Light level transitioned over a set point"); // sends message to phone
    }

    lightLow = false;
    lightMid = false;
    lightHigh = true;
  }

  // calc temp and update gauge
  reading = analogRead(A5);
  voltage = (reading * 3.3) / 4095.0;  //converts the reading to a voltage value
  temperature = (voltage - 0.5) * 100; //converts the voltage value to temperature in celsius
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V2, temperature * 1.8 + 32);

  // temp mode
  if (display.pressedA())
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(temperature);
    display.println(" C");
    display.print(temperature * 1.8 + 32); // convert to farenheit
    display.println(" F");
    display.display();
  }

  if (display.pressedB())
  {
    // Calibrating low and high
    Serial.println("Set the low value");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Set the low value");
    display.display();
    if (digitalRead(D5))
    {
      low = analogRead(A4);
      Serial.println(low);
    }
  }

  if (display.pressedC())
  {
    // Calibrating low and high
    Serial.println("Set the high value");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Set the High value");
    display.display();
    if (digitalRead(D5))
    {
      high = analogRead(A4);
      Serial.println(high);
    }
  }
}

BLYNK_WRITE(V1)
{
  int pinData = param.asInt();

  if (pinData == 1)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("You pressed the virtual button");
    display.display();
  }
}