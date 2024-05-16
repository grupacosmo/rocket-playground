
<<<<<<< HEAD
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
=======






#include <Arduino.h>
#include <FreeRTOS.h>

#define BUTTON_PIN 0 // Change this to your button pin

QueueHandle_t xQueue;

void IRAM_ATTR button_isr() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t isr_msg = 1;
    xQueueSendFromISR(xQueue, &isr_msg, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

void TaskButton(void *pvParameters) {
    uint32_t rx_msg;
    for (;;) {
        if (xQueueReceive(xQueue, &rx_msg, portMAX_DELAY)) {
            // Button press detected, do something here
        }
    }
}

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_isr, FALLING);

    xQueue = xQueueCreate(10, sizeof(uint32_t));
    if (xQueue != NULL) {
        xTaskCreate(TaskButton, "TaskButton", 10000, NULL, 1, NULL);
    }
        xTaskCreate(
        TaskBlinkLED,          /* Task function. */
        "TaskBlinkLED",        /* String with name of task. */
        10000,                  /* Stack size in bytes. */
        NULL,                   /* Parameter passed as input of the task */
        1,                      /* Priority of the task. */
        NULL);                  /* Task handle. */
}

void loop() {
    // put your main code here, to run repeatedly:
}




>>>>>>> a6e1e0c21321a2b29161f177a0311dfdbd8c53a4
