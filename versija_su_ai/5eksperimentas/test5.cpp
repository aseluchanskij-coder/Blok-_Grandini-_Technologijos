#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;


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
    
    size_t blokuSkaicius = padded.size() / 32;

    for (size_t b = 0; b < blokuSkaicius; b++) {
        vector<uint8_t> dabartinisBlokas(32);
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


struct KolizijosPavyzdys {
    string ivestisA;
    string ivestisB;
    string hash;
};

string generuotiAtsitiktineEilute(int ilgis, std::mt19937& gen, std::uniform_int_distribution<int>& dist) {
    string s = "";
    s.reserve(ilgis);
    for (int i = 0; i < ilgis; ++i) {
        s += static_cast<char>(dist(gen));
    }
    return s;
}

int main() {
    const unsigned int SEED = 2026;
    std::mt19937 generatorius(SEED);
    
    std::uniform_int_distribution<int> distribucija(32, 126);

    int ilgiai[] = {10, 100, 500, 1000};
    const int poruSkaicius = 100000;

    cout << "========================================================" << endl;
    cout << "          KOLIZIJU EKSPERIMENTAS (custom_hashas)        " << endl;
    cout << "========================================================" << endl;
    cout << "Abecele: ASCII spausdinami simboliai [32..126]" << endl;
    cout << "Generatoriaus pradine reiksme (Seed): " << SEED << endl;
    cout << "Poru skaicius kiekvienam ilgiui: " << poruSkaicius << endl;
    cout << "========================================================\n" << endl;

    for (int ilgis : ilgiai) {
        cout << "--------------------------------------------------------" << endl;
        cout << "Tikrinamas ilgis: " << ilgis << " baitu" << endl;
        cout << "--------------------------------------------------------" << endl;

        int poriniaiKolizijos = 0;
        
        std::unordered_map<string, string> hashIvestisMap;
        hashIvestisMap.reserve(poruSkaicius * 2);

        std::unordered_set<string> unikaliosIvestys;
        unikaliosIvestys.reserve(poruSkaicius * 2);

        vector<KolizijosPavyzdys> pavyzdziai;

        for (int i = 0; i < poruSkaicius; ++i) {
            string strA = generuotiAtsitiktineEilute(ilgis, generatorius, distribucija);
            string strB = generuotiAtsitiktineEilute(ilgis, generatorius, distribucija);

            while (strA == strB) {
                strB = generuotiAtsitiktineEilute(ilgis, generatorius, distribucija);
            }

            unikaliosIvestys.insert(strA);
            unikaliosIvestys.insert(strB);

            vector<uint8_t> vecA(strA.begin(), strA.end());
            vector<uint8_t> vecB(strB.begin(), strB.end());

            string hashA = custom_hashas(vecA);
            string hashB = custom_hashas(vecB);

            if (hashA == hashB) {
                poriniaiKolizijos++;
            }

            auto itA = hashIvestisMap.find(hashA);
            if (itA != hashIvestisMap.end()) {
                if (itA->second != strA) {
                    pavyzdziai.push_back({itA->second, strA, hashA});
                }
            } else {
                hashIvestisMap[hashA] = strA;
            }

            auto itB = hashIvestisMap.find(hashB);
            if (itB != hashIvestisMap.end()) {
                if (itB->second != strB) {
                    pavyzdziai.push_back({itB->second, strB, hashB});
                }
            } else {
                hashIvestisMap[hashB] = strB;
            }
        }

        cout << "  * Porose rastu koliziju skaicius: " << poriniaiKolizijos << " / " << poruSkaicius << endl;
        cout << "  * Skirtingu sugeneruotu ivesciu skaicius: " << unikaliosIvestys.size() << endl;
        cout << "  * Skirtingu ivesciu grupiu (unikaliu hash): " << hashIvestisMap.size() << endl;
        cout << "  * Viso rinkinio koliziju skaicius: " << pavyzdziai.size() << endl;

        if (!pavyzdziai.empty()) {
            cout << "  * Rastu koliziju pavyzdziai:" << endl;
            for (size_t k = 0; k < std::min<size_t>(pavyzdziai.size(), 3); ++k) {
                cout << "    [" << k + 1 << "] Hash: " << pavyzdziai[k].hash << endl;
                cout << "        Ivestis 1: " << pavyzdziai[k].ivestisA << endl;
                cout << "        Ivestis 2: " << pavyzdziai[k].ivestisB << endl;
            }
        }
        cout << endl;
    }

    cout << "========================================================" << endl;
    cout << "          STRUKTŪRUOTŲ ĮVESČIŲ TIKRINIMAS               " << endl;
    cout << "========================================================" << endl;

    vector<string> strukturinesIvestys = {
        "ABCDEFGHIJKLMNOP",
        "BACDEFGHIJKLMNOP", 
        "PONMLKJIHGFEDCBA", 
        "AAAAAAAAAAAAAAAA",
        "ABABABABABABABAB",
        "0000000000000000",
        "1111111111111111",
        "0000000000000001",
        "0000000000000002"
    };

    std::unordered_map<string, string> strukturuotiHashai;
    int strukturuotosKolizijos = 0;

    for (const string& str : strukturinesIvestys) {
        vector<uint8_t> vec(str.begin(), str.end());
        string h = custom_hashas(vec);

        cout << "Tekstas: " << std::setw(20) << str << " | Hash: " << h << endl;

        auto it = strukturuotiHashai.find(h);
        if (it != strukturuotiHashai.end() && it->second != str) {
            strukturuotosKolizijos++;
            cout << "  [!] RASTA KOLIZIJA tarp: \"" << it->second << "\" ir \"" << str << "\"" << endl;
        } else {
            strukturuotiHashai[h] = str;
        }
    }

    cout << "\nStrukturiniu koliziju rasta: " << strukturuotosKolizijos << endl;

    return 0;
}