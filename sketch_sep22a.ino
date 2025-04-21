
#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"


// DHT setup
#define MOTOR_IN1 14
#define MOTOR_IN2 12
#define MOTOR_EN  13
#define DHTPIN 25
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// WiFi credentials
const char* ssid = "ESP32_AP";
const char* password = "12345678";


WebServer server(80);


// CSV log (in RAM)
String csvData = "Timestamp,Temperature,Humidity\n";


// Get current time in milliseconds (since boot)
String getTimestamp() {
  unsigned long ms = millis();
  unsigned long seconds = ms / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  return String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
}


void controlMotor(bool turnOn) {
  if (turnOn) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_EN, HIGH);  // Enable motor
  } else {
    digitalWrite(MOTOR_EN, LOW);   // Disable motor
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
  }
}


void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>DHT11 Monitor</title>
      <style>
        body { font-family: Arial; text-align: center; padding: 20px; }
        .card { background: #f4f4f4; padding: 20px; border-radius: 10px; display: inline-block; box-shadow: 2px 2px 12px #aaa; }
        canvas { max-width: 600px; margin-top: 20px; }
        button { padding: 10px 20px; font-size: 16px; margin-top: 20px; }
      </style>
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    </head>
    <body>
      <h2>DHT11 Sensor Data</h2>
      <div class="card">
        <p><strong>Temperature:</strong> <span id="temp">--</span> °C</p>
        <p><strong>Humidity:</strong> <span id="hum">--</span> %</p>
        <p style="font-size: 18px; color: red;" id="msg">Status: --</p>
      </div>
      <canvas id="tempChart"></canvas>
      <canvas id="humChart"></canvas>
      <br/>
      <button onclick="downloadCSV()">Download CSV</button>


      <script>
        const tempData = {
          labels: [],
          datasets: [{
            label: 'Temperature (°C)',
            backgroundColor: 'rgba(255, 99, 132, 0.2)',
            borderColor: 'rgba(255, 99, 132, 1)',
            data: [],
            tension: 0.4
          }]
        };


        const humData = {
          labels: [],
          datasets: [{
            label: 'Humidity (%)',
            backgroundColor: 'rgba(54, 162, 235, 0.2)',
            borderColor: 'rgba(54, 162, 235, 1)',
            data: [],
            tension: 0.4
          }]
        };


        const tempChart = new Chart(document.getElementById('tempChart'), {
          type: 'line',
          data: tempData,
          options: { responsive: true }
        });


        const humChart = new Chart(document.getElementById('humChart'), {
          type: 'line',
          data: humData,
          options: { responsive: true }
        });


        function getData() {
          fetch("/data")
            .then(response => response.json())
            .then(data => {
              const now = new Date();
              const time = now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds();


              document.getElementById("temp").innerText = data.temperature;
              document.getElementById("hum").innerText = data.humidity;
              document.getElementById("msg").innerText = "Status: " + data.message;

              if (tempData.labels.length > 20) {
                tempData.labels.shift();
                tempData.datasets[0].data.shift();
                humData.labels.shift();
                humData.datasets[0].data.shift();
              }


              tempData.labels.push(time);
              tempData.datasets[0].data.push(data.temperature);
              humData.labels.push(time);
              humData.datasets[0].data.push(data.humidity);


              tempChart.update();
              humChart.update();
            });
        }


        setInterval(getData, 5000);
        getData();


        function downloadCSV() {
          window.open('/download');
        }
      </script>
    </body>
    </html>
  )rawliteral";


  server.send(200, "text/html", html);
}


// Return latest sensor data and update CSV log
void handleData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();


  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "application/json", "{\"error\": \"Failed to read from DHT sensor\"}");
    return;
  }


  // Control motor based on temperature
  if (temperature > 30.0) {
    controlMotor(true);
  } else {
    controlMotor(false);
  }


  // Log to CSV
  String line = getTimestamp() + "," + String(temperature) + "," + String(humidity) + "\n";
  csvData += line;


  // Return JSON
  String message = (temperature > 30.0) ? "Anomaly Detected" : "Normal";
  String json = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"message\":\"" + message + "\"}";
  server.send(200, "application/json", json);
}


// Serve the CSV data as a downloadable file
void handleDownload() {
  server.sendHeader("Content-Type", "text/csv");
  server.sendHeader("Content-Disposition", "attachment; filename=data.csv");
  server.send(200, "text/csv", csvData);
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);
  digitalWrite(MOTOR_EN, LOW);  // Start motor disabled
  WiFi.softAP(ssid, password);
  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());


  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/download", handleDownload);


  server.begin();
  Serial.println("Web server started");
}


void loop() {
  server.handleClient();
}