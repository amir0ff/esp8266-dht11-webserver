// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "DHT.h"

// Replace with your network details
const ssid = "YOUR_NETWORK_SSID";
const password = "YOUR_NETWORK_PASSWORD";

// Web Server on port 80
WiFiServer server(80);

// Initialize DHT sensor (pin_no, sensor type).
DHT dht(D5, DHT11);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();

  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);

  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
          // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
          float hum = dht.readHumidity();
          // Read temperature as Celsius (the default)
          float temp = dht.readTemperature();
          // Read temperature as Fahrenheit (isFahrenheit = true)
          float f = dht.readTemperature();
          // Check if any reads failed and exit early (to try again).
          if (isnan(hum) || isnan(temp) || isnan(f)) {
            Serial.println("Failed to read from DHT sensor!");
            strcpy(celsiusTemp, "Failed");
            strcpy(fahrenheitTemp, "Failed");
            strcpy(humidityTemp, "Failed");
          }
          else {
            // Computes temperature values in Celsius + Fahrenheit and Humidity
            float hic = dht.computeHeatIndex(temp, hum, false);
            dtostrf(hic, 6, 2, celsiusTemp);
            float hif = dht.computeHeatIndex(f, hum);
            dtostrf(hif, 6, 2, fahrenheitTemp);
            dtostrf(hum, 6, 2, humidityTemp);
            // You can delete the following Serial.print's, it's just for debugging purposes
            Serial.print("Humidity: ");
            Serial.print(hum);
            Serial.print(" %\t Temperature: ");
            Serial.print(temp);
            Serial.print(" *C ");
            Serial.print(f);
            Serial.print(" *F\t Heat index: ");
            Serial.print(hic);
            Serial.print(" *C ");
            Serial.print(hif);
            Serial.print(" *F");
            Serial.print("Humidity: ");
            Serial.print(hum);
            Serial.print(" %\t Temperature: ");
            Serial.print(temp);
            Serial.print(" *C ");
            Serial.print(f);
            Serial.print(" *F\t Heat index: ");
            Serial.print(hic);
            Serial.print(" *C ");
            Serial.print(hif);
            Serial.println(" *F");
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          // your actual web page that displays temperature and humidity
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head><style>body {background:#151515; color:white;font-family:sans-serif ;font-size: 14px;}span {display: block; line-height: 36px;}</style><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Thermostat</title><link href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAABILAAASCwAAAAAAAAAAAAAAAAAAAAAAAAAAABEAAACQAAAA0QAAAMEAAABUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAYAAADAAAAAYf8LAFG6CABFAAAArQAAAGEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABVAAAAjv8OAJT/BwD//woA9/8NAEIAAADUAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAeQAAAGT/DQDV/wgA//8IAP//DQBvAAAA0wAAAAkAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEYAAACn/w4AXP8LAP3/CwDZcgQANwAAAM4AAAAAAAAAAAAAAAAAAAAA/wAAAP8AAiX/AAIxAAAAAAAAAAAAAAACAAAAsgABAGX/DQD1/wwAjwAAAMgAAABJAAAAAAAAAAAAAAAA/wACD/8AAoj/AQL2/wACkgAAAAAAAAAAAAAAAAAAAF4AAACH/wwA9f8MAI8AAADcAAAAKAAAAMsAAADMZwABW/8AAuX/DQH//xQA//8AApIAAAAAAAAAAAAAAAAAAABeAAAAh/8NAPX/DQCPAAAA2wAAAAoAAABZAAAAWgwAABX/AABm/wIA6f8KAP//AACSAAAAAAAAAAAAAAAAAAAAXgAAAIf/DQD1/w0AjwAAANsAAAATAAAAfgAAAIAAAAAgAAAAAP8AABD/AACO/wAAdQAAAAAAAAAAAAAAAAAAAF4AAQCH/w0A9f8NAI8AAADbAAAAHgAAAKYAAACnAAAALwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABeAAEAh/8OAPX/DgCPAAAA2wAAAAMAAAAxAAAAMwAAAAcAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAXgAAAH7/EACF/xEAPAAAANsAAAAvAAAA7wAAAPAAAABIAAAAAAAA/wAAAP8tAAD/NwAAAAAAAAAAAAAAAAAAAF4AAAB+AAAAAAAAAAAAAADbAAAAAAAAAAUAAAAGAAAAAAAA/xQAAP+TAAD/+QAA/5IAAAAAAAAAAAAAAAAAAABcAAAAfwAAAAAAAAAAAAAA2wAAAC0AAADiAAAA4wAAcGQAAP/qAAD//wAA//8AAP+SAAAAAAAAAAAAAAAAAAAALwAAAL0AAAATAAAAQwAAALgAAAAEAAAAQAAAAEIAACAMAAD/WwAA/+IAAP//AAD/kgAAAAAAAAAAAAAAAAAAAAAAAABfAAAAzAAAALEAAAAeAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP8NAAD/ggAA/28AAAAAwP8AAID/AACAfwAAgH8AAIBxAACA4QAAwAEAAMABAADAEQAAwB8AAMAfAADAEQAAzQEAAMgBAADAAQAAwfEAAA==\" rel=\"icon\" type=\"image/x-icon\"><meta http-equiv=\"refresh\" content=\"60\"></head>");
          client.println("<body><span>Temperature:  ");
          client.println(celsiusTemp);
          client.println("&deg;C</span>");
          client.println("<span>Humidity:  ");
          client.println(humidityTemp);
          client.println("%</span>");
          client.println("</body></html>");
          break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
