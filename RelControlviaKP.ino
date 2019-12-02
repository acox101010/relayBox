
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
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char entryInt[8]; //array to store user cycle count
char timInt[8]; // array to store cycle time (on/off)milliseconds
int nums; //variable to convert char array to integer for cycle count
int seconds; //variable to convert char array to integer for cycle time

int i = 0; //integer counter for cycle count
int j = 0; //integer counter for time count
int z = 1; //integer counter for relay loop
int testloopid = 0; //checks the state of the program we are in
int timeloopid = 0; //checks the state of the program we are in
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
}

void loop() {
  //loop to capture cycle count user input
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      i = 0;
      key = 0;
      testloopid = 0; //initial state of program
      Serial.println("");
      Serial.println("Canceled");
    } else if (key != '#') {
      entryInt[i] = key;
      i++;
      testloopid = 0; // initial state of program
      Serial.print(key);
    }
    else {
      Serial.println("");
      i = 0;
      key = 0;
      testloopid = 1; //sets variable to 1 once entryInt captured from user
      //Serial.println(entryInt);
      Serial.println("Num loop complete");
    }
  }
  delay(100); //delay 0.100 second

  //loop to capture cycle time
  if (testloopid == 1) {
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
        timeloopid = 1; //sets variable to 1 once timInt captured from user
        //Serial.println(timInt);
        Serial.println("Time loop complete");
      }
    }
  }
  // delay(100); //delay 0.100 second

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
      lcd.setCursor(0,0);
      lcd.print("Testing");
      lcd.setCursor(0,1);
      lcd.print(z);
      if (z == nums) {
        testComp();
      }
    }
  }
}
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
  loop();
}
//
//
//
//
//
//Serial.println(z);
//checkSt(); //function checks for character to halt testing
//screenLCD(); //function to display test status
//if (z == entryInt) {
//testComp(); //function checking if test criteria has been met
//}
// }
//}
//}
//function to print test output complete
//void testComp() {
//lcd.clear();
//lcd.setCursor(0, 0);
//lcd.print("Test Complete");
//Serial.println("Test Complete");
//Serial.println(z);
//lcd.setCursor(0, 1);
//lcd.print(z);
