#ifndef H_AnsiTerminal
#define H_AnsiTerminal

// The following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define C_RESET         "\033[0m"               // Reset formatting
#define C_BOLD          "\033[1m"               // Bold font
#define C_UNDERLINE     "\033[4m"               // Underline font
#define C_INVERT_FG_BG  "\033[7m"               // Inverse background and foregroung colors

// Foreground
#define C_BLACK_F       "\033[30m"              // Black
#define C_RED_F         "\033[31m"              // Red
#define C_GREEN_F       "\033[32m"              // Green
#define C_YELLOW_F      "\033[33m"              // Yellow
#define C_BLUE_F        "\033[34m"              // Blue
#define C_MAGENTA_F     "\033[35m"              // Magenta
#define C_CYAN_F        "\033[36m"              // Cyan
#define C_WHITE_F       "\033[37m"              // White

// Background
#define C_BLACK_B       "\033[40m"              // Black
#define C_RED_B         "\033[41m"              // Red
#define C_GREEN_B       "\033[42m"              // Green
#define C_YELLOW_B      "\033[43m"              // Yellow
#define C_BLUE_B        "\033[44m"              // Blue
#define C_MAGENTA_B     "\033[45m"              // Magenta
#define C_CYAN_B        "\033[46m"              // Cyan
#define C_WHITE_B       "\033[47m"              // White

#endif
