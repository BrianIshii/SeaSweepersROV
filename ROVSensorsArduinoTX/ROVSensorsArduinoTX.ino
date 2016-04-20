#include <OneWire.h> 


#include <Wire.h>
#include "MS5837.h"

//compass sensor needs wire.h too
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

//temp probe
int DS18S20_Pin = 2;
OneWire ds(DS18S20_Pin);
//pressure sensor
MS5837 sensor;

//code for inside temp sensor
#include "DHT.h" //library
#define DHTPIN 7  // what digital pin we're connected to for temperature sensor
#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE); //set varaibles



// bytes to send
int data [46];    
//starting bytes
int start [2];    
//analog pins
int sensorPinZero = A0;
int sensorPinOne = A1;
int sensorPinFour = A4;
int sensorPinFive = A5;
int sensorPinEight = A8;
int sensorPinNine = A9;
int sensorPinTen = A10;
int sensorPinEleven = A11;
int sensorPinTwelve = A12;
int sensorPinThirteen = A13;
int sensorPinFourteen = A14;
int sensorPinFifteen = A15;

// variable to store the value coming from the sensor
int currentV1 = 0;  
int currentV2 = 0;  
int currentV3 = 0;  
int currentV4 = 0;  
int currentH1 = 0;
int currentH2 = 0;
int currentH3 = 0;
int currentH4 = 0;
int totalVolt = 0;
int totalAmp = 0;

//water sensors
int waterOne = 0;
int waterTwo = 0;

unsigned char checksum0;
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char checksum8;
unsigned char checksum9;
unsigned char checksum10;
unsigned char checksum11;
unsigned char checksum12;
unsigned char checksum13;
unsigned char checksum14;
unsigned char checksum15;
unsigned char checksum16;
unsigned char checksum17;
unsigned char checksum18;
unsigned char checksum19;
unsigned char checksum20;
unsigned char checksum21;
unsigned char checksum22;
unsigned char handshake;
unsigned char handshake2;
unsigned char handshake3;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
    if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  
  //pressure sensor
  Wire.begin();
  sensor.init();
  sensor.setFluidDensity(997);
  //inside temp sensor
   dht.begin();
} 

void loop() {
  //DHT21 read inside temp sensor
  float hum = dht.readHumidity();
  float inTemp = dht.readTemperature();
  int checkNumber;
  //inside temp sensor check
  checkNumber = checkSensorDHT21(hum,inTemp);
  //Serial1.writeln(checkNumber);//delete when final
  if (checkNumber == 1) {
    hum = 999;
    inTemp = 999;
  }
  sensor.read();

  float alt = sensor.altitude();
  float dep = sensor.depth();
  int dataDepth = dep* 100;

  float outTemp = sensor.temperature();
  float pre = sensor.pressure();

  // READ THE SENSORS:

  //DHT21 read inside temp sensor
  int insideTemperature = inTemp;
  data[0] = insideTemperature & 0xFF;      //least significant 8 bit byte
  data[1] = (insideTemperature >> 8);      //most significant 2 bits
  
  int insideHumidity = hum;
  data[2] = insideHumidity & 0xFF;
  data[3] = (insideHumidity >> 8);
 
  int altitude = -1* alt;
  data[4] = altitude & 0xFF;
  data[5] = (altitude >> 8);
  
  int depth = dep * 100;   // reads the value of the potentiometer (Joystick2B between 0 and 1023)
  data[6] = depth & 0xFF;      
  data[7] = (depth >> 8);      
 
  int outsideTemperature = -1*outTemp;
  data[8] = outsideTemperature & 0xFF;
  data[9] = (outsideTemperature >> 8);

  int pressure = pre;
  data[10] = pressure & 0xFF;
  data[11] = (pressure >> 8);

  int probeTemperature = getTemp() * 100;
  data[12] = probeTemperature & 0xFF;
  data[13] = (probeTemperature >> 8);

//currents

  currentV1 = analogRead(sensorPinEight);
  data[14] = currentV1 & 0xFF;
  data[15] = (currentV1 >> 8);

  currentV2 = analogRead(sensorPinNine);
  data[16] = currentV2 & 0xFF;
  data[17] = (currentV2 >> 8);

  currentV3 = analogRead(sensorPinTen);
  data[18] = currentV3 & 0xFF;
  data[19] = (currentV3 >> 8);

  currentV4 = analogRead(sensorPinEleven);
  data[20] = currentV4 & 0xFF;
  data[21] = (currentV4 >> 8);

  currentH1 = analogRead(sensorPinTwelve);
  data[22] = currentH1 & 0xFF;
  data[23] = (currentH1 >> 8);

  currentH2 = analogRead(sensorPinThirteen);
  data[24] = currentH2 & 0xFF;
  data[25] = (currentH2 >> 8);

  currentH3 = analogRead(sensorPinFourteen);
  data[26] = currentH3 & 0xFF;
  data[27] = (currentH3 >> 8);

  currentH4 = analogRead(sensorPinFifteen);
  data[28] = currentH4 & 0xFF;
  data[29] = (currentH4 >> 8);

//volts and amps

  totalVolt = analogRead(sensorPinZero);
  data[30] = totalVolt & 0xFF;
  data[31] = (totalVolt >> 8);

  totalAmp = analogRead(sensorPinOne);
  data[32] = totalAmp & 0xFF;
  data[33] = (totalAmp >> 8);

////compass
  sensors_event_t accelEvent; 
  sensors_event_t magEvent; 
  
  accel.getEvent(&accelEvent);
  mag.getEvent(&magEvent);

  int XAcceleration = accelEvent.acceleration.x;
  data[34] = XAcceleration & 0xFF;
  data[35] = (XAcceleration >> 8);

  int YAcceleration = accelEvent.acceleration.y;
  data[36] = YAcceleration & 0xFF;
  data[37] = (YAcceleration >> 8);

  int ZAcceleration = accelEvent.acceleration.z;
  data[38] = ZAcceleration & 0xFF;
  data[39] = (ZAcceleration >> 8);

  float Pi = 3.14159;
  int angle = (atan2(magEvent.magnetic.y,magEvent.magnetic.x) * 180) / Pi; //variable for compass
    if (angle < 0)
  {
    angle = 360 + angle;
  }
  data[40] = angle & 0xFF;
  data[41] = (angle >> 8);

int waterOne = 0;
int waterTwo = 0;
  waterOne = analogRead(sensorPinFour);
  data[42] = waterOne & 0xFF;
  data[43] = (waterOne >> 8);

  waterTwo = analogRead(sensorPinFive);
  data[44] = waterTwo & 0xFF;
  data[45] = (waterTwo >> 8);
  
  // CREATE CHECKSUMS:

  checksum0 = ~(data[0] + data[1]) + 1;

  checksum1 = ~(data[2] + data[3]) + 1;

  checksum2 = ~(data[4] + data[5]) + 1;

  checksum3 = ~(data[6] + data[7]) + 1;

  //checksum4 = ~(data[8] + data[9]) + 1;

  checksum5 = ~(data[10] + data[11]) + 1;
  
  checksum6 = ~(data[12] + data[13]) + 1;

  checksum7 = ~(data[14] + data[15]) + 1;

  checksum8 = ~(data[16] + data[17]) + 1;

  checksum9 = ~(data[18] + data[19]) + 1;

  checksum10 = ~(data[20] + data[21]) + 1;

  checksum11 = ~(data[22] + data[23]) + 1;

  checksum12 = ~(data[24] + data[25]) + 1;
  
  checksum13 = ~(data[26] + data[27]) + 1;

  checksum14 = ~(data[28] + data[29]) + 1;

  checksum15 = ~(data[30] + data[31]) + 1;

  checksum16 = ~(data[32] + data[33]) + 1;

  checksum17 = ~(data[34] + data[35]) + 1;

  checksum18 = ~(data[36] + data[37]) + 1;

  checksum19 = ~(data[38] + data[39]) + 1;
  
  checksum20 = ~(data[40] + data[41]) + 1;

  checksum21 = ~(data[42] + data[43]) + 1;

  checksum22 = ~(data[44] + data[45]) + 1;

  // WRITE VALUES AND CHECKSUMS TO SERIAL:
//  Serial.print(insideTemperature);
//  Serial.print(" ");
//  //Serial.print(insideHumidity);
//  //Serial.print(" ");
//  //Serial.print(altitude);
//  //Serial.print(" ");
//  Serial.print(depth);
//  Serial.print(" ");
//  //Serial.print(outsideTemperature);
//  //Serial.print(" ");
//  Serial.print(pressure);
//  Serial.print(" ");
//  Serial.print(probeTemperature);
//  Serial.print(" ");
//  Serial.print(currentV1);
//  Serial.print(" ");
//  Serial.print(currentV2);
//  Serial.print(" ");
//  Serial.print(currentV3);
//  Serial.print(" ");
//  Serial.print(currentV4);
//  Serial.print(" ");
//  Serial.print(currentH1);
//  Serial.print(" "); 
//  Serial.print(currentH2);
//  Serial.print(" "); 
//  Serial.print(currentH3);
//  Serial.print(" ");  
//  Serial.print(currentH4);
//  Serial.print(" ");
//  Serial.print(totalVolt);
//  Serial.print(" ");
//  Serial.print(totalAmp);
//  Serial.print(" ");
//  Serial.print(XAcceleration);
//  Serial.print(" ");
//  Serial.print(YAcceleration);
//  Serial.print(" ");
//  Serial.print(ZAcceleration);
//  Serial.print(" ");
//  Serial.print(angle);
//  Serial.print(" ");
//  Serial.print(waterOne);
//  Serial.print(" ");
//  Serial.println(waterTwo);
  
    while (Serial1.available() < 3) {
    ; //wait for request from receiver
  }
  handshake = Serial1.read();
  handshake2 = Serial1.read();
  handshake3 = Serial1.read();

  if ((handshake == 1) && (handshake2 == 2) && (handshake3 == 3)) {    
    
    Serial1.write(255);
    Serial1.write(255);
    Serial1.write(255);

    Serial1.write(data[0]);    
    Serial1.write(data[1]);
    Serial1.write(checksum0);
//
//    Serial1.write(data[2]);
//    Serial1.write(data[3]); 
//    Serial1.write(checksum1);
        
//    Serial1.write(data[4]);
//    Serial1.write(data[5]);
//    Serial1.write(checksum2);
    
    Serial1.write(data[6]);
    Serial1.write(data[7]);
    Serial1.write(checksum3);
    
//    Serial1.write(data[8]);
//    Serial1.write(data[9]); 
//    Serial1.write(checksum4);
    
    Serial1.write(data[10]);
    Serial1.write(data[11]);
    Serial1.write(checksum5);
    
    Serial1.write(data[12]);
    Serial1.write(data[13]);
    Serial1.write(checksum6);
    
    Serial1.write(data[14]);    
    Serial1.write(data[15]);
    Serial1.write(checksum7);
    
    Serial1.write(data[16]);
    Serial1.write(data[17]); 
    Serial1.write(checksum8);

    Serial1.write(data[18]);
    Serial1.write(data[19]);
    Serial1.write(checksum9);

    Serial1.write(data[20]);
    Serial1.write(data[21]);
    Serial1.write(checksum10);
  
    Serial1.write(data[22]);  
    Serial1.write(data[23]); 
    Serial1.write(checksum11);

    Serial1.write(data[24]);
    Serial1.write(data[25]);
    Serial1.write(checksum12);

    Serial1.write(data[26]);
    Serial1.write(data[27]);
    Serial1.write(checksum13);

    Serial1.write(data[28]);
    Serial1.write(data[29]);
    Serial1.write(checksum14);

    Serial1.write(data[30]);
    Serial1.write(data[31]);
    Serial1.write(checksum15);

    Serial1.write(data[32]);
    Serial1.write(data[33]);
    Serial1.write(checksum16);

    Serial1.write(data[34]);
    Serial1.write(data[35]);
    Serial1.write(checksum17);

    Serial1.write(data[36]);    
    Serial1.write(data[37]); 
    Serial1.write(checksum18);
    
    Serial1.write(data[38]);
    Serial1.write(data[39]);
    Serial1.write(checksum19);

    Serial1.write(data[40]);
    Serial1.write(data[41]);
    Serial1.write(checksum20);
    
    Serial1.write(data[42]);
    Serial1.write(data[43]);
    Serial1.write(checksum21);
    
    Serial1.write(data[44]);
    Serial1.write(data[45]);
    Serial1.write(checksum22);
    
    Serial1.write(2);
    delay(50);     // delay in between reads for stability
  }

}
//function for getting probe temp
int getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return 999;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   //Serial1.writeln("CRC is not valid!");
   return 999;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return 999;
 }

 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}
int checkSensorDHT21(float h,float t) {
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 1;
  }
  return 0;
}    
