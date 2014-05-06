#include <WiFlyHQ.h>
 
WiFly wifly; //create WiFly object
 
/* Change these to match your WiFi network */
const char mySSID[] = "mechanicalhuman";
const char myPassword[] = "jorge2000";
 
const char site[] = "54.235.78.70";
 
boolean startCharRecvd = false; // to see if msg is detected in server response
String valueAsString; //store value as a string

int totalToday_a;
int totalToday_b;
int todayGoal_a;
int todayGoal_b;
int buttonR_a;
int buttonG_a;
int buttonB_a;
int buttonR_b;
int buttonG_b;
int buttonB_b;

void setup() {
  
  Serial.begin(9600); //start up the USB serial port for debugging
 
  //**** FOR DEBUG ONLY ****
  while(!Serial){ //-- WILL WAIT FOR SERIAL MONITOR TO OPEN 
    ;             // BEFORE CONTINUING WITH ANY CODE
  }               //-- REMOVE THIS SECTION WHEN INSTALLING FOR REAL
  //**** END DEBUG ****
 
  Serial.println("Starting");
  int freeMem = wifly.getFreeMemory();
  Serial.println(freeMem);
 
  Serial1.begin(9600); //start up the Serial1 port for communication with WiFly
  if (!wifly.begin(&Serial1,&Serial)) { 
    Serial.println("*** Failed to start wifly ***");
  }
 
  if (wifly.getFlushTimeout() != 15) { //this will run every time
    Serial.println("--> Restoring flush timeout to 10msecs");
    wifly.setFlushTimeout(10);  //we want it to be set to 10ms  
    Serial.println("--> WiFly Factory Restore");
    wifly.factoryRestore();
    wifly.save();
    wifly.reboot();
  }
 
  /* Join wifi network if not already associated */
  if (!wifly.isAssociated()) {
    /* Setup the WiFly to connect to a wifi network */
    Serial.println("Joining network");
    wifly.setSSID(mySSID);
    wifly.setPassphrase(myPassword);
    wifly.enableDHCP();
 
    while(!wifly.join()){
      Serial.println("Failed to join wifi network");
      Serial.println("trying again >>");
      delay(3000); 
    }
  } 
  else {
    Serial.println("Already joined network");
  }
  char buf[32];
  Serial.print("MAC: ");
  Serial.println(wifly.getMAC(buf, sizeof(buf)));
  Serial.print("IP: ");
  Serial.println(wifly.getIP(buf, sizeof(buf)));
  Serial.print("Netmask: ");
  Serial.println(wifly.getNetmask(buf, sizeof(buf)));
  Serial.print("Gateway: ");
  Serial.println(wifly.getGateway(buf, sizeof(buf)));
  wifly.setDeviceID("Wifly-WebClient");
  Serial.print("DeviceID: ");
  Serial.println(wifly.getDeviceID(buf, sizeof(buf)));
  if (wifly.isConnected()) {
    Serial.println("Old connection active. Closing");
    wifly.close();
  }
  
      

  checkMyWebsite(); //initial call to server

}
 
void loop() {
     


 
 
  while (wifly.available() > 0) {       /* check to see if there is any data recvd by wifly */
    char ch = wifly.read();
    Serial.write(ch);
    if (ch == '\n') { /* if new line char */
      Serial.write('\r'); /* add a carriage return */
    }
    if (ch == '!'){
      startCharRecvd = false;
      setLightColor(valueAsString);
      valueAsString = "";
      break;
    }
    if(startCharRecvd) valueAsString += ch;
    if (ch == '#') startCharRecvd = true;
  }
 
  if (Serial.available() > 0) {
    wifly.write(Serial.read());
  }
  uploadToMyWebsite("Quincy", "today", "noon", "1pm");
}
 
 
/* function to change LED color after receiving a new value */
void setLightColor(String color){
  //search for the location of all the commas
  int commaIndex1 = color.indexOf(',');
  int commaIndex2 = color.indexOf(',', commaIndex1+1);
  int commaIndex3 = color.indexOf(',', commaIndex2+1);
  int commaIndex4 = color.indexOf(',', commaIndex3+1);
  int commaIndex5 = color.indexOf(',', commaIndex4+1);
  int commaIndex6 = color.indexOf(',', commaIndex5+1);
  int commaIndex7 = color.indexOf(',', commaIndex6+1);
  int commaIndex8 = color.indexOf(',', commaIndex7+1);
  int commaIndex9 = color.indexOf(',', commaIndex8+1);
  
  String value1 = color.substring(0, commaIndex1);
  String value2 = color.substring(commaIndex1+1, commaIndex2);
  String value3 = color.substring(commaIndex2+1, commaIndex3);
  String value4 = color.substring(commaIndex3+1, commaIndex4);
  String value5 = color.substring(commaIndex4+1, commaIndex5);
  String value6 = color.substring(commaIndex5+1, commaIndex6);
  String value7 = color.substring(commaIndex6+1, commaIndex7);
  String value8 = color.substring(commaIndex7+1, commaIndex8);
  String value9 = color.substring(commaIndex8+1, commaIndex9);
  String value10 = color.substring(commaIndex9+1);

  totalToday_a = value1.toInt();
  totalToday_b = value2.toInt();
  todayGoal_a = value3.toInt();
  todayGoal_b = value4.toInt();
  buttonR_a = value5.toInt();
  buttonG_a = value6.toInt();
  buttonB_a = value7.toInt();
  buttonR_b = value8.toInt();
  buttonG_b = value9.toInt();
  buttonB_b = value10.toInt();
  
  Serial.print("-->COLORVAL: ");
  Serial.println(totalToday_a);
  Serial.println(totalToday_b);
  Serial.println(buttonG_a);
  Serial.println(buttonG_b); 
  // here is where you change the LED values! 
  //   hint: use colorVal
//  analogWrite(redLedPin, colorVal);
}
 
 
/* function that requests data from server */
void checkMyWebsite() {
  Serial.println("\r-->start request from server");
  String host(site);
  if (wifly.open(site, 80)) {
    Serial.print("Connected to ");
    Serial.println(site);
    /* Send the request */
    //point wifly to correct folder directory on the server
    wifly.println("GET /name/queryData.php HTTP/1.1"); //change me!
    wifly.println("Host: " + host);
    wifly.println("Connection: close");
    wifly.println();
  } else {
    Serial.println("Failed to connect to the website");
  }
}

/* function that gives data to server */
void uploadToMyWebsite(String username, String date, String startTime, String endTime) {
  Serial.println("\r-->start sending to server");
  String host(site);
  if (wifly.open(site, 80)) {
    Serial.print("Connected to ");
    Serial.println(site);
    /* Send the request */
    //point wifly to correct folder directory on the server
    wifly.println("GET /name/receiveData.php?userName=Quincy&date=today&startTime=noon&endTime=1pm HTTP/1.1"); //change me!
//    wifly.println("GET /name/logAChore.php?userName="+username+"&date="+date+"&startTime="+startTime+"&endTime="+endTime+" HTTP/1.1"); //change me!
    wifly.println("Host: " + host);
    wifly.println("Connection: close");
    wifly.println();
  } else {
    Serial.println("Failed to connect to the website");
  }
}
