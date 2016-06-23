/*
  Receiving Motor Code
  Reads bytes from Transmitting Motor Code and put thems back together
  Uses read bytes to control motors, servo, and light

  Attempt 3 of mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2016
*/

#include <Servo.h>
Servo myservo;
int ServoVal;
int beautiful;

Servo mylight;
int LightVal;
int beautiful2;

unsigned char val[16];    // variable to read the value from the analog pin
int Joystick1A;      // 0-1024 received from serial
int Joystick1B;
int Joystick2A; 
  int Joystick2AMap;
int Joystick2B;      // 0-1024 received from serial
  int Joystick2BMap;
int Joystick3A;
  int Joystick3AMap;
int Joystick3B;
  int Joystick3BMap;

unsigned char checksum0;
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char FirstByte;
unsigned char LastByte;

unsigned char checkit0;
unsigned char checkit1;
unsigned char checkit2;
unsigned char checkit3;
unsigned char checkit4;
unsigned char checkit5;
unsigned char checkit6;
unsigned char checkit7;

//ALL MOTORS:
int V1Direction = 31;
int V1PWM = 2;     //MOTOR V1 front

int V2Direction = 33;
int V2PWM = 3;     //MOTOR V2 front

int V3Direction = 35;
int V3PWM = 4;     //MOTOR V3 back

int V4Direction = 37;
int V4PWM = 5;     //MOTOR V4 back

int H1Direction = 45;
int H1PWM = 9;     //MOTOR H1  forward left

int H2Direction = 43;
int H2PWM = 8;     //MOTOR H2 forward right

int H3Direction = 41;
int H3PWM = 7;     //MOTOR H3 back left

int H4Direction = 39;
int H4PWM = 6;     //MOTOR H4 back right



int Vertical;
int Pitch;
int Crab;
int ForBack;
int Yaw;

int Left;
int LeftMap;
int Right;
int RightMap;
int Front;
int FrontMap;
int Back;
int BackMap;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  //PWM
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(37, OUTPUT);  //DIR
  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  pinMode(43, OUTPUT);
  pinMode(45, OUTPUT);

  pinMode(30, OUTPUT);
  digitalWrite (30, HIGH);
  pinMode(32, OUTPUT);
  digitalWrite (32, HIGH);
  pinMode(34, OUTPUT);
  digitalWrite (34, HIGH);
  pinMode(36, OUTPUT);
  digitalWrite (36, HIGH);
  pinMode(38, OUTPUT);
  digitalWrite (38, HIGH);     //Driving HIGH for SLP pins
  pinMode(40, OUTPUT);
  digitalWrite (40, HIGH);
  pinMode(42, OUTPUT);
  digitalWrite (42, HIGH);
  pinMode(33, OUTPUT);
  digitalWrite (44, HIGH);


  myservo.attach(11);
  mylight.attach(13);
  delay(1000);
}

void loop() {

//  Serial.println (analogRead(Current1));

  Serial1.write(1);
  Serial1.write(2);
  Serial1.write(3);

  if(Serial1.available()<1) {
    //Serial.println("bye");
  }

  if (Serial1.available()>26) {  
    //Serial.println("hi");

  FirstByte=Serial1.read();

  if (FirstByte == 1) {
    //Serial.print("HII");
    val[0]=Serial1.read();         // least significant 8 bits
    val[1]=Serial1.read();         // most significant 2 bits   
     checksum0=Serial1.read();     // checksum

    val[2]=Serial1.read();
    val[3]=Serial1.read();
     checksum1=Serial1.read();

    val[4]=Serial1.read();
    val[5]=Serial1.read(); 
     checksum2=Serial1.read();

    val[6]=Serial1.read();         // least significant 8 bits
    val[7]=Serial1.read();         // most significant 2 bits   
     checksum3=Serial1.read();     // checksum

    val[8]=Serial1.read();
    val[9]=Serial1.read();
     checksum4=Serial1.read();

    val[10]=Serial1.read();
    val[11]=Serial1.read(); 
     checksum5=Serial1.read();

    val[12]=Serial1.read();
    val[13]=Serial1.read();
     checksum6=Serial1.read();

    val[14]=Serial1.read();
    val[15]=Serial1.read();
     checksum7=Serial1.read();

   LastByte = Serial1.read();
  //Serial.print("HI");
// get the checksum last
    checkit0 = val[0]+val[1]+checksum0;
    checkit1 = val[2]+val[3]+checksum1;
    checkit2 = val[4]+val[5]+checksum2; 
    checkit3 = val[6]+val[7]+checksum3;
    checkit4 = val[8]+val[9]+checksum4;
    checkit5 = val[10]+val[11]+checksum5; 
    checkit6 = val[12]+val[13]+checksum6;
    checkit7 = val[14]+val[15]+checksum7;
  }
//if (LastByte == 2) {
  if (checkit0==0)  {
    Joystick1A = val[1]<<8 | val[0]; }

  if (checkit1==0)  {
    Joystick1B = val[3]<<8 | val[2]; }

  if (checkit2==0)  {
    Joystick2A = val[5]<<8 | val[4]; }

  if (checkit3==0)  {
    Joystick2B = val[7]<<8 | val[6]; }

  if (checkit4==0)  {
    Joystick3A = val[9]<<8 | val[8]; }

  if (checkit5==0)  {
    Joystick3B = val[11]<<8 | val[10]; }

  if (checkit6==0)  {
    ServoVal = val[13]<<8 | val[12]; }

  if (checkit7==0)  {
    LightVal = val[15]<<8 | val[14]; }

//}
//else (Serial.println("Goodbye"));
//    Serial.print(Joystick1A);
//    Serial.print(" ");
//    Serial.print(Joystick1B);
//    Serial.print(" ");
//    Serial.print(Joystick2A);
//    Serial.print(" ");
//    Serial.print(Joystick2B);
//    Serial.print(" ");
//    Serial.print(Joystick3A);
//    Serial.print(" ");
//    Serial.print(Joystick3B);
//    Serial.print(" ");
//    Serial.println(LightVal);
//    Serial.print(" ");
//    Serial.println(ServoVal);

  
  }

//Guarantee numbers don't exceed expectations
if (Joystick1A > 1024) {
  Joystick1A = 1024; }
if (Joystick1B > 1024) {
  Joystick1B = 1024; }
if (Joystick2A > 1024) {
  Joystick2A = 1024; }
if (Joystick2B > 1024) {
  Joystick2B = 1024; }
if (Joystick3A > 1024) {
  Joystick3A = 1024; }
if (Joystick3B > 1024) {
  Joystick3B = 1024; }
if (ServoVal > 1024) {
  ServoVal = 1024; }
if(LightVal>1024){
  LightVal=1024;
}

if (Joystick1A < 0) {
  Joystick1A = 0; }
if (Joystick1B < 0) {
  Joystick1B = 0; }
if (Joystick2A < 0) {
  Joystick2A = 0; }
if (Joystick2B < 0) {
  Joystick2B = 0; }
if (Joystick3A < 0) {
  Joystick3A = 0; }
if (Joystick3B < 0) {
  Joystick3B = 0; }
if (ServoVal < 0) {
  ServoVal = 0; }
if (LightVal<0){
  LightVal=0;
}

Joystick2AMap = map(Joystick2A, 0, 1023, -512, 512);
Joystick2BMap = map(Joystick2B, 0, 1023, -512, 512);
  if (Joystick2AMap > 512){
  Joystick2AMap = 512;}
  if (Joystick2AMap < -512){
  Joystick2AMap = -512;}
  if (Joystick2BMap > 512){
  Joystick2BMap = 512;}
  if (Joystick2BMap < -512){
  Joystick2BMap = -512;}

Joystick3AMap = map(Joystick3A, 0, 1023, -512, 512);
Joystick3BMap = map(Joystick3B, 0, 1023, -512, 512);
  if (Joystick3AMap > 512){
  Joystick3AMap = 512;}
  if (Joystick3AMap < -512){
  Joystick3AMap = -512;}
  if (Joystick3BMap > 512){
  Joystick3BMap = 512;}
  if (Joystick3BMap < -512){
  Joystick3BMap = -512;}


  
// FOR GOING ALL VERTICAL:
if ((ServoVal != 67)) {
beautiful = map(ServoVal, 0, 1023, 60, 170); //maps number for use with servo, 180 degrees
myservo.write(beautiful); }

if (LightVal < 400) {
  mylight.write(1100);
}
if (LightVal >= 400); {
  beautiful2 = map(LightVal, 400, 1023, 1100, 1900);
  mylight.write(beautiful2);
}

if (((Joystick1A < 542) && (Joystick1A > 482)) && ((Joystick2A < 542) && (Joystick2A > 482))) {
  analogWrite (V1PWM, 0);
  analogWrite (V2PWM, 0); 
  analogWrite (V3PWM, 0); 
  analogWrite (V4PWM, 0); 
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, LOW);}   // Both idle

if ((Joystick1A > 542) && ((Joystick2A < 542) && (Joystick2A > 482))) {
   // First joystick going up
    Vertical = map(Joystick1A, 542, 1023, 0, 255);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical);
  analogWrite (V3PWM, Vertical);
  analogWrite (V4PWM, Vertical);
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, HIGH);
  digitalWrite (V3Direction, HIGH);
  digitalWrite (V4Direction, LOW);}

if ((Joystick1A < 482) && ((Joystick2A < 542) && (Joystick2A > 482))) {
    // First joystick going down
    Vertical = map(Joystick1A, 0, 482, 255, 0);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical); 
  analogWrite (V3PWM, Vertical); 
  analogWrite (V4PWM, Vertical); 
  delayMicroseconds(100);
  digitalWrite (V1Direction, HIGH);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, HIGH);}

    
// FOR PITCH and VERTICAL (on joystick 2):
// note: Joystick 1 will only work when joystick 2's vertical axis is not in use

if (Joystick2AMap > 30) { //2A is up
  if (Joystick2BMap > 30) { //2B is to right 
    if ((Joystick2AMap > Joystick2BMap) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))) { //2A is bigger
      Front = Joystick2AMap;
      Back = (Joystick2AMap - Joystick2BMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
    }
    if ((Joystick2AMap < Joystick2BMap) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))) { //2B is bigger
      Front = Joystick2BMap;
      Back = (Joystick2BMap - Joystick2AMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
    if (abs(abs(Joystick2AMap) - abs(Joystick2BMap)) < 30) { //if 2A and 2B are equal (within 30)
      Front = Joystick2BMap;
      Back = 0;
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, LOW);
    }
  }
  if (Joystick2BMap < -30) { //2B is to left
    if ((Joystick2AMap > abs(Joystick2BMap)) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ //2A is bigger
      Front = (Joystick2AMap - abs(Joystick2BMap));
      Back = Joystick2AMap;
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
    }
    if ((Joystick2AMap < abs(Joystick2BMap)) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ //2B is bigger
      Front = (abs(Joystick2BMap) - Joystick2AMap);
      Back = abs(Joystick2BMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
    }
    if (abs(abs(Joystick2AMap) - abs(Joystick2BMap)) < 30) { //if 2A and 2B are equal (within 30)
      Front = 0;
      Back = Joystick2AMap;
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
    }
  }
}

if (-30 > Joystick2AMap) { //2A down
  if (Joystick2BMap > 30) { //2B right
    if ((abs(Joystick2AMap) > abs(Joystick2BMap)) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ //2A is bigger
      Front =(abs(Joystick2AMap) - abs(Joystick2BMap)); 
      Back = abs(Joystick2AMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
    if ((abs(Joystick2AMap) < abs(Joystick2BMap)) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ //2B is bigger
      Front = (abs(Joystick2BMap) - abs(Joystick2AMap));
      Back = abs(Joystick2BMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
    if (abs(abs(Joystick2AMap) - abs(Joystick2BMap)) < 30) { //if 2A and 2B are equal (within 30)
      Front = 0;
      Back = abs(Joystick2AMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
  }
  if (Joystick2BMap < -30) { //2B left
    if ((abs(Joystick2AMap) > (abs(Joystick2BMap))) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ // 2A is bigger
      Front = abs(Joystick2AMap);
      Back = (abs(Joystick2AMap) - abs(Joystick2BMap));
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
    if ((abs(Joystick2AMap) < (abs(Joystick2BMap))) && ((abs(abs(Joystick2AMap) - abs(Joystick2BMap)) > 30))){ //2B is bigger
      Front = abs(Joystick2BMap);
      Back = (abs(Joystick2BMap) - abs(Joystick2AMap));
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
    }
    if (abs(abs(Joystick2AMap) - abs(Joystick2BMap)) < 30) { //if 2A and 2B are equal (within 30)
      Front = abs(Joystick2BMap);
      Back = 0;
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, LOW);
    }
  }
}
if ((Joystick2AMap < 30) && (Joystick2AMap > -30))  { //2A at zero
  if (Joystick2BMap > 30) { //2B right
      Front = abs(Joystick2BMap);
      Back = abs(Joystick2BMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
    }
  if (Joystick2BMap < -30) { //2B left
      Front = abs(Joystick2BMap);
      Back = abs(Joystick2BMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);
  }
}
if ((Joystick2BMap < 30) && (Joystick2BMap > -30))  { //2B at zero
  if (Joystick2AMap > 30) { //2A up
      Front = abs(Joystick2AMap);
      Back = abs(Joystick2AMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, LOW);
      digitalWrite (V2Direction, HIGH);
      digitalWrite (V3Direction, HIGH);
      digitalWrite (V4Direction, LOW);  
    }
  if (Joystick2AMap < -30) { //2A down
      Front = abs(Joystick2AMap);
      Back = abs(Joystick2AMap);
      FrontMap = map(Front, 0, 512, 0, 255);
      BackMap = map(Back, 0, 512, 0, 255);
      
      analogWrite (V1PWM, FrontMap);
      analogWrite (V2PWM, FrontMap);
      analogWrite (V3PWM, BackMap);
      analogWrite (V4PWM, BackMap);
      delayMicroseconds(100);
      digitalWrite (V1Direction, HIGH);
      digitalWrite (V2Direction, LOW);
      digitalWrite (V3Direction, LOW);
      digitalWrite (V4Direction, HIGH);
  }
}
if (((Joystick2BMap < 30) && (Joystick2BMap > -30)) && ((Joystick2AMap < 30) && (Joystick2AMap > -30)) && ((Joystick1A < 542) && (Joystick1A > 482))) {
  analogWrite (V1PWM, 0);
  analogWrite (V2PWM, 0);
  analogWrite (V3PWM, 0);
  analogWrite (V4PWM, 0);
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, LOW);  
}




// FORBACK AND YAW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



if (Joystick3AMap > 30) { //3A is up
  if (Joystick3BMap > 30) { //3B is to right 
    if ((Joystick3AMap > Joystick3BMap) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))) { //3A is bigger
      Left = Joystick3AMap;
      Right = (Joystick3AMap - Joystick3BMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, HIGH);
    }
    if ((Joystick3AMap < Joystick3BMap) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))) { //3B is bigger
      Left = Joystick3BMap;
      Right = (Joystick3BMap - Joystick3AMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, HIGH);
    }
    if (abs(abs(Joystick3AMap) - abs(Joystick3BMap)) < 30) { //if 3A and 3B are equal (within 30)
      Left = Joystick3BMap;
      Right = 0;
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, HIGH);
    }
  }
  if (Joystick3BMap < -30) { //3B is to left
    if ((Joystick3AMap > abs(Joystick3BMap)) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ //3A is bigger
      Left = (Joystick3AMap - abs(Joystick3BMap));
      Right = Joystick3AMap;
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, HIGH);
    }
    if ((Joystick3AMap < abs(Joystick3BMap)) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ //3B is bigger
      Left = (abs(Joystick3BMap) - Joystick3AMap);
      Right = abs(Joystick3BMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, LOW);
    }
    if (abs(abs(Joystick3AMap) - abs(Joystick3BMap)) < 30) { //if 3A and 3B are equal (within 30)
      Left = 0;
      Right = Joystick3AMap;
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, LOW);
    }
  }
}

if (-30 > Joystick3AMap) { //3A down
  if (Joystick3BMap > 30) { //3B right
    if ((abs(Joystick3AMap) > abs(Joystick3BMap)) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ //3A is bigger
      Left =(abs(Joystick3AMap) - abs(Joystick3BMap)); 
      Right = abs(Joystick3AMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, LOW);
    }
    if ((abs(Joystick3AMap) < abs(Joystick3BMap)) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ //3B is bigger
      Left = (abs(Joystick3BMap) - abs(Joystick3AMap));
      Right = abs(Joystick3BMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, HIGH);
    }
    if (abs(abs(Joystick3AMap) - abs(Joystick3BMap)) < 30) { //if 3A and 3B are equal (within 30)
      Left = 0;
      Right = abs(Joystick3AMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, LOW);
    }
  }
  if (Joystick3BMap < -30) { //3B left
    if ((abs(Joystick3AMap) > (abs(Joystick3BMap))) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ // 3A is bigger
      Left = abs(Joystick3AMap);
      Right = (abs(Joystick3AMap) - abs(Joystick3BMap));
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, LOW);
    }
    if ((abs(Joystick3AMap) < (abs(Joystick3BMap))) && ((abs(abs(Joystick3AMap) - abs(Joystick3BMap)) > 30))){ //3B is bigger
      Left = abs(Joystick3BMap);
      Right = (abs(Joystick3BMap) - abs(Joystick3AMap));
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, LOW);
    }
    if (abs(abs(Joystick3AMap) - abs(Joystick3BMap)) < 30) { //if 3A and 3B are equal (within 30)
      Left = abs(Joystick3BMap);
      Right = 0;
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, LOW);
    }
  }
}
if ((Joystick3AMap < 30) && (Joystick3AMap > -30))  { //3A at zero
  if (Joystick3BMap > 30) { //3B right
      Left = abs(Joystick3BMap);
      Right = abs(Joystick3BMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, HIGH);
    }
  if (Joystick3BMap < -30) { //3B left
      Left = abs(Joystick3BMap);
      Right = abs(Joystick3BMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, LOW);
  }
}
if ((Joystick3BMap < 30) && (Joystick3BMap > -30))  { //3B at zero
  if (Joystick3AMap > 30) { //3A up
      Left = abs(Joystick3AMap);
      Right = abs(Joystick3AMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, HIGH);
      digitalWrite (H2Direction, LOW);
      digitalWrite (H3Direction, LOW);
      digitalWrite (H4Direction, HIGH);
    }
  if (Joystick3AMap < -30) { //3A down
      Left = abs(Joystick3AMap);
      Right = abs(Joystick3AMap);
      LeftMap = map(Left, 0, 512, 0, 255);
      RightMap = map(Right, 0, 512, 0, 255);
      
      analogWrite (H1PWM, LeftMap);
      analogWrite (H2PWM, RightMap);
      analogWrite (H3PWM, RightMap);
      analogWrite (H4PWM, LeftMap);
      delayMicroseconds(100);
      digitalWrite (H1Direction, LOW);
      digitalWrite (H2Direction, HIGH);
      digitalWrite (H3Direction, HIGH);
      digitalWrite (H4Direction, LOW);  
  }
}
if (((Joystick3BMap < 30) && (Joystick3BMap > -30)) && ((Joystick3AMap < 30) && (Joystick3AMap > -30))) {
  analogWrite (H1PWM, 0);
  analogWrite (H2PWM, 0);
  analogWrite (H3PWM, 0);
  analogWrite (H4PWM, 0);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, LOW);  
}



// NOTE: CRAB AND YAW DON'T WORK TOGETHER!!!
 
  
// FOR CRAB


if (((Joystick1B < 482) && ((Joystick3A < 542) && (Joystick3A > 482))) && ((Joystick3B < 542) && (Joystick3B > 482))) {
   Crab = map(Joystick1B, 0, 482, 255, 0);
  analogWrite (H1PWM, Crab);
  analogWrite (H2PWM, Crab);
  analogWrite (H3PWM, Crab);
  analogWrite (H4PWM, Crab);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, HIGH);
   Crab = map(Joystick1B, 0, 482, 255, 0);
  }

if (((Joystick1B > 542) && ((Joystick3A < 542) && (Joystick3A > 482))) && ((Joystick3B < 542) && (Joystick3B > 482))) {
    Crab = map(Joystick1B, 542, 1023, 0, 255);
  analogWrite (H1PWM, Crab);
  analogWrite (H2PWM, Crab);
  analogWrite (H3PWM, Crab);
  analogWrite (H4PWM, Crab);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, LOW);

  }
    delay(20);

}
