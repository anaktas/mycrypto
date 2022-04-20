/**
 * @file log.c
 * @author Anastasios Daris (t.daris@7linternational.com)
 */
#include <time.h>
#include "log.h"
#include "config.h"

/**
 * @brief Creates a beeping sound from the speakers
 * 
 */
void
beep()
{
    printf("\a\n");
}


/**
 * @brief Wrapper of the print_log() function
 * for debug log level.
 * 
 * @param format the message format
 * @param ...    the var args
 */
void
dbg(const char *format, ...)
{
#ifdef ENABLE_DEBUG
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    print_log(DEBUG, buf);
#endif
}

/**
 * @brief Wrapper of the print_log() function
 * for info log level.
 * 
 * @param format the message format
 * @param ...    the var args
 */
void
inf(const char *format, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    print_log(INFO, buf);
}

/**
 * @brief Wrapper of the print_log() function
 * for warning log level.
 * 
 * @param format the message format
 * @param ...    the var args
 */
void
warn(const char *format, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    print_log(WARNING, buf);
}

/**
 * @brief Wrapper of the print_log() function
 * for error log level.
 * 
 * @param format the message format
 * @param ...    the var args
 */
void
err(const char *format, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    print_log(ERROR, buf);
}

/**
 * @brief Prints to the stdout a log with a timestamp,
 * a log level tag and a given message.
 * 
 * @param level   the log_level enumeration
 * @param message the message to print
 */
void
print_log(enum log_level level, char *message)
{
    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);

    char buf[1024];
    char time_header[512];

    sprintf(
        time_header, 
        "%04d/%02d/%02d  %02d:%02d:%02d: - ", 
        local_time->tm_year + 1900,
        local_time->tm_mon + 1,
        local_time->tm_mday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    switch (level)
    {
        case DEBUG:
            sprintf(
                buf, 
                "%s[%sDBG%s]: %s\n", 
                time_header, 
                CYAN_COLOR, 
                WHITE_COLOR, 
                message
            ); 
            printf("%s", buf);
            break;
        
        case INFO:
            sprintf(
                buf, 
                "%s[%sINF%s]: %s\n", 
                time_header, 
                GREEN_COLOR, 
                WHITE_COLOR, 
                message
            );
            printf("%s", buf);
            break;

        case WARNING:
            sprintf(
                buf, 
                "%s[%sWRN%s]: %s\n", 
                time_header, 
                YELLOW_COLOR, 
                WHITE_COLOR, 
                message
            );
            printf("%s", buf);
            break;

        case ERROR:
            sprintf(
                buf, 
                "%s[%sERR%s]: %s\n", 
                time_header, 
                RED_COLOR, 
                WHITE_COLOR, 
                message
            );
            printf("%s", buf);
            break;
    
        default:
            printf("%s\n", time_header);
            break;
    }
}

/**
 * @brief Prints to the stdout a log with a timestamp,
 * a log level tag and a given message and returns the
 * formatted message to the caller.
 * 
 * @param level   the log_level enumeration
 * @param message the message to print
 * 
 * @return char* the formatted with timestamp and log level message
 */
char
*print_and_return_log(enum log_level level, char *message)
{
    time_t t = time(NULL);
    struct tm *local_time = localtime(&t);

    static char buf[1024];
    char time_header[512];

    sprintf(
        time_header, 
        "%04d/%02d/%02d  %02d:%02d:%02d: - ", 
        local_time->tm_year + 1900,
        local_time->tm_mon + 1,
        local_time->tm_mday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    switch (level)
    {
        case DEBUG:
            sprintf(
                buf, 
                "%s[DBG]: %s\n", 
                time_header, 
                message
            );
            printf("%s", buf);
            break;
        
        case INFO:
            sprintf(
                buf, 
                "%s[INF]: %s\n", 
                time_header, 
                message
            );
            printf("%s", buf);
            break;

        case WARNING:
            sprintf(
                buf, 
                "%s[WRN]: %s\n", 
                time_header, 
                message
            );
            printf("%s", buf);
            break;

        case ERROR:
            sprintf(
                buf, 
                "%s[ERR]: %s\n", 
                time_header, 
                message
            );
            printf("%s", buf);
            break;
    
        default:
            printf("%s\n", time_header);
            break;
    }

    return buf;
}