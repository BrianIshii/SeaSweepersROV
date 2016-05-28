/*
  Receving Code Sensors
  Reads bytes from Transmitting Code and put thems back together
  Then sends the bytes through serial to a raspberry pi
*/
//variables for motorSense
unsigned char motorFirstByte;
int x;


unsigned char val[48];
int dataOne;
int dataTwo;
int dataThree;
int dataFour;      
int dataFive;
int dataSix;
int dataSeven;
int dataEight;
int dataNine;      
int dataTen;
int dataEleven;
int dataTwelve;
int dataThirteen;
int dataFourteen;
int dataFifteen;      
int dataSixteen;
int dataSeventeen;
int dataEighteen;
int dataNineteen;
int dataTwenty;      
int dataTwentyOne;
int dataTwentyTwo;
int dataTwentyThree;

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

unsigned char FirstByte;
unsigned char SecondByte;
unsigned char ThirdByte;
unsigned char LastByte;

unsigned char checkit0;
unsigned char checkit1;
unsigned char checkit2;
unsigned char checkit3;
unsigned char checkit4;
unsigned char checkit5;
unsigned char checkit6;
unsigned char checkit7;
unsigned char checkit8;
unsigned char checkit9;

unsigned char checkit10;
unsigned char checkit11;
unsigned char checkit12;
unsigned char checkit13;
unsigned char checkit14;
unsigned char checkit15;
unsigned char checkit16;
unsigned char checkit17;
unsigned char checkit18;
unsigned char checkit19;
unsigned char checkit20;

unsigned char checkit21;
unsigned char checkit22;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  delay(1000);
}

void loop() {
 while (Serial.available() == 0) {

  Serial1.write(1);
  Serial1.write(2);
  Serial1.write(3);

  if (Serial1.available() > 2) {
  FirstByte=Serial1.read();
  SecondByte=Serial1.read();
  ThirdByte=Serial1.read();
  if ((FirstByte == 255) && (SecondByte==255) && (ThirdByte==255)) {
    val[0]=Serial1.read();         // least significant 8 bits
    val[1]=Serial1.read();         // most significant 2 bits   
     checksum0=Serial1.read();     // checksum

//    val[2]=Serial1.read(); //we did not need to use data
//    val[3]=Serial1.read();
//     checksum1=Serial1.read();

//    val[4]=Serial1.read(); //we did not need to use data
//    val[5]=Serial1.read(); 
//     checksum2=Serial1.read();

    val[6]=Serial1.read(); 
    val[7]=Serial1.read();     
     checksum3=Serial1.read();    

//    val[8]=Serial1.read(); //we did not need to use data
//    val[9]=Serial1.read();
//     checksum4=Serial1.read();

    val[10]=Serial1.read();
    val[11]=Serial1.read(); 
     checksum5=Serial1.read();
     
    val[12]=Serial1.read();
    val[13]=Serial1.read();
     checksum6=Serial1.read();

    val[30]=Serial1.read();
    val[31]=Serial1.read(); 
     checksum15=Serial1.read();
    val[32]=Serial1.read();
    val[33]=Serial1.read();
     checksum16=Serial1.read();

    val[34]=Serial1.read();
    val[35]=Serial1.read(); 
     checksum17=Serial1.read();

    val[36]=Serial1.read();
    val[37]=Serial1.read();  
     checksum18=Serial1.read(); 

    val[38]=Serial1.read();
    val[39]=Serial1.read();
     checksum19=Serial1.read();

    val[40]=Serial1.read();
    val[41]=Serial1.read(); 
     checksum20=Serial1.read();

    val[42]=Serial1.read();
    val[43]=Serial1.read(); 
     checksum21=Serial1.read();

    val[44]=Serial1.read();
    val[45]=Serial1.read(); 
     checksum22=Serial1.read();
   LastByte = Serial1.read();
//
//

// end of reading data
//check the bytes
    checkit0 = val[0]+val[1]+checksum0;
//    checkit1 = val[2]+val[3]+checksum1;
//    checkit2 = val[4]+val[5]+checksum2; 
    checkit3 = val[6]+val[7]+checksum3;
//    checkit4 = val[8]+val[9]+checksum4;
    checkit5 = val[10]+val[11]+checksum5;
    checkit6 = val[12]+val[13]+checksum6;
    checkit7 = val[14]+val[15]+checksum7;
    checkit8 = val[16]+val[17]+checksum8; 
    checkit9 = val[18]+val[19]+checksum9;
    checkit10 = val[20]+val[21]+checksum10;
    checkit11 = val[22]+val[23]+checksum11;
    checkit12 = val[24]+val[25]+checksum12; 
    checkit13 = val[26]+val[27]+checksum13;
    checkit14 = val[28]+val[29]+checksum14;
    checkit15 = val[30]+val[31]+checksum15;
    checkit16 = val[32]+val[33]+checksum16;
    checkit17 = val[34]+val[35]+checksum17;
    checkit18 = val[36]+val[37]+checksum18; 
    checkit19 = val[38]+val[39]+checksum19;
    checkit20 = val[40]+val[41]+checksum20;
    checkit21 = val[42]+val[43]+checksum21;
    checkit22 = val[44]+val[45]+checksum22;

  }
     
  
  }
  if (Serial2.available()>21){
    motorFirstByte = Serial2.read();
    if (motorFirstByte == 1){
    val[14]=Serial2.read();
    val[15]=Serial2.read(); 
    checksum7=Serial2.read();
    
    val[16]=Serial2.read();
    val[17]=Serial2.read();   
    checksum8=Serial2.read();

    val[18]=Serial2.read();
    val[19]=Serial2.read();
    checksum9=Serial2.read();

    val[20]=Serial2.read();
    val[21]=Serial2.read(); 
    checksum10=Serial2.read();

    val[22]=Serial2.read();
    val[23]=Serial2.read();
    checksum11=Serial2.read();

    val[24]=Serial2.read();
    val[25]=Serial2.read(); 
    checksum12=Serial2.read();

    val[26]=Serial2.read(); 
    val[27]=Serial2.read();    
    checksum13=Serial2.read();

    val[28]=Serial2.read();
    val[29]=Serial2.read();
    checksum14=Serial2.read();
    }
}
//if (LastByte == 2) {
  if (checkit0==0)  {
    dataOne = val[1]<<8 | val[0]; }

//  if (checkit1==0)  {
//    dataTwo = val[3]<<8 | val[2]; }

//  if (checkit2==0)  {
//    dataThree = val[5]<<8 | val[4]; }

  if (checkit3==0)  {
    dataFour = val[7]<<8 | val[6]; }

//  if (checkit4==0)  {
//    dataFive = val[9]<<8 | val[8]; }

  if (checkit5==0)  {
    dataSix = val[11]<<8 | val[10]; }
  if (checkit6==0)  {
    dataSeven = val[13]<<8 | val[12]; }

  if (checkit7==0)  {
    dataEight = val[15]<<8 | val[14]; }

  if (checkit8==0)  {
    dataNine = val[17]<<8 | val[16]; }

  if (checkit9==0)  {
    dataTen = val[19]<<8 | val[18]; }

  if (checkit10==0)  {
    dataEleven = val[21]<<8 | val[20]; }
  
  if (checkit11==0)  {
    dataTwelve = val[23]<<8 | val[22]; }

  if (checkit12==0)  {
    dataThirteen = val[25]<<8 | val[24]; }

  if (checkit13==0)  {
    dataFourteen = val[27]<<8 | val[26]; }

  if (checkit14==0)  {
    dataFifteen = val[29]<<8 | val[28]; }

  if (checkit15==0)  {
    dataSixteen = val[31]<<8 | val[30]; }
    
  if (checkit16==0)  {
    dataSeventeen = val[33]<<8 | val[32]; }

  if (checkit17==0)  {
    dataEighteen = val[35]<<8 | val[34]; }

  if (checkit18==0)  {
    dataNineteen = val[37]<<8 | val[36]; }

  if (checkit19==0)  {
    dataTwenty = val[39]<<8 | val[38]; }

  if (checkit20==0)  {
    dataTwentyOne = val[41]<<8 | val[40]; }
    
  if (checkit21==0)  {
    dataTwentyTwo = val[43]<<8 | val[42]; }
    
  if (checkit22==0)  {
    dataTwentyThree = val[45]<<8 | val[44]; }

//}
  int dataFifteen = (dataFifteen); //trying to fix volt reading
  int dataSixteen = (dataSixteen* 5 / 1023);
    Serial.print("A");
    Serial.print(dataOne);
    Serial.print("B");

    Serial.print("C");
    Serial.print(dataFour);
    Serial.print("D");
    Serial.print("E");
    Serial.print(dataSix);
    Serial.print("F");
    
    Serial.print("G");
    Serial.print(dataSeven);
    Serial.print("H");
    
    Serial.print("I");
    Serial.print(dataEight);
    Serial.print("J");
    
    Serial.print("K");
    Serial.print(dataNine);
    Serial.print("L");
    
    Serial.print("M");
    Serial.print(dataTen);
    Serial.print("N");
    
    Serial.print("O");
    Serial.print(dataEleven);
    Serial.print("P");
    
    Serial.print("Q");
    Serial.print(dataTwelve);
    Serial.print("R");
    
    Serial.print("S");
    Serial.print(dataThirteen);
    Serial.print("T");
     
    Serial.print("U");
    Serial.print(dataFourteen);
    Serial.print("V");
    
    Serial.print("W");
    Serial.print(dataFifteen);
    Serial.print("X");
    
    Serial.print("Y");
    Serial.print(dataSixteen);
    Serial.print("Z");
    
    Serial.print("a");
    Serial.print(dataSeventeen);
    Serial.print("b");
    
    Serial.print("c");
    Serial.print(dataEighteen);
    Serial.print("d");
    
    Serial.print("e");
    Serial.print(dataNineteen);
    Serial.print("f");
    
    Serial.print("g");
    Serial.print(dataTwenty);
    Serial.print("h");
    
    Serial.print("i");
    Serial.print(dataTwentyOne);
    Serial.print("j");
    
    Serial.print("k");
    Serial.print(dataTwentyTwo);
    Serial.print("l");
    
    Serial.print("m");
    Serial.print(dataTwentyThree);
    Serial.println("n");

//  if (dataTwentyTwo > 50 || dataTwentyThree >50) {
//    waterLeak();//water leak function will turn on lights after sending the data
//  }
    delay(100);

  }
  
//  if (Serial.available()>0){
//      char c = Serial.read();  //gets one byte from serial from rasperberry 2 is red lights 1 is yellow and 0 is off
//      if (c == 2){
//        digitalWrite(13, LOW); 
//      }
//      if (c == 1){
//        digitalWrite(12, LOW); 
//      }
//      if (c == 0){
//        digitalWrite(13, HIGH);  
//        digitalWrite(12, HIGH);
//      }
//      }
//  }
//  
//void waterLeak(){
//    int i = 0;
//  for(i=0; i<10; i++) {
//    digitalWrite(13, LOW); 
//    digitalWrite(12, LOW); 
//    delay(1); 
//    digitalWrite(13, HIGH);  
//    digitalWrite(12, HIGH);    
//    delay(2);   
//  }
}

