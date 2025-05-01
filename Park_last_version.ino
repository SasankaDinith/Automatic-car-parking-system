#include <IRremote.hpp>  // Include the IRremote library

#define IR_TRANSMITTER_PIN 4  // IR Transmitter pin
#define TRIG_PIN 5            // Ultrasonic sensor trigger pin
#define ECHO_PIN 18           // Ultrasonic sensor echo pin
#define BUZZER_PIN 19 
#define GATE 16        // Buzzer pin

#define DISTANCE_THRESHOLD 12  // Distance to detect car (in cm)
#define TOTAL_SLOTS 4         // Total parking slots

bool parkingSlots[TOTAL_SLOTS] = {false, false, false, false}; // False means free slot

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GATE, OUTPUT);
    digitalWrite(GATE,LOW);
    
    IrSender.begin(IR_TRANSMITTER_PIN);  // Initialize IR transmitter

    Serial.println("Smart Parking System Ready!");
}

void loop() {
    float distance = getUltrasonicDistance();  // Measure distance

    if (distance > 0 && distance < DISTANCE_THRESHOLD) {  // If car detected
        Serial.println("Car detected at the entrance!");

        int allocatedSlot = findAvailableSlot(); // Find a free parking slot

        if (allocatedSlot != -1) { // If slot found
            Serial.print("Allocating Slot: ");
            Serial.println(allocatedSlot + 1);

            sendIRSignal(allocatedSlot + 1); // Send slot number via IR
            beepBuzzer(allocatedSlot + 1);  // Beep buzzer for confirmation
            parkingSlots[allocatedSlot] = true; // Mark slot as occupied
            digitalWrite(GATE,HIGH);
            delay(5000);
            digitalWrite(GATE,LOW);
            
        } else { // If no slots are available
            Serial.println("Parking Full! Cannot allocate a slot.");
            beepBuzzer(10); // Beep continuously for 10 seconds
        }

        delay(3000); // Wait to avoid multiple detections
    }
}

// Function to measure distance using ultrasonic sensor
float getUltrasonicDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.0343 / 2; // Convert time to distance in cm
    return distance;
}

// Function to find an available parking slot
int findAvailableSlot() {
    for (int i = 0; i < TOTAL_SLOTS; i++) {
        if (!parkingSlots[i]) {  // Check if slot is free
            return i;  // Return the available slot index
        }
    }
    return -1; // Return -1 if no slots are available
}

// Function to send IR signal
void sendIRSignal(int slot) {
    uint8_t signal;
    switch (slot) {
        case 1: signal = 0x10; break;
        case 2: signal = 0x20; break;
        case 3: signal = 0x30; break;
        case 4: signal = 0x40; break;
        default: return;
    }
    IrSender.sendNEC(0x00FF, signal, 0);
    Serial.println("IR Signal Sent!");
}

// Function to beep buzzer
void beepBuzzer(int times) {
    Serial.print("Buzzer Beeping ");
    Serial.print(times);
    Serial.println(" times");

    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
    }

    if (times == 10) {  // If parking is full, beep continuously for 10 seconds
        digitalWrite(BUZZER_PIN, HIGH);
        delay(10000);
        digitalWrite(BUZZER_PIN, LOW);
    }
}
