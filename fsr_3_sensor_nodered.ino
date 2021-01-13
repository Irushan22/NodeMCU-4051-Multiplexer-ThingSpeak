#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MUX_A D4
#define MUX_B D3
#define MUX_C D0 //D2 privious

#define ANALOG_INPUT A0

// Replace with your network credentials
const char* ssid     = "";
const char* password = "";

const char* mqtt_server = "test.mosquitto.org";

String forefoot;
String Midfoot;
String Rarefoot;

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more outputs)
      //client.subscribe("esp8266/output");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}


void setup() {
  //Deifne output pins for Mux
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT);   

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}



void loop() {
  if (!client.connected()) {
    String b = "connected";
    client.publish("sprinterdev/shoeleft/constat", b.c_str());
    reconnect();
  }
  client.loop();
  
  
  Serial.begin(9600); 
    
  
    changeMux(LOW, LOW, LOW);
    forefoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux
    Serial.print("forefoot strike - ");Serial.println(forefoot);
  
    changeMux(LOW, HIGH, LOW);
    Midfoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux
    Serial.print("RareFoot strike - ");Serial.println(Midfoot);
     
    changeMux(LOW, LOW, HIGH);
    Rarefoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux
    Serial.print("Midfoot strike - ");Serial.println(Rarefoot);

    client.publish("esp8266/forefoot", forefoot.c_str()); //change the topic
    Serial.println("forefoot published");

    client.publish("esp8266/midfoot", Rarefoot.c_str()); //change the topic
    Serial.println("midfoot published");
    
   client.publish("esp8266/rarefooot", Midfoot.c_str()); //change the topic
    Serial.println("rarefooot published");

  Serial.println("");
 
  delay(1000);
  Serial.println("");

} 
