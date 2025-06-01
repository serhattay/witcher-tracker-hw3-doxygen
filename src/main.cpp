#include <iostream>
#include <string>
#include <vector>

bool execute_line(const std::string&);
void replaceEscapeSequences(std::string&);

int main() {
    std::string line;
    size_t index = 0;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (std::cin.eof() || line == "Exit")
            break;
        
        replaceEscapeSequences(line);
        
        // std::cerr << "Received: " << line << std::endl;

        // Uncomment and implement when ready:

        // SERHAT: I have changed the return type of execute_line from int to bool to conform tokenizer and parser's return types,
        // similarly it was result == -1 and i made it == false. Consider this if needed.
        
        bool result = execute_line(line);
        if (result == false) {
            std::cout << "INVALID\n";
        }
    }
    return 0;
}

void replaceEscapeSequences(std::string& str) {
    size_t pos = 0;
    // Replace all occurrences of "\\n" with '\n'
    while ((pos = str.find("\\n", pos)) != std::string::npos) {
        str.replace(pos, 2, "\n");
    }

    pos = 0;
    // Replace all occurrences of "\\t" with '\t'
    while ((pos = str.find("\\t", pos)) != std::string::npos) {
        str.replace(pos, 2, "\t");
    }
}
