#include "M5Atom.h"
#include "float2rgb_matrix.h"

uint8_t DisBuff[2 + 5 * 5 * 3];
float num;

void show_num(float num){
  DisBuff[0] = 0x05;
  DisBuff[1] = 0x05;
  float2rgb_matrix::conv_float2rgb_buf(num,(float2rgb_matrix::matrix_buf*)(&DisBuff[2]));
  M5.dis.displaybuff(DisBuff);
}

void setup()
{
    M5.begin(true, false, true);
    delay(10);
    num=0.0f;
    show_num(num);
    M5.update();
}

uint8_t FSM = 0;
int i=0;
float d=0.1;

void loop()
{
    if (M5.Btn.wasPressed()){
        num=0.0f;
        i=0;
        d=0.1;
        show_num(num);
        FSM++;
        if (FSM >= 4)
        {
            FSM = 0;
        }
    }
    if(FSM==1){
      num+=d;
      i++;
      if(i==10){
        i=1;
        d*=10;
      }
      show_num(num);
      delay((num<100 and num>-100?500:100));
    }
    if(FSM==3){
      num-=d;
      i++;
      if(i==10){
        i=1;
        d*=10;
      }
      show_num(num);
      delay((num<100 and num>-100?500:100));
    }

    delay(10);
    M5.update();
}
