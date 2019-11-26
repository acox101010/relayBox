
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //four rows on membrane switch
const byte COLS = 4; //four columns on membrane switch
const byte numChars = 32; //array to store user cycle count
const byte numTim = 32; // array to store cycle time (on/off)milliseconds
bool maxData = false;

int relayPin_1 = 5; //digital output to the first relay
int relayPin_2 = 6; //digital output to the second relay

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {30, 33, 34, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38, 41, 42, 45}; //connect to the column pinouts of the keypad

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); //LCD address after scan and 16 columns 2 rows
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //constructors per playground.arduino.cc/Code/Keypad/

//Setup the LCD and everything
void setup() { 
  Serial.begin(9600); //initiate serial comms
  lcd.init(); //initiate LCD comms
  lcd.backlight();//initiate LCD backlight
}

//function for capturing cycle count
void maxCyc() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Cycles:"); 
  lcd.setCursor(0, 1);
  char key = keypad.getKey();
  if (key){
    lcd.print(key);
    
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //maxCyc();
  //lcd.print("looping");
  }
