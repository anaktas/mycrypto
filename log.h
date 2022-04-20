/**
 * @file log.h
 * @author Anastasios Daris (darisanastasios@gmail.com)
 * 
 */
#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdarg.h>

/**
 * @brief Console colors for printf
 */
#define WHITE_COLOR  "\033[0m"
#define GREEN_COLOR  "\033[0;32m"
#define CYAN_COLOR   "\033[0;36m"
#define YELLOW_COLOR "\033[0;33m"
#define RED_COLOR    "\033[0;31m"

/**
 * @brief An enumeration which will
 * determine the log tag
 */
enum log_level
{
    DEBUG,   /*< [DGB] log tag */
    INFO,    /*< [INF] log tag */
    WARNING, /*< [WRN] log tag */
    ERROR    /*< [ERR] log tag */
};

void
beep();

void
dbg(const char *format, ...);

void
inf(const char *format, ...);

void
warn(const char *format, ...);

void
err(const char *format, ...);

void
print_log(enum log_level level, char *message);

char
*print_and_return_log(enum log_level level, char *message);

#endif