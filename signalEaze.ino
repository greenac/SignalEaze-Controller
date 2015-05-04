/*

Copyright (c) 2012-2014 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/*
 *    HelloWorld
 *
 *    HelloWorld sketch, work with the Chat iOS/Android App.
 *    It will send "Hello World" string to the App every 1 sec.
 *
 */

//"RBL_nRF8001.h/spi.h/boards.h" is needed in every new project
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

void setup()
{
  //  
  // For BLE Shield and Blend:
  //   Default pins set to 9 and 8 for REQN and RDYN
  //   Set your REQN and RDYN here before ble_begin() if you need
  //
  // For Blend Micro:
  //   Default pins set to 6 and 7 for REQN and RDYN
  //   So, no need to set for Blend Micro.
  //
  //ble_set_pins(3, 2);
  
  // Set your BLE advertising name here, max. length 10
  ble_set_name("SigEazeBLE");
  
  // Init. and start BLE library.
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);
  randomSeed(analogRead(0));
}

const int FLOAT_PRECISION = 3;
uint16_t randValue = 700;
int pin = 2;
int counter = 0;
uint16_t readValue = 0;

void loop()
{
  if (ble_connected()) {
    readValue = analogRead(pin);
    write(readValue);
    for (int i=0; i < 2; i++) {
      write(random_value(readValue, 50));
    }
  } else {
    Serial.println("not connected to bluetooth");
  }

  ble_do_events();
  
  if (ble_available()) {
    while (ble_available()) {
      Serial.write(ble_read());
      Serial.println("In ble available");
    }
    
    Serial.println();
  }
 
  delay(100);
  Serial.println("end of loop\n");
}

int length_of_float(const float value)
{
  float temp = value;
  int counter = 0;

  while (temp > 1.0) {
    temp = temp/10;
    counter++;
  }

  counter += FLOAT_PRECISION + 1;
  return counter;
}

int length_of_int(const int value)
{
  int counter = 0;
  int temp = value;
  while (temp > 0) {
    temp/10;
    counter++;
  }

  return temp;
}
void integer_to_char_array(char *char_array, const int value)
{
  int tempValue = value;
  byte digit = tempValue%10;
  int counter = 0;
  while (digit > 0) {
    Serial.print("digit");
    Serial.print(digit);
    Serial.print("\n");
    tempValue = tempValue/10;
    char_array[counter] = digit;
    digit = tempValue%10;
    counter++;
  }
}

uint16_t random_value(uint16_t currentValue, uint16_t offset)
{
  uint8_t rand_num = random(0, offset);
  uint16_t random_val = currentValue;
  if (random(0,2) == 1) {
    random_val += rand_num;
    if (random_val >= 1024) {
      while (random_val >= 1024) {
        random_val = currentValue + random(0, offset);
      }
    }
  } else {
    random_val -= rand_num;
    if (random_val < 0) {
      while (random_val < 0) {
        random_val = currentValue - random(0, offset);
      }
    }
  }

  return random_val;
}

void write(uint16_t value)
{
  ble_write(value >> 8);
  ble_write(value);
  Serial.print(value);
}
