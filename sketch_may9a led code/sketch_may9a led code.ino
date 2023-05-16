#define LED_PIN 0 // D3 pin
#define BUTTON_PIN 14 // D5 pin  




// initialize the LED state and button state variables
bool ledState = LOW;
bool buttonState = LOW;
bool lastButtonState = LOW;
int buttonPressCount = 0; // initialize the button press count variable
unsigned long buttonPressTime = 0; // initialize the button press time variable


void setup()
{
  // Serial.begin(921600);
  Serial.begin(19200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
}
   
void loop()
{
  // read the current state of the button
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    if (buttonState == 1) 
    { 
      // button is pressed down
      // update the button press time and count
      buttonPressTime = millis();
      buttonPressCount++;

      // turn on the LED
      digitalWrite(LED_PIN, HIGH);
      
      // print the button state and count to the serial monitor
      Serial.print("Button Pressed! Count: ");
      Serial.println(buttonPressCount);
      delay(5000);
    }
    else { 
      // button is released 
      // turn off the LED
      digitalWrite(LED_PIN, LOW);
    }
    // update the last button state
    lastButtonState = buttonState;
  }
}
