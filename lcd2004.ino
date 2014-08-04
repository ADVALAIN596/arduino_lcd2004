#include <dht.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

dht DHT; 

#define dht_dpin A1 
#define I2C_ADDR    0x3F  
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define DS1307_I2C_ADDRESS 0x68

int n = 1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


int redPin = 11;
int greenPin = 10;
int bluePin = 9;

int redPin1 = 7;
int greenPin1 = 6;
int bluePin1 = 5;

byte decToBcd(byte val) 
{ 
  return ( (val/10*16) + (val%10) ); 
} 
  
// Convert binary coded decimal to normal decimal numbers 
byte bcdToDec(byte val) 
{ 
  return ( (val/16*10) + (val%16) ); 
} 
  
// 1) Sets the date and time on the ds1307 
// 2) Starts the clock 
// 3) Sets hour mode to 24 hour clock 
// Assumes you're passing in valid numbers 
void setDateDs1307(byte second,        // 0-59 
                   byte minute,        // 0-59 
                   byte hour,          // 1-23 
                   byte dayOfWeek,     // 1-7 
                   byte dayOfMonth,    // 1-28/29/30/31 
                   byte month,         // 1-12 
                   byte year)          // 0-99 
{ 
   Wire.beginTransmission(DS1307_I2C_ADDRESS); 
   Wire.write(0); 
   Wire.write(decToBcd(second));    
   Wire.write(decToBcd(minute)); 
   Wire.write(decToBcd(hour));      
   Wire.write(decToBcd(dayOfWeek)); 
   Wire.write(decToBcd(dayOfMonth)); 
   Wire.write(decToBcd(month)); 
   Wire.write(decToBcd(year)); 
   Wire.endTransmission(); 
} 
  
// Gets the date and time from the ds1307 
void getDateDs1307(byte *second, 
          byte *minute, 
          byte *hour, 
          byte *dayOfWeek, 
          byte *dayOfMonth, 
          byte *month, 
          byte *year) 
{ 
  // Reset the register pointer 
  Wire.beginTransmission(DS1307_I2C_ADDRESS); 
  Wire.write(0); 
  Wire.endTransmission(); 
  
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7); 
  
  // A few of these need masks because certain bits are control bits 
  *second     = bcdToDec(Wire.read() & 0x7f); 
  *minute     = bcdToDec(Wire.read()); 
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm 
  *dayOfWeek  = bcdToDec(Wire.read()); 
  *dayOfMonth = bcdToDec(Wire.read()); 
  *month      = bcdToDec(Wire.read()); 
  *year       = bcdToDec(Wire.read()); 
} 
  

void setup()
{
  lcd.begin (20,4);
  
// Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();    
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
  Wire.begin(); 
  
  /*
  second = 00; 
  minute = 41; 
  hour   = 01; 
  //dayOfWeek  = 6; 
  dayOfMonth = 4; 
  month      = 8; 
  year       = 14; 
  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year); 
  */
}

void loop()
{
  int sensorValue = analogRead(A1);
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year; 
  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); 
  
  
  DHT.read11(dht_dpin);
  lcd.clear();
  lcd.print("Voltage: ");
  lcd.print(sensorValue/204);
  lcd.print(" Volt");
  lcd.setCursor ( 0, 1 );        
  lcd.print("H:");
  lcd.print(DHT.humidity);
  lcd.print(" %  ");
  lcd.print("T:");
  lcd.print(DHT.temperature);
  lcd.print(" C");  
  lcd.setCursor ( 0, 2 );       
  lcd.print("Time: ");
  lcd.print("  ");
  lcd.print(hour, DEC);  
  lcd.print(":");
  lcd.print(minute, DEC);
  lcd.print(":");
  lcd.print(second, DEC);
  lcd.setCursor ( 0, 3 );       
  lcd.print("Date:   ");
  lcd.print(dayOfMonth, DEC);
  lcd.print("-");
  lcd.print(month, DEC);
  lcd.print("-");
  lcd.print(year, DEC);
  delay(10000);
  lcd.clear();
  lcd.print("   linuxprofi.org ");
  lcd.setCursor ( 0, 1 ); 
  lcd.print("  Microelectronic");
  lcd.setCursor ( 0, 2 ); 
  lcd.print("    Prototyping");
  lcd.setCursor ( 0, 3 );
  lcd.print("   TRON-BOT v 1.2");
  
setColor(255, 0, 0); // red
delay(1000);
setColor(0, 255, 0); // green
delay(1000);
setColor(0, 0, 255); // blue
delay(1000);
setColor(255, 255, 0); // yellow
delay(1000);
setColor(80, 0, 80); // purple
delay(1000);
setColor(0, 255, 255); // aqua
delay(1000);
 }
 
void setColor(int red, int green, int blue)
{
#ifdef COMMON_ANODE
red = 255 - red;
green = 255 - green;
blue = 255 - blue;
#endif
analogWrite(redPin, red);
analogWrite(greenPin, green);
analogWrite(bluePin, blue);
analogWrite(redPin1, red);
analogWrite(greenPin1, green);
analogWrite(bluePin1, blue);
}


