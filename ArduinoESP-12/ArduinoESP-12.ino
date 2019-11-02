#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <WiFiUdp.h>
#include <stdint.h>
#include <stdbool.h>

#define MOT_LEFT_FWD 16
#define MOT_LEFT_BCK 12
#define MOT_RIGHT_FWD 13
#define MOT_RIGHT_BCK 14
#define TUNELADORA 10

#define V 350
#define V_TURN 500
#define V_FAST 700

#define SSID "InfernalRoombaAP"
#define PSSWD "12345678"  // *** CHANGE THIS **** //
#define UDP_PORT 42069 // local port to listen on
#define BUFFER_LEN 3 // bytes

enum directions {fwd, bck};
enum motors {left, right};
 
WiFiUDP Udp;
char recv[BUFFER_LEN];  // buffer for incoming data

void setup() {
  delay(1000);

  pinMode(MOT_LEFT_FWD, OUTPUT);
  pinMode(MOT_LEFT_BCK, OUTPUT);
  pinMode(MOT_RIGHT_FWD, OUTPUT);
  pinMode(MOT_RIGHT_BCK, OUTPUT);

  digitalWrite(MOT_LEFT_FWD, LOW);
  digitalWrite(MOT_LEFT_BCK, LOW);
  digitalWrite(MOT_RIGHT_FWD, LOW);
  digitalWrite(MOT_RIGHT_BCK, LOW);

  Serial.begin(115200);
    
  WiFi.mode(WIFI_AP);
  while(!WiFi.softAP(SSID, PSSWD))
  {
   Serial.println(".");
    delay(100);
  }
  
  Serial.print("AP initiated ");
  Serial.println(SSID);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
  
  Udp.begin(UDP_PORT);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), UDP_PORT);
}

/*void set_motor(motors motor, int speed, directions dir){
  if (dir == fwd){
    analogWrite(MOT_RIGHT_FWD ? motor == right : MOT_LEFT_FWD, speed);
  }else{
    analogWrite(MOT_RIGHT_BCK ? motor == right : MOT_LEFT_BCK, speed);
  }  
}

void move_vertical(directions dir, int speed){
  set_motor(left, speed, dir);
  set_motor(right, speed, dir);
}

void execute_command(int axis, int value){
  directions dir = fwd ? value > 0 : bck;

  switch (axis){
  case 1: // Y axis left joystick
    move_vertical(dir, value);
    break;
  case 2: // X axis right joystick
    move_horizontal(dir, value);
    break;
  default:
    break;
  }
}*/

void loop() {
  int packetSize = Udp.parsePacket();
  int_fast8_t len, gas, dir;
  bool boost;

  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    len = Udp.read(recv, BUFFER_LEN);
    if (len == BUFFER_LEN)
    {
      gas = recv[0];
      dir = recv[1];
      boost = recv[2];
    }
    Serial.printf("UDP packet contents: %x\n", recv);
    Serial.printf("GAS: %d DIR: %d BOOST: %d\n", gas, dir, boost);
    Udp.endPacket();
    bzero(recv, BUFFER_LEN);

    s

    if(gas == 1){
      analogWrite(MOT_RIGHT_FWD, 1000);
      analogWrite(MOT_RIGHT_BCK, 0);
      analogWrite(MOT_LEFT_FWD, 1000);
      analogWrite(MOT_LEFT_BCK, 0);
    }else if (gas == 255){
      analogWrite(MOT_RIGHT_FWD, 0);
      analogWrite(MOT_RIGHT_BCK, 1000);
      analogWrite(MOT_LEFT_FWD, 0);
      analogWrite(MOT_LEFT_BCK, 1000);
      
    }

    // send back a reply, to the IP address and port we got the packet from
    /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();*/
  } 
}
