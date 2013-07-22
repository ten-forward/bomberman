#include "printlog.hpp"

#include <cstdio>
#include <cstdarg>

#ifdef ANDROID
#define PRINTLOG_DEFINED
#include <android/log.h>

void printlog(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    __android_log_vprint(ANDROID_LOG_DEBUG, "BOMBERMAN", fmt, ap);
    va_end(ap);
}

#endif // ANDROID

// Final fallback: printf
#ifndef PRINTLOG_DEFINED

void printlog(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

#endif // PRINTLOG_DEFINED

