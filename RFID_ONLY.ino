
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 0 
#define RST_PIN 2 
#define WIFI_SSID "{{ connection name }}"
#define WIFI_PASSWORD "{{ connection password }}"
#define FIREBASE_HOST "{{ your firebase project host }}"
#define FIREBASE_AUTH "{{ your firebase project auth code }}"

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
LiquidCrystal_I2C lcd(0x3F,16,2);
void setup() 
{
  pinMode(16,OUTPUT);
  lcd.init();
  lcd.backlight();
  Wire.begin();
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  wifiConnect();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  delay(10);
}
void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("    WELCOME!");
  lcd.setCursor(0,1);
  lcd.print("Scan ID to Enter ");
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  lcd.setCursor(0,0);
  lcd.print("Scanning...");
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();

  Firebase.setString("RFID_TAG/"+content,content);
  Serial.println("Stored "+content.substring(1));
  Firebase.setString("VERF_TAG/temp/key","temp key");
  Firebase.setString("VERF_TAG/temp/name","temp name");
  if (Firebase.failed()) {  
      Serial.print("failed");  
      Serial.println(Firebase.error());    
      lcd.clear();
      return;  
  }  
  Serial.println();
  const ArduinoJson::JsonObject& obj = Firebase.get("VERF_TAG").getJsonVariant().asObject();
  if(Firebase.success()){
    for(auto kv : obj){
  //String dat=kv.value.as<String>();
   ArduinoJson::JsonObject& dat=kv.value.asObject();
  if (" "+content.substring(1) == dat.get<String>("key")) //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome "+dat.get<String>("name"));
    Serial.println("Received "+dat.get<String>("key"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" Access Granted ");
    lcd.setCursor(0,1);
    lcd.print(" Welcome "+dat.get<String>("name"));
    String i=" Welcome "+dat.get<String>("name");
    int ki=0;
    for(int j=i.length()-16; j>=0;j--){
      ki+=1;
      lcd.setCursor(ki,0);
      lcd.print(" Access Granted ");
      lcd.scrollDisplayLeft();
      delay(300);
    }
    digitalWrite(16,HIGH);
    delay(3000);
    digitalWrite(16,LOW);
    lcd.clear();
    Serial.println();
    statuss = 1;
    return;
  }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Access Denied "); 
  Serial.println(" Access Denied ");
  delay(2000);
  delay(1000);
  lcd.clear();
 }
  

   if(WiFi.status() != WL_CONNECTED)
  {
  wifiConnect();
  }
delay(10);
} 



void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
