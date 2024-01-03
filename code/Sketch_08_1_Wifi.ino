#include <WiFi.h>
#include <ESPAsyncWebSrv.h>

const char *ssid_AP = "Braille_Device"; //Enter the router name
const char *password_AP = "12345678"; //Enter the router password

IPAddress local_IP(192, 168, 1, 100); // Set the IP address of ESP32-S3 itself
IPAddress gateway(192, 168, 1, 10);  // Set the gateway of ESP32-S3 itself
IPAddress subnet(255, 255, 255, 0);   // Set the subnet mask for ESP32-S3 itself

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  delay(2000);
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    html += "<h1>Hello ESP32-S3</h1>";
    html += "<p>Welcome to the ESP32-S3 web interface.</p>";
    html += "<p>You can add your content and forms here.</p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Start the server
  server.begin();
}

void loop() {
  // Your loop code, if any, can go here
}
//now look in the serial Monitor in the arduino ide, there should be a line that says Ready
//below that line it should say soft-AP IP address = 192.168.1.100
//now on your phone or laptop connect to the "Braille_device" wifi this process may take a while
//Now you connect to the ESP32-S3's IP address in a web browser,
//you will see the simple HTML page with the "Hello ESP32-S3" message.
