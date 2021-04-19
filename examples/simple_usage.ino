#include "M5Atom.h"
#include "float2rgb_matrix.h"

uint8_t DisBuff[2 + 5 * 5 * 3];

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
    show_num(42);
    M5.update();
}

void loop()
{
}
