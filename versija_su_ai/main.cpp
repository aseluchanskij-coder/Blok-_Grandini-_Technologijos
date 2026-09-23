#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <fstream>

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
uint64_t suktiIKaire(uint64_t reiksme, unsigned int poslinkis) {
    return (reiksme << poslinkis) | (reiksme >> (64 - poslinkis));
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

        //maisymas
        for (int r = 0; r < 16; r++) {
            state[0] = suktiIKaire(state[0] ^ m[0], 19) + state[1];
            state[1] = suktiIKaire(state[1] ^ m[1], 29) + state[2];
            state[2] = suktiIKaire(state[2] ^ m[2], 37) + state[3];
            state[3] = suktiIKaire(state[3] ^ m[3], 43) + state[0];

            state[0] ^= state[2];
            state[1] ^= state[3];
        }
    } 
        
    std::stringstream ss;
    for (int i = 0; i < 4; i++) {
        ss << std::hex << std::setw(16) << std::setfill('0') << state[i];
    }

    return ss.str();
}

int main() {
    vector<uint8_t> duomenys;
    int pasirinkimas;

    cout << "Pasirinkite ivesties buda:" << endl;
    cout << "1 - Ivesti teksta ranka" << endl;
    cout << "2 - Nuskaityti is failo" << endl;
    cout << "Iveskite pasirinkima (1 arba 2): ";
    cin >> pasirinkimas;
    
    // Išvalome "Enter" paspaudimą po skaičiaus įvedimo, kad jis netrukdytų toliau
    cin.ignore(10000, '\n'); 

    if (pasirinkimas == 1) {
        string tekstas;
        cout << "Iveskite teksta (galima vesti kelis zodzius arba nieko): ";
        std::getline(cin, tekstas);

        for (size_t i = 0; i < tekstas.length(); i++) {
            duomenys.push_back(tekstas[i]);
        }
    } 
    else if (pasirinkimas == 2) {
        string failoPavadinimas;
        cout << "Iveskite failo pavadinima (pvz., test.txt): ";
        std::getline(cin, failoPavadinimas);

        // Atidarome failą binariniu režimu (kad nuskaitytų lygiai taip, kaip guli atmintyje)
        std::ifstream failas(failoPavadinimas, std::ios::binary);

        if (!failas) {
            cout << "KLAIDA: Nepavyko atidaryti failo '" << failoPavadinimas << "'!" << endl;
            return 1; // Baigiame programą su klaida
        }

        // Skaitome failą po vieną baitą iki pat pabaigos
        char baitas;
        while (failas.get(baitas)) {
            duomenys.push_back(static_cast<uint8_t>(baitas));
        }
        failas.close();
    } 
    else {
        cout << "Neteisingas pasirinkimas!" << endl;
        return 1;
    }

    cout << "Nuskaityta baitu: " << duomenys.size() << endl;
    
    string rezultatas = custom_hashas(duomenys);
    cout << "Gauta maisa (Hash): " << rezultatas << endl;

    return 0;
}

