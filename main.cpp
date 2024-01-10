#include <LiquidCrystal.h>  // includes the LiquidCrystal Library
#include <DHT.h>            //implement dht library
#include <string.h>
#include <IRremote.h>
#define Type DHT11  //declaring type of dht; this stays constant
int sensePin = 2;   //what pin we're plugging dht into to measure
DHT HT(sensePin, Type);

IRrecv IR(6);
int greenPin = A2;
int redPin = A1;
int yellowPin = A0;

int count = 0;
//int power = 0;
String status = "open";

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
const int trigPin = 3;
const int echoPin = 4;
long duration;
int distanceInch;
float tempF;
int humidity;

void setup() {
  Serial.begin(9600);  //start Serial monitor]
  IR.enableIRIn();
  HT.begin();        //start dht
  lcd.begin(16, 2);  // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  //delay(500);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceInch = duration * 0.0133 / 2;
  humidity = HT.readHumidity();      //dht11 reads humidity using funct from library
  tempF = HT.readTemperature(true);  //reads temperature in Fahrenheit

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempF);
  lcd.print((char)223);
  lcd.print("F");
  lcd.setCursor(10, 0);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(distanceInch);
  lcd.print("in");
  lcd.setCursor(8, 1);
  lcd.print("S:");
  lcd.print(status);

  Serial.println("T: " + String(tempF));
  Serial.println("H: " + String(humidity));
  Serial.print("D: " + String(distanceInch));

  
 delay(1000);  //delay before next measurement

if (IR.decode()) {
 if (IR.decodedIRData.decodedRawData == 0xF30CFF00) {
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, LOW);
      status = "Open";
      Serial.print("S: " + status);
    }
    else if (IR.decodedIRData.decodedRawData == 0xE718FF00){
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, HIGH);
      status = "Quiet";
      Serial.print("S: ");
      Serial.println(status);
    }
    else if (IR.decodedIRData.decodedRawData == 0xA15EFF00){
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);
      digitalWrite(yellowPin, LOW);
      status = "Closed";
      Serial.print("S: ");
      Serial.println(status);
    }
      IR.resume();
    }
  }