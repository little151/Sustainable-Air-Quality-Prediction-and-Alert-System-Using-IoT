#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"
#include <DHT.h>

// Sensor pin definitions
#define DHTPIN 4 // Use the pin number directly
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variables for sensor data
float humidity;
float temperature;
int airQualityPin = A0; // Renamed to avoid conflicts
String sheetHumidity = "";
String sheetTemperature = "";
String sheetAirQuality = "";

// Wi-Fi and server details
const char* wifiSSID = "choti";                // Renamed variable
const char* wifiPassword = "12345678900";     // Renamed variable

const char* serverHost = "script.google.com"; // Renamed variable
const char* scriptId = "AKfycbyHSo_RmnMVTUyLpRWLJlognuPC9iFAouGnHWRivlJUzcvsrvO7BNlbah9JLysdomlO"; // Renamed variable
const int httpsPort = 443;

// URLs for Google Sheets
String urlTemperature = String("/macros/s/") + scriptId + "/exec?value=Temperature";
String urlAirQuality = String("/macros/s/") + scriptId + "/exec?command=appendRow&sheet_name=air_quality_data_sheet&values=";

// Payloads for POST requests
String payloadBase = "{\"command\": \"appendRow\", \"sheet_name\": \"air_quality_data_sheet\", \"values\": ";
String payload = "";

HTTPSRedirect* httpsClient = nullptr;

void setup() {
  delay(1000);
  Serial.begin(115200);
  dht.begin();

  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(wifiSSID);

  WiFi.begin(wifiSSID, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  httpsClient = new HTTPSRedirect(httpsPort);
  httpsClient->setInsecure();
  httpsClient->setPrintResponseBody(true);
  httpsClient->setContentTypeHeader("application/json");

  Serial.print("Connecting to ");
  Serial.println(serverHost);

  bool connected = false;
  for (int i = 0; i < 5; i++) {
    if (httpsClient->connect(serverHost, httpsPort)) {
      connected = true;
      break;
    }
    Serial.println("Connection failed. Retrying...");
  }

  if (!connected) {
    Serial.print("Could not connect to server: ");
    Serial.println(serverHost);
    return;
  }

  Serial.println("\nFetching data from Google Sheet...");
  httpsClient->GET(urlTemperature, serverHost);

  Serial.println("\nFetching Google Calendar Data:");
  httpsClient->GET(urlAirQuality, serverHost);

  Serial.println("\nStarting to send sensor data to Google Sheet...");
  delete httpsClient;
  httpsClient = nullptr;
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  int airQualityValue = analogRead(airQualityPin);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Humidity: "); Serial.print(humidity); Serial.print(" % ");
  sheetHumidity = String(humidity) + " %";
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C ");
  sheetTemperature = String(temperature) + " °C";
  Serial.print("Air Quality: "); Serial.println(airQualityValue);
  sheetAirQuality = String(airQualityValue) + " PPM";

  payload = payloadBase + "\"" + sheetTemperature + "," + sheetHumidity + "," + sheetAirQuality + "\"}";

  if (!httpsClient) {
    httpsClient = new HTTPSRedirect(httpsPort);
    httpsClient->setInsecure();
    httpsClient->setPrintResponseBody(true);
    httpsClient->setContentTypeHeader("application/json");
  }

  if (httpsClient->connected()) {
    if (httpsClient->POST(urlAirQuality, serverHost, payload)) {
      Serial.println("Data sent successfully.");
    } else {
      Serial.println("Failed to send data.");
    }
  } else {
    if (httpsClient->connect(serverHost, httpsPort)) {
      httpsClient->POST(urlAirQuality, serverHost, payload);
      Serial.println("Reconnected and data sent.");
    } else {
      Serial.println("Reconnection failed.");
    }
  }

  delete httpsClient;
  httpsClient = nullptr;

  delay(30000); // Wait 30 seconds before the next loop
}
