#include <LiquidCrystal.h> //library for the LCD (LCD1602)
#include <SoftTimers.h> //library to do "multithreading" (protothreading)
#include <dht11.h> //library  to interface with the temperature/humidity sensor (DHT11)
#include <Servo.h> //library for the servo (SG90 Micro Servo)

#define DHT11PIN 10 //DHT11 data pin (leftmost for this specific make)
Servo point; //init of servo
dht11 DHT11; //init of DHT11
SoftTimer dht11timer; //timer for DHT11 thread
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //LCD init
int b_edge = 500; //at which resistance (of photoresistor) the weather icon should change
int h_edge = 86; //humidity edge
byte Cloudy[8] =
{
0b00000,
0b00000,
0b00000,
0b01100,
0b01010,
0b10001,
0b01110,
0b00000
};
byte Crescent[8] =
{
0b00000,
0b01000,
0b00100,
0b00010,
0b00010,
0b00100,
0b01000,
0b00000
};
byte Rainy[8] =
{
0b00000,
0b00100,
0b00100,
0b01010,
0b01010,
0b00100,
0b00000,
0b00000
};
byte Sunny[8] =
{
0b00000,
0b00000,
0b00000,
0b10101,
0b01110,
0b11011,
0b01110,
0b10101
};
void setup() {
  dht11timer.setTimeOutTime(2000); //init of dht11timer's thread thing
  point.attach(6); //servo pin attachment
  lcd.begin(16, 2); //LCD setup (16x2 chars)
  lcd.createChar(0, Cloudy); //"installation" of custom char for LCD
  lcd.createChar(1, Crescent);
  lcd.createChar(2, Rainy);
  lcd.createChar(3, Sunny);
  delay(500);
  dht11timer.reset(); //thread start
}

void loop() {
  delay(200); //loop speed (so servo has time to respond)
  int lgt = analogRead(A0); //read photoresistor, value 0-1024
  int servoangle = map(lgt, 200, 1000, 0, 180); //map the resistance to the possible angles of servo (0-180)
  point.write(servoangle); //move servo
  lcd.setCursor(15, 0); //move cursor to top right
  if (lgt > b_edge) { //check change threshold
    if (DHT11.humidity > h_edge) {
      //wet and bright -> rain (2)
      lcd.write(byte(2));
      lcd
      } else {
        //dry and bright -> sin (3)
        lcd.write(byte(3));
        }
  } else {
    if (DHT11.humidity > h_edge){
      //wet and dark -> cloud (0)
      lcd.write(byte(0));
    } else {
      //dry and dark -> crescent (1)
      lcd.write(byte(1));
      }yl
  }
  lcd.setCursor(10, 1); //print actual "resistance" value (debug)
  lcd.print(lgt);
  if (dht11timer.hasTimedOut()) //if 2000 ms have passed
  {
    int chk = DHT11.read(DHT11PIN); //read temperature and humidity
    lcd.clear(); //clear screen (to get rid of possible glitches
    lcd.setCursor(0, 0); //move cursor to top left
    lcd.print("hum: ");
    lcd.print(DHT11.humidity); //print the humidity
    lcd.setCursor(0, 1);
    lcd.print("tmp: ");
    lcd.print(DHT11.temperature); //print the temperature
    dht11timer.reset(); //reset the thread timer to 0 (starts counting back up to 2000)
  }
}
