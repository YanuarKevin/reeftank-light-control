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

float pwmValue_1;
float pwmValue_2;
float pwmValue_3;

float Pwm_A1;
float Pwm_B1;
float Pwm_C1;
float Pwm_A2;
float Pwm_B2;
float Pwm_C2;
float Pwm_A3;
float Pwm_B3;
float Pwm_C3;
float Pwm_A4;
float Pwm_B4;
float Pwm_C4;
float Nilai_Suhu;

String time_sunrise;
String time_daylight;
String time_sunset;
String time_moonlight;

unsigned long previousMillis = 0;
const long interval = 60000;  // Interval pemeriksaan waktu dalam milidetik

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
  // Blynk.syncVirtual(20);
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
      time_sunrise = String(WAKTU);
      break;
    case 1:
      time_daylight = String(WAKTU);
      break;
    case 2:
      time_sunset = String(WAKTU);
      break;
    case 3:
      time_moonlight = String(WAKTU);
      break;
  }

  Serial.println("Waktu Sunrise: " + time_sunrise);
  Serial.println("Waktu Dayligt: " + time_daylight);
  Serial.println("Waktu Sunset: " + time_sunset);
  Serial.println("Waktu Moonlight: " + time_moonlight);
}

//input nilai set suhu
BLYNK_WRITE(V1) {
  Nilai_Suhu = param.asFloat();
  Serial.print("SUHU: ");
  Serial.println(Nilai_Suhu, 1);
}
// ..................................BATAS............................................

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
// ..................................BATAS............................................

//input nilai presentase & waktu day light
BLYNK_WRITE(V3) {
  printTime(1, param);
}
BLYNK_WRITE(V9) {
  Pwm_A2 = param.asInt() * 10.23;
  Serial.print("PWM A2: ");
  Serial.println(Pwm_A2);
}
BLYNK_WRITE(V10) {
  Pwm_B2 = param.asInt() * 10.23;
  Serial.print("PWM B2: ");
  Serial.println(Pwm_B2);
}
BLYNK_WRITE(V11) {
  Pwm_C2 = param.asInt() * 10.23;
  Serial.print("PWM C2: ");
  Serial.println(Pwm_C2);
}
// ..................................BATAS............................................

//input nilai presentase & waktu sunrise
BLYNK_WRITE(V4) {
  printTime(2, param);
}
BLYNK_WRITE(V12) {
  Pwm_A3 = param.asInt() * 10.23;
  Serial.print("PWM A3: ");
  Serial.println(Pwm_A3);
}
BLYNK_WRITE(V13) {
  Pwm_B3 = param.asInt() * 10.23;
  Serial.print("PWM B3: ");
  Serial.println(Pwm_B3);
}
BLYNK_WRITE(V14) {
  Pwm_C3 = param.asInt() * 10.23;
  Serial.print("PWM C3: ");
  Serial.println(Pwm_C3);
}
// ..................................BATAS............................................

//input nilai presentase & waktu moon light / end time
BLYNK_WRITE(V5) {
  printTime(3, param);
}
BLYNK_WRITE(V15) {
  Pwm_A4 = param.asInt() * 10.23;
  Serial.print("PWM A4: ");
  Serial.println(Pwm_A4);
}
BLYNK_WRITE(V16) {
  Pwm_B4 = param.asInt() * 10.23;
  Serial.print("PWM B4: ");
  Serial.println(Pwm_B4);
}
BLYNK_WRITE(V17) {
  Pwm_C4 = param.asInt() * 10.23;
  Serial.print("PWM C4: ");
  Serial.println(Pwm_C4);
}
// ..................................BATAS............................................

// RUBAH NILAI PWM
void adjustAndFadePWM() {
 unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
// MENDAPATKAN VARIABEL JAM & MENIT
  int currentHour = hour();
  int currentMinute = minute();
  int sunrise_Hour = getHourFromString(time_sunrise);
  int sunrise_Minute = getMinuteFromString(time_sunrise);
  int daylight_Hour = getHourFromString(time_daylight);
  int daylight_Minute = getMinuteFromString(time_daylight);
  int sunset_Hour = getHourFromString(time_sunset);
  int sunset_Minute = getMinuteFromString(time_sunset);
  int moonlight_Hour = getHourFromString(time_moonlight);
  int moonlight_Minute = getMinuteFromString(time_moonlight);
// ..................................BATAS............................................
  
//PROSES MERUBAH NILAI PWM 
  if (currentHour == sunrise_Hour && currentMinute == sunrise_Minute) {
    // pwmValue_1 = Pwm_A1;
    // pwmValue_2 = Pwm_B1;
    // pwmValue_3 = Pwm_C1;
    sigmoidFade(pwmValue_1, Pwm_A1, pwmValue_2, Pwm_B1, pwmValue_3, Pwm_1); 
  } else if (currentHour == daylight_Hour && currentMinute == daylight_Minute) {
    sigmoidFade(Pwm_A1, Pwm_A2, Pwm_B1, Pwm_B2, Pwm_C1, Pwm_C2); 
  } else if (currentHour == sunset_Hour && currentMinute == sunset_Minute) {
    sigmoidFade(Pwm_A2, Pwm_A3, Pwm_B2, Pwm_B3, Pwm_C2, Pwm_C3);
  } else if (currentHour == moonlight_Hour && currentMinute == moonlight_Minute) {
    sigmoidFade(Pwm_A3, Pwm_A4, Pwm_B3, Pwm_B4, Pwm_C3, Pwm_C4);
  } else {
    Pwm_A4 = pwmValue_1;
    Pwm_B4 = pwmValue_2;
    Pwm_C4 = pwmValue_3;
    analogWrite(PWM_PIN_1, (int)pwmValue_1);
    analogWrite(PWM_PIN_2, (int)pwmValue_2);
    analogWrite(PWM_PIN_3, (int)pwmValue_3);
  }
// ..................................BATAS............................................

// MENETAPKAN NILAI PWM KE PWM 
  // analogWrite(PWM_PIN_1, pwmValue_1);
  // analogWrite(PWM_PIN_2, pwmValue_2);
  // analogWrite(PWM_PIN_3, pwmValue_3);

  // Serial.println("PWM 1: " + String(pwmValue_1));
  // Serial.println("PWM 2: " + String(pwmValue_2));
  // Serial.println("PWM 3: " + String(pwmValue_3));
 }
}
// ..................................BATAS............................................

void sigmoidFade(int startA, int endA, int startB, int endB, int startC, int endC) {
  int duration = 3600000;  // Durasi 1 jam dalam milidetik
  int steps = 1000;
  for (int i = 0; i <= steps; i++) {
    float progress = (float)i / (float)steps;
    float fadeA = startA + (endA - startA) * (0.5 * (1 + tanh(12 * (progress - 0.5))));
    float fadeB = startB + (endB - startB) * (0.5 * (1 + tanh(12 * (progress - 0.5))));
    float fadeC = startC + (endC - startC) * (0.5 * (1 + tanh(12 * (progress - 0.5))));
    
    analogWrite(PWM_PIN_1, (int)fadeA);
    analogWrite(PWM_PIN_2, (int)fadeB);
    analogWrite(PWM_PIN_3, (int)fadeC);

    delay(duration / steps);
  }
}
// MERUBAH DENGAN HALUS
// void sigmoidFade(float pwm1_start, float pwm1_end, float pwm2_start, float pwm2_end, float pwm3_start, float pwm3_end) {
//   float t = 0.0;
//   while (t <= 1.0) {
//     float pwm1 = pwm1_start + (pwm1_end - pwm1_start) * sigmoid(t);
//     float pwm2 = pwm2_start + (pwm2_end - pwm2_start) * sigmoid(t);
//     float pwm3 = pwm3_start + (pwm3_end - pwm3_start) * sigmoid(t);
    
//     analogWrite(PWM_PIN_1, pwm1);
//     analogWrite(PWM_PIN_2, pwm2);
//     analogWrite(PWM_PIN_3, pwm3);
    
//     t += 0.01;
//     delay(10);
//   }
// }

// float sigmoid(float x) {
//   return 1 / (1 + exp(-x));
// }
// ..................................BATAS............................................

// KIRIM KE LCD BLYNK
void sendLCDMessage() {
  char currentTime[9];
  sprintf(currentTime, "%02d:%02d:%02d", hour(), minute(), second());
  lcd1.print(0, 0, currentTime);
}
// ..................................BATAS............................................

// RUBAH VARIABEL KE JAM
int getHourFromString(String timeString) {
  int colonIndex = timeString.indexOf(':');
  String hourString = timeString.substring(0, colonIndex);
  return hourString.toInt();
}
// ..................................BATAS............................................

// RUBAH VARIABEL KE MENIT
int getMinuteFromString(String timeString) {
  int colonIndex1 = timeString.indexOf(':');
  int colonIndex2 = timeString.lastIndexOf(':');
  String minuteString = timeString.substring(colonIndex1 + 1, colonIndex2);
  return minuteString.toInt();
}
// ..................................BATAS............................................
