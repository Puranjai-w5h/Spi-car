#include <WiFi.h>
#include <WebServer.h>

// ====== Motor Pins ======
#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6
#define PWM_LEFT 5
#define PWM_RIGHT 4

// ====== Wi-Fi AP Setup ======
const char* ssid = "SPI_Car";
const char* password = "12345678";

WebServer server(80);

// ====== Setup ======
void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(PWM_LEFT, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(PWM_RIGHT, OUTPUT);
  stopMotors();

  // Start WiFi in Access Point mode
  WiFi.softAP(ssid, password);
  Serial.println("WiFi started");
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());

  // Web button routes
  server.on("/", handleRoot);
  server.on("/forward", []() { moveForward(150); server.send(200, "text/html", controlPage()); });
  server.on("/backward", []() { moveBackward(150); server.send(200, "text/html", controlPage()); });
  server.on("/left", []() { turnLeft(150); server.send(200, "text/html", controlPage()); });
  server.on("/right", []() { turnRight(150); server.send(200, "text/html", controlPage()); });
  server.on("/stop", []() { stopMotors(); server.send(200, "text/html", controlPage()); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// ====== Web Page ======
String controlPage() {
  return R"rawliteral(
  <html><head><title>SPI Car Control</title></head><body style='text-align:center; font-family:sans-serif;'>
  <h2>🚗 SPI Car Controller</h2>
  <a href="/forward"><button style='width:100px;height:50px;'>Forward</button></a><br><br>
  <a href="/left"><button style='width:100px;height:50px;'>Left</button></a>
  <a href="/stop"><button style='width:100px;height:50px;'>Stop</button></a>
  <a href="/right"><button style='width:100px;height:50px;'>Right</button></a><br><br>
  <a href="/backward"><button style='width:100px;height:50px;'>Backward</button></a>
  </body></html>
  )rawliteral";
}

void handleRoot() {
  server.send(200, "text/html", controlPage());
}

// ====== Motor Functions ======
void moveForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(PWM_LEFT, speed);
  analogWrite(PWM_RIGHT, speed);
}

void moveBackward(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(PWM_LEFT, speed);
  analogWrite(PWM_RIGHT, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(PWM_LEFT, speed);
  analogWrite(PWM_RIGHT, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(PWM_LEFT, speed);
  analogWrite(PWM_RIGHT, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(PWM_LEFT, 0);
  analogWrite(PWM_RIGHT, 0);
}
