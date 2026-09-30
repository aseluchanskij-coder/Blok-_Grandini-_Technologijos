#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>

using std::cout;
using std::vector;
using std::string;
using std::endl;

// --- TAVO HASH ALGORITMAS (Nepakeistas) ---
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
        0xD6D12E7B5A03A401ULL, 0x8A59B51F41029312ULL, 
        0x32A398246E20349AULL, 0x7158932402138901ULL  
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
// --- PABAIGA TAVO HASH ALGORITMO ---


int main() {
    string failoPavadinimas = "konstitucija.txt";
    std::ifstream failas(failoPavadinimas);

    if (!failas) {
        cout << "KLAIDA: Nepavyko atidaryti '" << failoPavadinimas << "'!" << endl;
        cout << "Isitikinkite, kad failas yra tame paciame kataloge." << endl;
        return 1;
    }

    vector<string> visosEilutes;
    string eilute;
    while (std::getline(failas, eilute)) {
        visosEilutes.push_back(eilute + '\n'); 
    }
    failas.close();

    int totalLines = visosEilutes.size();
    cout << "Failas sekmingai nuskaitytas. Viso eiluciu: " << totalLines << "\n\n";

    cout << std::left << std::setw(10) << "Eilutes" 
         << std::setw(12) << "Baitai" 
         << std::setw(20) << "Min laikas (ns)" 
         << std::setw(20) << "Max laikas (ns)" 
         << std::setw(20) << "Vidurkis (ns)" << endl;
    cout << string(80, '-') << endl;

    long long dummySum = 0; 

    for (int linesToUse = 1; linesToUse <= totalLines; ) {
        
        vector<uint8_t> duomenys;
        for (int i = 0; i < linesToUse; i++) {
            for (char c : visosEilutes[i]) {
                duomenys.push_back(static_cast<uint8_t>(c));
            }
        }
        
        for(int w = 0; w < 10; w++) {
            dummySum += custom_hashas(duomenys)[0];
        }

        const int iteracijos = 100; 
        long long min_laikas = -1, max_laikas = -1, sum_laikas = 0;

        for (int matavimas = 0; matavimas < 5; matavimas++) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int it = 0; it < iteracijos; it++) {
                dummySum += custom_hashas(duomenys)[0];
            }
            auto end = std::chrono::high_resolution_clock::now();
            
            long long duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            long long laikas_vienam_hashui = duration_ns / iteracijos;

            if (min_laikas == -1 || laikas_vienam_hashui < min_laikas) min_laikas = laikas_vienam_hashui;
            if (max_laikas == -1 || laikas_vienam_hashui > max_laikas) max_laikas = laikas_vienam_hashui;
            sum_laikas += laikas_vienam_hashui;
        }

        long long vidurkis = sum_laikas / 5;

        cout << std::left << std::setw(10) << linesToUse 
             << std::setw(12) << duomenys.size() 
             << std::setw(20) << min_laikas 
             << std::setw(20) << max_laikas 
             << std::setw(20) << vidurkis << endl;
             
        if (linesToUse == totalLines) {
            break; 
        }
        
        linesToUse *= 2; 
        
        if (linesToUse > totalLines) {
            linesToUse = totalLines; 
        }
    }
    if(dummySum == 0) cout << " "; 

    return 0;
}