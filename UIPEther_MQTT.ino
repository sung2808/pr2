#include <UIPEthernet.h> // Used for Ethernet
#include <PubSubClient.h>
//
//#define lamp 7


const char* mqtt_server = "192.168.1.3";
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                      
EthernetClient Ethclient;
PubSubClient client(Ethclient);

char server[] = "192.168.1.99"; // IP Adres (or name) of server to dump data to
uint8_t IP[]={192,168,1,10};
const int lamp = 7;
void callback(char* topic, byte* message, unsigned int length);
void reconnect();




void setup() {
  Serial.begin(9600);
  pinMode(lamp, OUTPUT);
  Ethernet.begin(mac,IP);

  Serial.println("-----day la code gui data len server-----");
  Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("arduinoClient");
 
}

//-----------------------------------------------------
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
 Serial.println("Topic is: "+String(topic));
  if((String)topic=="room/lamp"){
      Serial.print("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, LOW);
        Serial.print(" lamp On");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, HIGH);
        Serial.print("lamp Off");
      }
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    
    if (client.connect("arduinoClient")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
