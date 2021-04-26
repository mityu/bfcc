#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::string IntermediateCode = R"(
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Buffer {
public:
    Buffer() : index_(0) {
        buf_.push_back(0);
    }
    void nextPtr() {
        if (++index_ == buf_.size()) {
            buf_.push_back(0);
        }
    }
    void prevPtr() {
        if (index_-- == 0) {
            buf_.insert(buf_.begin(), 0);
        }
    }
    void increment() {
        buf_[index_]++;
    }
    void decrement() {
        buf_[index_]--;
    }
    void serValue(char c) {
        buf_[index_] = c;
    }
    char getValue() const {
        return buf_[index_];
    }
private:
    std::vector<char> buf_;
    std::size_t index_;
};

int main() {
    Buffer buf;
)";


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Too few arguments" << std::endl;
        return -1;
    }

    const std::string source_file = argv[1];
    const std::string intermediate_file = source_file + ".cpp";
    std::ifstream source(source_file);
    if (!source) {
        std::cerr << "Cannot read " << source_file << std::endl;
        return -1;
    }

    std::string source_code;
    {
        char c;
        while (source.get(c)) {
            if (c == '.' || c == '+' || c == '-' || c == '<' || c == '>' ||
                    c == '[' || c == ']' || c == ',') {
                source_code += c;
            }
        }
        if (!source.eof()) {
            std::cerr << "File read error: " << source_file << std::endl;
        }
    }

    for (int i = 0; i < source_code.size(); ++i) {
        char c = source_code[i];
        switch (c) {
        case '.':
            IntermediateCode += "\tstd::cout << buf.getValue();\n";
            break;
        case '+':
            IntermediateCode += "\tbuf.increment();\n";
            break;
        case '-':
            IntermediateCode += "\tbuf.decrement();\n";
            break;
        case '<':
            IntermediateCode += "\tbuf.prevPtr();\n";
            break;
        case '>':
            IntermediateCode += "\tbuf.nextPtr();\n";
            break;
        case '[':
            IntermediateCode += R"(
            while (static_cast<int>(buf.getValue())) {
            )";
            break;
        case ']':
            IntermediateCode += R"(
            }
            )";
            break;
        case ',':
            IntermediateCode += R"(
            {
                char input;
                std::cin >> std::setw(1) >> input;
                buf.serValue(input);
            }
            )";
            break;
        default:
            std::cerr << "Cannot reach here" << std::endl;
            return -1;
            // Cannot reach here
        }
    }
    IntermediateCode += "std::cout << std::endl;\n}";

    std::ofstream ofs(intermediate_file);
    if (!ofs) {
        std::cerr << "Open intermediate-file failed" << std::endl;
        return -1;
    }
    ofs << IntermediateCode << std::endl;;
}
