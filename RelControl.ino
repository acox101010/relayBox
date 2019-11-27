
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const byte ROWS = 4; //four rows on membrane switch
const byte COLS = 4; //four columns on membrane switch
const byte numChars = 32; //array to store user cycle count
const byte numTim = 32; // array to store cycle time (on/off)milliseconds
bool newData = false;

int relayPin_1 = 5; //digital output to the first relay
int relayPin_2 = 6; //digital output to the second relay

int numCycles = 0;
int numTime = 0;
int j=1;
char incomingByte = '*'; //character to cancel test
char outgoingByte = '#'; //character to restart test
char receivedChars[numChars]; //array for received data

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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting Up"); //prints to lcd
  pinMode(relayPin_1, OUTPUT);
  pinMode(relayPin_2, OUTPUT);
  digitalWrite(relayPin_1, HIGH);
  digitalWrite(relayPin_2, HIGH);
}
void loop() {
  Serial.println("Enter # of Cycles");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Cycles");
  while (Serial.available() == 0){  //waits for user input
  }
  numCycles = Serial.parseInt(); //parses user input into integer variable
  Serial.println(numCycles);
  lcd.setCursor(0,1);
  lcd.print(numCycles); //prints to lcd
  delay(250);
  Serial.println("Enter on/off time in sec");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("on/off time");
  while (Serial.available() == 0){ //waits for user input
  }
  numTime = Serial.parseInt(); //parses user input into integer variable
  Serial.println(numTime);
  lcd.setCursor(0,1);
  lcd.print(numTime); //prints to lcd
  delay(250);
  for (j=1;j<=numCycles;j=j+1){
    digitalWrite(relayPin_1,HIGH);
    delay(numTime*1000);
    digitalWrite(relayPin_1,LOW);
    delay(numTime*1000);
    Serial.println(j);
    checkSt(); //function checks for character to halt testing
    screenLCD(); //function to display test status
    if (j==numCycles){ 
      testComp(); //function checking if test criteria has been met
    }
  }  
}
void checkSt(){
  //incomingByte = Serial.read();
  Serial.println("Checking Function");
  if (j <= numCycles && Serial.read() == incomingByte){
    digitalWrite(relayPin_1,LOW);
    Serial.println("Test Cancelled by user");
    Serial.println(+j);
    reEnter();
  }
}
void reEnter(){
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Test Cancelled");
 Serial.println("Test Cancelled");
 Serial.println(j);
 lcd.setCursor(0,1);
 lcd.print(j);
 while (Serial.available() == 0){ //waits for user input
  }
 if (j <= numCycles && Serial.read() == outgoingByte){
  Serial.println("Test restarted by user");
 loop();
}
}
void screenLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cycle#");
  lcd.setCursor(0,1);
  lcd.print(j);
}
void testComp(){
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Test Complete");
  Serial.println("Test Complete");
 Serial.println(j);
 lcd.setCursor(0,1);
 lcd.print(j);
 while (Serial.available() == 0){ //waits for user input
  }
 if (j <= numCycles && Serial.read() == outgoingByte){
  Serial.println("Test restarted by user");
 loop();
}
}
