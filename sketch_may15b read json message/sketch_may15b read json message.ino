#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <WebSocketsClient_Generic.h>
#include <SocketIOclient_Generic.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

// Select the IP address according to your local network
IPAddress serverIP(49, 249, 200, 132);
uint16_t serverPort = 3000;

void socketIOEvent(const socketIOmessageType_t& type, uint8_t * payload, const size_t& length)
{
  const char* message = ""; // Moved the declaration outside the switch statement

  switch (type)
  {
    case sIOtype_DISCONNECT:
      Serial.println("[IOc] Disconnected");
      break;

    case sIOtype_CONNECT:
      Serial.print("[IOc] Connected to url: ");
      Serial.println((char*)payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");
      break;

    case sIOtype_EVENT:
      Serial.print("[IOc] Get event: ");
      Serial.println((char*)payload);

      // // Parse the received JSON payload
      // StaticJsonDocument<200> doc;
      // deserializeJson(doc, payload, length);

      // Access the JSON data
      message = doc["message"];

      // Print the message in the serial monitor
      Serial.print("Received message: ");
      Serial.println(message);

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
  Serial.begin(115200);
  while (!Serial);

  Serial.print("\nStart ESP8266_WebSocketClientSocketIO on ");
  Serial.println(ARDUINO_BOARD);
  Serial.println(WEBSOCKETS_GENERIC_VERSION);

  if (WiFi.getMode() & WIFI_AP)
  {
    WiFi.softAPdisconnect(true);
  }

  WiFiMulti.addAP("mkp", "11111111");

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println();

  Serial.print("WebSockets Client started @ IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to WebSockets Server @ IP address: ");
  Serial.print(serverIP);
  Serial.print(", port: ");
  Serial.println(serverPort);

  socketIO.setReconnectInterval(10000);
  socketIO.setExtraHeaders("Authorization: 1234567890");
  socketIO.setExtraHeaders("data:test,test123,station");
  socketIO.begin(serverIP, serverPort);

  socketIO.onEvent(socketIOEvent);
}


void loop()
{
  socketIO.loop();
  
  socketIOmessageType_t getLastEventType()
{
  return socketIO.getLastEventType();
}

uint8_t* getLastEventPayload()
{
  return socketIO.getLastEventPayload();
}

size_t getLastEventPayloadLength()
{
  return socketIO.getLastEventPayloadLength();
}
// // Check if a new JSON message is available
  // if (getLastEventType() == sIOtype_EVENT)
  // {
  //   // Get the payload and length of the received JSON message
  //   uint8_t* payload = getLastEventPayload();
  //   size_t length = getLastEventPayloadLength();



    // Parse the received JSON payload
StaticJsonDocument<200> doc;
deserializeJson(doc, payload, length);


    // Access the JSON data
    const char* message = doc["message"];

    // Print the message in the serial monitor
    Serial.print("Received message: ");
    Serial.println(message);
  }
  
  delay(1000);  // Adjust delay as needed
}
