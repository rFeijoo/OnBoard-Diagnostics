//Include the SoftwareSerial library
#include "SoftwareSerial.h"

//Define GPIOs
#define bt_state 7

//Create a new instance of software  serial
SoftwareSerial bluetooth(2, 3);     //TX(HC05) = 2(Arduino), RX(HC05) 3.3v = 3(Arduino)

//Global variables
bool  bt_aux = false;

//BT get data from ELM-327
void ft_bt_get_data(char *cmd)
{
  char r;
  int i = 0;
  bool prompt = false, aux = true;
      
  while (!prompt)
  {
    while ((bluetooth.available() > 0))
    {
      r = bluetooth.read();
      if (cmd[i] == r)
      {
        Serial.print(r);
        i++;
      }
      else
      {
        if (aux)
        {
          Serial.print(":\t\t");
          aux = false;
        }
        if (r == 62)
        {
          prompt = true;
          Serial.println(F("\n-------------------------------------\n"));
        }
        else
          Serial.print(r);
      }
    }
  }
}

//BT check connection with ELM-327
void ft_bt_connect(void)
{
  if (digitalRead(bt_state))
  {
    if (!bt_aux)
    {
      Serial.println(F("\n\n*********** BT CONNECTED! ***********\n\n"));
      
      // Module Reset
        bluetooth.print("ATZ\r\n");
        ft_bt_get_data("ATZ");
        delay(250);
        
      // Module Reset
        bluetooth.print("ATSP0\r\n");
        ft_bt_get_data("ATSP0");
        delay(250);

      // Module Reset
        bluetooth.print("ATDP\r\n");
        ft_bt_get_data("ATDP");
        delay(250);
        
        Serial.println(F("\n\nType the AT commands:\n"));
        bt_aux = true;
    }
  }
  else
  {
    if (bt_aux)
    {
      Serial.println(F("\n\n** BT DISCONNECTED **\n"));
      bt_aux = false;
    }
  }
}

//Get data from Serial Console
void ft_get_serial(void)
{
  int  i = 0, data;
  char cmd[5] = "";

  delay(100);
  
  while (Serial.available())
  {
    data = Serial.read();
    if (data >= 97 && data <= 122)
      data -= 32;
    cmd[i++] = data;
  }
  cmd[i] = '\0';

  bluetooth.print(cmd);
  bluetooth.print("\r");
  
  delay(100);
  
  if (bluetooth.available())
    ft_bt_get_data(cmd);
}

void setup(void)
{
  //Initialize the hardware serial
  Serial.begin(38400);
  while (!Serial)
    ;

  //Initialize the software serial
  bluetooth.begin(38400);
  while (!bluetooth)
    ;

  //Initialize GPIOs
  pinMode(bt_state, INPUT);

  Serial.print(F("*************** START ***************"));
}

void loop(void)
{
  ft_bt_connect();

  if (Serial.available())
    ft_get_serial();
}
