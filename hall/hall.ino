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
// 01/2015 modified by Wendy Huang by using Arduino Yun & desk fan
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
//const int sensorPIN = A0;  // define the Arduino sensor pin
//int sensVal;  // variable to store the value coming from the sensor

// for yun connection
int count=0;

//delay time setup
long colDelayTime = 1140; // delay time before writing next char's column.
long charDelayTime = 2500; // delay time before writing the next character.

const long debounceTime = 120; // delay time before writing the next character.
long lastSavePointTime = 0; // used for storing a savepoint time (achieve time measurement).


YunServer server;

void setup()
{
  //setTime(int hr,int min,int sec,int day, int month, int yr);
  //2015/1/16 23:36:16
  //setTime(23,51,30,26,1,2015);
  //pinMode(13, OUTPUT);

  for (int i = 0; i < charHeight; i++)
   pinMode(LEDpins[i], OUTPUT);  
    
  Serial.begin(9600);
  
  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
  
  //for (int i = 0; i < charHeight; i++)
  //  pinMode(LEDpins[i], HIGH); 
  //digitalWrite(13, HIGH);   // set the LED off
  //digitalWrite(13, LOW);   // set the LED on
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
    Serial.println(count);
    Serial.println(millis());
    Serial.println(" 1microseconds");

    
    //if (abs(millis() - lastSavePointTime) > debounceTime) //{
    //  // store a new savepoint time for future reference.
    //  lastSavePointTime = millis();
    
    //sensVal = digitalRead(sensorPIN);
    //Serial.println(sensVal);
    //if(sensVal==0)
    { //process 
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
      else if(client_cmd==5)
      {
        charDelayTime=strcontent.toInt();
      }
     
    }
    

 }
 if(count==10) count=0;
 //delay(10);  //Ben debug
  
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
  /*
  if (second() < 10)
    str3 = "0"+String(second());
  else
    str3 = String(second());
  result = str1+":"+str2+":"+str3;*/
  result = str1+":"+str2+" ";
  
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
    for (int i=0; i<charWidth; i++) { // {0x3e,0x51,0x49,0x45,0x3e}   // 0 0x30 48
      byte b = font[ch][i]; //i=0, 0x3e, i=1, 0x51, i=2, 0x49, i=3, 0x45, i=4, 0x3e
      //Serial.println(b,HEX); //0x3e
    
      // bit shift through the byte and output it to the pin
      for (int j=0; j<charHeight; j++) {
        digitalWrite(LEDpins[charHeight-1-j], !bitRead(b,j));  //digitalWrite(LEDpins[charHeight-1-j], !!!(b & (1 << j)));
        //Serial.print(!bitRead(b,j));
      }
      // space between columns
      delayMicroseconds(colDelayTime);  
      //delay(colDelayTime);
    }
    //clear the LEDs
    for (int i = 0; i < charHeight; i++)
      digitalWrite(LEDpins[i] , HIGH); // set the LED off
   
    // space between letters
    delayMicroseconds(charDelayTime);
}  
 
void printByte(String pixel)  //like 004428120101020101122844000000000000000000000000
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
     }
     delayMicroseconds(colDelayTime);
     //Serial.println(millis());
     //Serial.println(" col microseconds");
   }
  
   //clear the LEDs
    for (int i = 0; i < charHeight; i++)
      digitalWrite(LEDpins[i] , HIGH);
   
   delayMicroseconds(charDelayTime);
   
   //Serial.println(millis());
   //Serial.println(" char microseconds");
 }
 
 void HandleDraw(String content)
 {
    printByte(content);
 }

void HandleText(String content)
{
  for (int k=0; k<content.length(); k++){
       //Serial.println(content[k]);
       printLetterboven(content[k]);
   }
}

void HandleClock(String content)
{
  String tmp_str;
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
    
  else if(command=="COL")
    *cmd=4;
    
   else if(command=="CHAR")
    *cmd=5;
    
  return content;

}

