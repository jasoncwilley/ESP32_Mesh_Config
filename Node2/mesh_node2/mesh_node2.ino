#include "painlessMesh.h"
// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
//Pin Declaration
#define relay1 D1
#define relay2 D2
//Variables
bool relay1_status = false;
bool relay2_status = false;
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

if (doc["relay1"] == false) {
  digitalWrite(relay1, LOW);
} else {
  digitalWrite(relay1, HIGH);
}

if (doc["relay2"] == false) {
  digitalWrite(relay2, LOW);
} else {
  digitalWrite(relay2, HIGH);
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
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
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
