/*
 * This tutorial is followed from Digi-Key's youtube channel
 * 
 * 23 February, 2021
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

// task
void testTask(void *parameter){
  // Debug exception reason: Stack canary watchpoint triggered (Test Task)
  while(true){
    int a = 1;
    int b[100];

    //
    for (int i = 0; i < 100 ; i++){
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    // print out remaining stack memory in words
    Serial.print("High water mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    // print number of free heap memory in bytes before  malloc
    Serial.print("Heap before malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    int *ptr = (int*)pvPortMalloc(1024 * sizeof(int));

    if(ptr == NULL){ // stack full check
      Serial.println("Not enough memory");
    }
    else{ // use memory if allocated
      for (int i = 0; i < 1024 ; i++){
        ptr[i] = 2;
      }
    }
    // print number of free heap memory in bytes after malloc
    Serial.print("Heap after malloc (bytes): ");
    Serial.println(xPortGetFreeHeapSize());
    // free up allocated memory
    vPortFree(ptr);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // give time to connect
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memory Demo---");

  // Start the task
  xTaskCreatePinnedToCore(testTask,
                          "Test Task",
                          1500, // RAM allocation
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  // delete setup and loop task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
