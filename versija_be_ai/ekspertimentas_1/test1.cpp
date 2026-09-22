#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <iomanip>

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

void bigEndian(size_t dydis_bitais, vector<unsigned char>& data){
    for (int i = 7; i >= 0; --i) {
        // 1. Paslenkame bitus į dešinę, kad norimas baitas atsidurtų pačiame gale (i * 8)
        // 2. Naudojame & 0xFF, kad "nukirptume" viską, išskyrus tą paskutinį baitą
        unsigned char baitas = (dydis_bitais >> (i * 8)) & 0xFF;
        data.push_back(baitas);
    }
}

void padding(vector<unsigned char>& data) {
    size_t dydis = data.size();
    size_t dydis_bitais = dydis * 8;
    data.insert(data.end(),0x80); // Pridedame skyrikli
    while(data.size() % 32 != 24){
        data.push_back(0x00); // Pridedame nulius uzpildyti, paliekame 8 baitus vietos dydzio bitams
    }
    bigEndian(dydis_bitais, data); // Pridedame dydzio bitus
}

void vertimasZodziais(const vector<unsigned char>& data, uint32_t* zodziai, const int& i){
    for(int j = 0; j < 8; ++j){
        int index = i + j * 4;
        zodziai[j] = (data[index] << 24) | (data[index + 1] << 16) | (data[index + 2] << 8) | data[index + 3];
    }
}

void lavina(uint32_t* state) {
    int pirminiai_sk[] = {7, 11, 13, 17, 19, 23, 29, 31};
    state[0] ^= state[7];
    for(int i = 1; i < 8; ++i){
        state[i] ^= state[i - 1];
    }
    for(int i = 0; i < 8; ++i){
        state[i] += state[i] << pirminiai_sk[i];
    }
    
}

string hash(const string& filename) {
    vector<unsigned char> text;
    uint32_t state[8] = {
        0x12345678, // seed 1
        0x8abcd123, // seed 2
        0xabcabcab, // seed 3
        0x87654321, // seed 4
        0xdefdefff, // seed 5
        0xaabbccdd, // seed 6
        0x11223344, // seed 7
        0xfedbc111 // seed 8
    };
    // for (unsigned char c : text) {
    //     std::cout << c;
    // }
    padding(text);
    for(int i = 0; i < text.size(); i += 32){
        uint32_t zodziai[8];
        vertimasZodziais(text, zodziai, i);
        for(int j = 0; j < 8; ++j){ //xor mixinimas
            state[j] ^= zodziai[j];
        }
        for(int k = 0; k < 16; ++k){
            lavina(state);
        }
    }

    std::stringstream hash_result;
    for (int i = 0; i < 8; ++i) {
        hash_result << std::hex           // Vertimas i sesioliktaine
                << std::setfill('0')      // Uzpildome nuliais
                << std::setw(8)           
                << state[i];             
    }   

    return hash_result.str();
}
int main() {
    string final_hashas = hash("input.txt");
    std::cout << "Hash: " << final_hashas << std::endl;
    return 0;
}