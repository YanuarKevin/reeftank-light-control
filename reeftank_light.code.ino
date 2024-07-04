#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

WidgetLCD lcd1(V0);

char auth[] = "Vgmoy3JDJnoFyxPRfp7KHFXnr5CBUiMB";
char ssid[] = "OKE-JON-1";
char pass[] = "banyuwangi";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000);  // 25200 seconds for GMT+7 (WIB)

#define PWM_PIN_1 12
#define PWM_PIN_2 13
#define PWM_PIN_3 14

String pwmValue_1 = 0;
String pwmValue_2 = 0;
String pwmValue_3 = 0;

String Pwm_A1 = 0;
String Pwm_B1 = 0;
String Pwm_C1 = 0;
String Pwm_A2 = 0;
String Pwm_B2 = 0;
String Pwm_C2 = 0;
String Pwm_A3 = 0;
String Pwm_B3 = 0;
String Pwm_C3 = 0;
String Pwm_A4 = 0;
String Pwm_B4 = 0;
String Pwm_C4 = 0;

float Nilai_Suhu;

String timeSunrise;
String timePagi;
String timeSiang;
String timeSore;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN_1, OUTPUT);
  pinMode(PWM_PIN_2, OUTPUT);
  pinMode(PWM_PIN_3, OUTPUT);

  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);
  timeClient.begin();
  for (int i = 1; i <= 17; i++) Blynk.syncVirtual(i);
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
  for (int i = 1; i <= 17; i++) Blynk.syncVirtual(i);
  // Blynk.syncVirtual(26);
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
  }

  Serial.println("Waktu Sunrise: " + timeSunrise);
  Serial.println("Waktu Pagi: " + timePagi);
  Serial.println("Waktu Siang: " + timeSiang);
  Serial.println("Waktu Sore: " + timeSore);
}

//input nilai set suhu
BLYNK_WRITE(V1) {
  Nilai_Suhu = param.asFloat();
  Serial.print("SUHU: ");
  Serial.println(Nilai_Suhu, 1);
}

//input nilai presentase & waktu sunrise
BLYNK_WRITE(V2) {
  printTime(0, param);
}
BLYNK_WRITE(V6) {
  Pwm_A1 = param.asInt() * 10.23;
  Serial.print("PWM A1: ");
  Serial.println(Pwm_A1);
}
BLYNK_WRITE(V7) {
  Pwm_B1 = param.asInt() * 10.23;
  Serial.print("PWM B1: ");
  Serial.println(Pwm_B1);
}
BLYNK_WRITE(V8) {
  Pwm_C1 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C1);
}

//input nilai presentase & waktu day light
BLYNK_WRITE(V3) {
  printTime(1, param);
}
BLYNK_WRITE(V9) {
  Pwm_A2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A2);
}
BLYNK_WRITE(V10) {
  Pwm_B2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B2);
}
BLYNK_WRITE(V11) {
  Pwm_C2 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C2);
}

//input nilai presentase & waktu sunrise
BLYNK_WRITE(V4) {
  printTime(2, param);
}
BLYNK_WRITE(V12) {
  Pwm_A3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A3);
}
BLYNK_WRITE(V13) {
  Pwm_B3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B3);
}
BLYNK_WRITE(V14) {
  Pwm_C3 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C3);
}

//input nilai presentase & waktu moon light / end time
BLYNK_WRITE(V5) {
  printTime(3, param);
}
BLYNK_WRITE(V15) {
  Pwm_A4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_A4);
}
BLYNK_WRITE(V16) {
  Pwm_B4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_B4);
}
BLYNK_WRITE(V17) {
  Pwm_C4 = param.asInt() * 10.23;
  Serial.print("PWM C1: ");
  Serial.println(Pwm_C4);
}


void adjustAndFadePWM() {
  int currentHour = hour();
  int currentMinute = minute();

  int sunriseHour = getHourFromString(timeSunrise);
  int sunriseMinute = getMinuteFromString(timeSunrise);

  int pagiHour = getHourFromString(timePagi);
  int pagiMinute = getMinuteFromString(timePagi);
  
  int siangHour = getHourFromString(timeSiang);
  int siangMinute = getMinuteFromString(timeSiang);

  int soreHour = getHourFromString(timeSore);
  int soreMinute = getMinuteFromString(timeSore);
 

  if (currentHour == sunriseHour && currentMinute == sunriseMinute) {
    pwmValue_1 = Pwm_A1;
    pwmValue_2 = Pwm_B1;
    pwmValue_3 = Pwm_C1;
  } else if (currentHour == pagiHour && currentMinute == pagiMinute) {
    sigmoidFade(Pwm_A1, Pwm_A2, Pwm_B1, Pwm_B2, Pwm_C1, Pwm_C2);
  } else if (currentHour == siangHour && currentMinute == siangMinute) {
    sigmoidFade(Pwm_A2, Pwm_A3, Pwm_B2, Pwm_B3, Pwm_C2, Pwm_C3);
  } else if (currentHour == soreHour && currentMinute == soreMinute) {
    sigmoidFade(Pwm_A3, Pwm_A4, Pwm_B3, Pwm_B4, Pwm_C3, Pwm_C4);
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
  lcd1.print(0, 0, currentTime);
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
