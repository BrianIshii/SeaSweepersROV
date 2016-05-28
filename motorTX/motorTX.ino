/*
  Transmitting Code
  Reads potentiometers and sends them through serial
*/

int data [16];    // to send bytes
int start [2];

int Pin1A = A5;           
int Joystick1A;   // crab

int Pin1B = A4;
int Joystick1B;   // up down numbers are backwards

int Pin2A = A3;
int Joystick2A;   // pitch

int Pin2B = A2;          
int Joystick2B;   // up down broken

int Pin3A = A0;   // yaw r-l
int Joystick3A;

int Pin3B = A1;
int Joystick3B;   // forward-back   fdsafdsaf

int ServoPot = A6;
int ServoVal;

int LightPot = A7;
int LightVal;


unsigned char checksum0;
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char handshake;
unsigned char handshake2;
unsigned char handshake3;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);
  delay(1000);
}


void loop() {

  // READ THE POTENTIOMETERS:

  //  Serial.println(analogRead(ServoPot));

  Joystick1A = analogRead(Pin1A);   // reads the value of the potentiometer (Joystick1A between 0 and 1023)
  Joystick1A = map(Joystick1A, 365, 654, 0, 1024);  //readjusts for bad joystick numbers
  data[0] = Joystick1A & 0xFF;      //least significant 8 bit byte
  data[1] = (Joystick1A >> 8);      //most significant 2 bits

  Joystick1B = analogRead(Pin1B);
  Joystick1B = map(Joystick1B, 370, 646, 0, 1024);
  data[2] = Joystick1B & 0xFF;
  data[3] = (Joystick1B >> 8);

  Joystick2A = analogRead(Pin2A);
  Joystick2A = map(Joystick2A, 371, 650, 0, 1024);
  data[4] = Joystick2A & 0xFF;
  data[5] = (Joystick2A >> 8);

  Joystick2B = analogRead(Pin2B);   // reads the value of the potentiometer (Joystick2B between 0 and 1023)
  Joystick2B = map(Joystick2B, 370, 635, 0, 1024);
  data[6] = Joystick2B & 0xFF;      //least significant 8 bit byte
  data[7] = (Joystick2B >> 8);      //most significant 2 bits

  Joystick3A = analogRead(Pin3A);
  Joystick3A = map(Joystick3A, 364, 640, 0, 1024);
  data[8] = Joystick3A & 0xFF;
  data[9] = (Joystick3A >> 8);

  Joystick3B = analogRead(Pin3B);
  Joystick3B = map(Joystick3B, 376, 649, 0, 1024);
  data[10] = Joystick3B & 0xFF;
  data[11] = (Joystick3B >> 8);

  ServoVal = analogRead(ServoPot);
  //  ServoVal = map(ServoVal, 0, 1023, 0, 180);
  data[12] = ServoVal & 0xFF;
  data[13] = (ServoVal >> 8);

  LightVal = analogRead(LightPot);
  data[14] = LightVal & 0xFF;
  data[15] = (LightVal >> 8);

  // CREATE CHECKSUMS:

  checksum0 = ~(data[0] + data[1]) + 1;

  checksum1 = ~(data[2] + data[3]) + 1;

  checksum2 = ~(data[4] + data[5]) + 1;

  checksum3 = ~(data[6] + data[7]) + 1;

  checksum4 = ~(data[8] + data[9]) + 1;

  checksum5 = ~(data[10] + data[11]) + 1;

  checksum6 = ~(data[12] + data[13]) + 1;

  checksum7 = ~(data[14] + data[15]) + 1;
  // WRITE VALUES AND CHECKSUMS TO SERIAL:

  while (Serial1.available() < 3) {
    ; //wait for request from receiver
  }
  //  if (Serial1.available() < 1) {
  //    Serial.println("bye");
  //  }

  handshake = Serial1.read();
  handshake2 = Serial1.read();
  handshake3 = Serial1.read();

  if ((handshake == 1) && (handshake2 == 2) && (handshake3 == 3)) {

    Serial1.write(1);

    Serial1.write(data[0]);
    Serial1.write(data[1]);
    Serial1.write(checksum0);

    Serial1.write(data[2]);
    Serial1.write(data[3]); //bytes sent
    Serial1.write(checksum1);

    Serial1.write(data[4]);
    Serial1.write(data[5]);
    Serial1.write(checksum2);

    Serial1.write(data[6]);
    Serial1.write(data[7]);
    Serial1.write(checksum3);

    Serial1.write(data[8]);
    Serial1.write(data[9]); //bytes sent
    Serial1.write(checksum4);

    Serial1.write(data[10]);
    Serial1.write(data[11]);
    Serial1.write(checksum5);

    Serial1.write(data[12]);
    Serial1.write(data[13]);  //servp
    Serial1.write(checksum6);
    
    Serial1.write(data[14]);
    Serial1.write(data[15]);  //light
    Serial1.write(checksum7);

    Serial1.write(2);

    //    Serial1.flush();
    delay(20);     // delay in between reads for stability
  }

  // VIEW IN SERIAL MONTIOR:



//  //  delay(20);
    Serial2.write(255);
    Serial2.write(255);
    Serial2.write(255);
    
    Serial2.write(data[0]);
    Serial2.write(data[1]);
    Serial2.write(checksum0);

    Serial2.write(data[2]);
    Serial2.write(data[3]); //bytes sent
    Serial2.write(checksum1);

    Serial2.write(data[4]);
    Serial2.write(data[5]);
    Serial2.write(checksum2);

    Serial2.write(data[6]);
    Serial2.write(data[7]);
    Serial2.write(checksum3);

    Serial2.write(data[8]);
    Serial2.write(data[9]); //bytes sent
    Serial2.write(checksum4);

    Serial2.write(data[10]);
    Serial2.write(data[11]);
    Serial2.write(checksum5);

    Serial2.write(data[12]);
    Serial2.write(data[13]);  //servp
    Serial2.write(checksum6);
}
