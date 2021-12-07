//
//  float_compare.c
//  C_Learning
//
//  Created by Vousmevoyez 刘 on 2019/4/29.
//  Copyright © 2019 Vousmevoyez. All rights reserved.
//

#include <stdio.h>
#include <math.h>
const double eps = le-8;
#define Equ(a,b) ((fabs((a)-(b)))<(eps))
int main(){
    double db=1.23;
    if (Equ(db,1.23)){
        printf("true");
    }else {
        printf("false");
    }
    return 0;
}
