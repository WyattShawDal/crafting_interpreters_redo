#ifndef LOGGING_H
#define LOGGING_H
#include <windows.h>  // For QueryPerformanceCounter, QueryPerformanceFrequency, Sleep
#include <time.h>
#include <stdio.h>
#include <stdbool.h>


void log_test_event(const char *test_name, const char *event_type, double duration_seconds, bool result);
// --- Macro to simplify running tests and logging ---
#define RUN_TEST_MS(test_func) \
    do { \
        LARGE_INTEGER start_li, end_li, frequency; \
        double duration_ms = 0.0; \
        const char* test_name = #test_func; /* Get function name as string */ \
        \
        log_test_event(test_name, "beginning", -1.0, false); \
        \
        if (!QueryPerformanceFrequency(&frequency)) { \
            fprintf(stderr, "QueryPerformanceFrequency failed!\n"); \
            /* Handle error or skip test */ \
            break; \
        } \
        if (!QueryPerformanceCounter(&start_li)) { \
            fprintf(stderr, "QueryPerformanceCounter start failed!\n"); \
            /* Handle error or skip test */ \
            break; \
        } \
        \
        bool result = test_func(); \
        \
        if (!QueryPerformanceCounter(&end_li)) { \
            fprintf(stderr, "QueryPerformanceCounter end failed!\n"); \
            /* Handle error or skip test */ \
            break; \
        } \
        \
        /* Calculate duration in milliseconds */ \
        if (frequency.QuadPart > 0) { \
            duration_ms = (double)(end_li.QuadPart - start_li.QuadPart) * 1000.0 / frequency.QuadPart; \
        } else { \
            fprintf(stderr, "Frequency is zero, cannot calculate duration for %s.\n", test_name); \
        } \
        \
        log_test_event(test_name, "returned", duration_ms, result); \
    } while (0)
#endif
