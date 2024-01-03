#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//librarys used for translation program
#include <string.h>
#include <stdio.h>

// Structure to hold English to Braille mappings
struct E2B_KeyValue {
    char key;
    char value[7];
};

// English to Braille dictionary
struct E2B_KeyValue EngToBrailleDict[] = {
    {'a', "100000"}, {'b', "110000"}, {'c', "100100"}, {'d', "100110"}, {'e', "100010"},
    {'f', "110100"}, {'g', "110110"}, {'h', "110010"}, {'i', "010100"}, {'j', "010110"},
    {'k', "101000"}, {'l', "111000"}, {'m', "101100"}, {'n', "101110"}, {'o', "101010"},
    {'p', "111100"}, {'q', "111110"}, {'r', "111010"}, {'s', "011100"}, {'t', "011110"},
    {'u', "101001"}, {'v', "111001"}, {'w', "010111"}, {'x', "101101"}, {'y', "101111"},
    {'z', "101011"}, {'A', "100000"}, {'B', "110000"}, {'C', "100100"}, {'D', "100110"}, 
    {'E', "100010"}, {'F', "110100"}, {'G', "110110"}, {'H', "110010"}, {'I', "010100"}, 
    {'J', "010110"}, {'K', "101000"}, {'L', "111000"}, {'M', "101100"}, {'N', "101110"}, 
    {'O', "101010"}, {'P', "111100"}, {'Q', "111110"}, {'R', "111010"}, {'S', "011100"}, 
    {'T', "011110"}, {'U', "101001"}, {'V', "111001"}, {'W', "010111"}, {'X', "101101"}, 
    {'Y', "101111"}, {'Z', "101011"}, {'0', "011101"}, {'1', "100000"}, {'2', "101000"}, 
    {'3', "100100"}, {'4', "100110"}, {'5', "100010"}, {'6', "101100"}, {'7', "101110"}, 
    {'8', "101010"}, {'9', "011100"}, {',', "001000"}, {' ', "000000"}, {'.', "001001"}, 
    {'?', "001010"}, {'!', "001011"}, {'@', "001110"}, {'#', "001111"}, {'$', "010010"}, 
    {'%', "010001"}, {'&', "001101"}, {'*', "010000"}, {'(', "011000"}, {')', "001100"},
    {'_', "010111"}, {'+', "011010"}, {'^', "011011"}, {'-', "010011"}, {'=', "011001"}, 
    {'{', "011010"}, {'}', "001101"}, {'[', "011110"}, {']', "001111"}, {':', "010101"}, 
    {';', "001101"}, {'"', "001010"}, {'\'', "001000"}, {'<', "011011"}, {'>', "001011"}, 
    {',', "001000"}, {'.', "001001"}, {'?', "001010"}, {'/', "010100"}, {'_', "010111"},
};

// Function to convert an English character to Braille representation
char *getBraille(char character) {
    for (size_t i = 0; i < sizeof(EngToBrailleDict) / sizeof(EngToBrailleDict[0]); ++i) {
        if (EngToBrailleDict[i].key == character) {
            return EngToBrailleDict[i].value;
        }
    }
    return NULL; // Character not found
}

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)


int angleToPulse(int ang){
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
  return pulse;
}

void setup() {
    Serial.begin(9600); // Initialize serial communication
    while (!Serial);    // Wait for the Serial Monitor to open

    Serial.println("32 channel Servo test!");

    board1.begin(); 
    board1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}




void loop() {
    int count = 1;
    const char *input = "H";

    Serial.println("Translating 'HELLO, WORLD' to Braille:");

    // Translate each character to Braille
    for (int i = 0; input[i] != '\0'; i++) {
        char character = input[i];
        
        // Get Braille representation for the current character
        char *braille = getBraille(character);
        
        if (braille != NULL) {
            Serial.print(character);
            Serial.print(": ");
            Serial.println(braille);
            for (int j = 0; braille[j] != '\0'; j++) {
              Serial.print(braille[j]); // Print each character of the Braille representation
              Serial.print(" ");
              if(braille[j] == '0'){
                board1.setPWM(j, 0, angleToPulse(0) );
              } else if (braille[j] =='1'){
                  board1.setPWM(j, 0, angleToPulse(90) );
              } else{
                Serial.println("Invalid Braille character");
              }
            }       
        } else {
            Serial.print(character);
            Serial.println(": Braille representation not found");
        }
    }
      // code to move one servo, probablly change that 10 to a higher degree
    delay(200);

    for( int i = 0; i < 16; i++){  
        board1.setPWM(i, 0, angleToPulse(0));
    }

    delay(500); // Delay for 30 second

    Serial.print(count);
    count = count + 1;
    Serial.println("loop");
}

