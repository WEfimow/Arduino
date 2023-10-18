//Example CRMui3  /  Пример CRMui3

#include "CRMui3.h"
#include <Ticker.h> // Входит в состав ядра

// Объявление объектов
CRMui3 crm;     // CRMui
Ticker myLoop;  // Ticker

// Переменные в примере
bool st3, st4, st5, speedTestState, card1;
uint8_t cpu;


void btnResponce(const char *name);

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, INPUT_PULLUP);            // Button pin

  // Отключает управление WiFi средствами библиотеки
  //crm.disableWiFiManagement();

  // Включает возможность прошивать модуль по сети через Arduino IDE
  crm.useArduinoOta();

  // callBackButtonEvent. Обработка web кнопок по событию
  // crm.btnCallback([Event function]);
  crm.btnCallback(btnResponce);

  // Контакты
  // Позволяет задать свои контактные данные для связи в меню.
  // crm.contacts([Электронная почта], [Телеграм], [Домашняя страница]);
  crm.contacts("e***ow@mail.ru", "904*****15");

  // Версия прошивки вашего проекта, если не используется, то отображается версия CRMui
  // crm.version ("[Любая строка]");
  crm.version ("1.0");

  // Инициализация библиотеки, памяти и прочих функций
  // Параметры со * обязательны.
  // crm.begin("[*Название проекта]", [*Ф-я интерфейса], [Ф-я обновления переменных], [Ф-я API], [Скорость серийного порта, доп. отладка]);
  //crm.begin("Project-28", interface, update);
  //crm.begin("Project-28", interface, NULL, NULL, 115200);
  crm.begin("Проект", interface, update, api, 115200);

  // Авторизация в веб интерфейсе.
  // Параметры со * обязательны.
  // crm.setWebAuth("[*Имя пользователя]", "[Пароль]");
  //crm.setWebAuth("admin","admin");

  // Авторизация на устройстве для API запросов. Без setWebAuth не работает.
  // Пример запроса: http://IP/api?k=d1h6d5&p1=2&...,
  //  - где первый параметр должен быть ключ, имя параметра любое
  // crm.setApiKey("[API ключ]");
  
  // Планировщик
  // NAME.once_ms(time_ms, FunctionName); - Выполнить единожды через указанный интервал
  // NAME.attach_ms(time_ms, FunctionName); - Цикличное выполнение через указанный интервал
  // NAME.detach(); - Деактивировать
  myLoop.attach_ms(3000, myLoopRun);

  // crm.wifiScan()
  // Возвращает список найденных точек доступа в виде JSON строки.
  // Переменные: s - статус / количество сетей; n - массив сетей [Имя, канал, rssi, шифрование], ...
  // Статус: -2 - сканирование не запущено; -1 - сканирует диапазоны; 0+ - количество найденных сетей
  // Способ опроса асинхронный, задержка минимум, ответ по готовности при следующем запросе

  // Конвертирование uint64_t в String
  // crm.uint64ToStr(uint64_t);
  //
  // Только для ESP32
  //
  // Глубокий / Лёгкий сон
  // Режим: 1 - глубокий, 0 - Лёгкий (сохраняются значения всех переменных)
  // crm.espSleep([Время в секундах], [режим]);
}


void loop() {
  // Обслуживание системных функций библиотеки
  crm.run();

  // Проверка состояния нажатия совтовых кнопок. Проверка не обязательна.
  if (crm.btnSwStatus()) {
    // Проверка конкретных кнопок на нажатие
    // crm.btnCallback("[ID кнопки]", [Функция для выполнения]);
    crm.btnCallback("reboot", reboot);    // Check "reboot" SW button
  }
  // Проверка аппаратных кнопок на нажатие
  // crm.btnCallback("[пин подключения кнопки]", [Функция для выполнения], [уровень при нажатии]);
  crm.btnCallback(4, hw_butt, LOW);      // Check pin4 HW button

}


void myLoopRun() {
  static int8_t a[3] = {0};
  static uint8_t i = 0;
  if (++i > 2) i = 0;
  a[i] = WiFi.RSSI();

  // Обновление значений элементов веб интерфейса
  // crm.webUpdate("[ID элемента]", "[Значение]");
  // Интервал отправки 1 раз в сек.
  int b = (a[0] + a[1] + a[2]) / 3;
  crm.webUpdate("rssi", String(b));
  crm.webUpdate("rssiraw", String(a[i]));
  crm.webUpdate("G_0", String(b));
  crm.webUpdate("G_1", String(random(-20, 60)));
  crm.webUpdate("G_2", String(random(0, 100)));
}


void update() {
  // Метод вызывается при каждом изменении значения элементов через веб интерфейса
  Serial.println("Method update() run");
  Serial.println("Language: " + lng());

  // Получить(Записать) значение переменной из(в) конфига
  // crm.var("ID")
  // crm.var("ID", "Значение")
  bool _card1 = card1;
  card1 = crm.var("card1") == "1";
  if (_card1 != card1) {
    digitalWrite(2, card1 ? HIGH : LOW);

    // Отправить уведомление на страницу веб интерфейса
    // crm.webNotif("[Цвет]", "[Сообщение]", [время показа, с], [крестик закрыть (1, true)]);
    // Цвет: green, red, orange, blue, "" - без цвета
    crm.webNotif(card1 ? "Red" : "Green", card1 ? "Мотор запущен" : "Мотор остановлен", 5);
  }

  // Только для ESP32
  // Изменение частоты процессора, значения в МГц (80, 160, 240)
#ifdef ESP32
  uint8_t _cpu = cpu;
  cpu = crm.var("cpu").toInt();
  if (_cpu != cpu) {
    setCpuFrequencyMhz(cpu);
    speedTestState = true;
  }
#endif
}


void api(String p) {
  // Метод вызывается при API запросах
  // Обязательно с параметром, тип String
  //
  // Ответ на запрос, тип JSON
  // crm.apiResponse("[ID]", "[Значение]");

  Serial.print("API: ");
  Serial.println(p);

  DynamicJsonDocument doc(200);
  deserializeJson(doc, p);

  //Запрос http://IP/api?hum
  const char* hum = doc["hum"];
  if (hum != NULL) {
    crm.apiResponse("Humidity", "51");
  }

  //Использование параметров
  //Запрос http://IP/api?random=500
  const char* rnd = doc["random"];
  if (rnd != NULL) {
    crm.apiResponse("Random", String(random(atoi(rnd))));
  }

  //Запрос http://IP/api?print=[any_text]
  const char* prt = doc["print"];
  if (prt != NULL) {
    Serial.println(prt);
  }
}


// Только для ESP32
// Изменение частоты процессора, значения в МГц (80, 160, 240)
#ifdef ESP32
void speedTestRun() {
  if (speedTestState) {
    crm.webNotif("i", "CPU speed test START", 10, 1);
    Serial.print("\nCPU Speed test (Frequency: ");
    Serial.print(getCpuFrequencyMhz());   // Get CPU frequency, Mhz
    Serial.println(" MHz)\nTest start...");
    uint32_t m = millis();
    for (uint32_t i = 0; i < 200000000; i++) {
      asm volatile ("nop");
    }
    m = millis() - m;
    Serial.print("Test completed in: ");
    Serial.print(m);
    Serial.println(" ms\n");
    crm.webNotif("i", "CPU speed test completed in: " + String(m) + " ms", 10, 1);
    speedTestState = false;
  }
}
#endif


void sleepTest() {
  int sleepTime = crm.var("SLT").toInt();
  Serial.println("Start sleep on " + String(sleepTime) + " s");
  Serial.flush();

  // Управление сном по внутреннему таймеру
  // http://wikihandbk.com/wiki/ESP32:Примеры/Режим_глубокого_сна:_пробуждение_при_помощи_таймера
  // crm.espSleep([Время сна в сек], [Режим сна]);
  // Режим сна: true - глубокий сон (+-100 мкА), легкий сон (0,8 мА)
  crm.espSleep(sleepTime);

  Serial.println("ESP WakeUP after " + String(sleepTime) + " s");
}


//функции кнопок
void hw_butt() {
  Serial.println("HW BUTTON PRESS!");
}

// ------- Through a callBackButton Event -------------
void btnResponce(const char *name) {
  if (strcmp(name, "card3") == 0) {
    Serial.println("Card 3 Button press.");
    st3 = !st3;
    crm.webUpdate("card3", st3 ? "Открыта" : "Закрыта");
  }
}
// ----------------------------------------------------



void reboot() {
  crm.webNotif("info", "Reboot ESP", 5, 1);

  // Отправляет модуль на перезагрузку, с сохранением настроек, если требуется.
  crm.espReboot();
}



String lng() {
  // Вариант реализации многоязычности
  // Получить индекс языка
  // crm.getLang();
  String L(crm.getLang());
  uint8_t l = 0;
  if (L == "de") l = 1;
  else if (L == "ru") l = 2;
  switch (l) {
    case 0: return "English";
    case 1: return "Deutsch";
    case 2: return "Russian";
    default: return "n/a";
  }
}


// Метод, вызывается при открытии веб интерфейса.
void interface() {
  // Заголовок новой страницы
  crm.page("&#xe802; Главная");


  crm.page("&#xe806; Датчики");
  

  crm.page("&#xf1de; Настройки");

   crm.page("&#xf1eb; Wi-Fi");
  // форма с полями для WiFi
  // crm.wifiForm([Режим работы], ["Название ТД"], ["Пароль ТД"], ["WiFi сеть для подключения", ["Пароль сети"], ["Время ожидания подключения"]]);
  // Режим работы: WIFI_AP - точка доступа, WIFI_STA - клиент, WIFI_AP_STA - ТД + Клиент
  crm.wifiForm(WIFI_AP, "MY-AP");
  crm.input({INPUT_BUTTON, "reboot", "&#xe82b; Перезагрузить"});

}
