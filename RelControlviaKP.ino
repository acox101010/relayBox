
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {30, 33, 34, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38, 41, 42, 45}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char entryInt[8]; //entry for cycle count
char timInt[8]; //entry for cycle time
int i = 0; //integer counter for cycle count
int j = 0; //integer counter for time count
int testloopid = 0; //checks where we are in the loop

void setup() {
  Serial.begin(9600);
}

void loop() {
  //if (strlen(entryInt) == NULL && strlen(timInt) == NULL) { //waits for user input
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      //memset(entryStr, 0, sizeof(entryStr));
      i = 0;
      key = 0;
      testloopid = 0; //tests where we are
      Serial.println("");
      Serial.println("Canceled");
    } else if (key != '#') {
      entryInt[i] = key;
      i++;
      testloopid = 0; // tests where we are
      Serial.print(key);
    }
    else {
      Serial.println("");
      i = 0;
      key = 0;
      testloopid = 1; ///tests where we are
      Serial.println(entryInt);
      Serial.println("Num loop complete");
    }
  }

delay(100); //delay 0.100 second

if ( testloopid == 1) {
  //Serial.println("test looper");
  char time_key = keypad.getKey();
  if (time_key) {
    if (time_key == '*') {
      j = 0;
      time_key = 0;
      Serial.println("");
      Serial.println("Canceled");
    } else if (time_key != '#') {
      timInt[j] = time_key;
      j++;
      Serial.print(time_key);
    }
    else {
      Serial.println("");
      j = 0;
      time_key = 0;
      Serial.println(timInt);
      Serial.println("Time loop complete");
    }
  }
}
}
