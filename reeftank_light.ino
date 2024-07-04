#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

WidgetLCD lcd1(V20);
WidgetLCD lcd2(V21);
WidgetLCD lcd3(V22);
WidgetLCD lcd4(V23);
WidgetLCD lcd5(V24);
WidgetLCD lcd6(V25);

char auth[] = "Vgmoy3JDJnoFyxPRfp7KHFXnr5CBUiMB";
char ssid[] = "OKE-JON-1";
char pass[] = "banyuwangi";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);  // 25200 seconds for GMT+7 (WIB)

#define PWM_PIN_1 12
#define PWM_PIN_2 13
#define PWM_PIN_3 14

int pwmValue_1 = 0;
int pwmValue_2 = 0;
int pwmValue_3 = 0;

int Pwm_A1 = 0;
int Pwm_B1 = 0;
int Pwm_C1 = 0;
int Pwm_A2 = 0;
int Pwm_B2 = 0;
int Pwm_C2 = 0;
int Pwm_A3 = 0;
int Pwm_B3 = 0;
int Pwm_C3 = 0;
int Pwm_A4 = 0;
int Pwm_B4 = 0;
int Pwm_C4 = 0;
int Pwm_A5 = 0;
int Pwm_B5 = 0;
int Pwm_C5 = 0;

float Nilai_Suhu;

String timeSunrise;
String timePagi;
String timeSiang;
String timeSore;
String timeSunset;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  pinMode(PWM_PIN_3, OUTPUT);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timeClient.begin();
  for (int i = 0; i <= 19; i++) Blynk.syncVirtual(i);
  Blynk.syncVirtual(26);
}

void loop() {
  Blynk.run();
  updateTime();
  adjustAndFadePWM();
  sendLCDMessage();
}

void updateTime() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  setTime(epochTime);
  delay(1000);
}

BLYNK_CONNECTED() {
  for (int i = 0; i <= 19; i++) Blynk.syncVirtual(i);
  Blynk.syncVirtual(26);
}

void printTime(int pin, BlynkParam param) {
  int value = param.asInt();
  int hours = value / 3600;
  int minutes = (value % 3600) / 60;
  int seconds = value % 60;
  char WAKTU[9];
  sprintf(WAKTU, "%02d:%02d:%02d", hours, minutes, seconds);

  switch (pin) {
    case 0:
      timeSunrise = String(WAKTU);
      break;
    case 1:
      timePagi = String(WAKTU);
      break;
    case 2:
      timeSiang = String(WAKTU);
      break;
    case 3:
      timeSore = String(WAKTU);
      break;
    case 4:
      timeSunset = String(WAKTU);
      break;
  }

  Serial.println("Waktu Sunrise: " + timeSunrise);
  Serial.println("Waktu Pagi: " + timePagi);
  Serial.println("Waktu Siang: " + timeSiang);
  Serial.println("Waktu Sore: " + timeSore);
  Serial.println("Waktu Sunset: " + timeSunset);
}

BLYNK_WRITE(V0) {
  printTime(0, param);
}
BLYNK_WRITE(V1) {
  printTime(1, param);
}
BLYNK_WRITE(V2) {
  printTime(2, param);
}
BLYNK_WRITE(V3) {
  printTime(3, param);
}
BLYNK_WRITE(V4) {
  printTime(4, param);
}

BLYNK_WRITE(V5) {
  Pwm_A1 = param.asInt() * 10.23;
  Serial.print("PWM A1: ");
  Serial.println(Pwm_A1);
}
BLYNK_WRITE(V6) {
  Pwm_B1 = param.asInt() * 10.23;
  Serial.print("PWM B1: ");
  Serial.println(Pwm_B1);
}
BLYNK_WRITE(V7) {
  Pwm_C1 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C1);
}
BLYNK_WRITE(V8) {
  Pwm_A2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A2);
}
BLYNK_WRITE(V9) {
  Pwm_B2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B2);
}
BLYNK_WRITE(V10) {
  Pwm_C2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C2);
}
BLYNK_WRITE(V11) {
  Pwm_A3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A3);
}
BLYNK_WRITE(V12) {
  Pwm_B3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B3);
}
BLYNK_WRITE(V13) {
  Pwm_C3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C3);
}
BLYNK_WRITE(V14) {
  Pwm_A4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A4);
}
BLYNK_WRITE(V15) {
  Pwm_B4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B4);
}
BLYNK_WRITE(V16) {
  Pwm_C4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C4);
}
BLYNK_WRITE(V17) {
  Pwm_A5 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A5);
}
BLYNK_WRITE(V18) {
  Pwm_B5 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B5);
}
BLYNK_WRITE(V19) {
  Pwm_C5 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C5);
}
BLYNK_WRITE(V26) {
  Nilai_Suhu = param.asFloat();
  Serial.print("SUHU: ");
  Serial.println(Nilai_Suhu, 1);
}

void adjustAndFadePWM() {
  int currentHour = hour();
  int currentMinute = minute();
  int currentSecond = second();

  int sunriseHour = getHourFromString(timeSunrise);
  int sunriseMinute = getMinuteFromString(timeSunrise);
  int sunriseSecond = getSecondFromString(timeSunrise);
  int pagiHour = getHourFromString(timePagi);
  int pagiMinute = getMinuteFromString(timePagi);
  int pagiSecond = getSecondFromString(timePagi);
  int siangHour = getHourFromString(timeSiang);
  int siangMinute = getMinuteFromString(timeSiang);
  int siangSecond = getSecondFromString(timeSiang);
  int soreHour = getHourFromString(timeSore);
  int soreMinute = getMinuteFromString(timeSore);
  int soreSecond = getSecondFromString(timeSore);
  int sunsetHour = getHourFromString(timeSunset);
  int sunsetMinute = getMinuteFromString(timeSunset);
  int sunsetSecond = getSecondFromString(timeSunset);

  if (currentHour == sunriseHour && currentMinute == sunriseMinute && currentSecond == sunriseSecond) {
    pwmValue_1 = Pwm_A1;
    pwmValue_2 = Pwm_B1;
    pwmValue_3 = Pwm_C1;
  } else if (currentHour == pagiHour && currentMinute == pagiMinute && currentSecond == pagiSecond) {
    sigmoidFade(Pwm_A1, Pwm_A2, Pwm_B1, Pwm_B2, Pwm_C1, Pwm_C2);
  } else if (currentHour == siangHour && currentMinute == siangMinute && currentSecond == siangSecond) {
    sigmoidFade(Pwm_A2, Pwm_A3, Pwm_B2, Pwm_B3, Pwm_C2, Pwm_C3);
  } else if (currentHour == soreHour && currentMinute == soreMinute && currentSecond == soreSecond) {
    sigmoidFade(Pwm_A3, Pwm_A4, Pwm_B3, Pwm_B4, Pwm_C3, Pwm_C4);
  } else if (currentHour == sunsetHour && currentMinute == sunsetMinute && currentSecond == sunsetSecond) {
    sigmoidFade(Pwm_A4, Pwm_A5, Pwm_B4, Pwm_B5, Pwm_C4, Pwm_C5);
  } else {
    pwmValue_1 = pwmValue_1;
    pwmValue_2 = pwmValue_2;
    pwmValue_3 = pwmValue_3;
  }

  analogWrite(PWM_PIN_1, pwmValue_1);
  analogWrite(PWM_PIN_2, pwmValue_2);
  analogWrite(PWM_PIN_3, pwmValue_3);

  Serial.println("PWM 1: " + String(pwmValue_1));
  Serial.println("PWM 2: " + String(pwmValue_2));
  Serial.println("PWM 3: " + String(pwmValue_3));
}

void sigmoidFade(float pwm1_start, float pwm1_end, float pwm2_start, float pwm2_end, float pwm3_start, float pwm3_end) {
  float t = 0.0;
  while (t <= 1.0) {
    float pwm1 = pwm1_start + (pwm1_end - pwm1_start) * sigmoid(t);
    float pwm2 = pwm2_start + (pwm2_end - pwm2_start) * sigmoid(t);
    float pwm3 = pwm3_start + (pwm3_end - pwm3_start) * sigmoid(t);
    
    analogWrite(PWM_PIN_1, pwm1);
    analogWrite(PWM_PIN_2, pwm2);
    analogWrite(PWM_PIN_3, pwm3);
    
    t += 0.01;
    delay(10);
  }
}

float sigmoid(float x) {
  return 1 / (1 + exp(-x));
}

void sendLCDMessage() {
  char currentTime[9];
  sprintf(currentTime, "%02d:%02d:%02d", hour(), minute(), second());
  lcd1.print(0, 0, timeSunrise);
  lcd1.print(13, 0, "    ");
  lcd1.print(10, 0, "A=" + String(Pwm_A1));
  lcd1.print(4, 1, "    ");
  lcd1.print(1, 1, "B=" + String(Pwm_B1));
  lcd1.print(11, 1, "    ");
  lcd1.print(9, 1, "C=" + String(Pwm_C1));

  lcd2.print(0, 0, timePagi);
  lcd2.print(13, 0, "    ");
  lcd2.print(10, 0, "A=" + String(Pwm_A2));
  lcd2.print(4, 1, "    ");
  lcd2.print(1, 1, "B=" + String(Pwm_B2));
  lcd2.print(11, 1, "    ");
  lcd2.print(9, 1, "C=" + String(Pwm_C2));

  lcd3.print(0, 0, timeSiang);
  lcd3.print(13, 0, "    ");
  lcd3.print(10, 0, "A=" + String(Pwm_A3));
  lcd3.print(4, 1, "    ");
  lcd3.print(1, 1, "B=" + String(Pwm_B3));
  lcd3.print(11, 1, "    ");
  lcd3.print(9, 1, "C=" + String(Pwm_C3));

  lcd4.print(0, 0, timeSore);
  lcd4.print(13, 0, "    ");
  lcd4.print(10, 0, "A=" + String(Pwm_A4));
  lcd4.print(4, 1, "    ");
  lcd4.print(1, 1, "B=" + String(Pwm_B4));
  lcd4.print(11, 1, "    ");
  lcd4.print(9, 1, "C=" + String(Pwm_C4));

  lcd5.print(0, 0, timeSunset);
  lcd5.print(13, 0, "    ");
  lcd5.print(10, 0, "A=" + String(Pwm_A5));
  lcd5.print(4, 1, "    ");
  lcd5.print(1, 1, "B=" + String(Pwm_B5));
  lcd5.print(11, 1, "    ");
  lcd5.print(9, 1, "C=" + String(Pwm_C5));

  lcd6.print(0, 0, currentTime);
}

int getHourFromString(String timeString) {
  int colonIndex = timeString.indexOf(':');
  String hourString = timeString.substring(0, colonIndex);
  return hourString.toInt();
}

int getMinuteFromString(String timeString) {
  int colonIndex1 = timeString.indexOf(':');
  int colonIndex2 = timeString.lastIndexOf(':');
  String minuteString = timeString.substring(colonIndex1 + 1, colonIndex2);
  return minuteString.toInt();
}

int getSecondFromString(String timeString) {
  int colonIndex = timeString.lastIndexOf(':');
  String secondString = timeString.substring(colonIndex + 1);
  return secondString.toInt();
}
