#include <Wire.h>
#include <SD.h>
#include <SPI.h>

#define IMU_SDA A0
#define IMU_SCL A1
#define IMU_ADDR 0x68

File outputFile;

#pragma region IMU_CODE

void setPin(int pin, int state)
{
  pinMode(pin, state ? INPUT : OUTPUT);
  if (state == 0) digitalWrite(pin, 0);
}

void i2cstart()
{
  setPin(IMU_SDA, 1);
  setPin(IMU_SCL, 1);

  setPin(IMU_SDA, 0);
  setPin(IMU_SCL, 0);  
}

void i2cstop()
{
  setPin(IMU_SDA, 0);
  setPin(IMU_SCL, 1);
  setPin(IMU_SDA, 1);
}

bool i2ctx(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    int thebit = data & (0b10000000 >> i) ? 1 : 0;
    setPin(IMU_SDA, thebit);

    setPin(IMU_SCL, 1);
    delay(1);
    setPin(IMU_SCL, 0);
  }

  setPin(IMU_SDA, 1);
  setPin(IMU_SCL, 1);
  
  bool ack = digitalRead(IMU_SDA);
  setPin(IMU_SCL, 0);

  return ack;
}

uint8_t i2crx(int ack)
{
  setPin(IMU_SDA, 1);
  uint8_t data = 0;

  for (int i = 0; i < 8; i++)
  {
    data = data << 1;
    
    // Wait for secondary to release control
    do
    {
      setPin(IMU_SCL, 1);
    } while (digitalRead(IMU_SCL) == 0);

    if (digitalRead(IMU_SDA)) data = data | 1;

    setPin(IMU_SCL, 0);
  }

  setPin(IMU_SDA, ack ? 0 : 1);

  setPin(IMU_SCL, 1);
  setPin(IMU_SCL, 0);

  setPin(IMU_SDA, 1);

  return data;
}

bool i2ctxaddr(uint8_t data, int doRead)
{
  i2ctx((data << 1) | doRead);
}

void i2c_write8(uint8_t addr, uint8_t reg, uint8_t value)
{
  i2cstart();
  i2ctxaddr(addr, 0);
  i2ctx(reg);
  i2ctx(value);
  i2cstop();
}

uint8_t i2c_read8(uint8_t addr, uint8_t reg)
{
  i2cstart();
  i2ctxaddr(addr, 0);
  i2ctx(reg | 0x80);
  i2cstart();
  i2ctxaddr(addr, 1);
  return i2crx(1);
}

int getTempFromHiLow(uint8_t high, uint8_t low)
{
  uint16_t uTemp = (high << 4) | low;
  return *(int *)&uTemp;
}

void gy521read(float* temp)
{
  //wake up gy521
  i2cstart();

  //send out address
  i2ctxaddr((byte)0x68,0);
  i2ctx(0x41 | 0x80);
  //  i2ctx(0x3b);

  //restart to receive
  i2cstart();

  i2ctxaddr((byte)0x68,1);
  int accxhi = i2crx(1);
  int accxlo = i2crx(0);

  int rawTempVal = (accxhi<<8)|accxlo;
  //int rawTemp = getTempFromHiLow(accxhi, accxlo);

  *temp = ((float)rawTempVal / 340.0) + 36.53;
}


void gy521init()
{
  i2cstart();
  i2ctxaddr(IMU_ADDR, 0);
  i2ctx(0x6B);
  i2ctx(0x00);
  i2cstop();
}


#pragma endregion //IMU_CODE


#pragma region RTC_CODE

int getMonth(){
  int month;
  Wire.beginTransmission(0x68);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    month = (c & 0xF) + (((c & 0x10) >> 4) * 10);
    //Serial.println(month);         // Print the character
  }
  return month;
}

int getYear(){
  int year;

  Wire.beginTransmission(0x68);
  Wire.write(0x06);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    year = (c & 0xF) + (((c & 0xF0) >> 4) * 10);
    //Serial.println(year);         // Print the character
  }

  return year;
}

int getDay(){
  int day;

  Wire.beginTransmission(0x68);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    day = (c & 0xF) + (((c & 0xF0) >> 4) * 10);
    //Serial.println(year);         // Print the character
  }

  return day;
}

int getHour(){
  int hour;

  Wire.beginTransmission(0x68);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    hour = (c & 0xF) + (((c & 0xF0) >> 4) * 10);
    //Serial.println(year);         // Print the character
  }
  
  return hour;
}

int getSecond(){
  int sec;

  Wire.beginTransmission(0x68);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    sec = (c & 0xF) + (((c & 0x70) >> 4) * 10);
    //Serial.println(year);         // Print the character
  }
  
  return sec;
}

int getMinute(){
  int hour;

  Wire.beginTransmission(0x68);
  Wire.write(0x02);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  while(Wire.available()) {
    unsigned char c = Wire.read();    // Receive a byte as character
    hour = (c & 0xF) + (((c & 0xF0) >> 4) * 10);
    //Serial.println(year);         // Print the character
  }
  
  return hour;
}

#pragma endregion //RTC_CODE


#pragma region SD Card



#pragma endregion

#pragma region CALL ME BABY!

float getTemperature()
{
  float x;
  gy521init();
  gy521read(&x);
  return x;
}

String getDate(){
  int month = getMonth();
  int day = getDay();
  int year = getYear();
  int hour = getHour();
  int min = getMinute();
  int sec = getSecond();
  String dateString = "";
  dateString.concat(hour);
  dateString.concat(":");
  dateString.concat(min);
  dateString.concat(":");
  dateString.concat(sec);
  dateString.concat("|");
  dateString.concat(month);
  dateString.concat("/");
  dateString.concat(day);
  dateString.concat("/20");
  dateString.concat(year);
  dateString.concat("|");
  return dateString;
}

#pragma endregion

void errorBlink(int delayMS){
  pinMode(3, OUTPUT);
  while(1){
    digitalWrite(3, HIGH);
    delay(delayMS);
    digitalWrite(3, LOW);
    delay(delayMS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  if (!SD.begin(10)) {
    errorBlink(1000);
  }

  outputFile = SD.open("readings.txt", FILE_WRITE);
  if (!outputFile) {
    errorBlink(500);
  }    

}

void loop() {
  // put your main code here, to run repeatedly:
  
  String output = getDate();
  //Serial.print(buffer);s stated above, on most Arduinos, sprint() does not support floats or doubles.
  //Serial.print("|temperature|");
  float temp = getTemperature();
  output.concat("temperature|");
  output.concat(temp);

  Serial.println(output);
  
  outputFile.println(output.c_str());
  outputFile.flush();
  delay(1000);
}

