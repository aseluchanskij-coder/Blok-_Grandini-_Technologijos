#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::vector;
using std::cin;
using std::endl;
using std::string;

uint64_t baitaiIUInt64(const vector<uint8_t>& blokas, size_t pradzia) {
    uint64_t rezultatas = 0;
    for (int i = 0; i < 8; i++) {
        rezultatas |= (static_cast<uint64_t>(blokas[pradzia + i]) << (56 - (i * 8)));
    }
    return rezultatas;
}

string custom_hashas(vector<uint8_t> ivestis) {
    uint64_t state[4] = {
        0xD6D12E7B5A03A401ULL, 
        0x8A59B51F41029312ULL, 
        0x32A398246E20349AULL, 
        0x7158932402138901ULL  
    };

    const uint8_t pirminiai[32] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
        59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131
    };

    vector<uint8_t> padded = ivestis;
    uint64_t originalusIlgis = ivestis.size();

    padded.push_back(0x80);

    while ((padded.size() + 8) % 32 != 0) {
        size_t index = padded.size() % 32;
        padded.push_back(pirminiai[index]);
    }

    for (int i = 7; i >= 0; i--) {
        padded.push_back((originalusIlgis >> (i * 8)) & 0xFF);
    }
    
    //suzinome kiek 32 baitu bloku
    size_t blokuSkaicius = padded.size() / 32;

    //einame per kiekvieną bloką
    for (size_t b = 0; b < blokuSkaicius; b++) {
        vector<uint8_t> dabartinisBlokas(32); //laikinai laikysime 32 baitus
        
        bool yraLyginis = ((b + 1) % 2 == 0); 

        for (int i = 0; i < 32; i++) {
            if (yraLyginis) {
                dabartinisBlokas[i] = padded[b * 32 + (31 - i)];
            } else {
                dabartinisBlokas[i] = padded[b * 32 + i];
            }
        }
        
        
        uint64_t m[4];
        m[0] = baitaiIUInt64(dabartinisBlokas, 0);
        m[1] = baitaiIUInt64(dabartinisBlokas, 8);
        m[2] = baitaiIUInt64(dabartinisBlokas, 16);
        m[3] = baitaiIUInt64(dabartinisBlokas, 24);

        
    }

    return "LaikinasHash...";
}

int main() {
    vector<uint8_t> duomenys;
    string tekstas;

    cout << "Iveskite teksta (viena zodi): ";
    cin >> tekstas;

    for (size_t i = 0; i < tekstas.length(); i++) { //dedame raides i baitu vektoriu
        duomenys.push_back(tekstas[i]);
    }

    cout << "Nuskaityta baitu: " << duomenys.size() << endl; //kad ateityje suzinot kiek baitu reik pripildyti pading,
    // kad pripildyt iki 32 baitu, tai darysiu pasitelkiant pirminiais skaiciais 
    
    string rezultatas = custom_hashas(duomenys);
    cout << "Gauta maisa (Hash): " << rezultatas << endl;

    return 0;
}

