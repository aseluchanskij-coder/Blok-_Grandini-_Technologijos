#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>

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
            if (yraLyginis) dabartinisBlokas[i] = padded[b * 32 + (31 - i)];
            else dabartinisBlokas[i] = padded[b * 32 + i];
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


string generuotiKandidata(int skaicius) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << skaicius;
    return ss.str();
}

vector<uint8_t> sujungtiBaitus(const string& tekstas, const vector<uint8_t>& druska = {}) {
    vector<uint8_t> rezultatas(tekstas.begin(), tekstas.end());
    rezultatas.insert(rezultatas.end(), druska.begin(), druska.end());
    return rezultatas;
}

int main() {
    string tikslineIvestis = "7392";
    cout << "========================================================" << endl;
    cout << "  PERRINKIMO (BRUTE-FORCE) ATAKOS EKSPERIMENTAS" << endl;
    cout << "========================================================" << endl;
    
    cout << "\n[1] PERRINKIMAS BE DRUSKOS" << endl;
    vector<uint8_t> taikinysBeDruskos = sujungtiBaitus(tikslineIvestis);
    string tikslinisHashBeDruskos = custom_hashas(taikinysBeDruskos);
    cout << "Ieskoma hash reiksme: " << tikslinisHashBeDruskos << endl;

    auto pradzia1 = std::chrono::high_resolution_clock::now();
    int bandymai1 = 0;
    string rastasKandidatas1 = "";

    for (int i = 0; i <= 9999; i++) {
        bandymai1++;
        string kandidatas = generuotiKandidata(i);
        if (custom_hashas(sujungtiBaitus(kandidatas)) == tikslinisHashBeDruskos) {
            rastasKandidatas1 = kandidatas;
            break;
        }
    }
    auto pabaiga1 = std::chrono::high_resolution_clock::now();
    auto laikas1 = std::chrono::duration_cast<std::chrono::milliseconds>(pabaiga1 - pradzia1).count();
    
    cout << " -> Rastas sutapimas: " << rastasKandidatas1 << endl;
    cout << " -> Atlikta bandymu: " << bandymai1 << endl;
    cout << " -> Uztruko laiko: " << laikas1 << " ms" << endl;

    cout << "\n[2] PERRINKIMAS SU VIESA DRUSKA" << endl;
    
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(0, 255);
    vector<uint8_t> druska(16);
    cout << "Generuojama druska (Hex): ";
    for (int i = 0; i < 16; i++) {
        druska[i] = static_cast<uint8_t>(dist(gen));
        cout << std::hex << std::setw(2) << std::setfill('0') << (int)druska[i];
    }
    cout << std::dec << endl;

    vector<uint8_t> taikinysSuDruska = sujungtiBaitus(tikslineIvestis, druska);
    string tikslinisHashSuDruska = custom_hashas(taikinysSuDruska);
    cout << "Ieskoma hash reiksme: " << tikslinisHashSuDruska << endl;

    auto pradzia2 = std::chrono::high_resolution_clock::now();
    int bandymai2 = 0;
    string rastasKandidatas2 = "";

    for (int i = 0; i <= 9999; i++) {
        bandymai2++;
        string kandidatas = generuotiKandidata(i);
        if (custom_hashas(sujungtiBaitus(kandidatas, druska)) == tikslinisHashSuDruska) {
            rastasKandidatas2 = kandidatas;
            break;
        }
    }
    auto pabaiga2 = std::chrono::high_resolution_clock::now();
    auto laikas2 = std::chrono::duration_cast<std::chrono::milliseconds>(pabaiga2 - pradzia2).count();

    cout << " -> Rastas sutapimas: " << rastasKandidatas2 << endl;
    cout << " -> Atlikta bandymu: " << bandymai2 << endl;
    cout << " -> Uztruko laiko: " << laikas2 << " ms" << endl;

    return 0;
}