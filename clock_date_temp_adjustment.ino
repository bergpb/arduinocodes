#include <Wire.h>
#include "DHT.h"
#include <RTClib.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define DHTPIN A3 // pino do dht11 conectado
#define DHTTYPE DHT11 // modelo do dth11
DHT dht(DHTPIN, DHTTYPE);

#define OLED_ADDR   0x3C //endereço do oled
RTC_DS1307 RTC; // modelo do rtc
Adafruit_SSD1306 display(-1); // modelo do display

// checa tamanho do display
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// variáveis
int hourupg;
int minupg;
int yearupg;
int monthupg;
int dayupg;
int menu = 0;

void setup()
{
  pinMode(6, INPUT); // set menu
  pinMode(7, INPUT); // button +
  pinMode(8, INPUT); //button -

  Wire.begin();
  RTC.begin();
  dht.begin();

  if (! RTC.isrunning()){
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
}

void loop(){

  if (digitalRead(6)){
    menu = menu + 1;
  }
  if (menu == 0){
    DisplayDateTimeTemp();
  }
  if (menu == 1){
    DisplaySetHour();
  }
  if (menu == 2){
    DisplaySetMinute();
  }
  if (menu == 3){
    DisplaySetDay();
  }
  if (menu == 4){
    DisplaySetMonth();
  }
  if (menu == 5){
    DisplaySetYear();
  }
  if (menu == 6){
    StoreAgg();
    menu = 0;
  }
  delay(50);
}

void DisplayDateTimeTemp (){
  DateTime now = RTC.now();

  const char daysOfTheWeek[7][4] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  if (now.hour() <= 9){
    display.print("0");

  }
  display.print(now.hour(), DEC);
  hourupg = now.hour();

  display.print(":");
  if (now.minute() <= 9){
    display.print("0");

  }

  display.print(now.minute(), DEC);
  minupg = now.minute();

  display.print(":");
  if (now.second() <= 9){
    display.print("0");

  }

  display.print(now.second(), DEC);
  display.setCursor(5, 20);
  if (now.day() <= 9){
    display.print("0");

  }

  display.print(now.day(), DEC);
  dayupg = now.day();
  display.print("/");
  if (now.month() <= 9){
    display.print(now.day(), DEC);

  }

  display.print(now.month(), DEC);
  monthupg = now.month();
  display.print("/");

  display.print(now.year(), DEC);
  yearupg = now.year();
  display.setCursor(70, 20);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  
  if (isnan(t) || isnan(h)) 
  {
    display.setCursor(5, 35);
    display.print("Falha ao ler dados do sensor");
  }
  else {
    display.setCursor(5, 35);
    display.print("T: ");
    display.print(t);
    display.print((char)247);
    display.setCursor(5, 45);
    display.print("H: ");
    display.print(h);
  }
  
  display.display();
  delay(100);
}

void DisplaySetHour(){
  // time setting
  DateTime now = RTC.now();
  if (digitalRead(7) == HIGH){
    if (hourupg == 23){
      hourupg = 0;
    }
    else{
      hourupg = hourupg + 1;
    }
  }
  if (digitalRead(8) == HIGH){
    if (hourupg == 0){
      hourupg = 23;
    }
    else{
      hourupg = hourupg - 1;
    }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Horas: ");
  display.print(hourupg, DEC);
  display.display();
  delay(100);
}

void DisplaySetMinute(){
  if (digitalRead(7) == HIGH)
  {
    if (minupg == 59){
      minupg = 0;
    }
    else{
      minupg = minupg + 1;
    }
  }
  if (digitalRead(8) == HIGH){
    if (minupg == 0){
      minupg = 59;
    }
    else{
      minupg = minupg - 1;
    }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Minutos: ");
  display.print(minupg, DEC);
  display.display();
  delay(100);
}

void DisplaySetYear(){
  if (digitalRead(7) == HIGH){
    yearupg = yearupg + 1;
  }
  if (digitalRead(8) == HIGH){
    yearupg = yearupg - 1;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Ano: ");
  display.print(yearupg, DEC);
  display.display();
  delay(100);

}

void DisplaySetMonth(){
  if (digitalRead(7) == HIGH)
  {
    if (monthupg == 12){
      monthupg = 1;
    }
    else{
      monthupg = monthupg + 1;
    }
  }
  if (digitalRead(8) == HIGH){
    if (monthupg == 1){
      monthupg = 12;
    }
    else{
      monthupg = monthupg - 1;
    }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Mes: ");
  display.print(monthupg, DEC);
  display.display();
  delay(100);
}

void DisplaySetDay(){

  if (digitalRead(7) == HIGH)
  {
    if (dayupg == 31){
      dayupg = 1;
    }
    else{
      dayupg = dayupg + 1;
    }
  }
  if (digitalRead(8) == HIGH){
    if (dayupg == 1){
      dayupg = 31;
    }
    else{
      dayupg = dayupg - 1;
    }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Dia: ");
  display.print(dayupg, DEC);
  display.display();
  delay(100);
}

void StoreAgg(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5, 5);

  display.print("Salvando...");
  display.display();

  RTC.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, 0));
  delay(100);
}
