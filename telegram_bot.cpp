#include "telegram_bot.h"

TelegramBot::TelegramBot() :
  _bot_token("6484152030:AAEijHYXmF9DDCc-l3TIOutcDamgzHTpgbs"), 
  _chat_id("1011605098"),
  _telegram_url("https://api.telegram.org/bot" + String(_bot_token) + "/sendMessage") {}

const char* TelegramBot::getBotToken() {
  return _bot_token;
}

const char* TelegramBot::getChatId() {
  return _chat_id;
}

String TelegramBot::getTelegramUrl() {
  return _telegram_url;
}

String TelegramBot::getQuery() {
    return _query;
}

void TelegramBot::setMessage(String message) {
  _query = "chat_id=" + String(_chat_id) + "&text=" + urlEncode(message);
}

void TelegramBot::sendMessageToTelegram(String telegram_url, String query, String message) {
  telegram_url = getTelegramUrl();
  setMessage(message);
  query = getQuery();
  
  HTTPClient http;
  http.begin(telegram_url);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Define HTTP Response Code Variable
  int response = http.POST(query);

  if (response == 200) {
    Serial.println("Pengiriman pesan menuju telegram berhasil!");
    Serial.println("Pesan yang terkirim : ");
    Serial.println(message);
  } else {
    Serial.print("Pengiriman pesan menuju telegram gagal dengan response code : ");
    Serial.println(response);
  }

  http.end();
}