#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define relay 2

String knownBLEAddresses[] = {"ac:23:3f:71:c9:8c"};  //Add your device address (more than one ex."ac:23:3f:71:c9:8c","xx:xx:xx:xx:xx:xx")
int RSSI_THRESHOLD = -70; // make it easy ---> range of scanner
bool device_found;
int scanTime = 1; //In seconds
BLEScan* pBLEScan;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      {
        //Uncomment to Enable Debug Information
       // Serial.println("*************Start**************");
        //Serial.println(sizeof(knownBLEAddresses));
        //Serial.println(sizeof(knownBLEAddresses[0]));
        //Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
        //Serial.println(advertisedDevice.getAddress().toString().c_str());
        //Serial.println(knownBLEAddresses[i].c_str());
        //Serial.println("*************End**************");
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0)
        {
          device_found = true;
          Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          break;
        }
        else
          device_found = false;
          
      }
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};
void setup() {
  Serial.begin(115200); //Enable UART on ESP32
  Serial.println("Scanning..."); // Print Scanning
  pinMode(relay, OUTPUT); //make BUILTIN_LED pin as output
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); // set Scan interval
  pBLEScan->setWindow(99);  // less or equal setInterval value
}
void loop() 
{
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  
  for (int i = 0; i < foundDevices.getCount(); i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();
    
    //Serial.print("RSSI: ");
    //Serial.println(rssi);
    if (rssi > RSSI_THRESHOLD && device_found == true)
    {
       Serial.println("FOUND");
       Serial.print("RSSI: ");
       Serial.println(rssi);
      
    }
     if (rssi < RSSI_THRESHOLD && device_found == true)
    {
       Serial.println("Tag Lost");
       Serial.print("RSSI: ");
       Serial.println(rssi);
      
    }

  }
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}
