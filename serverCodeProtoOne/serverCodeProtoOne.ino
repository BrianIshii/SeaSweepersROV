/*
  Web Server For sensors

 */

#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
 
#define dht_apin A0 // Analog Pin sensor is connected to
 
dht DHT;
 
int motorPin = 9;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip (169, 254, 90, 21);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {/*
  Web Server For sensors

 */

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h> 
//barometer Library
#include <Wire.h>
#include "MS5837.h"


int tempSensor = 2; //DS18S20 Signal pin on digital 2


MS5837 sensor; //barosensor

OneWire ds(tempSensor);
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip (192, 168, 7, 4);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  
  //barometer setup
  Wire.begin();
  sensor.init();
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip); 
//  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  delay(500);//Delay to let system boot
 
  Serial.println("DHT22 Humidity & temperature Sensor\n");

  //declares that we are using digital pin 9
 
  delay(500);//Wait before accessing Sensor

  Serial.print("Ready\n");

}


void loop() {
  //Fastest should be once every two seconds.
sensor.read();
  // listen for incoming clients
  int sense = sensor.depth() * 1000;
  Serial.print(sense);
    Serial.print("Pressure: "); 
  Serial.println(sensor.pressure()); 
  
  Serial.print("Temperature: "); 
  Serial.println(sensor.temperature()); 
  
  Serial.print("Depth: "); 
  Serial.println(sensor.depth()); 
  
  Serial.print("Altitude: "); 
  Serial.println(sensor.altitude()); 

  
float temperature = getTemp();
  delay(100);
  
  
  //webserver code starts here
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 0.1");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          
          client.println("<head>");
          //style of all the tables and blocks
          client.println("<style>");
          client.println("body{background-color:blue;}");
          client.println("div.header{text-align:center;font-size:200%;position: fixed;top: 0px;width: 300px;height: 50px;color: black;border: 3px solid white;}");
          client.println("table.column{bottom: 0px;position: fixed;height:350px;width:300px;color:white;font-size: 200%;border: 3px solid white;text-align:center;}");
          client.println("tr{height: 50px;border: 3px solid white;}");
          client.println("th{color:red;border: 3px solid white;}");
          client.println("table.motorVolts{left: 300px;bottom: 0px;position: fixed;height:200px;width:278px;color:white;border: 3px solid white;font-size: 150%;}");
          client.println("table.bottom{border: 3px solid white;font-size: 150%;bottom: 0px;position: fixed;height:200px;width:280px;color:white;text-align:center;}");
          client.println("#container {bottom: 216px;right: 585px;position: fixed;width: 274px;height: 410px;background:#8B4513;border: 3px solid white;}");
          client.println("#animate{top: 0px;width: 274px;height: 200px;position: absolute;background-color: lightblue;}");
          client.println("#line {top: 200px;width: 274px;height: 2px;position: absolute;background-color: black;}");
          client.println("p.button{margin-top: 100px;float:right;}");
          client.println("#depthData {bottom: 216px;right: 303px;position: fixed;width: 278px;height: 410px;background: lightblue;border: 3px solid white;}");
          client.println("#ROV {right: 40px;width: 50px;height: 50px;position: absolute;background-color: black;}");
          client.println("#depthLine {width: 278px;height: 2px;position: absolute;background-color: black;}");
          client.println("#motorLayout {bottom: 216px;right: 861px;position: fixed;width: 274px;height: 410px;background: grey;border: 3px solid white;}");
          client.println("#motorBlock {width: 20px;height: 20px;position: absolute;background-color: black;}");
          client.println("</style>");
          
          client.println("</head>");
          
          client.println("<body>");
          //warning title and stop
          client.println("<div class='header' style ='left: 0px; background-color:yellow;'>WARNING</div>");
          client.println("<div class='header' style = 'left: 500px; right: 300px; color: white; '>Sea Sweepers</div>");
          client.println("<div class='header' style= 'right: 0px; background-color:red;'>STOP</div>");
          //left table
          client.println("<table class='column' style='left: 0px;'>");
          client.println("<tr><th>Temperature(F)</th></tr>");
          client.println("<tr><td>"); client.print(temperature); client.print("</td></tr>");
          client.println("<tr><th>Humidity</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Temperature(C)</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th><button onclick='myMove()'>Click Me</button></th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Humidity</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Temperature(C)</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("</table>");

          //right table
          client.println("<table class= 'column' style = 'right: 0px;'>");
          client.println("<tr><th>Pressure (mbar) </th></tr>");
          client.println("<tr><td>"); client.print(sensor.pressure()); client.print("</td></tr>");
          client.println("<tr><th>water temperature (C)</th></tr>");
          client.println("<tr><td>"); client.print(sensor.temperature()); client.print("</td></tr>");
          client.println("<tr><th>depth (m)</th></tr>");
          client.println("<tr><td>"); client.print(sensor.depth()); client.print("</td></tr>");
          client.println("<tr><th>altitude</th></tr>");
          client.println("<tr><td>"); client.print(sensor.altitude()); client.print("</td></tr>");
          client.println("<tr><th>Humidity</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Temperature(C)</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("</table>");

          //angle measurer
          client.println("<div id ='container'>");
          client.println("<div id ='animate'></div>");
          client.println("<div id ='line'></div>");
          client.println("</div>");
          //java for angle measurer
          client.println("<script>");
          client.println("function myMove(){");
          client.println("var elem = document.getElementById('animate');");
          client.println("var line = document.getElementById('line');");
          client.println("var pos = 0;");
          client.println("var start = 40;");
          client.println("var id = setInterval(frame, 5);");
          client.println("function frame() {");
          client.println("if (pos == 350) {");
          client.println("clearInterval(id);");
          client.println("} else {");
          client.println("pos++;");
          client.println("start++;");
          client.println("line.style.top = start + 'px';");
          client.println("elem.style.height = start + 'px';");
          client.println("}");
          client.println("}");
          client.println("}");
          client.println("</script>");
          //depth animation
          client.println("<div id ='depthData'>");
          client.println("<div id ='ROV' style= 'margin-top: ");client.print(sense); client.print("px;'></div>");
          client.println("<div id ='depthLine' style= 'margin-top: 100px;'>10</div>");
          client.println("<div id ='depthLine' style= 'margin-top: 200px;'>20</div>");
          client.println("<div id ='depthLine' style= 'margin-top: 300px;'>30</div>");
          client.println("</div>");
          
          
          client.println("<div id ='motorLayout'>");
          client.println("<div id = 'motorBlock'style= 'top: 40px; right: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'top: 40px; left: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'top: 100px; right: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'top: 100px; left: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'bottom: 100px; right: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'bottom: 100px; left: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'bottom: 40px; right: 10px;'></div>");
          client.println("<div id = 'motorBlock'style= 'bottom: 40px; left: 10px;'></div>");
          client.println("</div>");
          
          client.println("<table class= 'bottom'  style= 'right: 300px;' border='1'>");
          client.println("<tr><th>depth</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Humiditdfdfy</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("</table>");

          client.println("<table class= 'bottom' style= 'right: 580px;' border='1'>");
          client.println("<tr><th>Temperature(F)</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>Humiditdfdfy</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td></tr>");
          client.println("</table>");

          client.println("<table class= 'motorVolts' border='1' style>");
          client.println("<tr><th>M1</th><th>M2</th><th>M3</th><th>M4</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td></tr>");
          client.println("<tr><th>M5</th><th>M6</th><th>M7</th><th>M8</th></tr>");
          client.println("<tr><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td><td>"); client.print(""); client.print("</td></tr>");
          client.println("</table>");
          
          client.println("</body>");
          
          client.println("</html>");


//old code
//          client.println("<header>");
//          client.println("<h1 style=‘font-size:500%;’>Sea Sweepers</h1>");
//          client.println("</header>");
//          
//          client.println("<table border=‘1’>");
//          client.print("<tr><th>Temperature(F)</th></tr><tr><td>"); client.println(temperatureF); client.print("</td></tr>");
//          client.print("<tr><th>Humidity</th></tr><tr><td>"); client.print(DHT.humidity); client.print("</td></tr>");
//          client.print("<tr><th>Temperature(C)</th></tr><tr><td>"); client.print(DHT.temperature); client.print("</td></tr>");
//          client.println("</table>");
//          
//          client.println("</body>");
//          client.print("<p class = 'ex'> humidity ");
//          client.print(DHT.humidity); 
//          client.print("%</p>");
//          client.println("<br />");
//          client.print("temperature = "); client.print(DHT.temperature); client.println("C  ");
//          client.println("<br />");
//          client.print("temperature = "); client.print(temperatureF); client.print(" degrees F");
//          client.println("<br />");
//          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Ethernet.maintain();
  }
}
float getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return -1000;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
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

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip); 
//  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
 
  delay(500);//Delay to let system boot
 
  Serial.println("DHT22 Humidity & temperature Sensor\n");

  pinMode(motorPin, OUTPUT);
  //declares that we are using digital pin 9
 
  delay(1000);//Wait before accessing Sensor

  Serial.print("Ready\n");

}


void loop() {

    DHT.read22(dht_apin);

    float temperatureF = (DHT.temperature * 9.0 / 5.0) + 32.0;
    //conversion 

  delay(10);//Wait .2 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: .01");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          
          client.println("<head>");
          
          client.println("<style>");
          client.println("body{background-color:lightblue;}header{color:black;text-align:center;padding:0px;}");
          client.println("table{background-color:blue;width:400px;height: 700px;text-align:center;padding:0px;}tr{font-size:200%;padding:10px;}th{color:black;}td{color:red;}");
          client.println("</style>");
          
          client.println("</head>");
          
          client.println("<body>");
          client.println("<header>");
          client.println("<h1 style=‘font-size:500%;’>Sea Sweepers</h1>");
          
          client.println("</header>");
          
          client.println("<table border=‘1’>");
          client.print("<tr><th>Temperature(F)</th></tr><tr><td>"); client.println(temperatureF); client.print("</td></tr>");
          client.print("<tr><th>Humidity</th></tr><tr><td>"); client.print(DHT.humidity); client.print("</td></tr>");
          client.print("<tr><th>Temperature(C)</th></tr><tr><td>"); client.print(DHT.temperature); client.print("</td></tr>");
          client.print("<tr><th>Temperature(C)</th></tr><tr><td>"); client.print(DHT.temperature); client.print("</td></tr>");
          client.print("<tr><th>Temperature(C)</th></tr><tr><td>"); client.print(DHT.temperature); client.print("</td></tr>");
          client.println("</table>");

//         client.println("</body>");
//          client.print("<p class = 'ex'> humidity ");
//          client.print(DHT.humidity); 
//          client.print("%</p>");
//          client.println("<br />");
//          client.print("temperature = "); client.print(DHT.temperature); client.println("C  ");
//          client.println("<br />");
//          client.print("temperature = "); client.print(temperatureF); client.print(" degrees F");
//          client.println("<br />");
        client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Ethernet.maintain();
  }
}
