/*
  Web client

 This sketch connects to a Ui digital mixer
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 Originally by Tom Igoe, based on work by Adrian McEwen
 Added Ui control by Danny Olesh 6th August 2020

 */

#include <SPI.h>
#include <Ethernet.h>

//Add some pins to test (making sure they are not used with the shield)
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value
int oldval=0;     //variable to store old val 



// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,0,44);  // numeric IP for  Ui24  (no DNS)
//char server[] = "Ui.IO";    // name address if we would use Ui.IO for example not connected via Ethernet

// It's still better to know the IP address


// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 142); 
IPAddress myDns(192, 168, 0, 1);  // This does not really matter in this situation 

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

// Setup the pin to read
   pinMode(inPin, INPUT);      // sets the digital pin 7 as input


  // Open serial communications and wait for port to open: Can get rid of this later when it's a stand alone device
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:  All this is really for debugging to start with once
  // you know it is working you would not need it
  
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
  
   client.println("GET /raw HTTP1.1\n\n");
   
 
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  beginMicros = micros();
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  int len = client.available();
  if (len > 0) {
    byte buffer[512];
    if (len > 512) len = 512;
    client.read(buffer, len);
    if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    endMicros = micros();
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    

    // do nothing forevermore:
    while (true) {
      delay(1);

                 }
                          }
    //read the pins we need here
       val = digitalRead(inPin);   // read the input pin
           if (oldval != val) {          //check if the value is the same, we don't want multiple sends
                Serial.println(val);
                oldval=val;             // Make the old value same as the new value
           if (val == 1) client.println("SETD^i.0.mute^1") ; // Send SETD Mute with NEW LINE
           if (val == 0) client.println("SETD^i.0.mute^0") ;
                              }
    

  
      }
