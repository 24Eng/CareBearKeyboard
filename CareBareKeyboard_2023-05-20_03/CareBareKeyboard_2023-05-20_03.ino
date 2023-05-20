/*
  created 6 Mar 2012
  modified 27 Mar 2012
  by Tom Igoe

  This example is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardLogout
*/

#include "Keyboard.h"

// Declare the pins
const int row0Pin = 4;
const int row1Pin = 5;
const int row2Pin = 6;
const int column0Pin = 7;
const int column1Pin = 8;
const int column2Pin = 9;
const int column3Pin = 10;
const int column4Pin = 11;
const int column5Pin = 12;


// Define some arrays to hold information about the key arrangement
const int keyboardRows[] = {row0Pin, row1Pin, row2Pin};
const int keyboardColumns[] = {column5Pin, column4Pin, column3Pin, column2Pin, column1Pin, column0Pin};

const int keyboardColumnCount = 6;
const int keyboardRowCount = 3;
int risingEdge[keyboardColumnCount][keyboardRowCount];
long waitingOnTime[keyboardColumnCount][keyboardRowCount];
int sustainedContact[keyboardColumnCount][keyboardRowCount];
int debounceTime = 5;

void setup() {
  // Set the rows as outputs and the columns as inputs
  for(int i;i<keyboardRowCount;i++){
    pinMode(keyboardRows[i], OUTPUT);
    digitalWrite(keyboardRows[i], LOW);
  }
  for(int i;i<keyboardColumnCount;i++){
    pinMode(keyboardColumns[i], INPUT);
  }
  Serial.begin(115200);
  Keyboard.begin();
}

void loop() {
  scanInputs();
  int decimalEquivalent = convertToDecimal();
  if(decimalEquivalent != 0){
    Serial.print(decimalEquivalent);
    Serial.print("\n");
    typeSomething(decimalEquivalent);
  }
  
//  displaySomeTable();
//  displayRisingEdges();
}

void typeSomething(int funLocation){
  if(funLocation == 0){
    return;
  }
  switch (funLocation){
    case 10:
    // Cut
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('X');
      Keyboard.releaseAll();
      break;
    case 11:
    // Copy
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('C');
      Keyboard.releaseAll();
      break;
    case 12:
    // Paste
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('V');
      Keyboard.releaseAll();
      break;
    case 20:
    // Go to the third most recently used window
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(KEY_TAB);
      delay(50);
      Keyboard.write(KEY_TAB);
      delay(50);
      Keyboard.write(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 21:
    // Send the END key
      Keyboard.write(KEY_END);
      break;
    case 22:
    // Type the OMEGA symbol Ω
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(226);
      Keyboard.write(227);
      Keyboard.write(228);
      Keyboard.releaseAll();
      break;
    case 30:
    // Go to the second most recently used window
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(KEY_TAB);
      delay(50);
      Keyboard.write(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 31:
    // Send the HOME key
      Keyboard.write(KEY_HOME);
      break;
    case 32:
    // Type the BULLET symbol •
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(231);
      Keyboard.releaseAll();
      break;
    case 40:
    // Go to the most recently used window
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 41:
    // Select all
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.write('A');
      Keyboard.releaseAll();
      break;
    case 42:
    // Type the DEGREES symbol °
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.write(226);
      Keyboard.write(228);
      Keyboard.write(232);
      Keyboard.releaseAll();
      break;
    case 50:
    // Type Arduino's print command
      Keyboard.print("Serial.print();");
      break;
    case 51:
    // Open Arduino's serial plotter
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.write('L');
      Keyboard.releaseAll();
      break;
    case 52:
    // Open Arduino's serial monitor
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.write('M');
      Keyboard.releaseAll();
      break;
    case 60:
    // Press ESCAPE and release any stuck keys
      Keyboard.write(KEY_ESC);
      Keyboard.releaseAll();
      break;
    case 61:
    // Press all the modifier keys and "Q"
    // This would be useful for triggering a macro in AutoHotkey
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.write('Q');
      Keyboard.releaseAll();
      break;
    case 62:
    // Press all the modifier keys and "W"
    // This would be useful for triggering a macro in AutoHotkey
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.write('W');
      Keyboard.releaseAll();
      break;
    default:
    // If there is an error, release any stuck keys
      Keyboard.releaseAll();
      break;
  }
}

int convertToDecimal(){
  int funDecimal = 0;
  for(int i=0;i<keyboardRowCount;i++){
    for(int k=0;k<keyboardColumnCount;k++){
      if(risingEdge[k][i]){
        funDecimal = 10*k + 10;
        funDecimal = funDecimal + i;
        return funDecimal;
      }
    }
  }
  return 0;
}

void displayRisingEdges(){
  for(int i=0;i<keyboardRowCount;i++){
    for(int k=0;k<keyboardColumnCount;k++){
      if(risingEdge[k][i]){
        Serial.print(k);
        Serial.print(", ");
        Serial.print(i);
        Serial.print("\n");
      } 
    }
  }
}


void displaySomeTable(){
  for(int i=0;i<keyboardRowCount;i++){
    for(int k=0;k<keyboardColumnCount;k++){
      if(risingEdge[k][i] != 0){
        Serial.print("+\t");
      }else{
        Serial.print("-\t");
      }
    }
    Serial.print("\n");
  }
  Serial.print("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n\n");
}


void scanInputs(){
  for(int i=0;i<keyboardRowCount;i++){
    digitalWrite(keyboardRows[i], HIGH);
    for(int k=0;k<keyboardColumnCount;k++){
      bool readKey = digitalRead(keyboardColumns[k]);
      if(!sustainedContact[k][i] && risingEdge[k][i]){
        sustainedContact[k][i] = HIGH;
        risingEdge[k][i] = LOW;
      }
      if(readKey){
        // A key is pressed
        if((waitingOnTime[k][i] == 0) && (sustainedContact[k][i] == LOW)){
          waitingOnTime[k][i] = millis();
        }else{
          if((millis() > (waitingOnTime[k][i] + debounceTime)) && !sustainedContact[k][i] && !risingEdge[k][i]){
            risingEdge[k][i] = HIGH;
          }
        }
      }else{
        // A key is unpressed
        waitingOnTime[k][i] = 0;
        sustainedContact[k][i] = LOW;
        risingEdge[k][i] = LOW;
      }
    }
    digitalWrite(keyboardRows[i], LOW);
  }
}
