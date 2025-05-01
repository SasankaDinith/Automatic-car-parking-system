#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "LTE";
const char* password = "nokia2.4";

// ESP32 Web Server on port 80
WebServer server(80);

// Pin Definitions for analog inputs from Arduino (A0, A1, A2, A3)
const int SLOT1_PIN = 34;  // A0 pin on ESP32 (GPIO34)
const int SLOT2_PIN = 35;  // A1 pin on ESP32 (GPIO35)
const int SLOT3_PIN = 32;  // A2 pin on ESP32 (GPIO32)
const int SLOT4_PIN = 26;  // A3 pin on ESP32 (GPIO26)

// Pins to send booking signals to Arduino Uno
const int BOOK_SLOT1_PIN = 15; // GPIO14
const int BOOK_SLOT2_PIN = 2; // GPIO12
const int BOOK_SLOT3_PIN = 18; // GPIO13
const int BOOK_SLOT4_PIN = 19; // GPIO15

// Initialize the booking status (free or booked)
bool parkingSlotStatus[4] = {false, false, false, false};  // false = free, true = booked

// Function to read the analog value from the Arduino Uno pins
int readParkingSlot(int pin) {
  return analogRead(pin); // Read the analog value (0 to 4095)
}

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set GPIO pins for booking signals to OUTPUT
  pinMode(BOOK_SLOT1_PIN, OUTPUT);
  pinMode(BOOK_SLOT2_PIN, OUTPUT);
  pinMode(BOOK_SLOT3_PIN, OUTPUT);
  pinMode(BOOK_SLOT4_PIN, OUTPUT);

  // Serve the web page that shows parking slots
  server.on("/", HTTP_GET, [](){
    String html = "<html><head><style>"
                  "body { font-family: Arial, sans-serif; }"
                  "table { width: 50%; margin: 20px auto; border-collapse: collapse; }"
                  "th, td { padding: 10px; text-align: center; border: 1px solid #ddd; }"
                  "th { background-color: #4CAF50; color: white; }"
                  "tr:nth-child(even) { background-color: #f2f2f2; }"
                  ".occupied { background-color: red; color: white; }"
                  ".free { background-color: green; color: white; }"
                  "</style></head><body><h1>Smart Parking System</h1>";
    html += "<table border='1'><tr><th>Slot</th><th>Status</th><th>Action</th></tr>";

    // Read the analog values and display slot status (occupied or free)
    for (int i = 0; i < 4; i++) {
      String slotStatus = "Free";
      int analogValue = 0;

      switch (i) {
        case 0:
          analogValue = readParkingSlot(SLOT1_PIN);
          break;
        case 1:
          analogValue = readParkingSlot(SLOT2_PIN);
          break;
        case 2:
          analogValue = readParkingSlot(SLOT3_PIN);
          break;
        case 3:
          analogValue = readParkingSlot(SLOT4_PIN);
          break;
      }

      // Assuming a threshold value of 2048 for analog value to determine occupied or free slot
      if (analogValue > 2048) {
        slotStatus = "Occupied";
      }

      // If slot is not occupied, show a button to book the slot
      String buttonHtml = "<button onclick=\"window.location.href='/book?slot=" + String(i) + "'\"" + 
                          (slotStatus == "Occupied" ? " disabled" : "") + ">Book Slot</button>";

      // Add slot details to the HTML table
      html += "<tr><td>Slot " + String(i + 1) + "</td><td class='" + (slotStatus == "Occupied" ? "occupied" : "free") + "'>" + slotStatus + "</td><td>" + buttonHtml + "</td></tr>";
    }

    html += "</table></body></html>";
    server.send(200, "text/html", html);
  });

  // Handle booking the parking slot
  server.on("/book", HTTP_GET, [](){
    String slot = server.arg("slot");
    int slotNumber = slot.toInt();

    if (slotNumber >= 0 && slotNumber < 4 && !parkingSlotStatus[slotNumber]) {
      // Mark the parking slot as booked
      parkingSlotStatus[slotNumber] = true;

      // Send signal to the Arduino Uno to mark the slot as booked
      switch (slotNumber) {
        case 0:
          digitalWrite(BOOK_SLOT1_PIN, HIGH);  // Send signal to Arduino
          break;
        case 1:
          digitalWrite(BOOK_SLOT2_PIN, HIGH);  // Send signal to Arduino
          break;
        case 2:
          digitalWrite(BOOK_SLOT3_PIN, HIGH);  // Send signal to Arduino
          break;
        case 3:
          digitalWrite(BOOK_SLOT4_PIN, HIGH);  // Send signal to Arduino
          break;
      }

      // Redirect back to the parking slots page
      server.sendHeader("Location", "/");
      server.send(303);  // HTTP redirect
    } else {
      server.send(200, "text/html", "<html><body><h1>Slot already booked or invalid slot number</h1><a href='/'>Back to Parking</a></body></html>");
    }
  });

  // Start the server
  server.begin();
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();
}
