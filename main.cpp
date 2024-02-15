#include "terminal.h"

std::vector<std::string> ReadDictionary(const std::string& filename = "dictionary.txt") {
    std::vector<std::string> words;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return words;
    }

    std::string word;
    while (file >> word) {
        words.push_back(word);
    }

    file.close();
    return words;
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

int main() {
    constexpr char BACKSPACE = 127;
    const std::string text = GenerateRandomText(ReadDictionary());
    char input_letter = 0;
    size_t current_position = 0;
    float wpm = -1.0f;
    std::vector<int> colors_of_letters(text.length(), 0);

    Terminal::Introduction(text);
    Terminal::PrintText(text, colors_of_letters, current_position);

    while (current_position < text.length()) {

        Terminal::PrintText(text, colors_of_letters, current_position, wpm);
        input_letter = Terminal::GetChar();
        static auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        if (duration.count() < 500) {
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
