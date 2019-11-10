#include <cstdio>
#include <windows.h>
#include <NoFlowers/Util/Log.h>

void Log::print(const char* format, ...)
{
    char buffer[4096];
    va_list ap;

    va_start(ap, format);
    vsnprintf(buffer, 4096, format, ap);
    va_end(ap);

    MessageBoxA(NULL, buffer, "Error", 0);
}
