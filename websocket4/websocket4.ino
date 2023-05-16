/****************************************************************************************************************************
  ESP8266_WebSocketClientSocketIO.ino
  For ESP8266

  Based on and modified from WebSockets libarary https://github.com/Links2004/arduinoWebSockets
  to support other boards such as  SAMD21, SAMD51, Adafruit's nRF52 boards, etc.

  Built by Khoi Hoang https://github.com/khoih-prog/WebSockets_Generic
  Licensed under MIT license
 
  Originally Created on: 06.06.2016
  Original Author: Markus Sattler
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is intended to run only on the ESP8266 boards ! Please check your Tools->Board setting.
#endif

#define _WEBSOCKETS_LOGLEVEL_     2

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

 #include <WebSocketsClient_Generic.h>    
#include <SocketIOclient_Generic.h>

#include <Hash.h>


#define LED_PIN 0 // D3 pin
#define BUTTON_PIN 14 // D5 pin


ESP8266WiFiMulti WiFiMulti;  
SocketIOclient socketIO;

// initialize the LED state and button state variables
bool ledState = LOW;
bool buttonState = LOW;
bool lastButtonState = LOW;
int buttonPressCount = 0; // initialize the button press count variable
unsigned long buttonPressTime = 0; // initialize the button press time variable

// Select the IP address according to your local network
IPAddress serverIP(49, 249, 200, 132);
uint16_t  serverPort = 3000;    //8080;    //3000;

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
  switch (type)
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");


      
      break;

    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*) payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");

      break;

    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*) payload);
      
      break;

    case sIOtype_ACK:
      Serial.print("[IOc] Get ack: ");
      Serial.println(length);

      hexdump(payload, length);
      break;

    case sIOtype_ERROR:
      Serial.print("[IOc] Get error: ");
      Serial.println(length);

      hexdump(payload, length);
      break;

    case sIOtype_BINARY_EVENT:
      Serial.print("[IOc] Get binary: ");
      Serial.println(length);

      hexdump(payload, length);
      break;

    case sIOtype_BINARY_ACK:
      Serial.print("[IOc] Get binary ack: ");
      Serial.println(length);

      hexdump(payload, length);
      break;

    case sIOtype_PING:
      Serial.println("[IOc] Get PING");

      break;

    case sIOtype_PONG:
      Serial.println("[IOc] Get PONG");

      break;

    default:
      break;
  }
}
void setup()
{
  // Serial.begin(921600);
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  while (!Serial);

  Serial.print("\nStart ESP8266_WebSocketClientSocketIO on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  //Serial.setDebugOutput(true);

  // disable AP
  if (WiFi.getMode() & WIFI_AP)
  {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("mkp", "11111111");

  //WiFi.disconnect();
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();

  // Client address
  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());

  // server address, port and URL
  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);

  // setReconnectInterval to 10s, new from v2.5.1 to avoid flooding server. Default is 0.5s
  socketIO.setReconnectInterval(10000);

  socketIO.setExtraHeaders("Authorization: 1234567890");
  //1= User Name , 2= userId , 3=usertype
  socketIO.setExtraHeaders("data:test,test123,station");
  // server address, port and URL
  // void begin(IPAddress host, uint16_t port, String url = "/socket.io/?EIO=4", String protocol = "arduino");
  // To use default EIO=4 fron v2.5.1
  socketIO.begin(serverIP, serverPort);

  // event handler  
  socketIO.onEvent(socketIOEvent);


}

unsigned long messageTimestamp = 0;

void loop()
{
  socketIO.loop();
  Serial.println("hello websocket");
  uint64_t now = millis();

   buttonState = digitalRead(BUTTON_PIN);
// Serial.println(buttonState);prints high=1 in the serial monitor continously 
  // check if the button is pressed
  if (buttonState != lastButtonState) {
    if (buttonState == 1) { // button is pressed down
      // update the button press time and count
      buttonPressTime = millis();
      buttonPressCount++;
     delay(5000);
      // turn on the LED
      digitalWrite(LED_PIN, HIGH);
      
      // print the button state and count to the serial monitor
      Serial.print("Button Pressed! Count: ");
      Serial.println(buttonPressCount);
     delay(4000);
     socketIO.sendEVENT("unlock");
     
     
     
     }
     else { // button is released
      // turn off the LED
      digitalWrite(LED_PIN, LOW);
     }
  }
  // if (now - messageTimestamp > 30000)
  // {
  //   messageTimestamp = now;

  //   // creat JSON message for Socket.IO (event)
  //   DynamicJsonDocument doc(1024);
  //   JsonArray array = doc.to<JsonArray>();

  //   // add evnet name
  //   // Hint: socket.on('event_name', ....
  //   array.add("event_name");

  //   // add payload (parameters) for the event
  //   JsonObject param1 = array.createNestedObject();
  //   param1["now"]     = (uint32_t) now;

  //   // JSON to String (serializion)
  //   String output;
  //   serializeJson(doc, output);

  //   // Send event
  //   socketIO.sendEVENT(output);

  //   // Print JSON for debugging
  //   Serial.println(output);
  
   // }
}

  