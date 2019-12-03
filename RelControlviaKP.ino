
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
char timInt[8]; // array to store cycle time (on/off)milliseconds
char incomingByte = '*'; //character to cancel test
char can_key; //created global variable for cancelling test

int nums; //variable to convert char array to integer for cycle count
int seconds; //variable to convert char array to integer for cycle time
int i = 0; //integer counter for cycle count
int j = 0; //integer counter for time count
int z = 1; //integer counter for relay loop
int testloopid = 0; //checks the state of the program we are in for cycles
int timeloopid = 0; //checks the state of the program we are in for time
int relayPin_1 = 5; //digital output to the first relay
int relayPin_2 = 6; //digital output to the second relay

void setup() {
  Serial.begin(9600);
  lcd.init(); //initiate LCD comms
  lcd.backlight();//initiate LCD backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting Up"); //prints to lcd
  pinMode(relayPin_1, OUTPUT);
  pinMode(relayPin_2, OUTPUT);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Cycle #");
}

void loop() {
  //loop to capture cycle count user input
  char key = keypad.getKey();
  if (key) {
    lcd.setCursor(0, 0);
    lcd.print("Enter Cycle #"); //prints to lcd
    if (key == '*') {
      i = 0;
      key = 0;
      testloopid = 0; //initial state of program
      Serial.println("");
      Serial.println("Canceled");
      lcd.setCursor(0, 0);
      lcd.print("Test Canceled");
    } else if (key != '#') {
      entryInt[i] = key;
      i++;
      testloopid = 0; // initial state of program
      lcd.setCursor(0, 1);
      lcd.print(entryInt);
    }
    else {
      Serial.println("");
      i = 0;
      key = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cycles Captured");
      delay(2000);
      lcd.clear();
      Serial.println("Num loop complete");
      testloopid = 1; //sets variable to 1 once entryInt captured from user
    }
  }

  //loop to capture cycle time
  if (testloopid == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Enter On/Off");
    char time_key = keypad.getKey();
    if (time_key) {
      lcd.setCursor(0, 0);
      lcd.print("Enter On/Off"); //prints to lcd
      if (time_key == '*') {
        j = 0;
        time_key = 0;
        Serial.println("");
        Serial.println("Canceled");
        lcd.setCursor(0, 0);
        lcd.print("Test Canceled");
      } else if (time_key != '#') {
        timInt[j] = time_key;
        j++;
        Serial.print(time_key);
        lcd.setCursor(0, 1);
        lcd.print(timInt);
      }
      else {
        Serial.println("");
        j = 0;
        time_key = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("On/Off Captured");
        delay(2000);
        timeloopid = 1; //sets variable to 1 once timInt captured from user
      }
    }
  }

  //loop to start execution of relay program
  if (testloopid == 1 && timeloopid == 1) {
    nums = atoi(entryInt);
    seconds = atoi(timInt);
    seconds = seconds * 1000; //converts milliseconds to seconds
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(nums);
    lcd.setCursor(0, 1);
    lcd.print(seconds / 1000);
    for (z = 1; z <= nums; z = z + 1) {
      digitalWrite(relayPin_1, HIGH);
      digitalWrite(relayPin_2, HIGH);
      delay(seconds);
      digitalWrite(relayPin_1, LOW);
      digitalWrite(relayPin_2, LOW);
      delay(seconds);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Testing");
      lcd.setCursor(0, 1);
      lcd.print(String("Cycle: ") +String(z));
      can_key = keypad.getKey();
      if (can_key == incomingByte) {
        checkSt();
      }
      if (z == nums) {
        testComp();
      }
    }
  }
}

//function executes when testing is complete
void testComp() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Complete");
  lcd.setCursor(0, 1);
  lcd.print(z);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  testloopid = 0;
  timeloopid = 0;
  i = 0;
  j = 0;
  z = 1;
  nums = 0;
  seconds = 0;
  delay(5000);
  loop();
}

//function executes when user cancels testing
void checkSt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test Canceled");
  lcd.setCursor(0, 1);
  lcd.print(z);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
  testloopid = 0;
  timeloopid = 0;
  i = 0;
  j = 0;
  z = 1;
  nums = 0;
  seconds = 0;
  delay(5000);
  loop();
}
