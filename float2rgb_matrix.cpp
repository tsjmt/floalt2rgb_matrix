#include "float2rgb_matrix.h"

namespace float2rgb_matrix{

enum class content_type{
    one_digit,
    two_digit,
    inf,
};

struct display_content{
    content_type type;
    int digit[2]; // 0: 1st digit, 1: 2nd digit. should in 0~9
    bool has_point;
    USE_COLOR color[3]; // 0: 1st digit, 1: 2nd digit, 2: other(point, inf, ...) 
};

display_content conv_float2dec(float input){
    const float epsilon = 1e-7;
    USE_COLOR color =  input > 0.0f ? USE_COLOR_FOR_POSITIVE : USE_COLOR_FOR_NEGATIVE;
    if(input < 0.0f)input = -input;

    display_content res = {};
    if(input == 0.0f){
        res.type=content_type::one_digit;
        res.digit[0]=0;
        res.color[0]=USE_COLOR_FOR_OTHERS;
    }else if(input < 10.0f){
        res.type=content_type::two_digit,
        res.digit[0]=(int)input;
        res.digit[1]=(int)((input-(int)input)*10+epsilon);
        res.has_point=true;
        res.color[0]=color;
        res.color[1]=color;
        res.color[2]=USE_COLOR_FOR_OTHERS;
    }else if(input < 100.0f){
        res.type=content_type::two_digit;
        res.digit[0]=((int)input)/10;
        res.digit[1]=((int)input)%10;
        res.has_point=false;
        res.color[0]=color;
        res.color[1]=color;
        res.color[2]=USE_COLOR_FOR_OTHERS;
    }else if (input < 1e10){
        int d=0;
        while(input>=10.0f){
            input/=10.0f;
            d++;
        }
        res.type=content_type::two_digit;
        res.digit[0]=(int)(input+epsilon);
        res.digit[1]=d;
        res.has_point=false;
        res.color[0]=color;
        res.color[1]=USE_COLOR_FOR_OTHERS;
        res.color[2]=USE_COLOR_FOR_OTHERS;
    }else{
        res.type=content_type::inf;
        res.color[2]=color;
    }

    return res;
}

void clear_buf(matrix_buf* buf){
    for(int h=0;h<height;h++){
        for(int w=0;w<width;w++){
            for(int c=0;c<rgb_num;c++){
                buf->data[h][w][c]=0;
            }
        }
    }
}

void add_num(int digit, int digit_place, USE_COLOR uc, matrix_buf* buf){
    int w_offset = (digit_place == 0 ? 0 : 3);
    for(int h=0;h<height;h++){
        for(int w=0;w<2;w++){
            if(digit_shape[digit][h][w]){
                for(int c=0;c<rgb_num;c++){
                    buf->data[h][w+w_offset][c]=colors[uc][c];
                }
            }
        }
    }
}

void add_point(USE_COLOR uc, matrix_buf* buf){
    for(int c=0;c<rgb_num;c++){
        buf->data[4][2][c]=colors[uc][c];
    }
}

void add_inf(USE_COLOR uc, matrix_buf* buf){
    for(int h=0;h<height;h++){
        for(int w=0;w<width;w++){
            if(inf_shape[h][w]){
                for(int c=0;c<rgb_num;c++){
                    buf->data[h][w][c]=colors[uc][c];
                }
            }
        }
    }
}

void add_error(USE_COLOR uc, matrix_buf* buf){
    for(int h=0;h<height;h++){
        for(int w=0;w<width;w++){
            if(error_shape[h][w]){
                for(int c=0;c<rgb_num;c++){
                    buf->data[h][w][c]=colors[uc][c];
                }
            }
        }
    }
}

bool is_one_digit(int num){
    return (num>=0 && num<=9);
}

void set_buf(const display_content* cont, matrix_buf* buf){
    switch (cont->type){
    case content_type::one_digit:
        if(is_one_digit(cont->digit[0])){
            add_num(cont->digit[0],1,cont->color[0], buf);
        } else add_error(USE_COLOR_FOR_OTHERS, buf);
        break;
    case content_type::two_digit:
        if(is_one_digit(cont->digit[0]) && is_one_digit(cont->digit[1])){
            add_num(cont->digit[0],0,cont->color[0], buf);
            add_num(cont->digit[1],1,cont->color[1], buf);
            if(cont->has_point)add_point(cont->color[2], buf);
        } else add_error(USE_COLOR_FOR_OTHERS, buf);
        break;
    case content_type::inf:
        add_inf(cont->color[2], buf);
        break;
    
    default:
        add_error(USE_COLOR_FOR_OTHERS, buf);
    }
}

void conv_float2rgb_buf(float input, matrix_buf* out_buf){
    display_content cont=conv_float2dec(input);
    clear_buf(out_buf);
    set_buf(&cont, out_buf);
}

} // namespace float2rgb_matrix

#ifdef UNITTEST // unit test

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

using namespace std;
using namespace float2rgb_matrix;

const char color_name[3]={'P','N','O'};

string str_disp_cont(const display_content& cont){
    
    stringstream ss;
    switch (cont.type){
    case content_type::one_digit:
        ss<<color_name[cont.color[0]]<<cont.digit[0];
        break;
    case content_type::two_digit:
        ss<<color_name[cont.color[0]]<<cont.digit[0];
        if(cont.has_point)ss<<color_name[cont.color[2]]<<".";
        ss<<color_name[cont.color[1]]<<cont.digit[1];
        break;
    case content_type::inf:
        ss<<color_name[cont.color[2]]<<"inf";
        break;
    default:
        break;
    }
    return ss.str();
}

void test_conv(float input, string expected){
    auto actual=str_disp_cont(conv_float2dec(input));
    if(actual==expected){
        cout<<"OK"<<endl;
    }else{
        cout<<"ERROR input:"<<input<<" expected:"<<expected<<" actual:"<<actual<<endl;
    }
};

char show_color(int r, int g, int b){
    if( r && !g && !b)return 'R';
    if(!r &&  g && !b)return 'G';
    if(!r && !g &&  b)return 'B';
    if( r &&  g && !b)return 'Y';
    if( r && !g &&  b)return 'P';
    if(!r &&  g &&  b)return 'S';
    if( r &&  g &&  b)return 'W';
    if(!r && !g && !b)return '.';
    return 'E';
}

void show_matrix(matrix_buf* buf){
    for(int h=0;h<height;h++){
        for(int w=0;w<width;w++){
          cout << " "
               << show_color(buf->data[h][w][0],
                             buf->data[h][w][1],
                             buf->data[h][w][2]);
        }
        cout<<endl;
    }
}

void test_disp(display_content cont){
    cout<<str_disp_cont(cont)<<endl;
    matrix_buf buf;
    clear_buf(&buf);
    set_buf(&cont, &buf);
    show_matrix(&buf);
}

int main(){
    // tset for float -> disp content
    test_conv(0.0f,     "O0");
    test_conv(0.05f,    "P0O.P0");
    test_conv(0.1f,     "P0O.P1");
    test_conv(9.99999,  "P9O.P9");
    test_conv(10.0f,    "P1P0");
    test_conv(99.9999,  "P9P9");
    test_conv(100.0f,   "P1O2");
    test_conv(9.99999e9,"P9O9");
    test_conv(1e10,     "Pinf");
    test_conv(-10.0f,   "N1N0");   
    test_conv(-99.9999, "N9N9");
    test_conv(-1e10,    "Ninf");


    // test for disp content -> rgb_matrix
    display_content positive_one_digit={
        .type=content_type::one_digit,
        .digit={0},
        .color={USE_COLOR_FOR_POSITIVE}
    };
    test_disp(positive_one_digit);

    display_content two_negative_digit_with_point={
        .type=content_type::two_digit,
        .digit={1,2},
        .has_point=true,
        .color={USE_COLOR_FOR_NEGATIVE,USE_COLOR_FOR_NEGATIVE,USE_COLOR_FOR_OTHERS}
    };
    test_disp(two_negative_digit_with_point);

    display_content expo_disp={
        .type=content_type::two_digit,
        .digit={8,9},
        .color={USE_COLOR_FOR_NEGATIVE, USE_COLOR_FOR_OTHERS}
    };
    test_disp(expo_disp);

    display_content positive_inf={
        .type=content_type::inf,
        .color={USE_COLOR_FOR_OTHERS,USE_COLOR_FOR_OTHERS,USE_COLOR_FOR_POSITIVE}
    };
    test_disp(positive_inf);

    display_content digit_over_10={
        .type=content_type::two_digit,
        .digit={10,5},
        .has_point=true,
        .color={USE_COLOR_FOR_POSITIVE,USE_COLOR_FOR_POSITIVE,USE_COLOR_FOR_OTHERS}
    };
    test_disp(digit_over_10);
    return 0;
}

#endif
