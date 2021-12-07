//
//  C_code_style_demo.c
//  C_Learning
//
//  Created by Vousmevoyez 刘 on 2019/4/25.
//  Copyright © 2019 Vousmevoyez. All rights reserved.
//

#include <stdio.h>

//在自己编写函数的时候请注意在开头(main函数之前)写上你的函数的原型，并且在末尾(main函数之后)写上你的函数定义，这是一个很好的习惯以及规范
int function(int arg_1,float arg_2);

int main(int argc,char* argv[])
{
    int output =function(11, 22.0);
    printf("%d\n",output);
    return 0;
}

int function(int arg_1,float arg_2)
{
    int return_value = arg_1;
    float temp_float = arg_2;
    return return_value;
}

