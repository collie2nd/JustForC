//
// Created by Vousmevoyez 刘 on 2019-05-27.
//

#ifndef CMAKE_LESSON6_HELLO_H
#define CMAKE_LESSON6_HELLO_H
#if defined _WIN32
#if LIBHELLO_BUILD
        #define LIBHELLO_API __declspec(dllexport)
    #else
        #define LIBHELLO_API __declspec(dllimport)
    #endif
#else
#define LIBHELLO_API
#endif
LIBHELLO_API void hello(const char* name);
#endif //CMAKE_LESSON6_HELLO_H