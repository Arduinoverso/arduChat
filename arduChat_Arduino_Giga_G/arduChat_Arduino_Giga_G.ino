#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "arduChat_secrets.h" 
#include <ArduinoJson.h>
#include <WiFiSSLClient.h>
#include <string>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
std::string apikey = APIKEY_OPENAI; // your Apikey from Open Ai
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(93,184,216,34);  // IP address for example.com (no DNS)
char server[] = "api.openai.com";       // host name for example.com (using DNS)
const int serverPort = 443;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display(); // Mostrar el logotipo de Adafruit (opcional)
  delay(2000); // Esperar 2 segundos
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("HEY, I am : )");
  display.setCursor(16, 16);
  display.println("arduChat");
  display.display();

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    delay(1000);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Accessing Wi-Fi...");
    display.display();
    Serial.println("Accessing Wi-Fi...");
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 3 seconds for connection:
    delay(3000);
  }
  delay(1000);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connected to Wi-Fi:");
  display.setCursor(16,16);
  display.println(ssid);
  display.display();
  Serial.println("Connected to Wi-Fi:");
  Serial.println(ssid);
  delay(1000);
 

  display.clearDisplay(); // limpia la pantalla
  display.setCursor(0, 0); // establece el cursor para escribir en la pantalla
  display.println("I have"); // escribe en la pantalla OLED
  display.setCursor(16, 16);
  display.println("super powers!!!");
  display.display(); // actualiza la pantalla
  delay(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Answer to Me");
  display.setCursor(16, 16);
  display.println("What is Arduino?");
  display.display();
  delay(3000);

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:

  WiFiSSLClient client;
  if (client.connect(server, serverPort)) {
    Serial.println(client.status());
    Serial.println("Connected to the Server: api.openai.com");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Connected to the Server");
    display.setCursor(16,16);
    display.println("api.openai.com");
    display.display();
    Serial.println("Connected to the Server");
    Serial.println("api.openai.com");
    delay(1000);

    // Send HTTP POST request and change the prompt to your own
    String requestBody = "{\"model\":\"text-davinci-003\",\"prompt\":\"what is arduino?\",\"max_tokens\":18}";
    String request = "POST /v1/completions HTTP/1.1\r\n";
    request += "Host: api.openai.com\r\n";
    request += "Authorization: Bearer " + String(apikey.c_str()) + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + String(requestBody.length()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += requestBody;

    client.print(request);

     // Read response
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }


    String response = client.readString();
    Serial.println("Response:");
    Serial.println(response);

     // Parse JSON
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, response);

    if (error) {
      Serial.println("Failed to parse JSON");
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("arduChat Failed to parse JSON:");
      display.display();
    } else {
      String completionText = doc["choices"][0]["text"];
      Serial.println("arduChat response: "+ completionText);
      // Change the size here to whatever you want (1-4) 2 is lower
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("arduChat Response" + completionText);
      display.display();
      }
  } else {
    Serial.println("OpenAi Connection failed");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("OpenAi Connection failed");
    display.display();
  }
}

void loop() {
  // Do nothing
}