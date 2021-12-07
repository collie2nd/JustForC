//
// Created by Vousmevoyez 刘 on 2019-06-19.
//

#include <stdio.h>

typedef enum L7{
    L7_SUCCESS = 0,
    L7_FAILURE = 2,


    L7_TIMEOUT,
    L7_ERROR,
    L7_NO_VALUE
}L7_RC_t;

int main(){
    printf("%d\n",L7_NO_VALUE);
    L7_RC_t l7Rc;
    printf("%d",l7Rc);
}