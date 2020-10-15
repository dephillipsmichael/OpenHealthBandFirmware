#include <bluefruit.h>

BLEClientService        IMUService(0x1101);

BLEClientCharacteristic IMUCharacteristic(0x1102);
BLEClientCharacteristic GyroCharacteristic(0x1103);

BLEService timeStampService = BLEService(0x1165);
BLECharacteristic timeStampCharacteristic = BLECharacteristic(0x1166);

long myTimer=0;
long receivedSamplesNb=0;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float accelRange=16.0;
float gyroRange=2000.0;
uint8_t* accelBuf;

void setup() {
  Serial.begin(115200);
//  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println(F("Custom Service and Characteristic Example Rx"));
  Serial.println(F("--------------------------------------\n"));

  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);

  Bluefruit.setName("OHB Receiver");

  // Initialize Custom Service Client
  IMUService.begin();
  timeStampService.begin();
  
  // set up callback for receiving measurement
  IMUCharacteristic.setNotifyCallback(IMUCharacteristic_notify_callback);
  IMUCharacteristic.begin();
   // set up callback for receiving measurement
  GyroCharacteristic.setNotifyCallback(GyroCharacteristic_notify_callback);
  GyroCharacteristic.begin();

    // set up callback for receiving measurement
  timeStampService.begin();
  delay(20);
 // timeStampCharacteristic.setNotifyCallback(timeStampCharacteristic_notify_callback);
 // timeStampCharacteristic.begin();
  
  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(IMUService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); 
}

void loop() {
  // put your main code here, to run repeatedly:

}