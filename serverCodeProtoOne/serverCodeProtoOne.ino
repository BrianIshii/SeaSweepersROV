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

void setup() {
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
          client.println("table{background-color:blue;text-align:center;padding:0px;}tr{font-size:200%;padding:10px;}th{color:black;}td{color:red;}");
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
          client.println("</table>");
          
          client.println("</body>");
          client.print("<p class = 'ex'> humidity ");
          client.print(DHT.humidity); 
          client.print("%</p>");
          client.println("<br />");
          client.print("temperature = "); client.print(DHT.temperature); client.println("C  ");
          client.println("<br />");
          client.print("temperature = "); client.print(temperatureF); client.print(" degrees F");
          client.println("<br />");
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
