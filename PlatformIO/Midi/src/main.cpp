#include "JeeUI2.h"
#include <AppleMIDI.h> //-------------------
#include <WiFiUdp.h>

unsigned long t0 = millis();
int8_t isConnected = 0;
int state = 1;


APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

//-------------------------------------------
bool rst = false;
unsigned long tm;

void restart_handle();
void interface();
void update();
void tap1(int st); // параметр 1 - нажата, 2 - отжата
void tap2(int st);
void tap3(int st);
void tap4(int st);
void tap1E();
void tap2E();
void tap3E();
void tap4E();
jeeui2 jee;

bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;
uint32_t btnTimer = 0;
char smin1 ;
char smax1 ;
int min1 = 0;
int max1 = 0;
int old_fader1 = 0;
bool old_key1 = false ;
bool old_key2 = false ;
bool old_key3 = false ;
bool old_key4 = false ;



void setup()    //////////////////////////////////////////////////////////////////////////////
{
  Serial.begin(115200);

 pinMode(12, INPUT_PULLUP);
 pinMode(13, INPUT_PULLUP);
 pinMode(14, INPUT_PULLUP);
 pinMode(4, INPUT_PULLUP);
 pinMode(16, OUTPUT);
 pinMode(10, OUTPUT);
 pinMode(15, OUTPUT);
 pinMode(5, OUTPUT);
 pinMode(3, OUTPUT);

 digitalWrite(16, 1);
 digitalWrite(10, 1);
 digitalWrite(15, 1);
 digitalWrite(5, 1);
 digitalWrite(3, 1);


  jee.ap(5000);
  jee.led(2, true);
  jee.ui(interface);
  jee.update(update);

 
 // jee.var("wifi", "STA");
 // jee.var("ssid", "Elserv");
 // jee.var("pass", "p6dgss8u");
 
  jee.begin(true);



if (jee.param("wifi") == "STA"){

 //---------------------------------
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
    AM_DBG(F("Establishing connection to WiFi.."));
  }
}

  AM_DBG(F("Connected to network"));
  AM_DBG(F("OK, now make sure you an rtpMIDI session that is Enabled"));
  AM_DBG(F("Add device named Arduino with Host"), WiFi.localIP(), "Port", AppleMIDI.getPort(), "(Name", AppleMIDI.getName(), ")");
  AM_DBG(F("Select and then press the Connect button"));
  AM_DBG(F("Then open a MIDI listener and monitor incoming notes"));
  AM_DBG(F("Listen to incoming MIDI commands"));



MIDI.begin();

  AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
    isConnected++;
    AM_DBG(F("Connected to session"), ssrc, name);
    Serial.println("Подключились");
    digitalWrite(16, 0); 
  });
  AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
    isConnected--;
    AM_DBG(F("Disconnected"), ssrc);
  });
  
  MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
    AM_DBG(F("NoteOn"), note);
  });
  MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
    AM_DBG(F("NoteOff"), note);
  });

   MIDI.setHandleControlChange([](byte channel, byte note, byte velocity) {
    AM_DBG(F("ControlChange"), note);
  });

 //-------------------------------
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
 

 
 
  jee.btnCallback("key1", tap1E);
  jee.btnCallback("key2", tap2E);
  jee.btnCallback("key3", tap3E);
  jee.btnCallback("key4", tap4E);
   
  MIDI.read();

   // читаем КНОПКИ
   // Кноапка 1 ------------------------------------------
  bool btnState = !digitalRead(12);
  if (btnState && !flag1 && millis() - btnTimer > 50) {
    flag1 = true;
    btnTimer = millis();
    tap1(1);
  //  Serial.println("press1");
  }
  if (!btnState && flag1 && millis() - btnTimer > 50) {
    flag1 = false;
    btnTimer = millis();
    tap1(2);
  //  Serial.println("release1");
  }
   // Кноапка 2 ------------------------------------------
    bool  btnState2 = !digitalRead(13);
  if (btnState2 && !flag2 && millis() - btnTimer > 50) {
    flag2 = true;
    btnTimer = millis();
    tap2(1);
  //  Serial.println("press2");
  }
  if (!btnState2 && flag2 && millis() - btnTimer > 50) {
    flag2 = false;
    btnTimer = millis();
    tap2(2);
  //  Serial.println("release2");
  }
  // Кноапка 3 ------------------------------------------
    bool  btnState3 = !digitalRead(14);
  if (btnState3 && !flag3 && millis() - btnTimer > 50) {
    flag3 = true;
    btnTimer = millis();
    tap3(1);
  //  Serial.println("press3");
  }
  if (!btnState3 && flag3 && millis() - btnTimer > 50) {
    flag3 = false;
    btnTimer = millis();
    tap3(2);
  //  Serial.println("release3");
  }
 // Кноапка 4 ------------------------------------------
    bool  btnState4 = !digitalRead(4);
  if (btnState4 && !flag4 && millis() - btnTimer > 50) {
    flag4 = true;
    btnTimer = millis();
    tap4(1);
  //  Serial.println("press4");
  }
  if (!btnState4 && flag4 && millis() - btnTimer > 100) {
    flag4 = false;
    btnTimer = millis();
    tap4(2);
  //  Serial.println("release4");
  }
 // Читаем Фейдер ---------------------------------------

if (millis() - btnTimer > 100)
  {
   btnTimer = millis();
   
    int rotat1 = analogRead(A0);
    int V_fader1 = map(rotat1,0,1024,0 ,127);

    if (old_fader1 != V_fader1)
    {
   Serial.println(V_fader1);
   
     MIDI.sendControlChange(7, V_fader1, 1);  // Установить регулятор

   old_fader1=V_fader1;
    }

  }


 ////////////////////////////////////////////////////////
}

void interface()
{
  jee.app("♫ RTP-MIDI КОНТРОЛЛЕР");   // Название проекта

  jee.menu("•  Настройка WiFi");
  jee.menu("•  Кнопка 1");    // Пункты меню (страниц)
  jee.menu("•  Кнопка 2");
  jee.menu("•  Кнопка 3");
  jee.menu("•  Кнопка 4");
  jee.menu("•  Фейдер");
  jee.menu("•  Эмулятор");

  jee.page(); // Разделитель страниц
  jee.text("ssid", "Точка доступа");
  jee.password("pass", "Пароль");
  jee.button("btn1", "#7585d7", "Рестарт");
   
  jee.page();
  jee.number("keyPress1","♪ MIDI Команда при нажатии (NoteOn):");
  jee.number("keyRelease1","♪ MIDI Команда при отпускании (NoteOff):");
  jee.button("key1", "#7585d7", "1 (только нажатие)");
  jee.checkbox("check1", "Фиксация");
  //jee.color("col","col");

  jee.page();
  jee.number("keyPress2","♪ MIDI Команда при нажатии (NoteOn):");
  jee.number("keyRelease2","♪ MIDI Команда при отпускании (NoteOff):");
  jee.button("key2", "#7585d7", "2 (только нажатие)");
  jee.checkbox("check2", "Фиксация");


  jee.page();
  jee.number("keyPress3","♪ MIDI Команда при нажатии (NoteOn):");
  jee.number("keyRelease3","♪ MIDI Команда при отпускании (NoteOff):");
  jee.button("key3", "#7585d7", "3 (только нажатие)");
  jee.checkbox("check3", "Фиксация");


  jee.page();
  jee.number("keyPress4","♪ MIDI Команда при нажатии (NoteOn):");
  jee.number("keyRelease4","♪ MIDI Команда при отпускании (NoteOff):");
  jee.button("key4", "#7585d7", "4 (только нажатие)");
  jee.checkbox("check4", "Фиксация");


  jee.page();
  jee.option("7","Громкость");
  jee.option("11","Экспрессия");
  jee.option("73","Атака");
  jee.option("93","Хорус");
  jee.option("91","Реверберация");
  jee.select("FaderTip1","Тип фейдера");
  jee.range("FaderMin1",0,127,1,"Минимальное значение:");
  jee.range("FaderMax1",0,127,1,"Максимальное значение:");
  jee.button("Fad1", "#7585d7", "Применить");
  
    
  jee.page();
  jee.button("key1", "#7585d7", "1 (только нажатие)");
  jee.button("key2", "#7585d7", "2 (только нажатие)");
  jee.button("key3", "#7585d7", "3 (только нажатие)");
  jee.button("key4", "#7585d7", "4 (только нажатие)");
  jee.range("Fader1",0,127,1,"Фейдер");

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

//-------------------------------
void tap1E()
{

  tap1(1);
}
//------------------------------
void tap2E()
{
  tap2(1);
}
//------------------------------
void tap3E()
{
  tap3(1);
}
//------------------------------
void tap4E()
{
  tap4(1);
}
//------------------------------
/////////////////////////////////////////////////////
void tap1(int st)
{
  if (st == 1)
   {
     if (isConnected > 0) 
      {
       byte note = jee.param("keyPress1").toInt();
       byte velocity = 127;
       byte channel = 1;
        if (jee.param("check1") == "true")
        {
               if (old_key1) 
               {
                 //  MIDI.sendControlChange(note, velocity, channel);  // Установить регулятор
                 //  MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту
               old_key1 = false;
               digitalWrite(10, 1);
               }
               else
               {
               MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               old_key1 = true;
               digitalWrite(10, 0);
               }        
        }
        else
        {
       MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
       old_key1 = true;
       digitalWrite(10, 0);
        }
      }

   } 
  else if (st == 2)
   {
        if (isConnected > 0 ) 
         {
                if (jee.param("check1")!="true")
                {
           byte note = jee.param("keyRelease1").toInt();
           byte velocity = 127;
           byte channel = 1;

           MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту 
           old_key1 = false;
           digitalWrite(10, 1);
                }
          }
   }      
  
}
/////////////////////////////////////////////////////
void tap2(int st)
{
  if (st == 1)
   {
     if (isConnected > 0) 
      {
       byte note = jee.param("keyPress2").toInt();
       byte velocity = 127;
       byte channel = 1;
        if (jee.param("check2") == "true")
        {
               if (old_key2) 
               {
                 //  MIDI.sendControlChange(note, velocity, channel);  // Установить регулятор
                 //  MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту
               old_key2 = false;
               digitalWrite(15, 1);
               }
               else
               {
               MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               old_key2 = true;
               digitalWrite(15, 0);
               }        
        }
        else
        {
       MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
       old_key2 = true;
       digitalWrite(15, 0);

        }
      }

   } 
  else if (st == 2)
   {
        if (isConnected > 0 ) 
         {
                if (jee.param("check2")!="true")
                {
           byte note = jee.param("keyRelease2").toInt();
           byte velocity = 127;
           byte channel = 1;

           MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту 
           old_key2 = false;
           digitalWrite(15, 1);
                }
          }
   }      
  
}
/////////////////////////////////////////////////////
void tap3(int st)
{
  if (st == 1)
   {
     if (isConnected > 0) 
      {
       byte note = jee.param("keyPress3").toInt();
       byte velocity = 127;
       byte channel = 1;
        if (jee.param("check3") == "true")
        {
               if (old_key3) 
               {
                 //  MIDI.sendControlChange(note, velocity, channel);  // Установить регулятор
                 //  MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту
               old_key3 = false;
               digitalWrite(5, 1);

               }
               else
               {
               MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               old_key3 = true;
               digitalWrite(5, 0);

               }        
        }
        else
        {
       MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
       old_key3 = true;
       digitalWrite(5, 0);

        }
      }

   } 
  else if (st == 2)
   {
        if (isConnected > 0 ) 
         {
                if (jee.param("check3")!="true")
                {
           byte note = jee.param("keyRelease3").toInt();
           byte velocity = 127;
           byte channel = 1;

           MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту 
           old_key3 = false;
           digitalWrite(5, 1);

                }
          }
   }      
  
}
/////////////////////////////////////////////////////
void tap4(int st)
{
  if (st == 1)
   {
     if (isConnected > 0) 
      {
       byte note = jee.param("keyPress4").toInt();
       byte velocity = 127;
       byte channel = 1;
        if (jee.param("check4") == "true")
        {
               if (old_key4) 
               {
                 //  MIDI.sendControlChange(note, velocity, channel);  // Установить регулятор
                 //  MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту
               old_key4 = false;
               digitalWrite(3, 1);
               }
               else
               {
               MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
               old_key4 = true;
               digitalWrite(3, 0);
               }        
        }
        else
        {
       MIDI.sendNoteOn(note, velocity, channel);         // Включить ноту
       old_key4 = true;
       digitalWrite(3, 0);
        }
      }

   } 
  else if (st == 2)
   {
        if (isConnected > 0 ) 
         {
                if (jee.param("check4")!="true")
                {
           byte note = jee.param("keyRelease4").toInt();
           byte velocity = 127;
           byte channel = 1;

           MIDI.sendNoteOff(note, velocity, channel);        // Выключить ноту 
           old_key4 = false;
           digitalWrite(3, 1);
                }
          }
   }      
  
}