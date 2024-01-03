#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <UniversalTelegramBot.h>

#define relayPin 26
RF24 radio(4, 5); // CE, CSN
const byte address[6] = "00001";

struct SensorValues
{
  int soilValue;
  // int airValue;
  // float fuzzyValue;
};

SensorValues receivedData;


const char *ssid = "bukan wifi negara";
const char *password = "wiraganteng123";

const char *telegramBotToken = "6745008293:AAG0XWL-2DDJOZkpubI01FB1cdpPBsFZxSs";
const char *telegramChatId = "784528716";

HTTPClient http;

void teleSendMessage(String payload);

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);

  //connect wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
  }
  Serial.println("Connected to WiFi:");
  Serial.println(WiFi.SSID());
  
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(receivedData));

    Serial.println("============================");
    Serial.print("Soil Moisture: ");
    Serial.println(receivedData.soilValue);
    // Serial.print("Air Humidity: ");
    // Serial.println(receivedData.airValue);
    // Serial.print("Fuzzy Result: ");
    // Serial.println(receivedData.fuzzyValue);

    // int durasiPompa = static_cast<int>(receivedData.fuzzyValue);

    // if (durasiPompa > 0) {
    //   digitalWrite(relayPin, LOW); // Pump menyala
    //   Serial.println("Pump ON");

    //   // Tambahkan logika untuk mengatur durasi pompa
    //   // delay(durasiPompa * 1000); // Konversi detik menjadi milidetik
    //   digitalWrite(relayPin, HIGH); // Pump mati setelah durasi tertentu
    //   Serial.println("Pump OFF");
      
    //   teleSendMessage("Pump is ON. Duration: ");
    //   // Kirim pesan ke Telegram
    //   // teleSendMessage("Pump is ON. Duration: " + String(durasiPompa) + " seconds");
    // } else {
    //   digitalWrite(relayPin, HIGH); // Pump mati
    //   Serial.println("Pump OFF");
      
    //   // Kirim pesan ke Telegram
    //   teleSendMessage("Pump is OFF.");
    // }
  }
}

void teleSendMessage(String message) {
  String url = "https://api.telegram.org/bot" + String(telegramBotToken) + "/sendMessage?chat_id=" + String(telegramChatId) + "&text=" + message;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("Telegram message sent successfully");
    } else {
      Serial.println("Failed to send Telegram message");
    }
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();
  delay(5000); // delay to avoid spamming
}