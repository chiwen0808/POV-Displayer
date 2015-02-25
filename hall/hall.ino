// Code for the Arduino Bike POV project

// Arduino Bike POV
//
// by Scott Mitchell
// www.openobject.org
// Open Source Urbanism
//
// Copyright (C) 2008 Scott Mitchell 12-10-2008
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//

//============================================================
// 6/2011 heavily modified by c. Dubois for my POV project
// Hall sensor is a switch so I  used different code for it
// also used a font.h that I found
// ------------------------------------------------------------
 
//============================================================
// 01/2015 modified by Wendy Huang by using Arduino Yun & normal fan
// ------------------------------------------------------------
 
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

#include <Time.h>
#include "convt.h" 
#include "font.h" // defining the alphabet

// define the Arduino LED pins in use
const int LEDpins[] = {2,3,4,5,6,7,8,9};

// number of LEDs
const int charHeight = 8;  // Total LED's in a row
const int charWidth = 5;

// sensor setup
// hall sensor for future use
const int sensorPIN = A0;  // define the Arduino sensor pin
int sensVal;  // variable to store the value coming from the sensor
int count=0;
String tmp_str;
 
const long debounceTime = 120; // delay time before writing the next character.
long colDelayTime = 2500; // delay time before writing next char's column.
long lastSavePointTime = 0; // used for storing a savepoint time (achieve time measurement).
 
YunServer server;

void setup()
{
  //setTime(int hr,int min,int sec,int day, int month, int yr);
  //2015/1/16 23:36:16
  //setTime(23,51,30,26,1,2015);
  pinMode(13, OUTPUT);

  for (int i = 0; i < charHeight; i++)
   pinMode(LEDpins[i], OUTPUT);  
    
  Serial.begin(9600);
  
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  
  //for (int i = 0; i < charHeight; i++)
  //  pinMode(LEDpins[i], HIGH); 
  digitalWrite(13, HIGH);   // set the LED on
}
int client_cmd=0;
String strcontent;
void loop()
{
  // if it is time to print the message again do it.
  
  // Get clients coming from server
  YunClient client = server.accept();
  
  
  // There is a new client?
  if (client) {
    Serial.println("receive the new event");
    // Process request
    strcontent=process(client,&client_cmd);
    Serial.println(strcontent);
    Serial.println(client_cmd);

    // Close connection and free resources.
    client.stop();
    Serial.println("finished");
  }
  while(count<10&&client_cmd!=0)
  {
    count++;
    //Serial.println(count);
    //if (abs(millis() - lastSavePointTime) > debounceTime) {
    //  // store a new savepoint time for future reference.
    //  lastSavePointTime = millis();
      
      if(client_cmd==1)
      {
        HandleText(strcontent);
      }
      else if(client_cmd==2)
      {
        HandleDraw(strcontent);
      }
      else if(client_cmd==3)
      {
        int hour, minu, sec, day, month, yr;
        //setTime(int hr,int min,int sec,int day, int month, int yr);
        hour=strcontent.substring(0,2).toInt();
        minu=strcontent.substring(2,4).toInt();
        sec=strcontent.substring(4,6).toInt();
        day=strcontent.substring(6,8).toInt();
        month=strcontent.substring(8,10).toInt();
        yr=strcontent.substring(10,14).toInt();
        setTime(hour,minu,sec,day,month,yr);
        HandleClock(klok());
      }
      else if(client_cmd==4)
      {
        colDelayTime=strcontent.toInt();
      }
    //}
 }
 if(count==10) count=0;
  

  delay(50); // Poll every 50ms
  /*
 
  digitalWrite(13, LOW);   // set the LED on
    

  for (int k=0; k<sizeof(textString); k++)
  {
    printLetterboven(textString[k]);
  }
  */
}
String klok()
{
  String result;
  String str1;
  String str2;
  String str3;
  if (hour() < 10)
    str1 = "0"+String(hour());
  else
    str1 = String(hour());
  if (minute() < 10)
    str2 = "0"+String(minute());
  else
    str2 = String(minute());
  if (second() < 10)
    str3 = "0"+String(second());
  else
    str3 = String(second());
  result = str1+":"+str2+":"+str3;
  //result = String(hour())+":"+String(minute())+":"+String(second());
  return result;
}
void printLetterboven(char ch)
{
    // make sure the character is within the alphabet bounds (defined by the font.h file)
    // if it's not, make it a blank character
    if (ch < 32 || ch > 126){
      ch = 32;
    }
    // subtract the space character (converts the ASCII number to the font index number)
    ch -= 32;
    // step through each byte of the character array
    for (int i=0; i<charWidth; i++) {
      byte b = font[ch][i];
      //Serial.println(b,HEX);
      // ventilator draai tegen de klok in
      for (int j=0; j<charHeight; j++) {
        digitalWrite(LEDpins[charHeight-1-j], !bitRead(b,j));
        //Serial.print(!bitRead(b,j));
        //digitalWrite(LEDpins[v-1-j], !!!(b & (1 << j)));
        //Serial.print("j\t");
        //Serial.print(!!(b & (1 << j)));
        //Serial.print("\t test\t");
        //Serial.println(!!!(b & (1 << j)));
      }
      delayMicroseconds(900);  
      //delay(1);
       // wait some time between columns.
       //delayMicroseconds(colDelayTime);
    }
    //clear the LEDs
    for (int i = 0; i < charHeight; i++)
    {
      digitalWrite(LEDpins[i] , HIGH);
      //delay(1000);
      //Serial.println(LEDpins[i]);
    }
    
    // space between letters
    //delayMicroseconds(2500);
    //delay(3);
    // wait some time before next character.
    delayMicroseconds(colDelayTime);
    
}  
 
void printByte(String pixel)  //like 05 A0
 {
   for(int i=0;i<pixel.length();i+=2)
   {
     byte b = hex_char_to_bin(pixel[i]);
     b = b << 4;
     b = b | hex_char_to_bin(pixel[i+1]);
     //Serial.print("b:");
     //Serial.println(b,HEX);
     // bit shift through the byte and output it to the pin
     
     for (int j=0; j<charHeight; j++) {
       digitalWrite(LEDpins[j], !!!(b & (1 << j)));
       
       //DEBUG
       /*
       Serial.print("Num\t");
       Serial.print(i,HEX);
       Serial.print("\t");
       Serial.println(j);
       Serial.println(!!(b & (1 << j)));
       */
     }
     delayMicroseconds(900);
     // space between next img
     //delayMicroseconds(2500);
     //delay(3);
   }
  
   //clear the LEDs
    for (int i = 0; i < charHeight; i++)
      digitalWrite(LEDpins[i] , HIGH);
   
   // space between columns
   //delayMicroseconds(500);
   //delay(1);
   delayMicroseconds(colDelayTime);
 }
 
 void HandleDraw(String content)
 {
   //digitalWrite(13, LOW);   // set the LED on  
   sensVal = digitalRead(sensorPIN);
   //Serial.println(sensVal);
   sensVal=0;  //For DEBUG ONLY
   if(sensVal==0)
   {
     printByte(content);
   }
 }

void HandleText(String content)
{
  //digitalWrite(13, LOW);   // set the LED on
  for (int k=0; k<content.length(); k++){
       //Serial.println(content[k]);
       printLetterboven(content[k]);
   }
}

void HandleClock(String content)
{
  //digitalWrite(13, LOW);   // set the LED on
  tmp_str=content;
  tmp_str=tmp_str+" ";
  for (int k=0; k<tmp_str.length(); k++){
       //Serial.println(tmp_str[k]);
       printLetterboven(tmp_str[k]);
   }
}

String process(YunClient client, int *cmd) {
  // read the command
  String command = client.readStringUntil('/');
  String content = client.readStringUntil('\r');
  if(command=="TEXT")
    *cmd=1;

  else if(command=="DRAW")
    *cmd=2;

  else if(command=="CLOCK")
    *cmd=3;
    
   else if(command=="DELAY")
    *cmd=4;
    
  return content;

}

