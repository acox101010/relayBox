#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //four rows on membrane switch
const byte COLS = 4; //four columns on membrane switch
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {30, 33, 34, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38, 41, 42, 45}; //connect to the column pinouts of the keypad

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); //LCD address after scan and 16 columns 2 rows
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //initiates constructors

char entryInt[8]; //array to store user cycle count
char timInt[8]; // array to store cycle time (on) milliseconds
char timOff[8]; // array to store cycle time (off) milliseconds
char incomingByte = '*'; //character to cancel test
char restartByte = 'B'; //character to restart parameter entry
char can_key; //created global variable for cancelling test
char waiting; //waits for keypad press

int nums; //variable to convert char array to integer for cycle count
int seconds; //variable to convert char array to integer for cycle time On
int secondsOff; //variable to convert char array to integer for cycle time Off
int i = 0; //integer counter for cycle count
int j = 0; //integer counter for time count (on)
int x = 0; //integer counter for time count (off)
int z = 1; //integer counter for relay loop
int testloopid = 0; //checks the state of the program we are in for cycles
int timeloopid = 0; //checks the state of the program we are in for time
int timeloopid_2 = 0;
int relayPin_1 = 5; //digital output to the first relay
int relayPin_2 = 6; //digital output to the second relay
int redLED = 10;
int greenLED = 11;

//====================Start of Program - Setup=======================================///
void setup() {
  Serial.begin(9600);
  lcd.init(); //initiate LCD comms
  lcd.backlight();//initiate LCD backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting Up"); //prints to lcd
  pinMode(relayPin_1, OUTPUT);
  pinMode(relayPin_2, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Cycle #");
  Serial.println("finished setup");
}
//====================Start of Program - Loop=======================================///
void loop() {//loop to capture cycle count user input
  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Cycle #"); //prints to lcd
    Serial.println("enter cycle #");
    if (key == '*') {
      i = 0;
      key = 0;
      testloopid = 0; //initial state of program for variables
      timeloopid = 0;
      timeloopid_2 = 0;
      Serial.println("");
      Serial.println("Canceled");
      lcd.setCursor(0, 0);
      lcd.print("Test Canceled");
      for ( int i = 0; i < sizeof(entryInt);  ++i ) //added for clearing array and setting index to 0
        entryInt[i] = (char)0;
    } else if (key != '#') {
      entryInt[i] = key;
      i++;
      testloopid = 0; // initial state of program
      lcd.setCursor(0, 1);
      lcd.print(entryInt);
      Serial.println(entryInt);
    }
    else {
      Serial.println("");
      i = 0;
      key = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cycles Captured");
      delay(1000);
      lcd.clear();
      Serial.println("Num loop complete");
      testloopid = 1; //sets variable to 1 once entryInt captured from user

    }
  }
  //++++++loop to capture ON cycle time+++++++++++++++++++++++++++++++++++++++++
  if (testloopid == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Enter On");
    char time_key = keypad.getKey();
    if (time_key) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter On"); //prints to lcd
      Serial.println("Enter on");
      if (time_key == '*') {
        j = 0;
        time_key = 0;
        lcd.clear();
        Serial.println("");
        Serial.println("Canceled");
        lcd.setCursor(0, 0);
        lcd.print("Test Canceled");
        delay(300);
        lcd.clear();
        for ( int j = 0; j < sizeof(timInt);  ++j ) //added for clearing array and setting index to 0
          timInt[j] = (char)0;
        for ( int i = 0; i < sizeof(entryInt);  ++i ) //added for clearing array and setting index to 0
          entryInt[i] = (char)0;
        testloopid = 0;
        timeloopid = 0;
        timeloopid_2 = 0;
        setup(); //Re-initiate program
      } else if (time_key != '#') {
        timInt[j] = time_key;
        j++;
        Serial.println(time_key);
        lcd.setCursor(0, 1);
        lcd.print(timInt);
        Serial.println(timInt);
      }
      else {
        Serial.println("");
        j = 0;
        time_key = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("On Captured");
        Serial.println("On Captured");
        delay(1000);
        lcd.clear();
        timeloopid = 1; //sets variable to 1 after timInt captured from user
        timeloopid_2 = 1;
      }
    }
  }

  //++++++loop to capture OFF cycle time+++++++++++++++++++++++++++++++++++++++++
  while (testloopid == 1 && timeloopid_2 == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Enter Off");
    char time_key_2 = keypad.getKey();
    if (time_key_2) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Off"); //prints to lcd
      Serial.println("Enter Off");
      if (time_key_2 == '*') {
        x = 0;
        time_key_2 = 0;
        lcd.clear();
        Serial.println("");
        Serial.println("Canceled");
        lcd.setCursor(0, 0);
        lcd.print("Test Canceled");
        delay(300);
        lcd.clear();
        for ( int j = 0; j < sizeof(timInt);  ++j ) //added for clearing On time array and setting index to 0
          timInt[j] = (char)0;
        for ( int x = 0; x < sizeof(timOff);  ++x ) //added for clearing Off time array and setting index to 0
          timOff[x] = (char)0;
        for ( int i = 0; i < sizeof(entryInt);  ++i ) //added for clearing array and setting index to 0
          entryInt[i] = (char)0;
        testloopid = 0;
        timeloopid = 0;
        timeloopid_2 = 0;
        setup(); //Re-initiate program
      } else if (time_key_2 != '#') {
        timOff[x] = time_key_2;
        x++;
        Serial.println(time_key_2);
        lcd.setCursor(0, 1);
        lcd.print(timOff);
        Serial.println(timOff);
      }
      else {
        Serial.println("");
        x = 0;
        time_key_2 = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Off Captured");
        Serial.println("Off Captured");
        delay(1000);
        lcd.clear();
        testloopid = 2;
        timeloopid_2 = 2; //sets variable to 2 after timInt captured from user
      }
    }
  }

  //loop to start execution of relay program
  if (testloopid == 2 && timeloopid_2 == 2) {
    nums = atoi(entryInt);
    seconds = atoi(timInt);
    secondsOff = atoi(timOff);
    seconds = seconds; //Use to convert milliseconds to seconds if requested
    secondsOff = secondsOff; //Use to convert milliseconds to seconds if requested
    Serial.println(nums);
    Serial.println(seconds);
    Serial.println(secondsOff);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(nums);
    lcd.setCursor(0, 1);
    lcd.print(seconds);
    lcd.setCursor(7, 1);
    lcd.print(secondsOff);
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    for (z = 1; z <= nums; z = z + 1) {
      can_key = keypad.getKey(); //capture keypad key
      if (can_key) {
        switch (can_key)
        {
          case '*':
            checkSt();
            break;
          case 'D':
            orig();
            break;
          default:
            Serial.println(can_key);
        }
      }
      digitalWrite(relayPin_1, HIGH);
      digitalWrite(relayPin_2, LOW);
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      delay(seconds); //On time defined by user
      can_key = keypad.getKey(); //capture keypad key
      if (can_key) {
        switch (can_key)
        {
          case '*':
            checkSt();
            break;
          case 'D':
            orig();
            break;
          default:
            Serial.println(can_key);
        }
      }
      digitalWrite(relayPin_1, LOW);
      digitalWrite(relayPin_2, HIGH);
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      delay(secondsOff);  //Off time defined by user
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Testing");
      lcd.setCursor(0, 1);
      lcd.print(String("Cycle: ") + String(z));
      can_key = keypad.getKey(); //capture keypad key
      if (can_key) {
        switch (can_key)
        {
          case '*':
            checkSt();
            break;
          case 'D':
            orig();
            break;
          default:
            Serial.println(can_key);
        }
      }
      if (z == nums) {
        Serial.println("Test Completed");
        testComp();
      }
    }
  }
}

//++++++++++++++++++++++function executes when testing is complete+++++++++++++++++++++++
void testComp() {
  Serial.println("Enter comp loop");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Completed");
  lcd.setCursor(0, 1);
  lcd.print(z);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  testloopid = 0;
  timeloopid = 0;
  timeloopid_2 = 0;
  delay(500);
  Serial.println("comp loop completed");
  Serial.println(entryInt);
  Serial.println(timInt);
  Serial.println(timOff);
  rest();
}

//++++++++++++++++++++++function executes when user cancels testing+++++++++++++++++++++++
void checkSt() {
  Serial.print("Enter Canceled loop");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Canceled");
  lcd.setCursor(0, 1);
  lcd.print(z);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  testloopid = 0;
  timeloopid = 0;
  timeloopid_2 = 0;
  delay(500);
  Serial.println("cancel loop completed");
  rest();
}

//+++++++++++++++++++++++function to lcd print original values+++++++++++++++++++++++++++++++
void orig() {
  Serial.println("In values loop");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(nums);
  lcd.setCursor(0, 1);
  lcd.print(seconds / 1000);
  Serial.println("leaving values loop");
}


//+++++++++++++++++++++++function to reset all array values++++++++++++++++++++++++++++++++++
void rest() {
  for ( int i = 0; i < sizeof(entryInt);  ++i )
    entryInt[i] = (char)0;
  Serial.println("reset entryInt");
  for ( int x = 0; x < sizeof(timOff);  ++x ) //added for clearing Off time array and setting index to 0
    timOff[x] = (char)0;
  Serial.println("reset timOff");
  for ( int j = 0; j < sizeof(timInt);  ++j )
    timInt[j] = (char)0;
  Serial.println("reset timeInt");
  Serial.println("im done");
  z = nums; //sets increment equal to test completion count
}
