#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <ctime>

std::vector<std::string> ReadDictionary(const std::string& filename = "dictionary.txt") {
    std::vector<std::string> words;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return words;
    }

    std::string word;
    while (file >> word) {
        words.push_back(word);
    }

    file.close();
    return words;
}

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

void PrintText(const std::string& text, std::vector<int>& colors_of_letters, size_t& current_position,
               float wpm = -1.0f) {
    if (wpm != -1) {
        std::cout << "\033[2A";
        std::cout << "\r\033[2K";
        std::cout << "\033[97mWPM: " << round(wpm) << "\033[0m" << std::endl;
        std::cout << "\033[2B";
    }
    std::cout << "\r\033[K";

    for (size_t i = 0; i < text.length(); ++i) {
        if (i == current_position) {
            std::cout << "\033[43;1m" << text[i] << "\033[0m";
            continue;
        }
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

std::string GenerateRandomText(const std::vector<std::string>& dictionary, const size_t length = 10) {
    srand(time(NULL));
    std::string text;
    for (size_t i = 0; i < length; ++i) {
        text += dictionary[rand() % dictionary.size()];
        text += " ";
    }
    text += dictionary[rand() % dictionary.size()];
    return text;
}

void Introduction(const std::string& text) {
    std::cout << "\033[?25l";
    std::cout << "Press any key to start typing \n" << std::endl;
    std::cout << text;
    std::cout.flush();
}

int main() {
    constexpr char BACKSPACE = 127;
    const std::string text = GenerateRandomText(ReadDictionary());
    char input_letter = 0;
    size_t current_position = 0;
    float wpm = -1.0f;
    std::vector<int> colors_of_letters(text.length(), 0);

    Introduction(text);
    PrintText(text, colors_of_letters, current_position);

    while (current_position < text.length()) {

        PrintText(text, colors_of_letters, current_position, wpm);
        input_letter = GetChar();
        static auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        if (duration.count() < 1000) {
            wpm = -1.0f;
        } else {
            wpm = (current_position / 5.0f) / (duration.count() / 60000.0f);
        }

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
