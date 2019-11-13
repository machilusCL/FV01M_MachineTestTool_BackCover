#pragma once

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>

#define  u8 unsigned char
#define u32 unsigned int
typedef SSIZE_T ssize_t;

extern void anngic_trace(const char* func,int line,const char*cmd, ...);

#define DEBUG

#ifdef DEBUG
#define atrace(format,...) anngic_trace(__FUNCTION__,__LINE__,format,##__VA_ARGS__)
#else
#define atrace(format,...)
#endif


#define  aprint(format,...) anngic_trace(__FUNCTION__,__LINE__,format,##__VA_ARGS__)