#ifndef __DEBUG_H
#define __DEBUG_H

#define COLOR_RED			"\x1b[31m"
#define COLOR_GREEN			"\x1b[32m"
#define COLOR_YELLOW		"\x1b[33m"
#define COLOR_BLUE			"\x1b[34m"
#define COLOR_MAGENTA		"\x1b[35m"
#define COLOR_CYAN			"\x1b[36m"
#define COLOR_ORANGE		"\033[38:5:208:0m"
#define COLOR_RESET			"\x1b[0m"

#define COLOR_GREEN_UNDERLINE	"\x1b[32;4m"
#define COLOR_GREEN_BOLD		"\x1b[32;1m"

#define dprintf(fmt, ...) \
do { printf(COLOR_YELLOW "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#define gprintf(fmt, ...) \
do { printf(COLOR_GREEN "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#define rprintf(fmt, ...) \
do { printf(COLOR_RED "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#define cprintf(fmt, ...) \
do { printf(COLOR_CYAN "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#define bprintf(fmt, ...) \
do { printf(COLOR_BLUE "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#define mprintf(fmt, ...) \
do { printf(COLOR_MAGENTA "[DBG u-boot] %s:%d %s(): " \
		COLOR_RESET fmt, __FILE__, __LINE__, __func__,\
		##__VA_ARGS__); } while (0)

#endif
