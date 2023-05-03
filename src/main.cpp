// #include <Arduino.h>
// #include <SPI.h>

// #define latch_pin 4
// #define blank_pin 22
// #define data_pin 23
// #define clock_pin 18

// long int maxSPI = 80000000;

// byte SR1[] = {B10000000, B01000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000001};
// int count = 0;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);

//   SPI.setBitOrder(MSBFIRST);
//   SPI.setDataMode(SPI_MODE0);
//   SPI.setFrequency(16000000);

//   pinMode(latch_pin, OUTPUT);
//   pinMode(data_pin, OUTPUT);
//   pinMode(clock_pin, OUTPUT);
//   pinMode(blank_pin, OUTPUT);

//   SPI.begin();


  
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   for (int i = 0; i < 8; i++) {
//     int64_t time_start = esp_timer_get_time();
//     digitalWrite(blank_pin, HIGH);
//     // Serial.println("SPI START");
//     SPI.transfer(SR1[i]);
//     // Serial.println("SPI END");
//     digitalWrite(latch_pin, HIGH);
//     digitalWrite(latch_pin, LOW);
//     digitalWrite(blank_pin, LOW);
//     int64_t time_end = esp_timer_get_time();
//     Serial.println("total transfer time: ");
//     Serial.println(time_end - time_start);
//     delay(1000);
//   }

// }

