#include <WiFlyHQ.h>
#include <Adafruit_NeoPixel.h>
 
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

Adafruit_NeoPixel ring_a = Adafruit_NeoPixel(16, 7);
Adafruit_NeoPixel ring_b = Adafruit_NeoPixel(16, 6);
Adafruit_NeoPixel button_a = Adafruit_NeoPixel(2, 8);
Adafruit_NeoPixel button_b = Adafruit_NeoPixel(2, 9);
const int buttonPin_a = 10; 
const int buttonPin_b = 11;
int sensorPin = A4; 
int sensorValue = 0;

String person_a = "John";
String person_b = "Quincy";

float light_a;
float light_b;
uint32_t color_a = ring_a.Color(255, 0, 0);
uint32_t color_b = ring_b.Color(255, 0, 0);

uint32_t buttonColor_a = button_a.Color(buttonR_a, buttonG_a, buttonB_a);
uint32_t buttonColor_b = button_b.Color(buttonR_b, buttonG_b, buttonB_b);

int buttonState_a = 0;
int buttonState_b = 0;
int previousButtonState_a = HIGH;
int previousButtonState_b = HIGH;

float fadeCounter_a = 0;
float fadeCounter_b = 0;
int confirmingCounter_a = 0;
int confirmingCounter_b = 0;
boolean confirming_a = false;
boolean confirming_b = false;

boolean recording_a = false;
boolean recording_b = false;

void setup() {
  ring_a.begin();
  ring_b.begin();
  button_a.begin();
  button_b.begin();
  ring_a.show(); // Initialize all pixels to 'off'
  ring_b.show();
  button_a.show();
  button_b.show();
  ring_a.setBrightness(64);
  ring_b.setBrightness(64);
  button_a.setBrightness(100);
  button_b.setBrightness(100);
  pinMode(buttonPin_a, INPUT);
  pinMode(buttonPin_b, INPUT);
  
  Serial.begin(9600); //start up the USB serial port for debugging
 
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
//  uploadToMyWebsite("Quincy", "today", "noon", "1pm");

}
 
void loop() {
  sensorValue = analogRead(sensorPin); 
  totalToday_b = sensorValue;

  senseButtonA();
  senseButtonB();
  displayButtonA();
  displayButtonB();
  displayRingA();
  displayRingB();
 
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
  
  delay(10);
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
  
  totalToday_a = color.substring(0, commaIndex1).toInt();
  totalToday_b = color.substring(commaIndex1+1, commaIndex2).toInt();
  todayGoal_a = color.substring(commaIndex2+1, commaIndex3).toInt();
  todayGoal_b = color.substring(commaIndex3+1, commaIndex4).toInt();
  buttonR_a = color.substring(commaIndex4+1, commaIndex5).toInt();
  buttonG_a = color.substring(commaIndex5+1, commaIndex6).toInt();
  buttonB_a = color.substring(commaIndex6+1, commaIndex7).toInt();
  buttonR_b = color.substring(commaIndex7+1, commaIndex8).toInt();
  buttonG_b = color.substring(commaIndex8+1, commaIndex9).toInt();
  buttonB_b = color.substring(commaIndex9+1).toInt();
}
 
 
/* function that requests data from server */
void checkMyWebsite() {
  Serial.println("\r-->start request from server");
  String host(site);
  if (wifly.open(site, 80)) {
    Serial.print("Connected to ");
    Serial.println(site);
    /* Send the request */
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
    wifly.println("GET /name/logAChore.php?userName="+username+"&date="+date+"&startTime="+startTime+"&endTime="+endTime+" HTTP/1.1"); //change me!
    wifly.println("Host: " + host);
    wifly.println("Connection: close");
    wifly.println();
  } else {
    Serial.println("Failed to connect to the website");
  }
}

//-----------------------------------------------------------------------

void senseButtonA(){
  buttonState_a = digitalRead(buttonPin_a);
  if ((buttonState_a != previousButtonState_a) && (buttonState_a == HIGH)) {
    if(recording_a == false){
      //set start time
    }
    else if(recording_a == true){
      //set end time
      //feed this start, end and person
      confirming_a = true;
    }
    recording_a = !recording_a;
    fadeCounter_a = 0;
//    Serial.println(recording_a);
  }
  previousButtonState_a = buttonState_a; 
//  Serial.println(buttonState_a);
}

//-----------------------------------------------------------------------

void senseButtonB(){
  buttonState_b = digitalRead(buttonPin_b);
  if ((buttonState_b != previousButtonState_b) && (buttonState_b == HIGH)) {
    if(recording_b == false){
      //set start time
    }
    else if(recording_b == true){
      //set end time
      //feed this start, end and person
      confirming_b = true;
    }
    recording_b = !recording_b;
    fadeCounter_b = 0;
  }
  previousButtonState_b = buttonState_b; 
}

//-----------------------------------------------------------------------

void displayButtonA(){
  //change button behavior when recording
  if(recording_a == true){
    button_a.setBrightness(int(cos(fadeCounter_a)*50 + 50));
    button_a.setPixelColor(0, 255, 15, 65);
    button_a.setPixelColor(1, 255, 15, 65);
    button_a.show();
    fadeCounter_a = fadeCounter_a + .03;
    if(fadeCounter_a > 6.2){
      fadeCounter_a = 0;
    }
  }
  else if(confirming_a){
    button_a.setBrightness(100);
    button_a.setPixelColor(0, 13, 255, 130);
    button_a.setPixelColor(1, 13, 255, 130);
    button_a.show();
    confirmingCounter_a ++;
    if(confirmingCounter_a > 150){
      confirming_a = false;
      confirmingCounter_a = 0;
    }
  }
  else{
    button_a.setBrightness(100);
    button_a.setPixelColor(0, buttonColor_a);
    button_a.setPixelColor(1, buttonColor_a);
    button_a.show();
  }
}

//-----------------------------------------------------------------------

void displayButtonB(){
  //change button behavior when recording
  if(recording_b == true){
    button_b.setBrightness(int(cos(fadeCounter_b)*50 + 50));
    button_b.setPixelColor(0, 255, 15, 65);
    button_b.setPixelColor(1, 255, 15, 65);
    button_b.show();
    fadeCounter_b = fadeCounter_b + .03;
    if(fadeCounter_b > 6.2){
      fadeCounter_b = 0;
    }
  }
  else if(confirming_b){
    button_b.setBrightness(100);
    button_b.setPixelColor(0, 13, 255, 130);
    button_b.setPixelColor(1, 13, 255, 130);
    button_b.show();
    confirmingCounter_b ++;
    if(confirmingCounter_b > 150){
      confirming_b = false;
      confirmingCounter_b = 0;
    }
  }
  else{
    button_b.setBrightness(100);
    button_b.setPixelColor(0, buttonColor_b);
    button_b.setPixelColor(1, buttonColor_b);
    button_b.show();
  }
}

//-----------------------------------------------------------------------


