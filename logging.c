#include "logging.h"
// --- Helper function to print timestamped log messages ---
// The event_timestamp (when the log line is printed) will still be wall-clock time.
// The duration is in milliseconds.
void log_test_event(const char *test_name, const char *event_type, double duration_ms, bool result) {
    time_t current_time_raw;
    struct tm *time_info;
    char time_buffer[26]; // Enough for "YYYY-MM-DD HH:MM:SS" + null terminator

    time(&current_time_raw); // Get current wall-clock time for the log entry
    if (current_time_raw == ((time_t)-1)) {
        perror("Failed to get current time for logging");
        sprintf_s(time_buffer, sizeof(time_buffer), "[TIME_ERROR]");
    } else {
        // localtime_s is the secure version on Windows, but localtime works too
        // For simplicity and wider C compatibility here, we'll use localtime
        // and check its return.
        time_info = localtime(&current_time_raw);
        if (time_info == NULL) {
            perror("Failed to convert to local time for logging");
            sprintf_s(time_buffer, sizeof(time_buffer), "[TIME_CONV_ERROR]");
        } else {
            // Format: YYYY-MM-DD HH:MM:SS
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);
        }
    }

    if (duration_ms < 0) { // Indicates a "beginning" message or no duration
        printf("[%s] | Test [%s] %s\n", time_buffer, test_name, event_type);
    } else {
        printf("[%s] | Test [%s] %s | result = %s duration = %.3f ms\n", // .3f for ms precision
               time_buffer, test_name, event_type, result ? "PASSED" : "FAILED",  duration_ms);
    }
    fflush(stdout); // Ensure the output is printed immediately
}
