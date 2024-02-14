#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>

char GetChar() {
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

void PrintText(const std::string& text, std::vector<int>& colors_of_letters) {
    std::cout << "\r\033[K";
    for (size_t i = 0; i < text.length(); ++i) {
        switch (colors_of_letters[i]) {
            case -1:
                std::cout << "\033[31m" << text[i] << "\033[0m";
                break;
            case 1:
                std::cout << "\033[32m" << text[i] << "\033[0m";
                break;
            default:
                std::cout << text[i];
                break;
        }
    }
    std::cout.flush();
}

int main() {
    constexpr char BACKSPACE = 127;
    std::string text = "Hello, World!";
    char input_letter = 0;
    char current_position = 0;
    std::vector<int> colors_of_letters(text.length(), 0);
    while (current_position < text.length()) {
        PrintText(text, colors_of_letters);
        input_letter = GetChar();
        if (input_letter == BACKSPACE) {
            if (current_position > 0) {
                --current_position;
                colors_of_letters[current_position] = 0;
            }
            continue;
        } else if (input_letter == text[current_position]) {
            colors_of_letters[current_position] = 1;
            ++current_position;
        } else {
            colors_of_letters[current_position] = -1;
            ++current_position;
        }
    }
    std::cout << std::endl;
}
