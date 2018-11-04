/**********************************************************************************************/
/*                                                                                            */
/*                                                                                            */
/*        OBD-II_Bluetooth                             Author: Rafael Feijó Leonardo          */
/*                                                     Email: goldcard99@hotmail.com          */
/*                                                     DF, BRAZIL, 70673-435                  */
/*        Data de criação   : 2018/10/01 20:36:13                                             */
/*        Última atualização: 2018/10/03 22:58:19                                             */
/*                                                                       All rights reserved  */
/**********************************************************************************************/

//Includes
#include <avr/wdt.h>
#include "SoftwareSerial.h"

//Definitions
#define bt_state 7

//Create a new instance of software serial || TX(HC05) = 2(Arduino), RX(HC05) 3.3v = 3(Arduino)
SoftwareSerial bluetooth(2, 3);

//Global variables
bool  bt_aux = false;

//BT get data from ELM-327
char *ft_bt_get_data(char *cmd)
{
  int  i = 0, j = 0;
  char r, data[30] = "";
  bool prompt = false, aux = true;

  bluetooth.print(cmd);
  bluetooth.print("\r\n");
      
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
        data[j++] = r;
      }
    }
  }
  data[j] = '\0';
  delay(50);
  return (data);
}

//BT check connection with ELM-327
void ft_bt_connect(void)
{
  if (digitalRead(bt_state))
  {
    if (!bt_aux)
    {
      Serial.println(F("\n\n*********** BT CONNECTED! ***********\n\n"));
      
      //Module Reset
        ft_bt_get_data("ATZ");
        
      //Set auto configure
        ft_bt_get_data("ATSP0");

      //Check protocols
        ft_bt_get_data("ATDP");

      //Check battery voltage
        ft_bt_get_data("ATRV");
        
        Serial.println(F("\n\nType the AT commands:\n"));
        bt_aux = true;
    }
  }
  else
  {
    if (bt_aux)
    {
      Serial.println(F("\n\n********* BT DISCONNECTED ***********\n\n"));
      bt_aux = false;
    }
  }
}

//Check speed limit
void ft_speed_limit(void)
{
  char *v = ft_bt_get_data("010D");

  if (v[14] = 'U')
    Serial.println("SPEED:\t\tInvalid Operand!\n");
  else
  {
    //separa so os 2 ultimos digitos hex e faz a operação direto
    int x = int(v[14], DEC) * 10;
    int y = int(v[15], DEC);
    int xy = x + y;
    Serial.print("SPEED:\t\t");
    Serial.println(xy);
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

  ft_bt_get_data(cmd);
}

//Setup Function
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

  Serial.print(F("\n*************** START ***************"));
  wdt_enable(WDTO_8S);
}

//Loop Function
void loop(void)
{
  //Check BT connection
    ft_bt_connect();

  //Check Speed Limit
  if (bt_aux)
    ft_speed_limit();

  //Check Serial Console
    if (Serial.available())
      ft_get_serial();
  
  wdt_reset();
}
