#include <WiFlyHQ.h>
 
/* 
- download visualight library from here:
https://github.com/lpercifield/visualight/releases/download/1.1/Visualight.zip
- unzip, put in the Documents > Arduino > libraries folder
*/
 
#include <EEPROM.h>
#include <Visualight.h>
 
//all pin definitions
#define button        7
#define wiflyResetPin 11
#define redLedPin     9
#define greenLedPin   13
#define blueLedPin    10
#define whiteLedPin   5
 
 
WiFly wifly; //create WiFly object
 
/* Change these to match your WiFi network */
const char mySSID[] = "mechanicalhuman";
const char myPassword[] = "jorge2000";
 
/* root host site you want to hit */
//const char site[] = "www.yuchenzhang.com";
const char site[] = "54.235.78.70";
//const char site[] = "www.google.com";
 
boolean startCharRecvd = false; // to see if msg is detected in server response
String valueAsString; //store value as a string
 
void setup() {
 
  pinMode(button, INPUT);
  pinMode(whiteLedPin, OUTPUT);
  pinMode(wiflyResetPin,OUTPUT);
  digitalWrite(wiflyResetPin, HIGH); 
 
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
 
  if(digitalRead(button)==LOW){   /*check for button press*/
    delay(2000);      //to prevent "double clicks"
    checkMyWebsite(); //check the website!
  }
 
  /* here you need to create a timer that calls checkMyWebsite()
  every 1 minute or every 5 minutes, however frequently as needed */
  
 
 
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
}
 
 
/* function to change LED color after receiving a new value */
void setLightColor(String color){
  //search for the location of all the commas
  int commaIndex1 = myString.indexOf(',');
  int CommaIndex2 = myString.indexOf(',', commaIndex+1);
  int CommaIndex3 = myString.indexOf(',', commaIndex2+1);
  int CommaIndex4 = myString.indexOf(',', commaIndex3+1);
  int CommaIndex5 = myString.indexOf(',', commaIndex4+1);
  int CommaIndex6 = myString.indexOf(',', commaIndex5+1);
  int CommaIndex7 = myString.indexOf(',', commaIndex6+1);
  int CommaIndex8 = myString.indexOf(',', commaIndex7+1);
  int CommaIndex9 = myString.indexOf(',', commaIndex8+1);
  
  
  
  Serial.print("-->COLORVAL: ");
  Serial.println(color);
  
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
