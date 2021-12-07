//
// Created by liuxing1 on 7/2/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

char *create_js(void);

int parse_js(char *out);


int main() {
    create_js();
    parse_js(create_js());
    return 0;
}

char *create_js(void) {
    cJSON *usr1, *usr2, *array, *root;

    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    usr1 = cJSON_CreateObject();
    usr2 = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "body", array);
    cJSON_AddItemToArray(array, usr1);
    cJSON_AddItemToArray(array, usr2);

    cJSON_AddStringToObject(usr1, "name", "liuxingj");      //加入键值，加字符串
    cJSON_AddStringToObject(usr1, "passwd", "12345678");
    cJSON_AddNumberToObject(usr1, "num", 3);             //加整数

    cJSON_AddStringToObject(usr2, "name", "lihaoy");   //加入键值，加字符串
    cJSON_AddStringToObject(usr2, "passwd", "12345678");
    cJSON_AddNumberToObject(usr2, "num", 3);             //加整数

    char *out = cJSON_PrintUnformatted(root);                       //将json形式打印成正常字符串形式
    printf("%s\n", out);

    // 释放内存
    cJSON_Delete(root);
    return out;
}

int parse_js(char *out) {
    cJSON *jsParser;
    jsParser = cJSON_Parse(out);
    cJSON *usr1, *usr2, *js_array, *js_name, *js_password, *js_num;

    js_array = cJSON_GetObjectItem(jsParser, "body");

    usr1 = cJSON_GetArrayItem(js_array, 1);

    js_name = cJSON_GetObjectItem(usr1, "name");
    js_password = cJSON_GetObjectItem(usr1, "passwd");
    js_num = cJSON_GetObjectItem(usr1, "num");

    printf("%s:%s:%d", js_name->valuestring, js_password->valuestring, js_num->valueint);
}