#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <esp_spiffs.h>

const char *ssid_Router = "Braille_Board"; //Enter the router name
const char *password_Router = "12345678"; //Enter the router password

IPAddress local_IP(192, 168, 1, 100); // Set the IP address of ESP32-S3 itself
IPAddress gateway(192, 168, 1, 10);  // Set the gateway of ESP32-S3 itself
IPAddress subnet(255, 255, 255, 0);   // Set the subnet mask for ESP32-S3 itself

AsyncWebServer server(80);

const char *htmlContent = R"html(

<html>

<head>
  <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
  <title>English Braille Translator</title>
  <script src="translate.js"></script>
  <link rel="stylesheet" href="styles.css">
</head>

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
          <input type="text" id="textField" placeholder="Enter text" onkeydown="TranslateOnKeyDown(event)">
          <button onclick="TranslateOnClick()">Submit</button>
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


const char *translate_js = R"javascript(
function isUpperCase(char) {
    let charCode = char.charCodeAt(0);
    return charCode >= 65 && charCode <= 90;
}

function isLowerCase(char) {
    let charCode = char.charCodeAt(0);
    return charCode >= 97 && charCode <= 122;
}

function Translate() {

    const textField = document.getElementById("textField");
    const text = textField.value;
    const translationField = document.getElementById("translationField");

    translationField.innerHTML = ""; // clear translationField

    // insert a web element for each character in the text variable
    for (var i = 0; i < text.length; i++) {

        let char = text[i];

        let img_source;

        if (isUpperCase(char)) { img_source = "brailleImages/upperChars/" + char + ".png"; }
        else if (isLowerCase(char)) { img_source = "brailleImages/lowerChars/" + char + ".png"; }
        else {

            switch (char) {
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

            img_source = "brailleImages/specialChars/" + char + ".png";
        }

        const braille_img = document.createElement("img");
        braille_img.src = img_source;
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

function TranslateOnClick() {
    Translate();
}

function TranslateOnKeyDown(event) {
    if (event.key === "Enter") {

        event.preventDefault(); // prevent default form submission

        Translate();
    }
}
)javascript";



void setup() {
  Serial.begin(115200);
  delay(20000);
  Serial.println("Setting soft-AP configuration ... ");
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);

  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.println("Setting soft-AP ... ");

  boolean result = WiFi.softAP(ssid_AP, password_AP);
  if (result) {
    Serial.println("Ready");
    Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
    Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
  } else {
    Serial.println("Failed!");
  }
  Serial.println("Setup End");

  // Serve the root page
// Serve the root page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", htmlContent); // Serve the HTML content
  });

  // Serve the JavaScript file
  server.on("/translate.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/javascript", translate_js);
  });

  // Start the server
  server.begin();

}

void loop() {

  // Your loop code, if any, can go here
}
