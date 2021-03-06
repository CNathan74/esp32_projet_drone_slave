
/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-websocket-server-arduino/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include "Definitions.h"
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <elapsedMillis.h>

elapsedMillis TimerCompteurPrincipal;  

WebSocketsClient webSocket;

// Replace with your network credentials
const char* ssid = "PALM_Wifi";
const char* password = "palmpalm";
char path[] = "/ws";
char host[] = "192.168.4.1";

uint8_t ui_Sequence = 0;

uint8_t ui_NbBtnDejaAppui_DRT = 0;
uint8_t b_BtnDejaAppui_DRT[3] = {false, false, false};
uint8_t b_EtatBtn_DRT[3] = {false, false, false};
uint8_t b_EtatBtn_DRT_Z1[3] = {false, false, false};
const uint8_t ui_PinBtn_DRT[3] = {DRT_BTN_1, DRT_BTN_2, DRT_BTN_3};
const uint8_t ui_PinBtn_RETRO_DRT[3] = {DRT_RETRO_BTN_1, DRT_RETRO_BTN_2, DRT_RETRO_BTN_3};

uint8_t ui_EtatBtn_DRT_CTRL = 0;
uint8_t ui_EtatBtn_DRT_CTRL_Z1 = 0;



StateBtnLaby stateBtnLaby;
StateBtnLaby stateBtnLaby_Z1;

//todo pas beau
uint8_t tache_ConvertBtnState_4(boolean b_Btn1, boolean b_Btn2, boolean b_Btn3, boolean b_Btn4)
{
  uint8_t ui_result = 0;
  if((b_Btn1 == true) && (b_Btn2 == false))
  {
    ui_result = ui_result + 1;
  }
  if((b_Btn2 == true) && (b_Btn1 == false))
  {
    ui_result = ui_result + 2;
  }
  if((b_Btn3 == true) && (b_Btn4 == false))
  {
    ui_result = ui_result + 4;
  }
  if((b_Btn4 == true) && (b_Btn3 == false))
  {
    ui_result = ui_result + 8;
  }
  return ui_result;
}

//todo pas beau
uint8_t tache_ConvertBtnState_3(boolean b_Btn1, boolean b_Btn2, boolean b_Btn3)
{
  uint8_t ui_result = 0;
  if(b_Btn1 == true)
  {
    ui_result = ui_result + 1;
  }
  if(b_Btn2 == true)
  {
    ui_result = ui_result + 2;
  }
  if(b_Btn3 == true)
  {
    ui_result = ui_result + 4;
  }
  return ui_result;
}

void tache_RAZ() {
  uint8_t i;
  ui_NbBtnDejaAppui_DRT = 0;
  for(i = 0; i < 3; i++)
  {
    b_BtnDejaAppui_DRT[i] = false;
  }
}

void setup(){
  uint8_t i;
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  
  pinMode(DRT_BTN_GAUCHE, INPUT_PULLUP);
  pinMode(DRT_BTN_DROITE, INPUT_PULLUP);
  pinMode(DRT_BTN_AV, INPUT_PULLUP);
  pinMode(DRT_BTN_AR, INPUT_PULLUP);
  for(i = 0; i < 3; i++)
  { pinMode(ui_PinBtn_DRT[i], INPUT_PULLUP);
    pinMode(ui_PinBtn_RETRO_DRT[i], OUTPUT);  }


  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  delay(5000);
 
  webSocket.begin("192.168.4.1", 80, "/ws");

}

void loop() {
  uint8_t i;
  boolean b_ChangementEtatBtn_DRT = false;
  String msg = "";
  webSocket.loop();
  
  if (TimerCompteurPrincipal >= FREQUENCE_TIMER_PRINCIPAL)      // toutes les 10ms
  {
    TimerCompteurPrincipal = TimerCompteurPrincipal - FREQUENCE_TIMER_PRINCIPAL;


    ui_EtatBtn_DRT_CTRL_Z1 = ui_EtatBtn_DRT_CTRL;
    ui_EtatBtn_DRT_CTRL = tache_ConvertBtnState_4(!digitalRead(DRT_BTN_GAUCHE), !digitalRead(DRT_BTN_DROITE), !digitalRead(DRT_BTN_AV), !digitalRead(DRT_BTN_AR));

    if(ui_EtatBtn_DRT_CTRL_Z1 != ui_EtatBtn_DRT_CTRL)
    {
      msg = "IO@DRT_CTRL:" + String(ui_EtatBtn_DRT_CTRL);
      webSocket.sendTXT(msg);
    }
    else if(ui_EtatBtn_DRT_CTRL !=0)
    {
      if(ui_Sequence == 0)
      {
        msg = "IO@DRT_CTRL:" + String(ui_EtatBtn_DRT_CTRL);
       webSocket.sendTXT(msg);
      }
    }

    for(i = 0; i < 3; i++)
    {
      b_EtatBtn_DRT_Z1[i] = b_EtatBtn_DRT[i];
      b_EtatBtn_DRT[i] = !digitalRead(ui_PinBtn_DRT[i]);
      //Serial.print(b_EtatBtn_DRT[i]);
      //Serial.print(" - ");
      /*if(b_EtatBtn_DRT_Z1[i] != b_EtatBtn_DRT[i])
      {
        b_ChangementEtatBtn_DRT = true;
        b_BtnDejaAppui_DRT[i] = true;
      }*/
      if((b_BtnDejaAppui_DRT[i] == false) && (b_EtatBtn_DRT[i] == true))
      {
        if(i > 0)
        {
          if(b_BtnDejaAppui_DRT[i - 1] == true)
          {
            b_BtnDejaAppui_DRT[i] = true;
            b_ChangementEtatBtn_DRT = true;
          }
        }
        else
        {
            b_BtnDejaAppui_DRT[i] = true;
            b_ChangementEtatBtn_DRT = true;
        }  
      }
    }
    //Serial.println();

    if(b_ChangementEtatBtn_DRT == true)
    {
      msg = "IO@DRT_INFO:";
      for(i = 0; i < 3; i++)
      {
        msg += String(b_EtatBtn_DRT[i]);
      }
      webSocket.sendTXT(msg);
    }

    for(i = 0; i < 3; i++)
    {
      digitalWrite(ui_PinBtn_RETRO_DRT[i], b_BtnDejaAppui_DRT[i]); 
    }

    if(ui_Sequence < NB_PERIODE_SEQUENCEUR)
    {
      ui_Sequence++;      
    }
    else
    {
      ui_Sequence = 0;
    }
  }

}
