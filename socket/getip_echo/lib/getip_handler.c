//
// Created by liuxing1 on 5/28/19.
//

#include "../include/getip_handler.h"

void getiplist() {
    char host[100] = {0};
    if (gethostname(host, sizeof(host)) < 0)
        ERR_EXIT("gethostname");
    hostentp hp;
    if ((hp = gethostbyname(host)) == NULL)
        ERR_EXIT("gethostbyname");

    int i = 0;
    while (hp->h_addr_list[i] != NULL) {
        printf("%s\n", inet_ntoa(*(struct in_addr *) hp->h_addr_list[i]));
        ++i;
    }
}

int getlocalip(char *ip) {
    char host[100] = {0};
    if (gethostname(host, sizeof(host)) < 0)
        return -1;
    hostentp hp;
    if ((hp = gethostbyname(host)) == NULL)
        return -1;
//    strcpy(ip, inet_ntoa(*(struct in_addr *) hp->h_addr_list[0]));
    strcpy(ip, inet_ntoa(*(struct in_addr *) hp->h_addr));
    return 0;
}