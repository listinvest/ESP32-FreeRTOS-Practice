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
// Limit core use to core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// string to print
const String msg = "This is a simple string to print.";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL; 

/////////////////////////////////////
// Tasks

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter){
  // count number of chars in the string
  int msg_len = msg.length();

  // print strin to terminal
  while(true){
    Serial.println();
    for ( int i = 0; i < msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task: print to Serial Terminal with higher priority
void startTask2(void *parameter){
  while(true){
    Serial.print('*');
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(300);

  // sma delay to let the serial port connect
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Task Demo---");

  // print priority of setup and loop
  Serial.print("Setup and loop task running on core: ");
  Serial.print(xPortGetCoreID());
  Serial.print(", with priority: ");
  Serial.println(uxTaskPriorityGet(NULL));

  // configure tasks
  // low priority task
  xTaskCreatePinnedToCore(startTask1,
                          "print string",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);
  // high priority task
  xTaskCreatePinnedToCore(startTask2,
                          "print '*'",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);
  
}

void loop() {
  // suspend the higher priority task for some intervals
  for ( int i = 0; i < 3; i++){
    vTaskSuspend(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  // delete the lower priority task
  if (task_1 != NULL){
    vTaskDelete(task_1);
    task_1 = NULL;
  }
  
}
