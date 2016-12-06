// Code based on http://iot-playground.com/2-uncategorised/40-esp8266-wifi-relay-switch-arduino-ide

/*
 * /on turns on.
 * /off turns off.
 */

#include <ESP8266WiFi.h>

// this will make the LED close to RST turn on. same behaviour as for D4.
#define D0 16
#define D1 5
#define D2 4
#define D3 
// the LED close to AI label.
// when AI label is HIGH, the output from pin 4 will be low and vice versa.
#define D4 2

const char* ssid = "Xperia Z3 Compact_f6f6";
const char* password = "123lol123";

// The actual pins that display information on the board (see comments above).
int pin1 = D0;
int pin2 = D4;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, 0);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/on") != -1)
    val = 0;
  else if (req.indexOf("/off") != -1)
    val = 1;
  else {
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(pin1, val);
  
  client.flush();

  // Prepare the response
   String s = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n 1 2\n ";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}


