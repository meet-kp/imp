#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

// Replace with your network credentials
const char* ssid = "mkp";
const char* password = "11111111";

// Set your web server port number, default is 80/3000/
WiFiServer server(3000);

// Initialize the WebSocket server on port 3000 
WebSocketsServer webSocket = WebSocketsServer(3000);

// Set the GPIO pin for the push button and LED
const int buttonPin = 14; // D5 pin
const int ledPin = 0; // D3 pin

// Set the authorization header
const char* authorizationHeader = "Authorization: 1234567890";
const char* data = "test,test123,station";

// initialize the LED state and button state variables
bool ledState = LOW;
bool buttonState = LOW;
bool lastButtonState = LOW;
int buttonPressCount = 0; // initialize the button press count variable
unsigned long buttonPressTime = 0; // initialize the button press time variable


void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set the button pin mode to input
  pinMode(buttonPin, INPUT_PULLUP);

  // Set the LED pin mode to output
  pinMode(ledPin, OUTPUT);

  // Start the WebSocket server
  webSocket.begin();
  Serial.println("WebSocket server started");

  // Set the authorization header
  webSocket.setAuthorization(authorizationHeader);
  webSocket.setAuthorization(data);
}


void loop() {
  // Check for WebSocket client connections
  webSocket.loop();

  // read the current state of the button
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) 
    { 
      // button is pressed down
      // update the button press time and count
      buttonPressTime = millis();
      buttonPressCount++;

      // turn on the LED
      digitalWrite(ledPin, HIGH);

      // Send a message to the WebSocket client
      webSocket.broadcastTXT("Button Pressed: LED ON");

      // print the button state and count to the serial monitor
      Serial.print("Button Pressed! Count: ");
      Serial.println(buttonPressCount);
      delay (2000);
    }
    else { 
      // button is released
      // turn off the LED
      digitalWrite(ledPin, LOW);

      // Send a message to the WebSocket client
      webSocket.broadcastTXT("Button Released: LED OFF");
    }
    // update the last button state
    lastButtonState = buttonState;
  }
}
