#define IR_SENSOR_RIGHT 11
#define IR_SENSOR_LEFT 12
#define MOTOR_SPEED 200
#define MAX_SPEED 250
// IR recieve
#include <IRremote.hpp>  // Include IRremote library for IR communication
#define IR_RECEIVER_PIN 3  // IR receiver pin
#define BUZZER_PIN 4        // Buzzer pin
int receivedSignal=0;
int option = 0;
int SLOT = 0;

//Right motor
int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;

void setup()
{
  //IR recieve
    Serial.begin(9600);  // Start serial communication for debugging
    pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer pin as output
    
    IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);  // Initialize IR receiver
    
    Serial.println("Self-driving car is ready to receive IR signals.");
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequecny as 7812.5 hz.
  TCCR0B = TCCR0B & B11111000 | B00000010 ;
  
  // put your setup code here, to run once:
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  rotateMotor(0,0);   
}


void loop()
{

  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
      rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
      rotateMotor(MOTOR_SPEED, -MOTOR_SPEED); 
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    rotateMotor(0, 0);
    Serial.print(option);
    Serial.print(SLOT);
    if(option==0 && SLOT==0 ){
      while(true){
        if (IrReceiver.decode()) {
        // Get the received signal value
        int receivedSignal = identifySignal(IrReceiver.decodedIRData.command);
        ///delay(5000);
        
        // If a valid signal is received, beep the buzzer
        if (receivedSignal > 0) {
            Serial.print("Received Signal: Slot ");
            Serial.println(receivedSignal);
            SLOT=receivedSignal;
            delay(10000);
            beepBuzzer(receivedSignal);// Beep buzzer the number of times equal to slot number
            option++;
            Serial.println(option);
            delay(2000);
            rotateMotor(MAX_SPEED, MAX_SPEED);
            delay(1500);
            rotateMotor(0, 0);
            Serial.println("Entered to the park(BEAKED)");
            break;
            
            
        } else {
            Serial.println("Unknown signal received.");
        }

        IrReceiver.resume();  // Prepare for the next IR signal
    }
      }
      delay(3000);
    }
    else if(SLOT==1 && option==1){
      Serial.println("reached receivedSignal(SLOT)==1 && option==1");
      delay(500);
      rotateMotor(0, 0);
      Serial.println("Car has parked in slot one !");
       while(true){  
        rotateMotor(-MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,MAX_SPEED);
          delay(1500);
          rotateMotor(0,0);
          break;
      }
      while(true){  
          Serial.println("Car has parked in slot one !");
          delay(3000);
      }
    }
    else if(SLOT==2 && option==1 ){
      Serial.println("reached receivedSignal(SLOT)==2 && option==1");
      delay(500);
      rotateMotor(0, 0);
      Serial.println("Car has parked in slot TWO !");
       while(true){  
        rotateMotor(-MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(-MAX_SPEED,MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,MAX_SPEED);
          delay(1500);
          rotateMotor(0,0);
          break;
      }
      while(true){  
          Serial.println("Car has parked in slot TWO !");
          delay(3000);
      }
    }
    else if(SLOT==3 && option==1 ){
      Serial.println("receivedSignal==3 && option==1");
      option++;
      delay(500);
      rotateMotor(MAX_SPEED, MAX_SPEED);
         delay(1500);
         rotateMotor(0, 0);
    }
    else if(SLOT==4 && option==1){
      Serial.println("receivedSignal==3 && option==1");
      option++;
      delay(500);
      rotateMotor(MAX_SPEED, MAX_SPEED);
         delay(1500);
         rotateMotor(0, 0);
      }
    
     else if(SLOT==3 && option==2 ){
      Serial.println("reached receivedSignal(SLOT)==3 && option==2");
      delay(500);
      rotateMotor(0, 0);
      Serial.println("Car has parked in slot three !");
       while(true){  
        rotateMotor(-MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,MAX_SPEED);
          delay(1500);
          rotateMotor(0,0);
          break;
      }
      while(true){  
          Serial.println("Car has parked in slot three !");
          delay(3000);
      }
    }
     else if(SLOT==4 && option==2 ){
      Serial.println("reached receivedSignal(SLOT)==4 && option==2");
      delay(500);
      rotateMotor(0, 0);
      Serial.println("Car has parked in slot four !");
       while(true){  
        rotateMotor(-MAX_SPEED,-MAX_SPEED);
          delay(2000);
          rotateMotor(-MAX_SPEED,MAX_SPEED);
          delay(2000);
          rotateMotor(MAX_SPEED,MAX_SPEED);
          delay(1500);
          rotateMotor(0,0);
          break;
      }
      while(true){  
          Serial.println("Car has parked in slot four !");
          delay(3000);
      }
    }
    
  }
}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else 
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}

// Function to identify the received IR signal and return corresponding slot number
int identifySignal(uint8_t value) {
    switch (value) {
        case 0x10: return 1;  // Slot 1
        case 0x20: return 2;  // Slot 2
        case 0x30: return 3;  // Slot 3
        case 0x40: return 4;  // Slot 4
        default: return 0;     // Unknown signal
    }
}

// Function to beep the buzzer the number of times equal to the slot number
void beepBuzzer(int times) {
    Serial.print("Buzzer Beeping ");
    Serial.print(times);
    Serial.println(" times");

    for (int i = 0; i < times; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);  // Beep duration
        digitalWrite(BUZZER_PIN, LOW);
        delay(500);  // Pause between beeps
    }
}
