#ifndef LOGGER_INCL
#define LOGGER_INCL

#include <string>
#include <iostream>
#include <time.h>

#define __STDC_WANT_LIB_EXT1__ 1 // needed for localtime_s

#if defined(_MSC_VER) // windows OS
    #include <intrin.h>
    // causes a debug breakpoint to be hit
    #define debug_break() __debugbreak()
#elif defined(__APPLE__) // macOS
    // https://stackoverflow.com/questions/44140778/resumable-assert-breakpoint-on-ios-like-debugbreak-with-ms-compiler
    #if defined(__x86_64__) // intel
        // causes a debug breakpoint to be hit
        #define debug_break() __asm__("int $3")
    #elif defined(__arm64__) // arm64 mac
        // causes a debug breakpoint to be hit
        #define debug_break() __asm__("brk #0")
    #else
        #error "Unsupported architecture"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    // causes a debug breakpoint to be hit
    #define debug_break() __builtin_trap()
#endif

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// disable debug and trace logging for release
#if RELEASE == 1
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif

// if you don't want to pass any variadic arguments to a log call
#define NO_ARGS ""

#if defined(__APPLE__) || defined(__linux__) || defined(__gnu_linux__)

    #define COLOR_TRACE "\x1b[0m"   // black
    #define COLOR_DEBUG "\x1b[34m"  // blue 
    #define COLOR_INFO "\x1b[32m"   // green
    #define COLOR_WARN "\x1b[33m"   // yellow
    #define COLOR_ERROR "\x1b[31m"  // red
    #define COLOR_FATAL "\x1b[35m"  // magenta
    #define COLOR_RESET "\x1b[0m"   // black
    
    /**
     * @brief log a message to the console. this is used in the macros defined below
     * @tparam Args the types of the variadic arguments to the log message
     * @param color the color of the text to be printed
     * @param priority_name the name of the log priority, e.g. "FATAL", "ERROR", "WARN", etc.
     * @param msg the message to be printed
     * @param args variadic arguments to the log message
     */
    template<typename Arg, typename ... Args>
    static void log(const std::string &color, const std::string &priority_name, const std::string &msg, Arg&& arg, Args&&... args)
    {   
        time_t t = time(0);
        struct tm buffer;
        char time_str[26];
        asctime_r(localtime_r(&t, &buffer), time_str);

        std::cout << color << priority_name << time_str << msg << " ";
        std::cout << std::forward<Arg>(arg);
        ((std::cout << ',' << std::forward<Args>(args)), ...);
        std::cout << COLOR_RESET << "\n";
    }

#elif _WIN64
    #define COLOR_TRACE 7   // white
    #define COLOR_DEBUG 9   // blue
    #define COLOR_INFO 6    // green
    #define COLOR_WARN 2    // yellow
    #define COLOR_ERROR 4   // red
    #define COLOR_FATAL 13  // magenta
    #define COLOR_RESET 7   // white

    #define _CRT_SECURE_NO_WARNINGS
    #pragma warning(disable:4996)
    #include <windows.h> // WinAPI header for changing color of console text
    template<typename Arg, typename ... Args>
    static void Dlog(int color, const std::string &priority_name, const std::string &msg, Arg&& arg, Args&&... args)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (WORD)color);
        time_t t = time(0);

        std::cout << priority_name << asctime(localtime(&t)) << msg << " ";
        std::cout << std::forward<Arg>(arg);
        ((std::cout << ',' << std::forward<Args>(args)), ...);
        SetConsoleTextAttribute(hConsole, (WORD)COLOR_RESET);
        std::cout << "\n";
    }
#endif

#define FATAL(msg, ...) log(COLOR_FATAL, "[FATAL] \t", msg, ##__VA_ARGS__)
#define FATAL_MSG(msg) log(COLOR_FATAL, "[FATAL] \t", msg, NO_ARGS)
#define ERROR(msg, ...) log(COLOR_ERROR, "[ERROR] \t", msg, ##__VA_ARGS__)
#define ERROR_MSG(msg) log(COLOR_ERROR, "[ERROR] \t", msg, NO_ARGS)
#if LOG_WARN_ENABLED == 1
    #define WARN(msg, ...) log(COLOR_WARN, "[WARN] \t", msg, ##__VA_ARGS__)
    #define WARN_MSG(msg) log(COLOR_WARN, "[WARN] \t", msg, NO_ARGS)
#else
    #define WARN(msg, ...)
#endif
#if LOG_INFO_ENABLED == 1
    #define INFO(msg, ...) log(COLOR_INFO, "[INFO] \t", msg, ##__VA_ARGS__)
    #define INFO_MSG(msg) log(COLOR_INFO, "[INFO] \t", msg, NO_ARGS)
#else
    #define INFO(msg, ...)
#endif
#if LOG_DEBUG_ENABLED == 1
    #define DEBUG(msg, ...) log(COLOR_DEBUG, "[DEBUG] \t", msg, ##__VA_ARGS__)
    #define DEBUG_MSG(msg) log(COLOR_DEBUG, "[DEBUG] \t", msg, NO_ARGS)
#else
    #define DEBUG(msg, ...)
    #define DEBUG_MSG(msg)
#endif
#if LOG_TRACE_ENABLED == 1
    #define TRACE(msg, ...) log(COLOR_TRACE, "[TRACE] \t", msg, ##__VA_ARGS__)
    #define TRACE_MSG(msg) log(COLOR_TRACE, "[TRACE] \t", msg, NO_ARGS)
#else
    #define TRACE(msg, ...)
    #define TRACE_MSG(msg)
#endif

#define AssertionFailure(expr, msg, file, line) \
    FATAL("Assertion failure: %s\n\t message: %s\n\t file: %s, line: %d\n", expr, msg, file, line)

#define Assert(expr, ...) { if (expr) {} else { AssertionFailure(#expr, ##__VA_ARGS__, __FILE__, __LINE__); debug_break(); } }

#endif // LOGGER_INCL