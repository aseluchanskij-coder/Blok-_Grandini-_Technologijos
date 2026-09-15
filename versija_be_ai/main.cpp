#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;

void readFile(const string& filename, vector<unsigned char>& data) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open file");
    }
    char buffer[32];
    while (in) {
    in.read(buffer, sizeof(buffer));
    data.insert(data.end(), buffer, buffer + in.gcount()); //gcount apskaiciuoja kiek baitu buvo perskaityta
    }
}

int main() {
    vector<unsigned char> text;
    try {
        readFile("input.txt", text);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    for (unsigned char c : text) {
        std::cout << c;
    }
    return 0;
}