
#include <MPU6050_tockn.h>
#include <Wire.h>
//------------------------ MQTT WIFI
#include <PubSubClient.h>
#include <WiFi.h>

// WiFi stuff
const char* ssid = "WiFi_Name";
const char* pwd = "password";
const IPAddress ip( ***, ***, ***, *** );// The IP assigned to the ESP32
const IPAddress gateway( ***, ***, *, *);
const IPAddress subnet(255, 255, 255, 0);
const char* server = "************"; // The IP where mosquitto is running
WiFiClient espClient;
PubSubClient client(espClient);
const int recv_port = 1883;
String clientID = "espStepL";

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[length];
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
}
//----------------------------- MQTT WIFI

MPU6050 mpu6050(Wire);

long timer = 0;
float firstAngleY;

bool up = false;
bool mid = false;
bool bck = false;

int aZ = 0;
float aY = 0;
String temp_str;
char aZmsg [3];

int current;
int abscurrent;
int diferencia;
int absdiff;
int prev;
int absprev;
int result;

void setup() {
  //----------------------------- MQTT
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor;

  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("WiFi connected, IP = ");
  Serial.println(WiFi.localIP());
  client.setServer(server, 1883);
  client.setCallback(callback);
  if (client.connect(clientID.c_str())) {
    Serial.println("connection");
    client.publish("status", "LEFT FOOT CONNECTED");

  }
  //----------------------------- MQTT

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  client.publish("status", "LEFT FOOT READY");

  firstAngleY = mpu6050.getAngleY();
}

void loop() {
  mpu6050.update();

  calculateStep();

  for (int i = 0; i < 30; i++) {
    aZ += mpu6050.getAngleZ();
  }
  aZ /= 30;

  current = aZ;
  abscurrent = abs(current);
  diferencia = abscurrent - absprev;
  absdiff = abs(diferencia);

  if (prev < current) {
    result = absdiff * -1;
  } else {
    result = absdiff;
  }
  // In UE4 you need to feed the angle little by little
  //  bc the orientation works relative to the one in
  //  the last frame.
  temp_str = String (result);
  temp_str.toCharArray (aZmsg, temp_str.length() + 1);

  if (result != 0) {
    Serial.println(aZmsg);
    client.publish("Lorientation", aZmsg);
  }
  client.loop();

  prev = current;
  absprev = abs(prev);
}

void calculateStep() {

  for (int i = 0; i < 30; i++) {
    aY += mpu6050.getAngleY();
  }
  aY /= 30;
  
  //An estimation in degrees of my footsteps
  // -22 --> -5
  if (aY > firstAngleY + 17 && up == false && mid == false && bck == false) {
    //Serial.println("patArriba");
    up = true;
  }

  //-25 --> -20
  if (aY > firstAngleY - 3 && aY < firstAngleY + 3 && up == true && mid == false && bck == false) {
    mid = true;
  }

  if (aY < firstAngleY - 22 && up == true && mid == true && bck == false) {
    bck = true;
  }

  if (up == true && mid == true && bck == true) {
    client.publish("leftstep", "Lstep");
    up = false; mid = false; bck = false;
  }
}
