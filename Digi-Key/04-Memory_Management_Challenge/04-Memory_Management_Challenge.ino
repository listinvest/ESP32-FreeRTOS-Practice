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

// settings
const uint8_t buf_len = 128;
// global variables
static volatile bool dataReceivedFlag = false;
static char *msg_pointer = NULL;

// task
void readSerial(void *parameter){
  char c;
  char char_buffer[buf_len];
  int index = 0;
  // clear buffer
  memset(char_buffer, 0, buf_len);
  while(true){
    // read data from serial
    if(Serial.available()>0){
      c = Serial.read();
 
      // store received msg if buffer is available
      if(index < buf_len - 1){
        char_buffer[index] = c;
        index++;
      }
      if(c=='\n'){ // end of line received
        char_buffer[index-1] = '\0';

        if(dataReceivedFlag == false){
          Serial.print("Heap before malloc (bytes): ");
          Serial.println(xPortGetFreeHeapSize());
          msg_pointer = (char*)pvPortMalloc(index * sizeof(char));
  
          // If malloc returns 0 (out of memory), throw an error and reset
          configASSERT(msg_pointer);
  
          // copy buffer to pointer buffer
          memcpy(msg_pointer, char_buffer, index);
  
          // raise flag
          dataReceivedFlag = true;
        }
        // clear buffer
        memset(char_buffer, 0, buf_len);
        index = 0;
      }
    }
  }
}
void writeSerial(void *paramtere){
  while(true){
    if(dataReceivedFlag == true){
      // print msg
      Serial.println(msg_pointer);
      // clear heap
      vPortFree(msg_pointer);
      //clear flag
      dataReceivedFlag = false;

      Serial.print("Heap freeing heap (bytes) : ");
      Serial.println(xPortGetFreeHeapSize());
    }
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
  xTaskCreatePinnedToCore(readSerial,
                          "read serial",
                          1500, // RAM allocation
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  // Start the task
  xTaskCreatePinnedToCore(writeSerial,
                          "write serial",
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
