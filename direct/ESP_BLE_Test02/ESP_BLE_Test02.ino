//Generated Date: Fri, 20 Sep 2024 04:06:24 GMT by Jason
#include <BLEDevice.h>

//abbreviations
#define PT(s) Serial.print(s)  //makes life easier
#define PTL(s) Serial.println(s)
#define PTF(s) Serial.print(F(s))//trade flash memory for dynamic memory with F() function
#define PTLF(s) Serial.println(F(s))
#define PTH(head, value) \
  { \
    PT(head); \
    PT('\t'); \
    PTL(value); \
  }

#define SERVICE_UUID_STR           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX_STR "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX_STR "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

static BLEUUID serviceUUID(SERVICE_UUID_STR);
static BLEUUID    CHARACTERISTIC_UUID_RX(CHARACTERISTIC_UUID_RX_STR);
static BLEUUID    CHARACTERISTIC_UUID_TX(CHARACTERISTIC_UUID_TX_STR);
static boolean doConnect = false;
static boolean btConnected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristicTx;
static BLERemoteCharacteristic* pRemoteCharacteristicRx;
static BLERemoteCharacteristic* pRemoteCharacteristicTemp;
static BLEAdvertisedDevice* ljjBtDevice;
BLERemoteDescriptor* pRD;
bool btReceiveDone=false;
String btRxLoad="";
uint8_t dataIndicate[2] = {0x02, 0x00};
String serverBtDeviceName="";
String sendTemp="";

int photoIndex = 1;

byte A_Pin=34;
byte B_Pin=35;
char myBtnStatus;
bool buttonPressed(char btnName)
{
  if (btnName=='A'){
    if (digitalRead(A_Pin) == 0)
      return false;
    else
      return true;
  }
  else if (btnName=='B'){
    if (digitalRead(B_Pin) == 0)
      return false;
    else
      return true;
  } else {
    if ((digitalRead(A_Pin) == 0) && (digitalRead(B_Pin) == 0))
      return false;
    else
      return true;
  }
}

char getBtnStatus(){
  char buttonStatus=' ';
  int checkButtonDelay=200;
  if (buttonPressed('A')){
    delay(checkButtonDelay);
    if (buttonPressed('A')){
      buttonStatus='A';
      if (buttonPressed('B'))
        buttonStatus='C';
    }
  } else if (buttonPressed('B')){
      delay(checkButtonDelay);
      if (buttonPressed('B')){
        buttonStatus='B';
        if (buttonPressed('A'))
          buttonStatus='C';
      }
  }
  return buttonStatus;
}

void nextPhoto() {
  photoIndex = photoIndex + 1;
  if (photoIndex > 9) {
    photoIndex = 1;
  }
  PTH("photoIndex:", photoIndex);
}

void previousPhoto() {
  photoIndex = photoIndex + -1;
  if (photoIndex < 1) {
    photoIndex = 9;
  }
  PTH("photoIndex:", photoIndex);
}

void ljjBtConnected(){

}

void ljjBtDisconnected(){

}

static void btLjjNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  btReceiveDone=false;
  
  if (length > 0) {
    btRxLoad="";
    for (int i = 0; i < length; i++)
      btRxLoad +=(char)pData[i];
  }
  btRxLoad.replace("\r","");
  btRxLoad.replace("\n","");
/* For the ESP32 library v2.0.12 the following codes should be commented
//  if (pBLERemoteCharacteristic->canIndicate())
//  {
//    pRD->writeValue(dataIndicate, 2, false);
//  }
*/
  btReceiveDone=true;
}

class btLjjClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    ljjBtConnected();
  }

  void onDisconnect(BLEClient* pclient) {
    btConnected = false;
    btReceiveDone=false;
    btRxLoad="";
    ljjBtDisconnected();
  }
};

bool connectToServer() {
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->setClientCallbacks(new btLjjClientCallback());
  pClient->connect(ljjBtDevice);
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    pClient->disconnect();
    return false;
  }
  pRemoteCharacteristicTx = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_TX);
  pRemoteCharacteristicRx = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID_RX);
  if (pRemoteCharacteristicTx == nullptr) {
    pClient->disconnect();
    return false;
  }
  if (pRemoteCharacteristicRx == nullptr) {
    pClient->disconnect();
    return false;
  }
  if (!pRemoteCharacteristicTx->canWrite()){
    pRemoteCharacteristicTemp=pRemoteCharacteristicTx;
    pRemoteCharacteristicTx=pRemoteCharacteristicRx;
    pRemoteCharacteristicRx=pRemoteCharacteristicTemp;
  }
  if (pRemoteCharacteristicRx->canIndicate() || pRemoteCharacteristicRx->canNotify())
  {
    pRemoteCharacteristicRx->registerForNotify(btLjjNotifyCallback);
    PTLF("===Registed===");
  }
  if (pRemoteCharacteristicRx->canIndicate()){
    pRD = pRemoteCharacteristicRx->getDescriptor(BLEUUID((uint16_t)0x2902));
    if (pRD == nullptr){
      pClient->disconnect();
      return false;
    }
    pRD->writeValue(dataIndicate, 2, false);
  }
  btConnected = true;
  return true;
}

class ljjAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    String tempDeviceName=advertisedDevice.getName().c_str();
    if (tempDeviceName.equals(serverBtDeviceName)) {
      BLEDevice::getScan()->stop();
      ljjBtDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void ljjBtStartScan(){
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new ljjAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}

void checkBtScan(){
  if (doConnect) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  delay(5);
  PTLF("Start...");
  serverBtDeviceName="BBC micro:bit [pogiv]";    // It should be modified according to your own board.
  ljjBtStartScan();
  pinMode(A_Pin, INPUT);
  pinMode(B_Pin, INPUT);

}

void loop()
{
  checkBtScan();
  myBtnStatus=getBtnStatus();
  
  if (myBtnStatus=='A'){
    PTH("myBtnStatus", myBtnStatus);
    sendTemp=String("heart")+String("\n");
    pRemoteCharacteristicTx->writeValue(sendTemp.c_str(), sendTemp.length());
    while(buttonPressed('A')){}
  }
  if (myBtnStatus=='B'){
    PTH("myBtnStatus", myBtnStatus);
    sendTemp=String("clear")+String("\n");
    pRemoteCharacteristicTx->writeValue(sendTemp.c_str(), sendTemp.length());
    while(buttonPressed('B')){}
  }

  if (btConnected && btReceiveDone && btRxLoad.length()>0){
    PTH("btRxLoad", btRxLoad);
    if (btRxLoad == "next") {
      nextPhoto();
    } else if (btRxLoad == "previous") {
      previousPhoto();
    }
    btRxLoad="";
  }
}
