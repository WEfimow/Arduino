/***************************************************
Это пример программы для цветного OLED экрана основанного на драйвере SSD1331 
 ****************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

// Номера подключенных пинов
#define sclk 14
#define mosi 13
#define cs   15
#define rst  0
#define dc   4

//Определяем Цвета
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//  Создаем объект дисплея SSD1331
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, rst);

void setup(void) {

     Serial.begin(9600);

     display.begin(); // Инициализирует дисплей
  
     Serial.println("init");

     example_draw_text();    //выводим текст 
}

void loop() {
}

void example_draw_text(void) {
  
     display.fillScreen(BLACK); //заливаем экран черным цветом
  
     display.setTextSize(1); //размер букв  
     display.setTextColor(RED); //цвет букв
     display.setCursor(0, 0); //координаты текста     
     display.println(utf8rus("aбвгд")); //выводим текст 
      display.setTextColor(GREEN); //цвет букв

     display.println(utf8rus("AБВГДЕЖЗИЙКЛМНОП"));
  display.println(utf8rus("РСТУФХЦЧШЩЪЫЬЭЮЯ"));
  display.println(utf8rus("абвгдежзийклмноп"));
  display.println(utf8rus("рстуфхцчшщъыьэюя"));
  display.println(utf8rus("Ёё123ABCabc!@#\xBC\xBD"));
  display.println(utf8rus("10\x83 10\x8A\x82 10\x81\x80 2\x85"));

  
}
