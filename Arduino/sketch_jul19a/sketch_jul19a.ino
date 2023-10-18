//https://github.com/jeecrypt/JeeUI2.git
#include "JeeUI2.h"

bool rst = false;
unsigned long tm;

void restart_handle();
void interface();
void update();

jeeui2 jee;

void setup()
{
  Serial.begin(115200);

  jee.ap(5000);

  jee.ui(interface);
  jee.update(update);
  jee.begin(true);
}

void btnUI()
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
}

void interface()
{
  jee.app("device");
  jee.menu("Настройка WiFi");

  jee.page();
  jee.text("ssid", "Точка доступа");
  jee.password("pass", "Пароль");
  jee.button("btn1", "#00FF00", "Рестарт");
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
