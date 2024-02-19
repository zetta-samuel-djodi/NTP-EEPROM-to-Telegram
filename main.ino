#include <WiFi.h>    
#include <WiFiUdp.h>
#include "telegram_bot.h"
#include "NTPClient.h"

TelegramBot telegramBot;

const char* ssid = "realme 7";
const char* password = "28112020";
const char* bot_token = telegramBot.getBotToken();
const char* chat_id = telegramBot.getChatId();
String telegram_url = telegramBot.getTelegramUrl();
String message;
String query = telegramBot.getQuery();

String currentTimeStamp;
String currentDateStamp;

struct TimeStamp {
  String Hour;
  String Minute;
  String Second;
};

struct DateStamp {
  int monthIndex;
  int dayIndex;
  String Date;
  String Day;
  String Month;
  String Year;
};

const unsigned int minute = 60;
const unsigned int hour = 60 * minute;
const unsigned int wita = 8 * hour;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", wita);

void wifiSetup(void);
void ntpSetup(void);

void setup() {
  Serial.begin(115200);
  wifiSetup();
  ntpSetup();
  telegramBot.sendMessageToTelegram(telegram_url, query, message);
}

void loop() {
}

void wifiSetup(void) {
  Serial.print("Connecting to Wifi SSID : ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to Wifi!");
  Serial.print("ESP 32 Local IP Address : ");
  Serial.println(WiFi.localIP());
}
void ntpSetup(void) {
  timeClient.begin();
  timeClient.setTimeOffset(wita);
  timeClient.update();

  struct TimeStamp timeStamp;
  struct DateStamp dateStamp;

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

  String customTimeStamp = timeStamp.Hour + ":" + timeStamp.Minute + ":" + timeStamp.Second;
  String customDate = dateStamp.Day + ", " + dateStamp.Date + " " + dateStamp.Month + " " + dateStamp.Year;
  
  message = "Halo Bot Telegram, aku punya pesan untukmu!\n";
  message += "This time is : ";
  message += customTimeStamp;
  message += "\nOn ";
  message += customDate;
  Serial.print("Ukuran message : ");
  Serial.println(message.length());
}
