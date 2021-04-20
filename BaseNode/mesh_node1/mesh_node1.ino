#include "painlessMesh.h"
// WiFi Credentials
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
bool button1_status;
bool button2_status;
bool button3_status;
bool button4_status;
bool relay1 = 0;
bool relay2 = 0;
bool relay3 = 0;
bool relay4 = 0;
//Pin Declaration
#define Button1 D1
#define Button2 D2
#define Button3 D5
#define Button4 D7
Scheduler userScheduler; 
painlessMesh  mesh;
void sendMessage() ; 
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
void sendMessage() 
{
// Reading Status of Pushbutton
    button1_status = digitalRead(Button1);
    if (button1_status==true){
      relay1=LOW;
    } else {
      relay1=HIGH;
    }
    
    button2_status = digitalRead(Button2);
    if (button2_status==true){
      relay2=LOW;
    } else {
      relay2=HIGH;
    }
    
        button3_status = digitalRead(Button3);
    if (button3_status==true){
      relay3=LOW;
    } else {
      relay3=HIGH;
    }

        button4_status = digitalRead(Button4);
    if (button4_status==true){
      relay4=LOW;
    } else {
      relay4=HIGH;
    }
    

    
// Serializing in JSON Format 
  DynamicJsonDocument doc(1024);

  doc["relay1"] = relay1;
 doc["relay2"] = relay2;    
 doc["relay3"] = relay3;    
  doc["relay4"] = relay4;

  String msg;
  
  serializeJson(doc, msg);
    mesh.sendBroadcast( msg );
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 1));
}
void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
}
  /*
   String Temp = doc["TEMP"];
   String Hum = doc["HUM"];
  Serial.println("From node1");
   Serial.println("Temperature:");
   Serial.print(Temp);
    Serial.println("Humidity:");
   Serial.print(Hum);
}
*/
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
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
   pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
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
  mesh.update();
}
