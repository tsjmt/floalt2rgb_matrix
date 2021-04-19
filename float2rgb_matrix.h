#pragma once

namespace float2rgb_matrix{

const int height = 5, width = 5;
const int rgb_num = 3;
typedef enum {
  USE_COLOR_FOR_POSITIVE = 0,
  USE_COLOR_FOR_NEGATIVE,
  USE_COLOR_FOR_OTHERS,
  USE_COLOR_NUM
} USE_COLOR;
const int colors[USE_COLOR_NUM][rgb_num] = {
    {0xFF, 0x00, 0x00}, 
    {0x00, 0x00, 0xFF}, 
    {0x00, 0xFF, 0x00}};
const int digit_width = 2;
const bool digit_shape[10][height][digit_width] = {
    { // 0
        {1,1},
        {1,1},
        {1,1},
        {1,1},
        {1,1},
    },
    { // 1 
        {0,1},
        {0,1},
        {0,1},
        {0,1},
        {0,1},
    },
    { // 2
        {1,1},
        {0,1},
        {1,1},
        {1,0},
        {1,1},
    },
    { // 3
        {1,1},
        {0,1},
        {1,1},
        {0,1},
        {1,1},
    },
    { // 4
        {1,0},
        {1,1},
        {1,1},
        {0,1},
        {0,1},
    },
    { // 5
        {1,1},
        {1,0},
        {1,1},
        {0,1},
        {1,1},
    },
    { // 6
        {1,1},
        {1,0},
        {1,1},
        {1,1},
        {1,1},
    },
    { // 7
        {1,1},
        {0,1},
        {0,1},
        {0,1},
        {0,1},
    },
    { // 8
        {1,1},
        {1,1},
        {0,0},
        {1,1},
        {1,1},
    },
    { // 9
        {1,1},
        {1,1},
        {1,1},
        {0,1},
        {1,1},
    },
};
const bool inf_shape[height][width]={
    {1,0,0,1,1},
    {0,0,0,1,0},
    {1,0,0,1,1},
    {1,1,1,1,0},
    {1,1,1,1,0},
};
const bool error_shape[height][width]={
    {1,1,0,0,0},
    {1,0,0,0,0},
    {1,1,0,1,1},
    {1,0,0,1,0},
    {1,1,0,1,0},
};
struct matrix_buf{
    char data[height][width][rgb_num];
};

void conv_float2rgb_buf(float input, matrix_buf* out_buf);

}
