/**********************************************************************
  Filename    : Combination Lock
  Description : Make a simple combination lock.
  Auther      : www.freenove.com
  Modification: 2022/10/26
**********************************************************************/
#include <Keypad.h>

#define SERVO_PIN 21  //define the pwm pin
#define SERVO_CHN 0   //define the pwm channel
#define SERVO_FRQ 50  //define the pwm frequency
#define SERVO_BIT 12  //define the pwm precision
#define BUZZER_PIN 17 // Define the buzzer pin

void servo_set_pin(int pin);
void servo_set_angle(int angle);

byte rowPins[4] = {14, 13, 12, 11}; // connect to the row pinouts of the keypad
byte colPins[4] = {10,  9,  8, 18}; // connect to the column pinouts of the keypad

// define the symbols on the buttons of the keypad
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// initialize an instance of class NewKeypad
Keypad myKeypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

char passWord[] = {"1234"}; // Save the correct password

void setup() {
  servo_set_pin(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  static char keyIn[4];     // Save the input character
  static byte keyInNum = 0; // Save the the number of input characters
  char keyPressed = myKeypad.getKey();  // Get the character input
  // Handle the input characters
  if (keyPressed) {
    // Make a prompt tone each time press the key
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    // Save the input characters
    keyIn[keyInNum++] = keyPressed;
    // Judge the correctness after input
    if (keyInNum == 4) {
      bool isRight = true;            // Save password is correct or not
      for (int i = 0; i < 4; i++) {   // Judge each character of the password is correct or not
        if (keyIn[i] != passWord[i])
          isRight = false;            // Mark wrong passageword if there is any wrong character.
      }
      if (isRight) {                  // If the input password is right
        servo_set_angle(90);           // Open the switch
        delay(2000);                  // Delay a period of time
        servo_set_angle(0);            // Close the switch
        Serial.println("passWord right!");
      }
      else {                          // If the input password is wrong
        digitalWrite(BUZZER_PIN, HIGH);// Make a wrong password prompt tone
        delay(1000);
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("passWord error!");
      }
      keyInNum = 0; // Reset the number of the input characters to 0
    }
  }
}

void servo_set_pin(int pin) {
  ledcAttachChannel(pin, SERVO_FRQ, SERVO_BIT, SERVO_CHN);
}

void servo_set_angle(int angle) {
  if (angle > 180 || angle < 0)
    return;
  long pwm_value = map(angle, 0, 180, 103, 512);
  ledcWrite(SERVO_PIN, pwm_value);
}