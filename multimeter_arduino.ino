
#include <ArduinoBLE.h>
#include <stdlib.h>

BLEService randomNumberService("1101");
BLECharacteristic latestRandomValue("2101", BLERead|BLENotify, 3);
unsigned char bytes[3];
int current_sensor = 0;

// Set value to true or false to control if random data is used or not
bool generate_random = true;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  
  // begin initialization
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while(1);
  }
  BLE.setLocalName("Bluetooth Multimeter");
  BLE.setDeviceName("Bluetooth Multimeter");
  BLE.setAdvertisedService(randomNumberService);
  randomNumberService.addCharacteristic(latestRandomValue);
  BLE.addService(randomNumberService);


  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop()
{
  // put your main code here, to run repeatedly:

  BLEDevice central = BLE.central();

  if(central)
  {
    int output_number = 30;
    int analog_value;
    
    
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while(central.connected())
    {

      if(generate_random){
        int random_number = rand();
        random_number = random_number %2;
        switch(random_number){
          case 0:
            output_number += 1;
            break;
          case 1:
            output_number -= 1;
            break;
        }

        if(current_sensor == 0){
          bytes[0] = 0x02;
        }
        if(current_sensor == 1){
          bytes[0] = 0x03;
        }
        analog_value = output_number * 1000;
        
      }else{
        Serial.println("Analog Read!");
        analog_value = analogRead(current_sensor);
        Serial.println(analog_value);
        
  
        if(current_sensor == 0){
          analog_value = map(analog_value, 0, 1023, 0, 25000);
          bytes[0] = 0x02;
        }
        if(current_sensor == 1){
          analog_value = map(analog_value, 0, 1023, -30000, 30000);
          bytes[0] = 0x03;
        }
      }
      
      Serial.println("Still Connected!");

      

      Serial.println("Converted Analog Read!");
      Serial.println(analog_value);
      

      bytes[1] = highByte(analog_value);
      bytes[2] = lowByte(analog_value);
      Serial.println(bytes[0]);
      Serial.println(bytes[1]);
      Serial.println(bytes[2]);
            
      latestRandomValue.writeValue(bytes, 3);
      delay(200);
      
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

    BLE.advertise();
    Serial.println("Bluetooth device active, waiting for connections...");
  }
  

  

}