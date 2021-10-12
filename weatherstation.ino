#include <LiquidCrystal.h>
#include <SoftTimers.h>
#include <dht11.h>
#include <Servo.h>
#define DHT11PIN 10

Servo point;
dht11 DHT11;
SoftTimer dht11timer;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
byte Sus[8] =
{
0b11111,
0b10001,
0b10001,
0b11111,
0b11111,
0b11111,
0b11011,
0b11011
};


void setup() {
  dht11timer.setTimeOutTime(2000);
  point.attach(6);
  lcd.begin(16, 2);
  lcd.createChar(0, Sus);
  lcd.print("temp: ");
  lcd.setCursor(0,1);
  lcd.print("hum: ");
  delay(500);
  dht11timer.reset();
}

void loop() {
  delay(200);
  int lgt = analogRead(A0);
  int servoangle = map(lgt, 200, 1000, 0, 180);
  point.write(servoangle);
  lcd.setCursor(15, 0);
  lcd.write(byte(0));
  lcd.setCursor(10, 1);
  lcd.print(lgt);
  if (dht11timer.hasTimedOut())
  {
    int chk = DHT11.read(DHT11PIN);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("hum: ");
    lcd.print(DHT11.humidity);
    lcd.setCursor(0, 1);
    lcd.print("tmp: ");
    lcd.print(DHT11.temperature);
    dht11timer.reset();
  }
}
