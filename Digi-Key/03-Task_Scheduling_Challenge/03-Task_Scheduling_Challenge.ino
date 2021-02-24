/*
 * This tutorial is followed from Digi-Key's youtube channel
 * 
 * 22 February, 2021
 * 
 * Settings:
 * Chip used: ESP32D0WDQ6 (revision 1)
 * Board: ESP32 Dev Module
 * Flash Frequency: 80MHz
 * Flash Mode: DIO
 * Flash Size: 4MB
 */
#include <stdlib.h>
 
// Limit core use to core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
const uint8_t buf_len = 20;

// pins 
const uint8_t led_pin = 2;

// global variables 
int led_delay = 500; // ms, initial delay

/////////////////////////////////////
// Tasks

// led blinker
void toggleLED( void *parameters){
  while(true){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
  }
}

// serial reader
void readSerial(void *parameters){
  char c;
  char buf[buf_len];
  uint8_t index = 0;

  // clear buffer
  memset(buf, 0, buf_len);

  while(true){
    // read chars from serial
    if(Serial.available()>0){
      c = Serial.read();

      if(c=='\n'){
//        vTaskSuspend(task_2); 
        led_delay = atoi(buf); //won't there be conflict if variables are shared between tasks?
//        vTaskResume(task_2);
//        Serial.printf("Updated LED delay to %d \r\n", led_delay); // printf causes "Guru Meditation Error: Core  1 panic'ed (LoadProhibited)."
        Serial.print("Updated LED delay to: ");
        Serial.println(led_delay);
        memset(buf, 0, buf_len);
        index = 0;
      }
      else{
        if(index < buf_len - 1){
          buf[index] = c;
          index++;
        }
      }
    }
  }
}
void setup() {
  // initiate output pin
  pinMode(led_pin, OUTPUT);
  // initiate serial
  Serial.begin(115200);
  // give time to serial to connect
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // give instruction to user
  Serial.println("Enter LED delay in milliseconds.");

  // start blink task
  xTaskCreatePinnedToCore(toggleLED, // function to execute
                          "LED blinker", // name
                          1024, // 
                          NULL, // parameter to pass
                          1, // priority
                          NULL, // task handle
                          app_cpu); // core to run
  
  // start serial input task
  xTaskCreatePinnedToCore(readSerial, // function to execute
                          "serial UI", // name
                          1024, // 
                          NULL, // parameter to pass
                          1, // priority
                          NULL, // task handle
                          app_cpu); // core to run
                                                  
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
