#include <Arduino.h>

const unsigned long delayTime = 5000; // Delay time in milliseconds (5 seconds)
const int sensorPin = 2; // Example pin for sensor input

volatile bool timerFlag = false;
volatile bool sensorFlag = false;

// Function prototypes
void setupTimerInterrupt();
void setupSensorInterrupt();
void handleTimerInterrupt();
void handleSensorInterrupt();
void delayedFunction();

void setup() {
  Serial.begin(115200);
  setupTimerInterrupt();
  setupSensorInterrupt();
}

void loop() {
  // Your main loop code here
  if (timerFlag) {
    timerFlag = false;
    delayedFunction();
  }
  if (sensorFlag) {
    sensorFlag = false;
    // Handle sensor input
    Serial.println("Sensor triggered!");
  }
  // Other loop logic
}

void setupTimerInterrupt() {
  // Set up timer interrupt to trigger every delayTime milliseconds
  // Adjust the timer configuration based on your specific microcontroller
  timerAlarmWrite(timerBegin(TIMER0), delayTime * 1000, true);
  timerAttachInterrupt(timerBegin(TIMER0), &handleTimerInterrupt, true);
  timerAlarmEnable(timerBegin(TIMER0));
}

void setupSensorInterrupt() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), handleSensorInterrupt, CHANGE);
}

void handleTimerInterrupt() {
  // This function will be called when the timer interrupt triggers
  timerFlag = true;
}

void handleSensorInterrupt() {
  // This function will be called when the sensor interrupt triggers
  sensorFlag = true;
}

void delayedFunction() {
  // This function will be executed after delayTime milliseconds
  Serial.println("Delayed function executed!");
}
