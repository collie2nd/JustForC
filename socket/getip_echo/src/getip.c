#include "../include/getip_handler.h"

int main() {
    getiplist();
    char ip[16]={0};
    getlocalip(ip);
    printf("local ip = %s\n",ip);
    return 0;
}
