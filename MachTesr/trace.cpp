#include "stdafx.h"
#include "trace.h"

void anngic_trace(const char* func,int line,const char*cmd, ...)
{
	char format[128];
	va_list args;

	sprintf_s(format,"%s [%s][%u]\n",cmd,func,line);
	va_start(args,cmd);
	vprintf(format,args);
	va_end(args);
}