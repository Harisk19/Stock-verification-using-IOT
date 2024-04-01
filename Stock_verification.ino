#include <GSM.h>
#include <MFRC522.h>
#include <SPI.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
const int trigPin = 2; // Change these pin numbers to match your Arduino board.
const int echoPin = 3;
const int trig1Pin = 4;
const int echo1Pin = 5;
int Motor = 0;
int Relay = 0;
byte card_ID[4];
byte Device1[] = {0xB8, 0x85, 0xE5, 0x12};
byte Device2[] = {0x62, 0x18, 0x12, 0x51};
int NumbCard[2];
int j=0; 
String Device;//user name
long Count;//user number
int n ;
GSM gsm; // Create a GSM object
GSM_SMS sms; // Create an SMS object
const char* phone_number = "+91 xxxxxxxxxx";
void setup() {
 Serial.begin(9600); // Initialize serial communications with the PC
 SPI.begin(); // Init SPI bus
 mfrc522.PCD_Init(); // Init MFRC522 card
 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(trig1Pin, OUTPUT);
 pinMode(echo1Pin, INPUT);
 
 Serial.println("CLEARDATA");
 Serial.println("LABEL,Date,Time,Device,Count,RFID UID");
 delay(1000);
 
 Serial.println("Scan PICC to see UID...");
 Serial.println("");
}
31
// --------------------------------------------------------------------
void loop() {
 rfid();
 delay
 Product1();
 Product2();
}
void Product1() {
 delay(500);
 Serial.println("\n");
 int duration, distance;
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = (duration / 2) / 29.1;
 if (distance < 23) {
 Relay = 5;
 Serial.println("\n");
 Serial.print(Relay);
 Serial.print("Relay");
 }
 if (distance > 23 && distance < 35) {
 Relay = 4;
 Serial.println("\n");
 Serial.print(Relay);
 Serial.print("Relay");
 }
 if (distance > 35 && distance < 47) {
 Relay = 3;
 Serial.println("\n");
 Serial.print(Relay);
 Serial.print("Relay");
 }
 if (distance > 47 && distance < 60) {
 Relay = 2;
 Serial.println("\n");
 Serial.print(Relay);
 Serial.print("Relay");
 }
 if (distance > 60 && distance < 70) {
 Relay = 1;
 Serial.println("\n");
31
 Serial.print(Relay);
 Serial.print("Relay");
 } 
 if(distance>70){
 Relay = 0;
 Serial.println("\n");
 Serial.print(Relay);
 Serial.print("Relay");
 }
 if (Relay < 2) { // Example threshold for low stock
 sendSMS("Low stock alert: Relay is running low!");
 }
 else if (Relay == 5) { // Example threshold for low stock
 sendSMS("stock alert: Relay stock is full!");
 }
 else if (Relay == 0) { // Example threshold for low stock
 sendSMS("Empty stock alert: Relay stock is empty!");
 }
}
void Product2() {
 delay(500);
 Serial.println("\n");
 int duration, distance;
 digitalWrite(trig1Pin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trig1Pin, LOW);
 duration = pulseIn(echo1Pin, HIGH);
 distance = (duration / 2) / 29.1;
 if (distance < 23) {
 Motor = 5;
 Serial.println("\n");
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if (distance > 23 && distance < 35) {
 Motor = 4;
 Serial.println("\n");
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if (distance > 35 && distance < 47) {
 Motor = 3;
31
 Serial.println("\n");
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if (distance > 47 && distance < 60) {
 Motor = 2;
 Serial.println("\n");
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if (distance > 60 && distance < 70) {
 Motor = 1;
 Serial.println("\n");
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if(distance>70){
 Motor = 0;
 Serial.print(Motor);
 Serial.print("Motor");
 }
 if (Motor < 2) { // Example threshold for low stock
 sendSMS("Low stock alert: Motor is running low!");
 }
 else if (Motor == 5) { // Example threshold for low stock
 sendSMS("stock alert: Motor stock is full!");
 }
 else if (Motor == 0) { // Example threshold for low stock
 sendSMS("Empty stock alert: Motor stock is empty!");
 }
}
void rfid(){
 if ( ! mfrc522.PICC_IsNewCardPresent()) {
 return;//got to start of loop if there is no card present
}
// Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial()) {
 return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
}
for (byte i = 0; i < mfrc522.uid.size; i++) {
 card_ID[i]=mfrc522.uid.uidByte[i];
 if(card_ID[i]==Device1[i]){
31
 Device="Relay";
 Count=5;
 j=0;//first number in the NumbCard array : NumbCard[j]
 }
 else if(card_ID[i]==Device2[i]){
 Device="Motor";//user name
 Count=5;//user number
 j=1;//Second number in the NumbCard array : NumbCard[j]
 }
 else{
 Serial.println("Wrong info");
 }
}
 if(NumbCard[j] == 1){//to check if the card already detect
 //if you want to use LCD
 //Serial.println("Already Exist");
 }
 else{
 NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the 
arduino know if the card was detecting 
 n++;//(optional)
 Serial.print("DATA,DATE,TIME," + Device);//send the Name to excel
 Serial.print(",");
 Serial.println(Count); //send the Number to excel
 }
 delay(1000);
}
void sendSMS(const char* message) {
 // Send an SMS using the GSM module
 Serial.println("Sending SMS: " + String(message));
 if (sms.beginSMS(phone_number)) {
 sms.print(message);
 sms.endSMS();
 Serial.println("SMS sent!");
 } else {
 Serial.println("SMS sending failed.");
 }
}
// --------------------------------------------------------------------
void array_to_string(byte array[], unsigned int len, char buffer[])
{
 for (unsigned int i = 0; i < len; i++)
 {
 byte nib1 = (array[i] >> 4) & 0x0F;
31
 byte nib2 = (array[i] >> 0) & 0x0F;
 buffer[i*2+0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
 buffer[i*2+1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
 }
 buffer[len*2] = '\0';
