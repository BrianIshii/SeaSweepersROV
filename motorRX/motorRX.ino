/*
  Receving Code
  Reads bytes from Transmitting Code and put thems back together
  Applies the read bytes to our motor control board
*/

#include <Servo.h>
Servo myservo;
int ServoVal;
int beautiful;

int LightVal;
int beautiful2;

unsigned char val[16];    // variable to read the value from the analog pin
int Joystick1A;      // 0-1024 received from serial
int Joystick1B;
int Joystick2A;
int Joystick2B;      // 0-1024 received from serial
int Joystick3A;
int Joystick3B;

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

int Light = 13;  // pin for light



int Vertical;
int Pitch;
int Crab;
int ForBack;
int Yaw;


void setup() {
  Serial.begin(9600);
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
}

void loop() {

//  Serial.println (analogRead(Current1));

  Serial1.write(1);
  Serial1.write(2);
  Serial1.write(3);

  if(Serial1.available()<1) {
    //Serial.println("bye");
  }

  if (Serial1.available()>23) {  
  //  Serial.println("hi");

  FirstByte=Serial1.read();

  if (FirstByte == 1) {
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
if (LastByte == 2) {
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

}
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
    Serial.print(Joystick3B);
    Serial.print(" ");
    Serial.println(ServoVal);
    delay(20);

  
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


  
// FOR GOING ALL VERTICAL:
if ((ServoVal != 67)) {
beautiful = map(ServoVal, 0, 1023, 60, 170); //maps number for use with servo, 180 degrees
myservo.write(beautiful); }

if (LightVal < 400) {
  analogWrite (Light, 0);
}
if (LightVal >= 400); {
  beautiful2 = map(LightVal, 400, 1023, 0, 255);
  analogWrite (Light, beautiful2);
}

if (((Joystick1A < 570) && (Joystick1A > 430)) && ((Joystick2A < 570) && (Joystick2A > 430))) {
  analogWrite (V1PWM, 0);
  analogWrite (V2PWM, 0); 
  analogWrite (V3PWM, 0); 
  analogWrite (V4PWM, 0); 
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, LOW);}   // Both idle

if ((Joystick1A > 570) && ((Joystick2A < 570) && (Joystick2A > 430))) {
   // First joystick going up
    Vertical = map(Joystick1A, 570, 1023, 0, 255);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical);
  analogWrite (V3PWM, Vertical);
  analogWrite (V4PWM, Vertical);
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, HIGH);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, HIGH);}

if ((Joystick2A > 570) && ((Joystick1A < 570) && (Joystick1A > 430))) {
   // Second joystick going up
    Vertical = map(Joystick2A, 570, 1023, 0, 255);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical);
  analogWrite (V3PWM, Vertical);
  analogWrite (V4PWM, Vertical);
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, HIGH);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, HIGH);}

if ((Joystick1A < 430) && ((Joystick2A < 570) && (Joystick2A > 430))) {
    // First joystick going down
    Vertical = map(Joystick1A, 0, 430, 255, 0);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical); 
  analogWrite (V3PWM, Vertical); 
  analogWrite (V4PWM, Vertical); 
  delayMicroseconds(100);
  digitalWrite (V1Direction, HIGH);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, HIGH);
  digitalWrite (V4Direction, LOW);}

if ((Joystick2A < 430) && ((Joystick1A < 570) && (Joystick1A > 430))) {
    // Second joystick going down
    Vertical = map(Joystick2A, 0, 430, 255, 0);
  analogWrite (V1PWM, Vertical);
  analogWrite (V2PWM, Vertical); 
  analogWrite (V3PWM, Vertical); 
  analogWrite (V4PWM, Vertical);  
  delayMicroseconds(100);
  digitalWrite (V1Direction, HIGH);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, HIGH);
  digitalWrite (V4Direction, LOW);}

    
// FOR PITCH:


if ((Joystick2B > 570) && (((Joystick1A < 570) && (Joystick1A > 430)) && ((Joystick2A < 570) && (Joystick2A > 430)))) {
    Pitch = map(Joystick2B, 570, 1023, 0, 255);
  analogWrite (V1PWM, Pitch);
  analogWrite (V2PWM, Pitch);
  analogWrite (V3PWM, Pitch);
  analogWrite (V4PWM, Pitch);
  delayMicroseconds(100);
  digitalWrite (V1Direction, LOW);
  digitalWrite (V2Direction, HIGH);
  digitalWrite (V3Direction, HIGH);
  digitalWrite (V4Direction, LOW);}

if ((Joystick2B < 430) && (((Joystick1A < 570) && (Joystick1A > 430)) && ((Joystick2A < 570) && (Joystick2A > 430)))) {
    Pitch = map(Joystick2B, 0, 430, 255, 0);
  analogWrite (V1PWM, Pitch);
  analogWrite (V2PWM, Pitch);
  analogWrite (V3PWM, Pitch);
  analogWrite (V4PWM, Pitch);  
  delayMicroseconds(100);
  digitalWrite (V1Direction, HIGH);
  digitalWrite (V2Direction, LOW);
  digitalWrite (V3Direction, LOW);
  digitalWrite (V4Direction, HIGH);}
  
// FOR FORWARD AND BACKWARDS:


if ((Joystick3A > 570) && ((Joystick3B > 430) && (Joystick3B < 570)) && ((Joystick1B > 430) && (Joystick1B < 570))) {
    ForBack = map(Joystick3A, 570, 1023, 0, 255);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, ForBack);
  analogWrite (H3PWM, ForBack);
  analogWrite (H4PWM, ForBack); 
  delayMicroseconds(100); 
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, HIGH);}

if (((Joystick3A < 570) && (Joystick3A > 430)) && ((Joystick3B > 430) && (Joystick3B < 570)) && ((Joystick1B > 430) && (Joystick1B < 570))) {
  analogWrite (H1PWM, 0);
  analogWrite (H2PWM, 0);
  analogWrite (H3PWM, 0);
  analogWrite (H4PWM, 0);  
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, LOW);}

if ((Joystick3A < 430) && ((Joystick3B > 430) && (Joystick3B < 570)) && ((Joystick1B > 430) && (Joystick1B < 570))) {
    ForBack = map(Joystick3A, 0, 430, 255, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, ForBack);
  analogWrite (H3PWM, ForBack);
  analogWrite (H4PWM, ForBack);  
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, LOW);}
  
// FOR YAW:

  
if (((Joystick3B > 570) && ((Joystick3A < 570) && (Joystick3A > 430))) && ((Joystick1B < 570) && (Joystick1B > 430))) {
    Yaw = map(Joystick3B, 570, 1023, 0, 255);
  analogWrite (H1PWM, Yaw);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, Yaw);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, HIGH);
  }
  
if (((Joystick3B < 430) && ((Joystick3A < 570) && (Joystick3A > 430))) && ((Joystick1B < 570) && (Joystick1B > 430))) {
    Yaw = map(Joystick3B, 0, 430, 255, 0);
  analogWrite (H1PWM, Yaw);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, Yaw);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, LOW);
  }
  
// FORBACK AND YAW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    
if ((Joystick3B > 570) && (Joystick3A > 570)) {
    ForBack = map(Joystick3A, 570, 1023, 0, 255);
    Yaw = map(Joystick3B, 570, 1023, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, HIGH); }
  
if ((Joystick3B < 430) && (Joystick3A > 570)) {
    ForBack = map(Joystick3A, 570, 1023, 0, 255);
    Yaw = map(Joystick3B, 430, 0, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, HIGH); }
  
if ((Joystick3B > 570) && (Joystick3A < 430)) {
    ForBack = map(Joystick3A, 0, 430, 255, 0);
    Yaw = map(Joystick3B, 570, 1023, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, LOW); }
  
if ((Joystick3B < 430) && (Joystick3A < 430)) {
    ForBack = map(Joystick3A, 0, 430, 255, 0);
    Yaw = map(Joystick3B, 430, 0, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, LOW); }




// FOR FORBACK AND CRAB~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`

if ((Joystick1B > 570) && (Joystick3A > 570)) {
    ForBack = map(Joystick3A, 570, 1023, 0, 255);
    Crab = map(Joystick1B, 570, 1023, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, HIGH); }
  
if ((Joystick1B < 430) && (Joystick3A > 570)) {
    ForBack = map(Joystick3A, 570, 1023, 0, 255);
    Crab = map(Joystick1B, 430, 0, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, HIGH);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, LOW);
  digitalWrite (H4Direction, HIGH); }
  
if ((Joystick1B > 570) && (Joystick3A < 430)) {
    ForBack = map(Joystick3A, 0, 430, 255, 0);
    Crab = map(Joystick1B, 570, 1023, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, LOW); }
  
if ((Joystick1B < 430) && (Joystick3A < 430)) {
    ForBack = map(Joystick3A, 0, 430, 255, 0);
    Crab = map(Joystick1B, 430, 0, ForBack, 0);
  analogWrite (H1PWM, ForBack);
  analogWrite (H2PWM, Yaw);
  analogWrite (H3PWM, Yaw);
  analogWrite (H4PWM, ForBack);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, HIGH);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, LOW); }


// NOTE: CRAB AND YAW DON'T WORK TOGETHER!!!
 
  
// FOR CRAB


if (((Joystick1B < 430) && ((Joystick3A < 570) && (Joystick3A > 430))) && ((Joystick3B < 570) && (Joystick3B > 430))) {
   Crab = map(Joystick1B, 0, 430, 255, 0);
  analogWrite (H1PWM, Crab);
  analogWrite (H2PWM, Crab);
  analogWrite (H3PWM, Crab);
  analogWrite (H4PWM, Crab);
  delayMicroseconds(100);
  digitalWrite (H1Direction, LOW);
  digitalWrite (H2Direction, LOW);
  digitalWrite (H3Direction, HIGH);
  digitalWrite (H4Direction, HIGH);
   Crab = map(Joystick1B, 0, 430, 255, 0);
  }

if (((Joystick1B > 570) && ((Joystick3A < 570) && (Joystick3A > 430))) && ((Joystick3B < 570) && (Joystick3B > 430))) {
    Crab = map(Joystick1B, 570, 1023, 0, 255);
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

}
