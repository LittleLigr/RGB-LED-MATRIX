#include "FastLED.h"

#define MATRIX_LEDS 256

#define SNOWBALL 100
#define RANDOM_COLORS 101
#define RUN_RAINBOW 102
#define LINES 103
#define FIGURE 104
#define FILL 105
#define FILL_PULSE 106
#define MUSIC_VISUALIZER 107
#define NOTHING 108
byte CURRENT_EFFECT = FIGURE;

byte balls [10];
byte music_data[16];
byte music_buffer[16];
byte coor[24][3];
byte buf_coor[24][3];
byte color = 100;

int delay_time=0;

void set_brightness(byte v)
{
  FastLED.setBrightness(v);
}

void setFigureCanvas()
{
  coor[0][0]=7;
  coor[0][1]=4;

  coor[1][0]=6;
  coor[1][1]=3;

  coor[2][0]=5;
  coor[2][1]=3;

  coor[3][0]=4;
  coor[3][1]=3;

  coor[4][0]=3;
  coor[4][1]=4;

  coor[5][0]=2;
  coor[5][1]=5;

  coor[6][0]=2;
  coor[6][1]=6;

  coor[7][0]=2;
  coor[7][1]=7;

  coor[8][0]=3;
  coor[8][1]=8;

  coor[9][0]=4;
  coor[9][1]=9;

  coor[10][0]=5;
  coor[10][1]=10;

  coor[11][0]=6;
  coor[11][1]=11;

  coor[12][0]=7;
  coor[12][1]=12;

  coor[13][0]=8;
  coor[13][1]=11;

  coor[14][0]=9;
  coor[14][1]=10;

  coor[15][0]=10;
  coor[15][1]=9;

  coor[16][0]=11;
  coor[16][1]=8;

  coor[17][0]=12;
  coor[17][1]=7;

  coor[18][0]=12;
  coor[18][1]=6;

  coor[19][0]=12;
  coor[19][1]=5;

  coor[20][0]=11;
  coor[20][1]=4;

  coor[21][0]=10;
  coor[21][1]=3;

  coor[22][0]=9;
  coor[22][1]=3;

  coor[23][0]=8;
  coor[23][1]=3;

  for(byte i = 0; i < 24; i++)
  {
    buf_coor[i][1]=coor[i][1];
    buf_coor[i][2]=10;
    coor[i][2] = 10;
  }
    
}

byte rotate_figure(float angle)
{
  float s = sin(angle*3.14/180);
  float c = cos(angle*3.14/180);
  
  for(byte x = 0; x<24;x+=1)
   {
    if( buf_coor[x][2]!=0)
    {
       int x1=round(c*(coor[x][0]-7));
    int z1 = round(-s*(coor[x][0]-7));
    //byte y1=s*(buf_coor[x][0]-7)+c*(buf_coor[x][1]-7);
     buf_coor[x][0]=x1+7;
     buf_coor[x][2]=z1+10;
    }
   
   }
   
}

void set_delay(int del)
{
  delay_time=del;
}

byte getPosition(byte pos)
{
     if(pos/16%2==1)
    {
      byte row = pos/16;
      byte cell = pos%16;
      byte cur_cell = 15-cell;
      return row*16+cur_cell;
    }
    else return pos;
}

byte current_color=255;
byte current_brigth=0;
bool up = true;
void draw_fill_pulse(CRGB *leds)
{
  for(int i = 0; i < 16; i++)
     for(int j = 0; j < 16; j++)
      leds[i*16+j].setHue(current_color);
   FastLED.setBrightness(current_brigth);

  if(up)
  {
    current_brigth+=1;
    if(current_brigth==255)
      up=!up;
    
  }
  else
  {
    current_brigth-=1;
    if(current_brigth==0)
    {
      up=!up;
      current_color=random(0,255);
    }
      
  }
    
}

void set_music_data(String a[])
{
  for(byte i = 0; i < 16; i++)
  {
   //Serial.println(a[i]);
    if(a[i]!="")
      music_data[i]=a[i].toInt()*2;    
  }
  
}

void draw_figure_effect(CRGB *leds)
{
  for(int i = 0; i < 24; i++)
    if((buf_coor[i][0]==buf_coor[23-i][0]&&buf_coor[i][2]>=buf_coor[23-i][2])||buf_coor[i][0]!=buf_coor[23-i][0])
        leds[getPosition(buf_coor[i][0]+buf_coor[i][1]*16)].setHSV(100,255,16*buf_coor[i][2]);
    
  for(int i = 0; i < 16; i++)
    draw_point(i,15,i*16,leds);
}

void random_snowball_position()
{
   for(int i = 0; i < 10;i++)
    balls[i]=(random(0,255));
}

void draw_snowballs(CRGB *leds)
{
  for(int i = 0; i < 10;i++)
  {
       leds[getPosition(balls[i])].setRGB(255,255,255);
       balls[i]+=16;
  }
}


byte current_lines_position=0;
byte line_hue_one=0;
byte line_hue_two=0;
byte line_direct=0;

void draw_lines_effect(CRGB *leds)
{
  if(current_lines_position==0)
   {
      line_direct=(byte)random(0,2);
      line_hue_one = (byte)random(0,255);
      line_hue_two = (byte)random(0,255);
   }
   if(line_direct==0)
   {
      for(int i = 0; i < 256; i=i+32)
        draw_point(i+current_lines_position,line_hue_one,leds);
      for(int i = 31; i < 256; i=i+32)
        draw_point(i-current_lines_position,line_hue_two,leds);
   }
   else
   {
     for(int i = 0; i < 16; i+=2)
      draw_point(i+16*current_lines_position,line_hue_one,leds);
     for(int i = 241; i < 256; i+=2)
      draw_point(i-16*current_lines_position,line_hue_two,leds);
   }

   current_lines_position++;
   if(current_lines_position==16)
     current_lines_position=0;
}

void draw_randoms_effect(CRGB *leds)
{
  for(int i = 0; i < MATRIX_LEDS;i++)
    leds[i].setHSV(random(0,255),255,random(0,255));
}

byte rainbow_position=0;
void draw_rainbow_effect(CRGB *leds)
{
      for(int i = rainbow_position; i < 16; i++)
        draw_line(i,0,i,15,16*(i-rainbow_position),leds);
      for(int i = rainbow_position-1; i >= 0; i--)
        draw_line(i,0,i,15,(17-rainbow_position+i)*16,leds);
        
      rainbow_position++;
      if(rainbow_position>15)
        rainbow_position=0;     
}
void draw_up_line(byte x,byte height, byte color, CRGB *leds)
{
  for(int i = 0; i < height;i++)
    leds[getPosition(x+i*16)].setHue(color);
}

void draw_line(byte start_pos1, byte end_pos1,byte start_pos2, byte end_pos2, byte color, CRGB *leds)
{  


  
  int w = start_pos2-start_pos1;
  int h = end_pos2-end_pos1;

     if(w>h)
       for(float x = 0; x <= w; x++)
        leds[getPosition(byte(x+round(x/w*h)*16 + start_pos1 + end_pos1*16))].setHue(color);
    else
       for(float x = 0; x <= h; x++)
        leds[getPosition(byte(round(x/h*w)+x*16 + start_pos1 + end_pos1*16))].setHue(color); 
}

void draw_point(byte x, byte y, byte color, CRGB *leds)
{
  leds[getPosition(x+y*16)].setHue(color);
}

void draw_point(byte pos, byte color, CRGB *leds)
{
  leds[getPosition(pos)].setHue(color);
}

void visualize_music(CRGB *leds)
{
   FastLED.clear();

    for(byte i = 0; i < 16; i+=1)
  {
    if(music_data[i]>music_buffer[i])
      music_buffer[i]+=1;
    else if(music_data[i]<music_buffer[i])
      music_buffer[i]-=1;

    draw_up_line(i, music_buffer[i], music_buffer[i]*16, leds);
  }

  
      delay(15);
}

void set_effect(byte effect)
{
   if(effect==RANDOM_COLORS )
     CURRENT_EFFECT=RANDOM_COLORS;
   else if(effect==SNOWBALL)
     CURRENT_EFFECT=SNOWBALL;
   else if(effect==RUN_RAINBOW)
     CURRENT_EFFECT=RUN_RAINBOW;  
   else if(effect==LINES)
     CURRENT_EFFECT=LINES;  
   else if(effect==FIGURE)
     CURRENT_EFFECT=FIGURE; 
   else if(effect==FILL_PULSE)
     CURRENT_EFFECT=FILL_PULSE; 
   else if(effect==MUSIC_VISUALIZER)
     CURRENT_EFFECT=MUSIC_VISUALIZER; 
}
int angle=0;
void draw(CRGB *leds)
{
  if(CURRENT_EFFECT==SNOWBALL)
  {
      FastLED.clear();
      draw_snowballs(leds);
  }
  else if(CURRENT_EFFECT==LINES)
    draw_lines_effect(leds);
  else if(CURRENT_EFFECT==RANDOM_COLORS)
    draw_randoms_effect(leds);
  else if(CURRENT_EFFECT==RUN_RAINBOW)
    draw_rainbow_effect(leds);
  else if(CURRENT_EFFECT==FIGURE)
  {
    FastLED.clear();
    rotate_figure(angle);
    draw_figure_effect(leds);
    angle+=2;
    if(angle>360)
      angle=0;
    
  }
  else if(CURRENT_EFFECT==FILL_PULSE)
  {
    draw_fill_pulse(leds);
  }
  else if(CURRENT_EFFECT==MUSIC_VISUALIZER)
    visualize_music(leds);

 if(delay_time!=0)   
  delay(delay_time);
  
}
