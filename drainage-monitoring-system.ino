#include <WiFi.h>
#include <WebServer.h>

// ---------------- WIFI CONFIG ----------------
const char* ssid = "ESP32TEST"; // Change this
const char* password = "12345678"; // Change this

WebServer server(80);

#define FLOW_PIN 26
#define TRIG_PIN 5
#define ECHO_PIN 18
#define GAS_AO_PIN 34   // Analog gas sensor pin
#define GAS_DO_PIN 25   // Digital gas alert pin


// ---------------- SIMULATION MODE ----------------
int mode = 1; // 1=🌊 Normal, 2=⚠️ Blockage+Gas, 3=🔍 Blockage Only
bool workerInside = false;  

// ---------------- VARIABLES ----------------
float flowRate = 0.0;
int waterLevel = 0;
int gasLevel = 0;
bool blockageDetected = false;
bool gasDetected = false;

// ---------------- GENERATE SIMULATED VALUES ----------------
void simulateValues() {
  if (mode == 1) {
    flowRate = 2.5;  
    waterLevel = 30; 
    gasLevel = 0;    
    blockageDetected = false;
    gasDetected = false;
  }
  else if (mode == 2) {
    flowRate = random(0, 2);  
    waterLevel = random(80, 100); 
    gasLevel = random(300, 800);  
    blockageDetected = true;
    gasDetected = true;
  }
  else if (mode == 3) {
    flowRate = random(0, 2);  
    waterLevel = random(80, 100); 
    gasLevel = 0;  
    blockageDetected = true;
    gasDetected = false;
  }
}

// ---------------- DASHBOARD HTML ----------------
void handleRoot() {
  simulateValues(); // Update values

  String html = "<!DOCTYPE html><html><head><title>Smart Drainage Dashboard</title>";
  html += "<meta http-equiv='refresh' content='3'>";
  html += "<meta charset='UTF-8'>";
  html += "<style>body{font-family:Arial;text-align:center;} ";
  html += ".alert{color:white;padding:10px;margin:10px;border-radius:8px;} ";
  html += ".blockage{background:red;} .gas{background:orange;} ";
  html += ".ok{background:green;} button{padding:10px 20px;margin:10px;font-size:22px;}</style></head><body>";

  html += "<h1>Smart Drainage Monitoring</h1>";

  // Worker status toggle (kept as text button)
  html += "<form action='/toggleWorker' method='POST'>";
  html += "<p><b>Worker Status:</b> " + String(workerInside ? "Inside" : "Outside") + "</p>";
  html += "<button type='submit'>Toggle Worker Status</button></form>";

  // Mode selection (emoji buttons 🌊 ⚠️ 🔍)
  html += "<form action='/setMode' method='POST'>";
  html += "<p style='font-size:30px;'>" + String(mode == 1 ? "🌊" : (mode == 2 ? "⚠️" : "🔍")) + "</p>";
  html += "<button name='mode' value='1'>🌊</button>";
  html += "<button name='mode' value='2'>⚠️</button>";
  html += "<button name='mode' value='3'>🔍</button>";
  html += "</form>";

  // Values
  html += "<h2>📊 Live Readings</h2>";
  html += "<p><b>💧 Water Flow:</b> " + String(flowRate, 1) + " L/min</p>";
  html += "<p><b>📏 Water Level:</b> " + String(waterLevel) + " cm</p>";

  if (gasDetected) {
    html += "<p><b>☣️ Gas Level:</b> " + String(gasLevel) + "</p>";
  } else {
    html += "<p><b>☣️ Gas Level:</b> -</p>";
  }

  // Alerts
  html += "<h2>🚨 Alerts</h2>";

  if (blockageDetected) {
    html += "<div class='alert blockage'>⚠️ Blockage Detected!</div>";
  } else {
    html += "<div class='alert ok'>✅ No Blockage</div>";
  }

  if (gasDetected) {
    html += "<div class='alert gas'>☠️ Harmful Gas Detected!</div>";
  } else {
    html += "<div class='alert ok'>✅ No Harmful Gas</div>";
  }

  if (workerInside && gasDetected) {
    html += "<div class='alert gas'>🚨 Worker Inside + Gas Present → Immediate Danger!</div>";
  }

  html += "</body></html>";
  server.send(200, "text/html; charset=UTF-8", html); // ✅ Fix for emoji
}

// ---------------- HANDLERS ----------------
void handleToggleWorker() {
  workerInside = !workerInside;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleSetMode() {
  if (server.hasArg("mode")) {
    mode = server.arg("mode").toInt();
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggleWorker", HTTP_POST, handleToggleWorker);
  server.on("/setMode", HTTP_POST, handleSetMode);
  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();
}
