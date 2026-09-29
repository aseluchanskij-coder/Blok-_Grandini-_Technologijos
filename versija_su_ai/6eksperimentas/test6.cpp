#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>
#include <map>

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



uint8_t hexSimbolisIVerte(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

int gautiHexSkirtuma(const string& h1, const string& h2) {
    int diff = 0;
    for (size_t i = 0; i < h1.length(); ++i) {
        if (h1[i] != h2[i]) diff++;
    }
    return diff;
}

int gautiBituSkirtuma(const string& h1, const string& h2) {
    int diffBits = 0;
    for (size_t i = 0; i < h1.length(); ++i) {
        uint8_t v1 = hexSimbolisIVerte(h1[i]);
        uint8_t v2 = hexSimbolisIVerte(h2[i]);
        uint8_t xorVal = v1 ^ v2;
        while (xorVal > 0) {
            diffBits += (xorVal & 1);
            xorVal >>= 1;
        }
    }
    return diffBits;
}

struct Statistika {
    double minBit = 100.0, maxBit = 0.0, sumBit = 0.0;
    double minHex = 100.0, maxHex = 0.0, sumHex = 0.0;
    long long visoPoru = 0;

    void atnaujinti(double bitProc, double hexProc) {
        minBit = std::min(minBit, bitProc);
        maxBit = std::max(maxBit, bitProc);
        sumBit += bitProc;

        minHex = std::min(minHex, hexProc);
        maxHex = std::max(maxHex, hexProc);
        sumHex += hexProc;

        visoPoru++;
    }

    double avgBit() const { return visoPoru > 0 ? sumBit / visoPoru : 0.0; }
    double avgHex() const { return visoPoru > 0 ? sumHex / visoPoru : 0.0; }
};

void spausdintiHistograma(const std::map<int, int>& bitKintamumas) {
    cout << "\n--- BITŲ SKIRTUMO (%) HISTOGRAMA ---" << endl;
    for (int i = 0; i < 100; i += 10) {
        int count = 0;
        for (int j = i; j < i + 10; ++j) {
            auto it = bitKintamumas.find(j);
            if (it != bitKintamumas.end()) count += it->second;
        }
        cout << std::setw(2) << i << "% - " << std::setw(2) << i + 9 << "% | ";
        int stulpelioIlgis = count / 1000; 
        for (int k = 0; k < stulpelioIlgis; ++k) cout << "*";
        cout << " (" << count << ")" << endl;
    }
}



int main() {
    const unsigned int SEED = 2026;
    std::mt19937 gen(SEED);
    std::uniform_int_distribution<int> asciiDist(32, 126); 

    const int ilgiai[] = {10, 100, 500, 1000};
    const int poruPerIlgi = 25000; 

    Statistika bendraStatSimbolis;
    Statistika bendraStatBitFlip;

    std::map<int, int> histogramaBit;

    cout << "=========================================================" << endl;
    cout << "           LAVINOS EFEKTO (AVALANCHE) TESTAS             " << endl;
    cout << "=========================================================" << endl;
    cout << "Generuojama 100,000 poru (po 25,000 pagal 4 ilgius)" << endl;
    cout << "Abecele: ASCII [32..126], Seed: " << SEED << endl;
    cout << "Orientaciniai vidurkiai: Bitams ~50.0%, Hex ~93.75%\n" << endl;

    for (int ilgis : ilgiai) {
        Statistika ilgisStatSimbolis;
        Statistika ilgisStatBitFlip;

        for (int i = 0; i < poruPerIlgi; ++i) {
            vector<uint8_t> ivestisA(ilgis);
            for (int j = 0; j < ilgis; ++j) {
                ivestisA[j] = static_cast<uint8_t>(asciiDist(gen));
            }

            vector<uint8_t> ivestisB_simbolis = ivestisA;
            std::uniform_int_distribution<int> posDist(0, ilgis - 1);
            int keiciamaPozicija = posDist(gen);

            uint8_t naujasSimbolis;
            do {
                naujasSimbolis = static_cast<uint8_t>(asciiDist(gen));
            } while (naujasSimbolis == ivestisA[keiciamaPozicija]);

            ivestisB_simbolis[keiciamaPozicija] = naujasSimbolis;

            string hashA = custom_hashas(ivestisA);
            string hashB_simbolis = custom_hashas(ivestisB_simbolis);

            int hexDiff = gautiHexSkirtuma(hashA, hashB_simbolis);
            int bitDiff = gautiBituSkirtuma(hashA, hashB_simbolis);

            double hexProc = (hexDiff / 64.0) * 100.0;
            double bitProc = (bitDiff / 256.0) * 100.0;

            ilgisStatSimbolis.atnaujinti(bitProc, hexProc);
            bendraStatSimbolis.atnaujinti(bitProc, hexProc);

            histogramaBit[static_cast<int>(std::round(bitProc))]++;

            vector<uint8_t> ivestisB_bitas = ivestisA;
            std::uniform_int_distribution<int> bitPosDist(0, 7);
            int bitIdx = bitPosDist(gen);
            ivestisB_bitas[keiciamaPozicija] ^= (1 << bitIdx); 

            string hashB_bitas = custom_hashas(ivestisB_bitas);

            int hexDiffB = gautiHexSkirtuma(hashA, hashB_bitas);
            int bitDiffB = gautiBituSkirtuma(hashA, hashB_bitas);

            double hexProcB = (hexDiffB / 64.0) * 100.0;
            double bitProcB = (bitDiffB / 256.0) * 100.0;

            ilgisStatBitFlip.atnaujinti(bitProcB, hexProcB);
            bendraStatBitFlip.atnaujinti(bitProcB, hexProcB);
        }

        cout << "---------------------------------------------------------" << endl;
        cout << "REZULTATAI ILGIUI: " << ilgis << " baitu (25,000 poru)" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << " [1 Simbolio pakeitimas]:" << endl;
        cout << "   Bitu skirtumas (%) | Min: " << std::fixed << std::setprecision(2) << ilgisStatSimbolis.minBit 
             << "% | Max: " << ilgisStatSimbolis.maxBit << "% | Vid: " << ilgisStatSimbolis.avgBit() << "%" << endl;
        cout << "   Hex  skirtumas (%) | Min: " << ilgisStatSimbolis.minHex 
             << "% | Max: " << ilgisStatSimbolis.maxHex << "% | Vid: " << ilgisStatSimbolis.avgHex() << "%" << endl;
        
        cout << " [1 Bito apvertimas (Bit-flip)]:" << endl;
        cout << "   Bitu skirtumas (%) | Min: " << ilgisStatBitFlip.minBit 
             << "% | Max: " << ilgisStatBitFlip.maxBit << "% | Vid: " << ilgisStatBitFlip.avgBit() << "%" << endl;
        cout << "   Hex  skirtumas (%) | Min: " << ilgisStatBitFlip.minHex 
             << "% | Max: " << ilgisStatBitFlip.maxHex << "% | Vid: " << ilgisStatBitFlip.avgHex() << "%" << endl;
        cout << endl;
    }

    cout << "=========================================================" << endl;
    cout << " BENDRI REZULTATAI (Iš viso 100,000 porų)" << endl;
    cout << "=========================================================" << endl;
    cout << " [1 Simbolio pakeitimas]:" << endl;
    cout << "   Bitu skirtumas (%) | Min: " << bendraStatSimbolis.minBit 
         << "% | Max: " << bendraStatSimbolis.maxBit << "% | Vid: " << bendraStatSimbolis.avgBit() << "%" << endl;
    cout << "   Hex  skirtumas (%) | Min: " << bendraStatSimbolis.minHex 
         << "% | Max: " << bendraStatSimbolis.maxHex << "% | Vid: " << bendraStatSimbolis.avgHex() << "%" << endl;
    
    cout << "\n [1 Bito apvertimas (Bit-flip)]:" << endl;
    cout << "   Bitu skirtumas (%) | Min: " << bendraStatBitFlip.minBit 
         << "% | Max: " << bendraStatBitFlip.maxBit << "% | Vid: " << bendraStatBitFlip.avgBit() << "%" << endl;
    cout << "   Hex  skirtumas (%) | Min: " << bendraStatBitFlip.minHex 
         << "% | Max: " << bendraStatBitFlip.maxHex << "% | Vid: " << bendraStatBitFlip.avgHex() << "%" << endl;

    spausdintiHistograma(histogramaBit);

    return 0;
}