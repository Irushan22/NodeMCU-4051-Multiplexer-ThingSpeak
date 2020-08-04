  #include <ESP8266WiFi.h> //wifi library
  #include "ThingSpeak.h" //include ThingSpeak library
  
  #define MUX_A D4
  #define MUX_B D3
  #define MUX_C D2
  
  #define ANALOG_INPUT A0
  
  unsigned long muchnum = ; //Thingspeak channel ID
  const char* mywriteapi = ""; //Thingspeak API key
  
  const char* WIFI_SSID = "";  //your wifi username
  const char* WIFI_PASSWORD = ""; //your wifi password
  
  float forefoot;
  float Midfoot;
  float Rarefoot;

 WiFiClient client; //declare wifi client

void setup() {
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT);     
  // put your setup code here, to run once:
  Serial.begin(115000);
  ThingSpeak.begin(client); //staring thingspeak with setup
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //call wifi crediantials
}
 
 void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}   


void loop() {
  // put your main code here, to run repeatedly:
   changeMux(LOW, LOW, LOW);
   forefoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 0 pin of Mux
   Serial.print("forefoot strike - ");Serial.println(forefoot);
   
   changeMux(LOW, LOW, HIGH);
   Midfoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 2 pin of Mux
   Serial.print("midfoot strike - ");Serial.println(Midfoot);

   changeMux(LOW, HIGH, LOW);
   Rarefoot = analogRead(ANALOG_INPUT); //Value of the sensor connected Option 1 pin of Mux
   Serial.print("rarefoot strike - ");Serial.println(Rarefoot);
   Serial.println();
  

    
//Thingspeak connect sensor value with fields

  ThingSpeak.setField(1,forefoot); 
  ThingSpeak.setField(2,Midfoot); 
  ThingSpeak.setField(3,Rarefoot);
  ThingSpeak.writeFields(muchnum,mywriteapi);
  
                  
  delay(500);

}
