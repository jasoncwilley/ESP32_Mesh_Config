#include "painlessMesh.h"
// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
//Pin Declaration
#define relay3 D1
#define relay4 D2
//Variables
bool relay3_status = false;
bool relay4_status = false;
Scheduler userScheduler; 
painlessMesh  mesh;
void sendMessage() ;
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
void sendMessage()
{
}
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
 
  Serial.println(json);
  DeserializationError error = deserializeJson(doc, json);

  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }

if (doc["relay3"] == false) {
  digitalWrite(relay3, LOW);
} else {
  digitalWrite(relay3, HIGH);
}

if (doc["relay4"] == false) {
  digitalWrite(relay4, LOW);
} else {
  digitalWrite(relay4, HIGH);
}
  Serial.println(json);
//  relay2_status = doc["Relay2"];
 // digitalWrite(Relay2, HIGH);
 // Serial.printf("From Node2");
}
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
void setup() {
  Serial.begin(115200);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
