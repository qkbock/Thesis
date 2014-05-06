/*
 * WiFlyHQ Example httpclient.ino
 *
 * This sketch implements a simple Web client that connects to a 
 * web server, sends a GET, and then sends the result to the 
 * Serial monitor.
 *
 * This sketch is released to the public domain.
 *
 */

#include <WiFlyHQ.h>

//#include <SoftwareSerial.h>
//SoftwareSerial wifiSerial(8,9);

//#include <AltSoftSerial.h>
//AltSoftSerial wifiSerial(8,9);

WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] =  "mechanicalhuman"; //"Mirte's iPhone"; //"DG860AF2";
const char myPassword[] =  "jorge2000"; //"helloworld"; //"DG860A9003F2";

const char site[] = "54.235.78.70";

void terminal();

boolean startCharRecvd = false; // to see if msg is detected in server response
String valueAsString; //store value as a string

//information from server
float totalToday_a = 0;
float totalToday_b = 0;
float todayGoal_a = 670;
float todayGoal_b = 670;
float buttonR_a = 0;
float buttonG_a = 0;
float buttonB_a = 255;
float buttonR_b = 120;
float buttonG_b = 160;
float buttonB_b = 180;

void setup()
{
    pinMode(6, INPUT);
    char buf[32];

    Serial.begin(9600);
    Serial.println("Starting");
    Serial.print("Free memory: ");
    Serial.println(wifly.getFreeMemory(),DEC);

    Serial1.begin(9600);
    delay(100);
    
    if (!wifly.begin(&Serial1, &Serial)) {
        Serial.println("Failed to start wifly");
	terminal();
    }
    else{
      wifly.factoryRestore();
      wifly.save();
      wifly.reboot();
      delay(100);
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

    //terminal();

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
    updateData();
}

void loop(){
 if(digitalRead(6)==HIGH){   /*check for button press*/
    updateData();
    Serial.println("Button");
 }
    
    if (wifly.available() > 0) {
	char ch = wifly.read();
	Serial.write(ch);
	if (ch == '\n') {
	    /* add a carriage return */ 
	    Serial.write('\r');
	}
    }
    
    while (wifly.available() > 0) {       /* check to see if there is any data recvd by wifly */
      char ch = wifly.read();
      Serial.write(ch);
      if (ch == '\n') { /* if new line char */
        Serial.write('\r'); /* add a carriage return */
      }
      if (ch == '!'){
        startCharRecvd = false;
        Serial.println("value as string: ");
        Serial.println(valueAsString);
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

/* Connect the WiFly serial to the serial monitor. */
void terminal()
{
    while (1) {
	if (wifly.available() > 0) {
	    Serial.write(wifly.read());
	}


	if (Serial.available() > 0) {
	    wifly.write(Serial.read());
	}
    }
}

void updateData(){
    if (wifly.open(site, 80)) {
        Serial.print("Connected to ");
	Serial.println(site);

	/* Send the request */
	wifly.println("GET /name/queryData.php");
	wifly.println();
    } else {
        Serial.println("Failed to connect");
    }
}

void uploadData(){
    if (wifly.open(site, 80)) {
        Serial.print("Connected to ");
	Serial.println(site);
	/* Send the request */
	wifly.println("GET /name/receiveData.php?userName=Q&startTime=5&endTime=12&duration=4 HTTP/1.0");
	wifly.println();
    } else {
        Serial.println("Failed to connect");
    }
}
