/*
 * This tutorial is followed from Digi-Key's youtube channel
 * 
 * 16 February, 2021
 * 
 * Settings:
 * Chip used: ESP32D0WDQ6 (revision 1)
 * Board: ESP32 Dev Module
 * Flash Frequency: 80MHz
 * Flash Mode: DIO
 * Flash Size: 4MB
 */

// Limit core use to core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// pins 
static const int led_pin = 2;
static int blink_delay = 1000;

// task: blink LED
void toggleLED(void *parameter){
  while(true){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(blink_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(blink_delay / portTICK_PERIOD_MS);
  }
}



void setup() {
  // put your setup code here, to run once:

  // pin configuration
  pinMode(led_pin, OUTPUT);

  // tack config
  xTaskCreatePinnedToCore( // create task
    toggleLED,            // function to be called
    "Toggle LED",         // Human readable name for the task.
    1024,                 // Stack size ( bytes in ESP32, words in FreeRTOS )
    NULL,                 // Parameter to pass to function
    1,                    // Task priority (0 to configMAX_PRIORITY - 1)
    NULL,                 // Task handle
    app_cpu);             // core to run on (ESP32 only, not available in vanilla FreeRTOS
}

void loop() {
  // put your main code here, to run repeatedly:

}
