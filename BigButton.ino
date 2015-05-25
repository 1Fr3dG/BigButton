#include <EEPROM.h>

const int buttonPin = 9;  // input pin for pushbutton
const int configMax = 6;  // config slots
const int configAddress = 0;  // button mode number, in EEPROM address

char* configNames[] = {
  "0. Default mode: Enter.",
  "1. Emergency STOP 1: ESC.",
  "2. Emergency STOP 2: Ctrl-C.",
  "3. Emergency STOP 3: ESC, Enter.",
  "4. Emergency STOP 4: ESC, tab, Enter.",
  "5. Emergency STOP 5: Ctrl-C, Y, Enter.",
  "6. Emergency Shutdown Windows: run \"shutdown -s -f -t 0\"."
};

int previousButtonState = HIGH;   // for checking the state of a pushButton
int buttonMode = 0;  // defautl mode: Enter

void setup() {
  pinMode(buttonPin, INPUT);
  buttonMode = EEPROM.read(configAddress);
  if(buttonMode > configMax){
    buttonMode = 0;
    EEPROM.write(configAddress, 0);
  }
  Keyboard.begin();
  delay (200);

  //config
  int buttonState = digitalRead(buttonPin);
  previousButtonState = buttonState;
  if (buttonState == HIGH){
    for (int i=0; i<20; i++){
      if(digitalRead(buttonPin) != HIGH){
        return;
      }
      delay (50);
    }//Enter config

    Keyboard.println("Welcome to Alfred's Big Button!");
    Keyboard.println("Entering configuration...\n----------");
    delay (1000);
    
    while(1){
      Keyboard.print("Current mode: ");
      Keyboard.println(configNames[buttonMode]);
      Keyboard.println("\tPress to change, Disconnect to save...");
      delay(500);
      previousButtonState = LOW;
      buttonState = LOW;
      while(buttonState == previousButtonState || buttonState == HIGH){
        previousButtonState = buttonState;
        buttonState = digitalRead(buttonPin);
      } // This is a press
      buttonMode = (buttonMode+1)%(configMax+1);
      EEPROM.write(configAddress, buttonMode);
      delay(50);
    }

    // shouldn't be here
  }
  
  
}

void loop() {

  int buttonState = digitalRead(buttonPin);

  if ((buttonState != previousButtonState)  && (buttonState == HIGH)) {
    
    switch (buttonMode)
    {
      case 1:
        Keyboard.press(KEY_ESC);
        break;
      case 2:
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press('c');
        break;
      case 3:
        Keyboard.press(KEY_ESC);
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(KEY_RETURN);
        delay(10);
        Keyboard.releaseAll();
        break;
      case 4:
        Keyboard.press(KEY_ESC);
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(KEY_TAB);
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(KEY_RETURN);
        delay(10);
        Keyboard.releaseAll();
        break;
      case 5:
        Keyboard.press(KEY_LEFT_CTRL);
        delay(10);
        Keyboard.press('c');
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press('y');
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.press(KEY_RETURN);
        delay(10);
        Keyboard.releaseAll();
        break;
      case 6:
        Keyboard.press(KEY_LEFT_GUI);
        delay(10);
        Keyboard.press('r');
        delay(10);
        Keyboard.releaseAll();
        delay(100);
        Keyboard.println("shutdown -s -f -t 0");
        break;
      case 0:
      default:
        Keyboard.press(KEY_RETURN);
    }
  }
  else if ((buttonState != previousButtonState)  && (buttonState == LOW)) {
    Keyboard.releaseAll();
  }
  
  previousButtonState = buttonState;
}
