#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

void bigEndian(size_t dydis_bitais, vector<unsigned char>& data){
    for (int i = 7; i >= 0; --i) {
        unsigned char baitas = (dydis_bitais >> (i * 8)) & 0xFF;
        data.push_back(baitas);
    }
}

void padding(vector<unsigned char>& data) {
    size_t dydis = data.size();
    size_t dydis_bitais = dydis * 8;
    data.insert(data.end(), 0x80);
    while(data.size() % 32 != 24){
        data.push_back(0x00); 
    }
    bigEndian(dydis_bitais, data); 
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

string skaiciuotiMaisa(vector<unsigned char> data) {
    uint32_t state[8] = {
        0x12345678, 0x8abcd123, 0xabcabcab, 0x87654321, 
        0xdefdefff, 0xaabbccdd, 0x11223344, 0xfedbc111 
    };
    padding(data);
    for(size_t i = 0; i < data.size(); i += 32){
        uint32_t zodziai[8];
        vertimasZodziais(data, zodziai, i);
        for(int j = 0; j < 8; ++j){ 
            state[j] ^= zodziai[j];
        }
        for(int k = 0; k < 16; ++k){
            lavina(state);
        }
    }
    stringstream hash_result;
    for (int i = 0; i < 8; ++i) {
        hash_result << hex << setfill('0') << setw(8) << state[i];             
    }   
    return hash_result.str();
}

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

int besiskiriantysBitai(const string& h1, const string& h2) {
    int diff = 0;
    for (size_t i = 0; i < h1.length(); i++) {
        int v1 = hexCharToInt(h1[i]);
        int v2 = hexCharToInt(h2[i]);
        diff += __builtin_popcount(v1 ^ v2); 
    }
    return diff;
}

int besiskiriantysHex(const string& h1, const string& h2) {
    int diff = 0;
    for (size_t i = 0; i < h1.length(); i++) {
        if (h1[i] != h2[i]) diff++;
    }
    return diff;
}

int main() {
    int ilgiai[] = {10, 100, 500, 1000};
    int poruSkaiciusVienamIlgiui = 25000;
    
    mt19937 gen(12345); 
    uniform_int_distribution<int> charDist(32, 126);

    cout << "--- 6 EKSPERIMENTAS: LAVINOS EFEKTAS ---" << endl;
    cout << "Viso poru: 100 000 (po 25 000 ilgiams 10, 100, 500, 1000)." << endl;
    
    long long bendrasBituSuma = 0;
    long long bendrasHexSuma = 0;
    int bendrasBitMin = 257, bendrasBitMax = -1;
    int bendrasHexMin = 65, bendrasHexMax = -1;
    
    vector<int> bituHistograma(257, 0);

    for (int ilgis : ilgiai) {
        uniform_int_distribution<int> posDist(0, ilgis - 1);
        
        long long sumBit = 0, sumHex = 0;
        int minBit = 257, maxBit = -1;
        int minHex = 65, maxHex = -1;

        for (int i = 0; i < poruSkaiciusVienamIlgiui; i++) {
            string strA = "";
            for (int j = 0; j < ilgis; j++) {
                strA += static_cast<char>(charDist(gen));
            }
            
            string strB = strA;
            int keitimoPozicija = posDist(gen);
            char naujasChar;
            do {
                naujasChar = static_cast<char>(charDist(gen));
            } while (naujasChar == strA[keitimoPozicija]);
            strB[keitimoPozicija] = naujasChar;

            vector<unsigned char> vecA(strA.begin(), strA.end());
            vector<unsigned char> vecB(strB.begin(), strB.end());

            string hashA = skaiciuotiMaisa(vecA);
            string hashB = skaiciuotiMaisa(vecB);

            int bitDiff = besiskiriantysBitai(hashA, hashB);
            int hexDiff = besiskiriantysHex(hashA, hashB);

            if (bitDiff < minBit) minBit = bitDiff;
            if (bitDiff > maxBit) maxBit = bitDiff;
            sumBit += bitDiff;

            if (hexDiff < minHex) minHex = hexDiff;
            if (hexDiff > maxHex) maxHex = hexDiff;
            sumHex += hexDiff;
            
            if (bitDiff < bendrasBitMin) bendrasBitMin = bitDiff;
            if (bitDiff > bendrasBitMax) bendrasBitMax = bitDiff;
            if (hexDiff < bendrasHexMin) bendrasHexMin = hexDiff;
            if (hexDiff > bendrasHexMax) bendrasHexMax = hexDiff;
            bendrasBituSuma += bitDiff;
            bendrasHexSuma += hexDiff;

            bituHistograma[bitDiff]++;
        }

        cout << "\n> Ilgis: " << ilgis << " baitu" << endl;
        cout << fixed << setprecision(2);
        cout << "  Bitu skirtumas (%): Min = " << (minBit / 256.0) * 100.0 
             << "%, Max = " << (maxBit / 256.0) * 100.0 
             << "%, Vidurkis = " << ((sumBit / (double)poruSkaiciusVienamIlgiui) / 256.0) * 100.0 << "%" << endl;
        cout << "  Hex skirtumas (%):  Min = " << (minHex / 64.0) * 100.0 
             << "%, Max = " << (maxHex / 64.0) * 100.0 
             << "%, Vidurkis = " << ((sumHex / (double)poruSkaiciusVienamIlgiui) / 64.0) * 100.0 << "%" << endl;
    }

    cout << "\n--- BENDRI REZULTATAI (100 000 poru) ---" << endl;
    cout << "Bitu skirtumas (%): Min = " << (bendrasBitMin / 256.0) * 100.0 
         << "%, Max = " << (bendrasBitMax / 256.0) * 100.0 
         << "%, Vidurkis = " << ((bendrasBituSuma / 100000.0) / 256.0) * 100.0 << "%" << endl;
    cout << "Hex skirtumas (%):  Min = " << (bendrasHexMin / 64.0) * 100.0 
         << "%, Max = " << (bendrasHexMax / 64.0) * 100.0 
         << "%, Vidurkis = " << ((bendrasHexSuma / 100000.0) / 64.0) * 100.0 << "%" << endl;

    cout << "\n--- HISTOGRAMOS DUOMENYS (Bitu skirtumas) ---" << endl;
    cout << "Besiskiriantys_bitai,Poru_skaicius" << endl;
    for (int i = 0; i <= 256; i++) {
        if (bituHistograma[i] > 0) {
            cout << i << "," << bituHistograma[i] << endl;
        }
    }

    return 0;
}