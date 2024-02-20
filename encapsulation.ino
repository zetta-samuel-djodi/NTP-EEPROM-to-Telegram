#include <WiFi.h>    
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "eepromSetup.h"
#include "telegram_bot.h"
#include "NTPClient.h"
#include <ArduinoOTA.h>

#define EEPROM_SIZE 512

TelegramBot telegramBot;

const char* ssid = "realme 7";
const char* password = "28112020";
const char* bot_token = telegramBot.getBotToken();
const char* chat_id = telegramBot.getChatId();
String telegram_url = telegramBot.getTelegramUrl();
String message;
String query = telegramBot.getQuery();

int esp32Address = 0;

String currentTimeStamp;
String currentDateStamp;
String disconnectedMessage;
String esp32StatusMessage;

String customDate;
String customTimeStamp;

String disconnectedDate;
String disconnectedTime;
String reconnectedDate;
String reconnectedTime;

String esp32Data;
char get_eeprom_esp32Data[150];
char eeprom_esp32Data[150];

enum WifiState {
  CONNECTED,
  DISCONNECTED,
  RECONNECTING,
} wifiState;

struct TimeStamp {
  String Hour;
  String Minute;
  String Second;
} timeStamp;

struct DateStamp {
  int monthIndex;
  int dayIndex;
  String Date;
  String Day;
  String Month;
  String Year;
} dateStamp;

const unsigned int minute = 60;
const unsigned int hour = 60 * minute;
const unsigned int wita = 8 * hour;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", wita);
EEPROMFeatures EEPROMF(esp32Address, EEPROM_SIZE);

void wifiSetup(void);
void ntpData(void);
void sendMessageToTelegram(String message);
void outputDCTime(String state);
void outputReconnectTime(String state);
String reconnectedMessage(String disconnectTime, String reconnectedTime);

void setup() {
  Serial.begin(115200);
  if(EEPROMF.begin() == false){
    ESP.restart();
  }
  wifiNTPSetup();
  ntpData();
  Serial.println("\nget_eeprom_esp32Data : ");
  Serial.println(EEPROMF.readString(0));
  Serial.println("\n");

  ArduinoOTA.setHostname("sam-esp32");
  ArduinoOTA.setPassword("samESP32");
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  ntpData();
  EEPROMF.writeString(0, esp32StatusMessage);

  if(WiFi.status() != WL_CONNECTED) {
    if (wifiState != DISCONNECTED && wifiState != RECONNECTING) {
        wifiState = RECONNECTING;
        disconnectedDate = customDate;
        disconnectedTime = customTimeStamp;
        outputDCTime(disconnectedDate, disconnectedTime);
    } else {
      wifiState = RECONNECTING;
    }
  } else {
    if (wifiState == RECONNECTING) {
      wifiState = CONNECTED;
      reconnectedDate = customDate;
      reconnectedTime = customTimeStamp;
      outputReconnectTime(reconnectedDate, reconnectedTime);
      telegramBot.sendMessageToTelegram(telegram_url, query, reconnectedMessage(disconnectedDate, disconnectedTime, reconnectedDate, reconnectedTime));
    }
  }
  delay(500);
}

void wifiNTPSetup(void) {
  Serial.print("Connecting to Wifi SSID : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi...");
  }

  wifiState = CONNECTED;
  Serial.println("Connected to Wifi!");
  Serial.print("ESP 32 Local IP Address : ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(wita);
}

void ntpData(void) {
  timeClient.update();

  currentTimeStamp = timeClient.getFormattedTime();
  currentDateStamp = timeClient.getFormattedDate();

  timeStamp.Hour = timeClient.getHours();
  timeStamp.Minute = timeClient.getMinutes();
  timeStamp.Second = timeClient.getSeconds();

  dateStamp.monthIndex = timeClient.getMonth();
  dateStamp.dayIndex = timeClient.getDay();

  dateStamp.Date = timeClient.getDate();
  dateStamp.Day = timeClient.getDayName();
  dateStamp.Month = timeClient.getMonthName();
  dateStamp.Year = timeClient.getYear();

  customTimeStamp = timeStamp.Hour + ":" + timeStamp.Minute + ":" + timeStamp.Second;
  customDate = dateStamp.Day + ", " + dateStamp.Date + " " + dateStamp.Month + " " + dateStamp.Year;

  esp32StatusMessage = "Status ESP 32\n";
  esp32StatusMessage += "ESP 32 last online time :\n";
  esp32StatusMessage += disconnectedDate;
  esp32StatusMessage += " - ";
  esp32StatusMessage += disconnectedTime;
  esp32StatusMessage += "\nESP 32 last power on :\n";
  esp32StatusMessage += customDate;
  esp32StatusMessage += " - ";
  esp32StatusMessage += customTimeStamp;
  esp32StatusMessage += "\n";
}

String reconnectedMessage(String disconnectedDate, String disconnectedTime, String reconnectedDate, String reconnectedTime) {
  String reconnectMessage = "Device disconnected at : " + disconnectedDate + " - " +  disconnectedTime + "\n" + "Device Back Online at : " + reconnectedDate + " - " + reconnectedTime;
  return reconnectMessage;
}

void outputDCTime(String date, String time) {
  Serial.print("Disconnected at : ");
  Serial.print(date);
  Serial.print(" - ");
  Serial.println(time);
}

void outputReconnectTime(String date, String time) {
  Serial.print("Reconnected at : ");
  Serial.print(date);
  Serial.print(" - ");
  Serial.println(time);
}
