#pragma once
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>


class Terminal {
public:
    enum Color {
        RESET = 0,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        BRIGHT_YELLOW = 93,
        BRIGHT_GREEN = 92
    };
    static void PrintColored(const std::string& text, Color color);
    static void ClearLine();
    static void MoveCursorUp(int lines);
    static void MoveCursorDown(int lines);
    static void HideCursor();
    static void ShowCursor();
    static char GetChar();
    static void PrintText(const std::string& text, const std::vector<int>& colors_of_letters, size_t current_position, float wpm = -1.0f);
    static void Introduction(const std::string& text);
};
