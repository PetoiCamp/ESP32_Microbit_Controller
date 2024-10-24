//Generated Date: Fri, 20 Sep 2024 04:06:24 GMT by Jason
#include <BLEDevice.h>

#define RIGHT    "right"
#define LEFT     "left"
#define FORWARD  "forward"
#define BACK     "back"
#define CRAWL    "crawl"
#define WALK     "walk"
#define TROT     "trot"
#define REST     "rest"
#define BALANCE  "balance"
#define HELLO    "hello"
#define SIT      "sit"
#define PICKUP   "pickup"
#define PUTDOWN   "putdown"

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

String ga = "wk";
char dir = 'F';

String bleParser(String raw) {

  String cmd="";

  if (raw == CRAWL) {
    ga = "cr";
  }
  else if (raw == WALK) {
    ga = "wk";
  }
  else if (raw == TROT) {
    ga = "tr";
  }

  if (raw == FORWARD || raw == LEFT || raw == RIGHT) {
    if (raw == FORWARD) {
        dir = 'F';
    } 
    else if (raw == LEFT) {
        dir = 'L';
    } else if (raw == RIGHT) {
        dir = 'R';
    }
    cmd = ga + dir;
  }
  else if (raw == BACK) {
    cmd = "bk";
  }
  else if (raw == REST) {
    cmd = "d";
  }
  else if (raw == BALANCE) {
    cmd = "up";
  }
  else if (raw == HELLO) {
    cmd = "hi";
  }
  else if (raw == SIT) {
    cmd = "sit";
  }
  else if (raw == PICKUP) {
    cmd = "pickF";
  }
  else if (raw == PUTDOWN) {
    cmd = "putF";
  }
  return cmd;
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

void bleClientSetup()
{
  PTLF("Start...");
  serverBtDeviceName="BBC micro:bit [zugog]";    // It should be modified according to your own board.  another one's name: pogiv
  ljjBtStartScan();
}

void readBleClient()
{
  checkBtScan();

  if (btConnected && btReceiveDone && btRxLoad.length()>0){
    PTHL("btRxLoad", btRxLoad);
    String bleSig = bleParser(btRxLoad);
    //PTL(bleSig);
    if (bleSig != "") {
      if (bleSig.length() == 1)
        token = bleSig[0];
      else {
        token = T_SKILL;
        strcpy(newCmd, bleSig.c_str());
        // if (!strcmp(newCmd, "up"))
        //   dir = 'F';
      }
      newCmdIdx = 2;
    }
    btRxLoad="";
  }
}
