#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "mkp";
const char* password = "11111111";
const char* serverUrl = "49.249.200.132";
const int serverPort = 3000;

WebSocketsClient webSocket;
bool ledState = false; // Track the state of the LED

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Set extra headers for WebSocket connection
  // webSocket.setExtraHeaders("data:test,test123,station"); // Add your desired extra headers here
  webSocket.setExtraHeaders("Authorization: 1234567890");
  webSocket.setExtraHeaders("data:test,test123,station");
  // Connect to WebSocket server
  webSocket.begin(serverUrl, serverPort);
  webSocket.onEvent(webSocketEvent);

  pinMode(LED_BUILTIN, OUTPUT); // Initialize LED pin as output
  digitalWrite(LED_BUILTIN, LOW); // Turn off LED initially
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    // case WStype_DISCONNECTED:
    //   Serial.println("Disconnected from WebSocket server");
    //   break;
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      break;
    case WStype_TEXT:
      payload[length] = '\0'; // Add null terminator to payload

      // Parse received JSON message
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
        return;
      }

      // Check for specific message field
      if (doc.containsKey("test lock")) {
        String value = doc["test lock"].as<String>();

        if (value == "1") {
          Serial.println("Received message: test lock = 1");
          // Turn on LED
          digitalWrite(LED_BUILTIN, HIGH);
          ledState = true;
        } else if (value == "0") {
          Serial.println("Received message: test lock = 0");
          // Turn off LED
          digitalWrite(LED_BUILTIN, LOW);
          ledState = false;
        }
      }

      break;
  }
}
