#include <WiFi.h>
#include "DHT.h"

#define DHTPIN 5          // DHT11 data pin connected to GPIO5
#define DHTTYPE DHT11     // DHT 11 sensor
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Weather_Station";
const char* password = "12345678";

WiFiServer server(80);

long mapVal(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.softAP(ssid, password);
  Serial.println("ESP32 AP started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;
  while (client.connected() && !client.available()) delay(1);
  client.readStringUntil('\r'); client.flush();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Determine colors
  String tempColor;
  if (temperature >= 12 && temperature <= 19) tempColor = "#B3E5FC";
  else if (temperature <= 27) tempColor = "#81C784";
  else if (temperature <= 34) tempColor = "#FFB74D";
  else if (temperature <= 45) tempColor = "#FF8A65";
  else tempColor = "#EF5350";
  String humColor;
  if (humidity < 30) humColor = "#B3E5FC";
  else if (humidity <= 60) humColor = "#81C784";
  else if (humidity <= 80) humColor = "#FFB74D";
  else humColor = "#EF5350";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println();

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<style>";
  html += "body{margin:0;padding:20px;background:#121212;color:#fff;font-family:sans-serif;text-align:center;}";
  html += "h1{color:#03dac6;margin-bottom:20px;}";
  html += ".legend-container{display:flex;justify-content:center;flex-wrap:wrap;margin-bottom:20px;}";
  html += ".legend-item{display:flex;align-items:center;margin:5px 15px;font-size:1rem;}";
  html += ".legend-item span{display:inline-block;width:14px;height:14px;margin-right:8px;border-radius:2px;}";
  html += ".section{margin:30px 0;}";
  html += ".circle{position:relative;width:180px;height:180px;border-radius:50%;background:#2c2c2c;margin:10px auto;}";
  html += ".circle:before{content:'';position:absolute;top:10px;left:10px;width:160px;height:160px;background:#121212;border-radius:50%;z-index:2;}";
  html += ".value{position:absolute;top:50%;width:100%;transform:translateY(-50%);z-index:3;font-size:1.8rem;}";
  html += ".label{position:absolute;bottom:15px;width:100%;z-index:3;color:#aaa;font-size:1rem;}";
  html += "</style></head><body>";

  html += "<h1>Mini Weather Station</h1>";

  // Temperature legend + chart
  html += "<div class='section'><h2>Temperature Ranges</h2><div class='legend-container'>";
  html += "<div class='legend-item'><span style='background:#B3E5FC;'></span>12–19°C (Cool)</div>";
  html += "<div class='legend-item'><span style='background:#81C784;'></span>20–27°C (Comfort)</div>";
  html += "<div class='legend-item'><span style='background:#FFB74D;'></span>28–34°C (Warm)</div>";
  html += "<div class='legend-item'><span style='background:#FF8A65;'></span>35–45°C (Hot)</div>";
  html += "<div class='legend-item'><span style='background:#EF5350;'></span>45°C+ (Very Hot)</div>";
  html += "</div>";
  html += "<div class='circle' style='background:conic-gradient(" + tempColor + " 0 " + String(mapVal((long)temperature,0,50,0,360)) + "deg,#2c2c2c 0 360deg);'>";
  html += "<div class='value'>" + String(temperature,1) + " °C</div><div class='label'>Temperature</div></div></div>";

  // Humidity legend + chart
  html += "<div class='section'><h2>Humidity Ranges</h2><div class='legend-container'>";
  html += "<div class='legend-item'><span style='background:#B3E5FC;'></span><30% (Dry)</div>";
  html += "<div class='legend-item'><span style='background:#81C784;'></span>30–60% (Normal)</div>";
  html += "<div class='legend-item'><span style='background:#FFB74D;'></span>61–80% (Humid)</div>";
  html += "<div class='legend-item'><span style='background:#EF5350;'></span>80%+ (Very Humid)</div>";
  html += "</div>";
  html += "<div class='circle' style='background:conic-gradient(" + humColor + " 0 " + String(mapVal((long)humidity,0,100,0,360)) + "deg,#2c2c2c 0 360deg);'>";
  html += "<div class='value'>" + String(humidity,1) + " %</div><div class='label'>Humidity</div></div></div>";

  html += "</body></html>";

  client.println(html);
  client.stop();
}
