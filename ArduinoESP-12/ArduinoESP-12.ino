#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <WiFiUdp.h>
#include <stdint.h>
#include <stdbool.h>
#include <Servo.h>

#define MOT_LEFT_FWD 16
#define MOT_LEFT_BCK 12
#define MOT_RIGHT_FWD 13
#define MOT_RIGHT_BCK 14
#define TUNELA 5

#define V 600
#define V_FAST 1023
#define V_TURN 600
#define V_TURN_GAS 400
#define V_TURN_FAST 1023
Servo tuneladora;

#define SSID "InfernalTuneladoraAP"
#define PSSWD "tevoyatunelarcrackk"  // *** CHANGE THIS **** //
#define UDP_PORT 42069 // local port to listen on
#define BUFFER_LEN 4 // bytes

enum directions {fwd, bck};
enum motors {left, right};
 
WiFiUDP Udp;
char recv[BUFFER_LEN];  // buffer for incoming data

void setup() {
  
  //delay(1000);
  tuneladora.attach(TUNELA);
  tuneladora.writeMicroseconds(1000);
  delay(5000);
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

void forward() {
  analogWrite(MOT_RIGHT_FWD, V);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V);
  analogWrite(MOT_LEFT_BCK, 0);
}

void forward_boost() {
  analogWrite(MOT_RIGHT_FWD, V_FAST);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_FAST);
  analogWrite(MOT_LEFT_BCK, 0);
}

void forward_right() {
  analogWrite(MOT_RIGHT_FWD, V_TURN_GAS);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN);
  analogWrite(MOT_LEFT_BCK, 0);
}

void forward_right_boost() {
  analogWrite(MOT_RIGHT_FWD, V_TURN);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN_FAST);
  analogWrite(MOT_LEFT_BCK, 0);
}

void forward_left() {
  analogWrite(MOT_RIGHT_FWD, V_TURN);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN_GAS);
  analogWrite(MOT_LEFT_BCK, 0);
}

void forward_left_boost() {
  analogWrite(MOT_RIGHT_FWD, V_TURN_FAST);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN);
  analogWrite(MOT_LEFT_BCK, 0);
}

void turn_right() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN);
  analogWrite(MOT_LEFT_BCK, 0);
}

void turn_right_boost() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, V_TURN_FAST);
  analogWrite(MOT_LEFT_BCK, 0);
}

void turn_left() {
  analogWrite(MOT_RIGHT_FWD, V_TURN);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, 0);
}

void turn_left_boost() {
  analogWrite(MOT_RIGHT_FWD, V_TURN_FAST);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, 0);
}

void backwards() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V);
}

void backwards_boost() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V_FAST);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V_FAST);
}

void backwards_left() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V_TURN);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V_TURN_GAS);
}

void backwards_left_boost() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V_TURN_FAST);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V_TURN);
}

void backwards_right() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V_TURN_GAS);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V_TURN);
}

void backwards_right_boost() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, V_TURN);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, V_TURN_FAST);
}

void stop() {
  analogWrite(MOT_RIGHT_FWD, 0);
  analogWrite(MOT_RIGHT_BCK, 0);
  analogWrite(MOT_LEFT_FWD, 0);
  analogWrite(MOT_LEFT_BCK, 0);
}

void loop() {
  int packetSize = Udp.parsePacket();
  int_fast8_t len, gas, dir, tun;
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
      tun = recv[3];
    }
    Serial.printf("UDP packet contents: %x\n", recv);
    Serial.printf("GAS: %d DIR: %d BOOST: %d TUN: %d\n", gas, dir, boost,tun);
    Udp.endPacket();
    bzero(recv, BUFFER_LEN);
    
    if(tun == 1) {
      tuneladora.write(100);
    } else if(tun == 0){
      tuneladora.write(0);
    }

    if(gas == 0) { /* stop */
      if(dir == 1) {
        if(boost == 1) {
          turn_right_boost();
        } else if(boost == 0) {
          turn_right();
        }     
      } else if(dir == 255) {
        if(boost == 1) {
          turn_left_boost();
        } else if(boost == 0) {
          turn_left();
        }  
      } else if(dir == 0) {
        if(boost == 1) { 
          stop();
        } else if(boost == 0){
          stop();
        }
      }
    } else if(gas == 1) { /* forward */
      if(dir == 1) {
        if(boost == 1) { 
          forward_right_boost();
        } else if(boost == 0){
          forward_right();
        }
      } else if(dir == 255) {
        if (boost == 1) { 
          forward_right_boost();
        } else if(boost == 0){
          forward_right();
        }
      } else if(dir == 0) {
        if(boost == 1) { 
          forward_boost();
        } else if(boost == 0){
          forward();
        }
      }
    }else if (gas == 255){ /* reverse */
      if(dir == 1) {
        if(boost == 1) { 
          backwards_right_boost();
        } else if(boost == 0){
          backwards_right();
        }
      } else if(dir == 255) {
        if (boost == 1) { 
          backwards_left_boost();
        } else if(boost == 0){
          backwards_left();
        }
      } else if(dir == 0) {
        if (boost == 1) { 
          backwards_boost();
        } else if(boost == 0){
          backwards();
        }
      }
    }
   
    

    // send back a reply, to the IP address and port we got the packet from
    /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();*/
  } 
}
