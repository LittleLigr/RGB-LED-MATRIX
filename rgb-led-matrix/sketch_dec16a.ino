#include "FastLED.h"
#include <SoftwareSerial.h>

#define MATRIX_LEDS 256
#define MATRIX_PIN 6
#define DATA_PARSER ','
#define DATA_START '['
#define DATA_END ';'
#define SET_EFFECT "0"


SoftwareSerial Bluetooth(2, 3);
//массив ледов
CRGB leds[MATRIX_LEDS];
String pars_data[16];
bool music_mode = false;
String incoming_data;

void get_data(String data)
{

    pars_data[0]="null";pars_data[1]="null";
    
    String current_data;
    byte id = 0;

    if(music_mode)
    {
       for(byte i = 0; i<16; i+=1){
          if(data[i]!=DATA_END){
              pars_data[i] = data[i];
          }
          else
          {
             music_mode=false;
             set_effect(108);
          }       
       }
    }
    else
    {
      for(int i = 0; i < data.length(); i++)
        if(data[i] == DATA_PARSER)
        {
          pars_data[id] = current_data;
          current_data="";
          id+=1;
        }
        else if(data[i] == DATA_END)
        {
          pars_data[id] = current_data;
          current_data="";
          i=data.length();
        }
        else
          current_data+=data[i];
    }

  
    
}
    

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Bluetooth.begin(57600);
  Bluetooth.setTimeout(0);
  FastLED.addLeds<WS2811, MATRIX_PIN>(leds, MATRIX_LEDS);
  FastLED.setBrightness(50);

   
  random_snowball_position();

  setFigureCanvas();
  
}

char current_byte;

unsigned long last_time=0;


void loop() {
      last_time=millis();
      while(Bluetooth.available()>0&&millis()-last_time<6)
      {

            current_byte=Bluetooth.read();

            if((byte)current_byte == 48 ||(byte)current_byte == 49 ||(byte)current_byte == 50 ||(byte)current_byte == 51 ||
           (byte)current_byte == 52 ||(byte)current_byte == 53 ||(byte)current_byte == 54 ||(byte)current_byte == 55 ||
           (byte)current_byte == 56 ||(byte)current_byte == 57 ||(byte)current_byte == 45||(byte)current_byte == 44){
             incoming_data+=(char)current_byte;
            //Serial.println(incoming_data);

              if(music_mode==true&& incoming_data.length()>=16)
              {
                 get_data(incoming_data);
                 set_music_data(pars_data);
                 incoming_data="";
              }
           }
           else if((byte)current_byte == 59)
           {      

                 if(pars_data[0]=="110")
                 {
                    incoming_data+=(char)current_byte;
                    get_data(incoming_data);
                  
                    set_brightness(pars_data[1].toInt());
                 }                    
                 else if(pars_data[0]=="115")
                   {
                    incoming_data+=(char)current_byte;
                    get_data(incoming_data);
                    set_delay(pars_data[1].toInt());
                 }   
                 else if(pars_data[0]=="107")
                 {
                  music_mode = true;
                 }
                 else if(music_mode)
                    music_mode = false;
                 else 
                 {
                  incoming_data+=(char)current_byte;
                  get_data(incoming_data);
                  
                  set_effect(pars_data[0].toInt());
                 
                 }
                 incoming_data="";
           }
           else if(incoming_data.length()>=16&&music_mode==false)
           {
            music_mode=true;
            set_effect(107);
            get_data(incoming_data);
            set_music_data(pars_data);
            incoming_data="";
           }
      }
      
  

    
  
   
  draw(leds);   
  FastLED.show();   
}

extern int __bss_end;
extern void *__brkval;
int memoryFree()
{
   int freeValue;
   if((int)__brkval == 0)
      freeValue = ((int)&freeValue) - ((int)&__bss_end);
   else
      freeValue = ((int)&freeValue) - ((int)__brkval);
   return freeValue;
}



/*String [] Split(String text, char c)
{
  String result = new String[2];
}*/
