//Library SPI              
#include <SPI.h>

//Library LCD
#include <LiquidCrystal_I2C.h>

//Library Web Server
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

//Library Sensor PZEM004T
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(Serial2, /*receivePin*/ 16, /*transmitPin*/ 17, /*addr*/ PZEM_DEFAULT_ADDR);

//GPIO LED
const int ledMerah = 27; //GPIO 27
const int ledKuning = 26; //GPIO 26
const int ledPutih = 25; //GPIO 25

//setting PWM properties
const int frekuensi = 5000;
const int ledChannelKuning = 0;
const int resolution = 8;

//Multitasking
TaskHandle_t Task1;
TaskHandle_t Task2;

//Inisialisasi Aktuator
const int relay = 18;
const int PIN_RST = 15;
const int PIN_SS  = 4;
const int BUZZER  = 2;

float daya, energi, tegangan, arus;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Interkoneksi WiFi
const char* ssid = "elisha";
const char* password = "Tethering";

//AsyncWebServer object pada port 80
AsyncWebServer server(80);

String getTegangan(){
    return String(tegangan);
}

String getArus(){
    return String(arus);
}

String getDaya(){
    return String(daya);
}

String getEnergi(){
    return String(energi);
}


void setup() {
  Serial.begin(115200);
  SPI.begin();
  pinMode(ledPutih, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.blink_on();

  //konfigurasi fungsi LED PWM
  ledcSetup(ledChannelKuning, frekuensi, resolution);

  //menghubungkan channel ke GPIO untuk bisa dikontrol
  ledcAttachPin(ledKuning,ledChannelKuning);
 
  //SPIFFS
  if (! SPIFFS.begin (true)) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }
   
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
   
  // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());


  // Route for web page
  server.on ("/", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send (SPIFFS, "/Webserver.html");
  });
 
  server.on ("/style.css", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send (SPIFFS, "/style.css", "text/css");
  });
 
  server.on("/image.png", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/image.png", "image/png");
  });
 
  server.on("/otomated.png", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/otomated.png", "otomated/png");
  });
   
  server.on ("/chart.js", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send (SPIFFS, "/chart.js", "text/js");
  });
 
  server.on ("/app.js", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send (SPIFFS, "/app.js", "app/js");
  });
 
  server.on ("/tegangan", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send_P (200, "text / plain", getTegangan().c_str ());
  });
 
  server.on ("/arus", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send_P (200, "text / plain", getArus().c_str ());
  });
 
  server.on ("/daya", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send_P (200, "text / plain", getDaya().c_str ());
  });
 
  server.on ("/energi", HTTP_GET, [] (AsyncWebServerRequest * request) {
  request-> send_P (200, "text / plain", getEnergi().c_str ());
  });


  // start server
  server.begin ();

  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
 
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
}


void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());


  for(;;){
    //menghitung besaran listrik
    tegangan = pzem.voltage(); //volt
    arus = pzem.current(); //ampere
    daya = pzem.power(); //watt
    energi = pzem.energy(); //kWh
    vTaskDelay(50 / portTICK_PERIOD_MS);


  //daya alat yang diukur rendah
    if(daya < 300){
      digitalWrite(ledPutih, HIGH);
      digitalWrite(ledKuning, LOW);
      digitalWrite(ledMerah, LOW);
  //daya alat yang diukur sedang
    } else if(daya >= 300 && daya < 500) {
      digitalWrite(ledPutih, LOW);
      digitalWrite(ledKuning, HIGH);
      digitalWrite(ledMerah, LOW);
  //daya alat yang diukur tinggi
    } else{
      digitalWrite(ledPutih, LOW);
      digitalWrite(ledKuning, LOW);
      digitalWrite(ledMerah, HIGH);

    if(energi >= 0.5 || daya > 500){
      digitalWrite(ledMerah, HIGH);
      for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
        // pengubahan brightnedd LED dengan PWM
        ledcWrite(ledChannelKuning, dutyCycle);
        delay(15);
      }
      // menurunkan LED brightness
      for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
        // ubah LED brightness dengan PWM
        ledcWrite(ledChannelKuning, dutyCycle);  
        delay(10);
      }  
    }
  }
}
}


void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    Serial.print("Daya : ");
    Serial.print(daya);
    Serial.println(" W");
    Serial.print("Energi : ");
    Serial.print(energi);
    Serial.println(" kWh");
    Serial.print("Tegangan : ");
    Serial.print(tegangan);
    Serial.println(" V");
    Serial.print("Arus : ");
    Serial.print(arus);
    Serial.println(" A");
    Serial.println(WiFi.localIP());
    Serial.println("");


// Buzzer berdasarkan paraameter daya
if (daya >= 500) {
  digitalWrite(BUZZER, HIGH);
    } else if (daya >= 300 && daya < 500) {
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(BUZZER, LOW);
    } else {
  digitalWrite(BUZZER, LOW);
   } vTaskDelay(60 / portTICK_PERIOD_MS);
  }
}



void displayParameter_Listrik(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms :");
  lcd.setCursor(7, 0);
  lcd.print(tegangan);
  lcd.print(" V          ");
  lcd.setCursor(0, 1);
  lcd.print("Irms :");
  lcd.setCursor(7, 1);
  lcd.print(arus);
  lcd.print(" A          ");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Daya  :");
  lcd.setCursor(8, 0);
  lcd.print(daya);
  lcd.print(" W          ");
  lcd.setCursor(0, 1);
  lcd.print("Energi:");
  lcd.setCursor(8, 1);
  lcd.print(energi);
  lcd.print(" kWh          ");
  delay(3500);
}


void loop() {
   if (isnan(tegangan) && isnan(arus) && isnan(daya) && isnan(energi)) {
      Serial.print("Gagal Membaca Parameter Listrik");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gagal Membaca");
      lcd.setCursor(0, 1);
      lcd.print("Param. Listrik");
    } else {
      displayParameter_Listrik();
      displayParameter_Listrik();
      displayParameter_Listrik();
      displayParameter_Listrik();
      displayParameter_Listrik();
    }
    Serial.println();
    delay(3500);
    lcd.clear();
  }
