#include "JeeUI2.h"

bool rst = false;
unsigned long tm;

void restart_handle();
void interface();
void update();

jeeui2 jee;

void setup()    //////////////////////////////////////////////////////////////////////////////
{
  Serial.begin(115200);
  
  jee.ap(50000);
  jee.led(2, true);
  jee.ui(interface);
  jee.update(update);
  jee.begin(true);

}             /////////////////////////////////////////////////////////////////////////////////

void btnUI()   // Обработка кнопки перезагрузки
{
  jee.var("wifi", "STA");
  jee.save();
  rst = true;
  tm = millis();
}

void loop()
{
  jee.handle();
  restart_handle();
  jee.btnCallback("btn1", btnUI);
 //////////////////  ЗДЕСЬ СВОЙ КОД   ///////////////////





 ////////////////////////////////////////////////////////
}

void interface()
{
  jee.app("Педаль RTP-MIDI");   // Название проекта
  jee.menu("Настройка WiFi");
  jee.menu("Настройка MIDI");    // Пункты меню (страниц)
  jee.menu("Другие настройки");

  jee.page(); // Разделитель страниц
  jee.text("ssid", "Точка доступа");
  jee.password("pass", "Пароль");
  jee.button("btn1", "#3877b2", "Рестарт");
  
  
  jee.page();
  jee.checkbox("chec1", "Чекбокс");
  jee.color("color1","Цвет");
  jee.range("range1",0,100,5,"Ползунок");
  
  jee.page();
  jee.datetime("dat1","Дата и время");
  jee.time("time1","Время");
  jee.page();
}


void restart_handle()
{
  if (!rst)
    return;
  if (tm + 2000 > millis())
    return;
  ESP.restart();
}

void update()
{
}