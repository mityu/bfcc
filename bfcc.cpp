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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Too few arguments" << std::endl;
        return -1;
    }

    std::ifstream source(argv[1]);
    if (!source) {
        std::cerr << "Cannot read " << argv[1] << std::endl;
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
            std::cerr << "File read error: " << argv[1] << std::endl;
        }
    }

    Buffer buf;
    for (int i = 0; i < source_code.size(); ++i) {
        char c = source_code[i];
        switch (c) {
        case '.':
            std::cout << buf.getValue();
            break;
        case '+':
            buf.increment();
            break;
        case '-':
            buf.decrement();
            break;
        case '<':
            buf.prevPtr();
            break;
        case '>':
            buf.nextPtr();
            break;
        case '[':
            if (static_cast<int>(buf.getValue()) == 0) {
                int count = 1;
                for (int j = i + 1; j < source_code.size(); ++j) {
                    if (source_code[j] == ']') {
                        if (--count == 0) {
                            i = j;
                            break;
                        }
                    } else if (source_code[j] == '[') {
                        count++;
                    }
                }
                if (count) {
                    std::cerr << "No matches for '['" << std::endl;
                    return -1;
                }
            }
            break;
        case ']':
            if (static_cast<int>(buf.getValue()) != 0) {
                int count = 1;
                for (int j = i - 1; j >= 0; --j) {
                    if (source_code[j] == '[') {
                        if (--count == 0) {
                            i = j;
                            break;
                        }
                    } else if (source_code[j] == ']') {
                        count++;
                    }
                }
                if (count) {
                    std::cerr << "No matches for ']'" << std::endl;
                    return -1;
                }
            }
            break;
        case ',':
            char input;
            std::cin >> std::setw(1) >> input;
            buf.serValue(input);
            break;
        default:
            std::cerr << "Cannot reach here" << std::endl;
            return -1;
            // Cannot reach here
        }
    }
    std::cout << std::endl;
}
