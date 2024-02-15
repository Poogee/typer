#include "terminal.h"

void Terminal::PrintColored(const std::string& text, Terminal::Color color) {
    std::cout << "\033[" << static_cast<int>(color) << "m" << text << "\033[0m";
}

void Terminal::ClearLine() {
    std::cout << "\r\033[K";
}

void Terminal::MoveCursorUp(int lines) {
    std::cout << "\033[" << lines << "A";
}

void Terminal::MoveCursorDown(int lines) {
    std::cout << "\033[" << lines << "B";
}

void Terminal::HideCursor() {
    std::cout << "\033[?25l";
}

void Terminal::ShowCursor() {
    std::cout << "\033[?25h";
}

char Terminal::GetChar() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    if (read(0, &buf, 1) < 0) {
        perror("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror("tcsetattr ~ICANON");
    }
    return buf;
}

void Terminal::PrintText(const std::string& text, const std::vector<int>& colors_of_letters, size_t current_position,
                         float wpm) {
    if (wpm != -1) {
        MoveCursorUp(2);
        ClearLine();
        PrintColored("WPM: ", WHITE);
        PrintColored(std::to_string(static_cast<int>(wpm)), CYAN);
        MoveCursorDown(2);
    }
    ClearLine();

    for (size_t i = 0; i < text.length(); ++i) {
        if (i == current_position) {
            PrintColored(std::string(1, text[i]), BRIGHT_YELLOW);
            continue;
        }
        switch (colors_of_letters[i]) {
            case -1:
                PrintColored(std::string(1, text[i]), RED);
                break;
            case 1:
                PrintColored(std::string(1, text[i]), GREEN);
                break;
            default:
                std::cout << text[i];
                break;
        }
    }
    std::cout.flush();
}

void Terminal::Introduction(const std::string& text) {
    HideCursor();
    std::cout << "Press any key to start typing \n" << std::endl;
    std::cout << text;
    std::cout.flush();
}
