#include <Adafruit_NeoPixel.h>
#include <WiFlyHQ.h>

WiFly wifly;

const char mySSID[] = "DG860AF2"; //"mechanicalhuman";
const char myPassword[] = "DG860A9003F2"; //"jorge2000";
 
const char site[] = "54.235.78.70";
 
boolean startCharRecvd = false; // to see if msg is detected in server response
String valueAsString;

//4, 6, 7, 8, 9, 10, 11 work for ring
// 5 not working
Adafruit_NeoPixel ring_a = Adafruit_NeoPixel(16, 6);
Adafruit_NeoPixel ring_b = Adafruit_NeoPixel(16, 9);
Adafruit_NeoPixel button_a = Adafruit_NeoPixel(2, 4);
Adafruit_NeoPixel button_b = Adafruit_NeoPixel(2, 10);
const int buttonPin_a = 8; 
const int buttonPin_b = 11;
int sensorPin = A4; 
int sensorValue = 0;

String person_a = "John";
String person_b = "Quincy";

//information from server
int totalToday_a = 25;
int totalToday_b = 50;
int todayGoal_a = 100;
int todayGoal_b = 100;
int buttonR_a = 0;
int buttonG_a = 0;
int buttonB_a = 255;

uint32_t color_a = ring_a.Color(255, 0, 0);
uint32_t color_b = ring_b.Color(255, 0, 0);

//uint32_t buttonColor_a = button_a.Color(buttonR_a, buttonG_a, buttonB_a);
uint32_t buttonColor_b = button_b.Color(120, 160, 180);

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
  
      

  checkMyWebsite();
//  uploadToMyWebsite("Quincy", "today", "noon", "1pm");

}

void loop(){
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
  
  senseButtonA();
  senseButtonB();
  displayButtonA();
  displayButtonB();

  displayRingA();
  displayRingB();
  delay(10);
}

//-----------------------------------------------------------------------

void setLightColor(String color){
  //search for the location of all the commas
  int commaIndex1 = color.indexOf(',');
  int commaIndex2 = color.indexOf(',', commaIndex1+1);
  int commaIndex3 = color.indexOf(',', commaIndex2+1);
  
  String value1 = color.substring(0, commaIndex1);
  String value2 = color.substring(commaIndex1+1, commaIndex2);
  String value3 = color.substring(commaIndex2+1, commaIndex3);
  String value4 = color.substring(commaIndex3+1);
  
  totalToday_a = value1.toInt();
  buttonR_a = value2.toInt();
  buttonG_a = value3.toInt();
  buttonB_a = value4.toInt();
}

//-----------------------------------------------------------------------

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

//-----------------------------------------------------------------------

void senseButtonA(){
  buttonState_a = digitalRead(buttonPin_a);
  if ((buttonState_a != previousButtonState_a) && (buttonState_a == HIGH)) {
    if(recording_a == false){
      totalToday_a += 10;
    }
    else if(recording_a == true){
      confirming_a = true;
    }
    recording_a = !recording_a;
    fadeCounter_a = 0;
  }
  previousButtonState_a = buttonState_a; 
}

//-----------------------------------------------------------------------

void senseButtonB(){
  buttonState_b = digitalRead(buttonPin_b);
  if ((buttonState_b != previousButtonState_b) && (buttonState_b == HIGH)) {
    if(recording_b == false){
      totalToday_b += 10;
    }
    else if(recording_b == true){
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
    button_a.setPixelColor(0, buttonR_a, buttonG_a, buttonB_a);
    button_a.setPixelColor(1, buttonR_a, buttonG_a, buttonB_a);
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

void displayRingA(){
  //calculate number of lights
  float light = (totalToday_a*16)/todayGoal_a;
  //color ring A
  if(light > 16){
    color_a = ring_a.Color(0, 255, 0);
  }
  else if(light > 15){
    color_a = ring_a.Color(40, 255, 0);
  }
  else if(light > 14){
    color_a = ring_a.Color(80, 255, 0);
  }
  else if(light > 13){
    color_a = ring_a.Color(100, 255, 0);
  }
  else if(light > 12){
    color_a = ring_a.Color(125, 255, 0);
  }
  else if(light > 11){
    color_a = ring_a.Color(150, 255, 0);
  }
  else if(light > 10){
    color_a = ring_a.Color(190, 255, 0);
  }
  else if(light > 9){
    color_a = ring_a.Color(220, 255, 0);
  }
  else if(light > 8){
    color_a = ring_a.Color(255, 240, 0);
  }
  else if(light > 7){
    color_a = ring_a.Color(255, 220, 0);
  }
  else if(light > 6){
    color_a = ring_a.Color(255, 190, 0);
  }
  else if(light > 5){
    color_a = ring_a.Color(255, 150, 0);
  }
  else if(light > 4){
    color_a = ring_a.Color(255, 100, 0);
  }
  else if(light > 3){
    color_a = ring_a.Color(255, 60, 0);
  }
  else if(light > 2){
    color_a = ring_a.Color(255, 30, 0);
  }
  else{
    color_a = ring_a.Color(255, 0, 0);
  }


  //light ring A
  if(light > 0){
    ring_a.setPixelColor(15, color_a);
  }
  else{
    ring_a.setPixelColor(15, 0, 0, 0);
  }
  if(light > 2){
    ring_a.setPixelColor(14, color_a);
  }
  else{
    ring_a.setPixelColor(14, 0, 0, 0);
  }
  if(light > 3){
    ring_a.setPixelColor(13, color_a);
  }
  else{
    ring_a.setPixelColor(13, 0, 0, 0);
  }
  if(light > 4){
    ring_a.setPixelColor(12, color_a);
  }
  else{
    ring_a.setPixelColor(12, 0, 0, 0);
  }
  if(light > 5){
    ring_a.setPixelColor(11, color_a);
  }
  else{
    ring_a.setPixelColor(11, 0, 0, 0);
  }
  if(light > 6){
    ring_a.setPixelColor(10, color_a);
  }
  else{
    ring_a.setPixelColor(10, 0, 0, 0);
  }
  if(light > 7){
    ring_a.setPixelColor(9, color_a);
  }
  else{
    ring_a.setPixelColor(9, 0, 0, 0);
  }
  if(light > 8){
    ring_a.setPixelColor(8, color_a);
  }
  else{
    ring_a.setPixelColor(8, 0, 0, 0);
  }
  if(light > 9){
    ring_a.setPixelColor(7, color_a);
  }
  else{
    ring_a.setPixelColor(7, 0, 0, 0);
  }
  if(light > 10){
    ring_a.setPixelColor(6, color_a);
  }
  else{
    ring_a.setPixelColor(6, 0, 0, 0);
  }
  if(light > 11){
    ring_a.setPixelColor(5, color_a);
  }
  else{
    ring_a.setPixelColor(5, 0, 0, 0);
  }
  if(light > 12){
    ring_a.setPixelColor(4, color_a);
  }
  else{
    ring_a.setPixelColor(4, 0, 0, 0);
  }
  if(light > 13){
    ring_a.setPixelColor(3, color_a);
  }
  else{
    ring_a.setPixelColor(3, 0, 0, 0);
  }
  if(light > 14){
    ring_a.setPixelColor(2, color_a);
  }
  else{
    ring_a.setPixelColor(2, 0, 0, 0);
  }
  if(light > 15){
    ring_a.setPixelColor(1, color_a);
  }
  else{
    ring_a.setPixelColor(1, 0, 0, 0);
  }
  if(light > 16){
    ring_a.setPixelColor(0, color_a);
  }
  else{
    ring_a.setPixelColor(0, 0, 0, 0);
  }


  ring_a.show();
}

//-----------------------------------------------------------------------

void displayRingB(){
  float light = (totalToday_b*16)/todayGoal_b;
  if(light > 16){
    color_b = ring_b.Color(0, 255, 0);
  }
  else if(light > 15){
    color_b = ring_b.Color(40, 255, 0);
  }
  else if(light > 14){
    color_b = ring_b.Color(80, 255, 0);
  }
  else if(light > 13){
    color_b = ring_b.Color(100, 255, 0);
  }
  else if(light > 12){
    color_b = ring_b.Color(125, 255, 0);
  }
  else if(light > 11){
    color_b = ring_b.Color(150, 255, 0);
  }
  else if(light > 10){
    color_b = ring_b.Color(190, 255, 0);
  }
  else if(light > 9){
    color_b = ring_b.Color(220, 255, 0);
  }
  else if(light > 8){
    color_b = ring_b.Color(255, 240, 0);
  }
  else if(light > 7){
    color_b = ring_b.Color(255, 220, 0);
  }
  else if(light > 6){
    color_b = ring_b.Color(255, 190, 0);
  }
  else if(light > 5){
    color_b = ring_b.Color(255, 150, 0);
  }
  else if(light > 4){
    color_b = ring_b.Color(255, 100, 0);
  }
  else if(light > 3){
    color_b = ring_b.Color(255, 60, 0);
  }
  else if(light > 2){
    color_b = ring_b.Color(255, 30, 0);
  }
  else{
    color_b = ring_b.Color(255, 0, 0);
  }

  //light ring B
  if(light > 0){
    ring_b.setPixelColor(15, color_b);
  }
  else{
    ring_b.setPixelColor(15, 0, 0, 0);
  }
  if(light > 2){
    ring_b.setPixelColor(14, color_b);
  }
  else{
    ring_b.setPixelColor(14, 0, 0, 0);
  }
  if(light > 3){
    ring_b.setPixelColor(13, color_b);
  }
  else{
    ring_b.setPixelColor(13, 0, 0, 0);
  }
  if(light > 4){
    ring_b.setPixelColor(12, color_b);
  }
  else{
    ring_b.setPixelColor(12, 0, 0, 0);
  }
  if(light > 5){
    ring_b.setPixelColor(11, color_b);
  }
  else{
    ring_b.setPixelColor(11, 0, 0, 0);
  }
  if(light > 6){
    ring_b.setPixelColor(10, color_b);
  }
  else{
    ring_b.setPixelColor(10, 0, 0, 0);
  }
  if(light > 7){
    ring_b.setPixelColor(9, color_b);
  }
  else{
    ring_b.setPixelColor(9, 0, 0, 0);
  }
  if(light > 8){
    ring_b.setPixelColor(8, color_b);
  }
  else{
    ring_b.setPixelColor(8, 0, 0, 0);
  }
  if(light > 9){
    ring_b.setPixelColor(7, color_b);
  }
  else{
    ring_b.setPixelColor(7, 0, 0, 0);
  }
  if(light > 10){
    ring_b.setPixelColor(6, color_b);
  }
  else{
    ring_b.setPixelColor(6, 0, 0, 0);
  }
  if(light > 11){
    ring_b.setPixelColor(5, color_b);
  }
  else{
    ring_b.setPixelColor(5, 0, 0, 0);
  }
  if(light > 12){
    ring_b.setPixelColor(4, color_b);
  }
  else{
    ring_b.setPixelColor(4, 0, 0, 0);
  }
  if(light > 13){
    ring_b.setPixelColor(3, color_b);
  }
  else{
    ring_b.setPixelColor(3, 0, 0, 0);
  }
  if(light > 14){
    ring_b.setPixelColor(2, color_b);
  }
  else{
    ring_b.setPixelColor(2, 0, 0, 0);
  }
  if(light > 15){
    ring_b.setPixelColor(1, color_b);
  }
  else{
    ring_b.setPixelColor(1, 0, 0, 0);
  }
  if(light > 16){
    ring_b.setPixelColor(0, color_b);
  }
  else{
    ring_b.setPixelColor(0, 0, 0, 0);
  }

  ring_b.show();
}
