/*
 * Arduino code for Smart Water Bottle Sensor to
 * detect water level and intake from user.
 * Created by Jalan Chau, Laurence DErcole,
 * Hao Guan, and Vivian Wu, March 4, 2020.
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

// create liquid crystal
LiquidCrystal_I2C lcd(0x27, 20, 4);

// create an ultrasonic
Ultrasonic mysensor;

// declare and initialize trigPin (green wire)
int trigPin = 12;//Trig

// declare and initialize echoPin (yellow wire)
int echoPin = 13;

/* 
  duration - takes the duration from echoPin
  cm - calculates in cm the distance water is from the sensor
  percentFull - calculation for the how full the bottle is
*/
long duration, cm, inches, percentFull;

// sets up Serial, LCD, and pins
void setup() {

  // initialize the lcd and turns on light
  lcd.init();
  lcd.backlight();

  // initializes Serial monitor
  Serial.begin (9600);

  // sets up pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}

// to keep ultrasonic reading values
void loop() {

  // sets cursor to print on first line
  lcd.setCursor(0, 0);
  lcd.clear();

  // prints "Percent full:"
  lcd.print("Percent full:");

  // sets trigPin off and delays
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // sets trigPin on and delays
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  // sets trigPin off
  digitalWrite(trigPin, LOW);

  // receives input from ultrasonic sensor
  pinMode(echoPin, INPUT);

  // puts input into duration
  duration = pulseIn(echoPin, HIGH);

  // calculate distance in cm
  cm = (duration / 2) / 29.1;

  // calculates percent full
  percentFull = (24 - cm) * 4.166667;

  // printing for NULL reads
  if (cm > 25) {

    // N/A distance
    Serial.print("N/A");
    lcd.print("N/A");

    // sets cursor at second line
    lcd.setCursor(0, 1);

    // invalid reads
    Serial.print("Invalid read");
    lcd.print("Invalid read");
  }

  // to keep drinking (bottle slightly/is full)
  else if (cm < 16) {

    // bottle is almost full
    if (cm > 3) {

      // prints percentages
      Serial.print(percentFull);
      lcd.print(percentFull);
    }

    // to account for sensor range under 3 cm
    else {

      // print full
      Serial.print("90+");
      lcd.print("90+");
    }

    // sets cursor at second line
    lcd.setCursor(0, 1);

    // reports to keep drinking
    Serial.print("Keep Drinking");
    lcd.print("Keep Drinking");
  }

  // bottle is nearing empty
  else {

    // to account for negative reads
    if (percentFull < 0) {
      percentFull = 0;
    }

    // print percentanges
    Serial.print(percentFull);
    lcd.print(percentFull);

    // set cursor at second line
    lcd.setCursor(0, 1);

    // reports to refill soon
    Serial.print("Refill soon");
    lcd.print("Refill soon");
  }

  // delay at 2 seconds
  delay(2000);

  // resets lcd
  lcd.clear();

  //next line for Serial monitor
  Serial.println();
}
