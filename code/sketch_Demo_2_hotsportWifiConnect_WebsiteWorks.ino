//library used for wifi
#include <WiFi.h>
//library used to make a web server
#include <ESPAsyncWebSrv.h>
//Library used to load information into the esp32
#include <SPIFFS.h>
//library used to move the servo hub and servo motors
#include <Wire.h>
//library used by the servo driver
#include <Adafruit_PWMServoDriver.h>
//librarys used for translation program
#include <string.h>
#include <stdio.h>

//info to be used for wifi
const char *ssid = "Hello"; //Enter the router name
const char *password = "kairuihu"; //Enter the router password

//holds user input
const char* PARAM_STRING = "inputString";

//starts a server
AsyncWebServer server(80);

// activate a object for the Servo Driver (0x40) is the default address.

Adafruit_PWMServoDriver board1 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  100 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

#define BUTTON_PIN_1 4
#define BUTTON_PIN_2 13
// Structure to hold English to Braille mappings
int curr_servo_pos[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
//make an array for the current servo positions, and set them all to 0

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

// 1,2,6 turn the other way


int angleToPulse(int ang){
    int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
    return pulse;
}




//holds the html file
const char *htmlContent = R"html(
<html>

<head>
  <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
  <title>English Braille Translator</title>
  <script src="translate.js"></script>
</head>


<script>

  // takes input and makes api calls to get images
  function Translate() {

      const textField = document.getElementById("textField");
      const text = textField.value;
      const translationField = document.getElementById("translationField");

      translationField.innerHTML = ""; // clear translationField

      // insert a web element for each character in the text variable
      for (var i = 0; i < text.length; i++) {

          // Braille images can be stored locally on ESP device, space permiting
          let char = text[i];

          switch(char) {
              case " ": char = "space"; break;
              case "(": char = "bracket"; break;
              case ")": char = "bracket"; break;
              case ",": char = "comma"; break;
              case "!": char = "exclamation"; break;
              case "/": char = "forward_slash"; break;
              case "-": char = "hyphen"; break;
              case ".": char = "period"; break;
              case "?": char = "question"; break;
              case ";": char = "semicolon"; break;
              case "'": char = "apostrophe"; break;
              case "Â´": char = "apostrophe"; break;
              case "$": char = "dollar"; break;
              }

          const braille_img = document.createElement("img");
          braille_img.src = "https://www.byronknoll.com/braille/Braille_"+char+".png";
          braille_img.alt = char;

          const outer_div = document.createElement("div");
          outer_div.className = "custom-tooltip";

          const inner_div = document.createElement("div");
          inner_div.className = "tooltip-text";
          inner_div.innerText = char; 

          outer_div.appendChild(inner_div);
          outer_div.appendChild(braille_img);

          translationField.appendChild(outer_div);
      }

  }
  // for onclick event to translate input
  function TranslateOnClick() {
      Translate();
  }
  
  //for onclick event to save input to esp32
  function submitMessage() {
    alert("Saved value to ESP SPIFFS");
    setTimeout(function(){ document.location }, 500);   
  }

  // originaly used but changed to a event listener as seen bellow
  //function TranslateOnKeyDown(event) {
  //    if (event.key === "Enter") {
  //        event.preventDefault(); // prevent default form submission
  //        Translate();
  //    }
  //}
  //  function submitMessageOnKeyDown(event) {
  //    if (event.key === "Enter") {
  //        event.preventDefault(); // prevent default form submission
  //        submitMessage();
  //    }
  //}

  //event listener for when the ENTER button is pressed, will both translate and save the info to esp32
  document.getElementById("textField").addEventListener("keydown", function (event) {
    if (event.key === "Enter") {
      event.preventDefault(); // prevent default form submission
      Translate();
      submitMessage();
      }});

</script>

<body bgcolor="#cccccc" style="font-family:sans-serif">
  <table align="center" width="740" cellpadding="0" cellspacing="0" border="0"
    style="background-color: #FFFFFF;font-size:80%">
    <tbody>
      <tr>
        <td width="8"><img src="./images/corner1.gif" width="8" height="8" border="0" alt="...">
        </td>
        <td width="724"></td>
        <td width="8"><img src="./images/corner2.gif" width="8" height="8" border="0" alt="...">
        </td>
      </tr>
      <tr>
        <td></td>
        <td>
          <h1 align="center">English Braille Translator</h1>
          <div align="center">Enter text to see how it is represented in Grade 1 Braille.</div>
          <div align="center">Hover over a braille cell to see its english representation.</div>
          <br><br>
          <div id="translationField"></div>



              <form action="/get" target="hidden-form">
                   <input type="text"  name="inputString" id="textField" placeholder="Enter text" >
                       <button onclick="TranslateOnClick(); submitMessage()">Submit</button>
              </form>



        </td>
        <td></td>
      </tr>
      <tr>
        <td><img src="./images/corner3.gif" width="8" height="8" border="0" alt="..."></td>
        <td></td>
        <td><img src="./images/corner4.gif" width="8" height="8" border="0" alt="..."></td>
      </tr>
    </tbody>
  </table>


</body>

</html>)html";

// error page for webpage
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

// readFiles reads the file thats saved in the ESP32
//   it takes 2 paramaters 
//    fs::FS -> reference to a filesystem object, the SPIFFS
//    const char *path -> the path to the file to be read
String readFile(fs::FS &fs, const char * path) //
{
  Serial.printf("Reading file: %s\r\n", path); // printout path
  // attempt to open file, fs is an instance of the filesystem, and r stands for read mode
  File file = fs.open(path, "r");
  //if file is not open return empty string
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }

  Serial.println("- read from file:");
  String fileContent;

  //if file is successfully opened then reads each character from the file and appends it to the string fileContent
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  
  // close the file
  file.close();
  // print the content to the serial monitor
  Serial.println(fileContent);
  // return the content as a string
  return fileContent;
}



// writeFile writes info to the ESP32
//  takes 3 parameters 
//   fs::FS &fs -> refrence to a filesystem object
//   const char *path -> Path to the file to be written
//   cons char *message -> the content to be written to the file
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  // opens file given path and "w" which stands for write mode
  File file = fs.open(path, "w");

  // if the file fails to open then return
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }

  //if the file successfully 
  //opens then write the content of message to the file using file.print(message)
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  //close file
  file.close();
}



// Replaces placeholder with stored values
//   const String& var represents the placeholder for the content to be generated
String processor(const String& var){
  // checks if var is the same as the inputString which is the name for the input text in th html
  if(var == "inputString"){
    // if true call readFile and pass SPIFFS which is an instance of the filesystem 
    //and /inputString which is the path to the file to be read
    return readFile(SPIFFS, "/inputString.txt");
  }
  // if false return a empty string
  return String();
}



char *getBraille(char character) {
    for (size_t i = 0; i < sizeof(EngToBrailleDict) / sizeof(EngToBrailleDict[0]); ++i) {
        if (EngToBrailleDict[i].key == character) {
            return EngToBrailleDict[i].value;
        }
    }
    return NULL; // Character not found
}


void setup() {

  Serial.begin(115200);
  Serial.println("start");
  while (!Serial);    // Wait for the Serial Monitor to open
  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  Serial.println("32 channel Servo test!");

  //  ensure that the SPIFFS filesystem is propery initalized during the setup phase
  #ifdef ESP32
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #else
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #endif

  // set wifi to station mode 
  // station mode makes the ESP32 act as a client
  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  // search for wifi with the specified SSID and password
  WiFi.begin(ssid, password);

  // if the wifi is in the middle of connecting print . so we know its loading
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }  

// Serve the root page, basiclly adds the HTML file to the server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", htmlContent); // Serve the HTML content
  });

// get request for the user input to be saved to the ep32
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // holds input message recived from GET request
    String inputMessage;
    
    //checks if the get request contains a parameter with the name specified by the PARAM_STRING
    //which is inputString in this case 
    if (request->hasParam(PARAM_STRING)) {
      // if param is present store it 
      inputMessage = request->getParam(PARAM_STRING)->value();
      //then write the message to the file named inputString.txt in the SPIFFS filesystem
      writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }
    else {
      // if there is no parameter in the GET request
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
    
  // print the wifi ip address
  Serial.print("IP is ");
  Serial.println(WiFi.localIP());
  server.begin();

  //start the servodriver board
  board1.begin();
  // set the PWM frequency to 60 hz which is common for analog servo motors
  board1.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

}
int currentLetter = 0; // Index of the current letter to translate










void loop() {
  // reads from the filesystem 
  String input = readFile(SPIFFS, "/inputString.txt");
  Serial.print("*** Your inputString: ");

  //prints out the result.
  Serial.println(input);
  //yourInputString is the input string that the user had submitted
  
  //__________________________Code for Servos_______________________________
  
  // code to move one servo, probablly change that 10 to a higher degree

    char character = input[currentLetter]; // Get the current letter from the string
    char character1 = input[(currentLetter + 1)% input.length()];
    Serial.println("Translating to Braille:");
    Serial.print(character);
    Serial.print(": ");
    char *braille = getBraille(character); // Get Braille representation for the current character
    char *braille1 = getBraille(character1);
      if (braille != NULL) {
          Serial.println(braille);
          for (int j = 0; j <= 6; j++) {
              // Process Braille representation and move servos accordingly
              if(j == 1 || j == 2 || j == 6){
                if (braille[j] == '0' && curr_servo_pos[j] != 1) {
                    board1.setPWM(j, 0, angleToPulse(30));
                    curr_servo_pos[j] = 1;
                    Serial.println('0');
                } else if (braille[j] == '1' && curr_servo_pos[j] != 0) {
                    board1.setPWM(j, 0, angleToPulse(0));
                    curr_servo_pos[j] = 0;
                    Serial.println('1');
                } 
              } else {
                if(braille[j] == '0' && curr_servo_pos[j] != 0){
                  board1.setPWM(j, 0, angleToPulse(0));
                  curr_servo_pos[j] = 0;
                  Serial.println('0');
                }
                else if (braille[j] == '1' && curr_servo_pos[j] != 1)
                {
                  board1.setPWM(j, 0, angleToPulse(30));
                  curr_servo_pos[j] = 1;
                  Serial.println('1');
                }
              }
          }
      } else {
          Serial.println(": Braille representation not found");
      }

/// next 6 servos
      if (braille1 != NULL) {
          Serial.println(braille1);
          for (int j = 6; j <= 11; j++) {
              // Process Braille representation and move servos accordingly
              if(j == 6 || j == 7 || j == 11){
                if (braille1[j-6] == '0' && curr_servo_pos[j] != 1) {
                    board1.setPWM(j, 0, angleToPulse(30));
                    curr_servo_pos[j] = 1;
                    Serial.println('0');
                } else if (braille1[j-6] == '1' && curr_servo_pos[j] != 0) {
                    board1.setPWM(j, 0, angleToPulse(0));
                    curr_servo_pos[j] = 0;
                    Serial.println('1');
                } 
              } else {
                if(braille1[j-6] == '0' && curr_servo_pos[j] != 0){
                  board1.setPWM(j, 0, angleToPulse(0));
                  curr_servo_pos[j] = 0;
                  Serial.println('0');
                }
                else if (braille1[j-6] == '1' && curr_servo_pos[j] != 1)
                {
                  board1.setPWM(j, 0, angleToPulse(30));
                  curr_servo_pos[j] = 1;
                  Serial.println('1');
                }
              }
          }
      } else {
          Serial.println(": Braille representation not found");
      }



    // Move to the next letter upon button 1 press
    if (digitalRead(BUTTON_PIN_1) == HIGH) {
        currentLetter = (currentLetter + 2) % input.length();
    }
    // Move to the previous letter upon button 2 press
    if (digitalRead(BUTTON_PIN_2) == HIGH) {
    currentLetter = (currentLetter - 2 + input.length()) % input.length();
    }




    Serial.println(input);
    Serial.println(currentLetter);

    delay(200); // Delay for stability

}
