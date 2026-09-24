#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <string>

using std::string;
using std::vector;
using std::cout;
using std::endl;


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

    std::stringstream hash_result;
    for (int i = 0; i < 8; ++i) {
        hash_result << std::hex << std::setfill('0') << std::setw(8) << state[i];             
    }   
    return hash_result.str();
}

int main() {
    int ilgiai[] = {10, 100, 500, 1000};
    int poruSkaicius = 100000;
    
    std::mt19937 generatorius(2026); 
    std::uniform_int_distribution<int> distribucija(32, 126); 

    cout << "--- 5 EKSPERIMENTAS: KOLIZIJU PAIESKA ---" << endl;
    cout << "Naudojamas seed: 2026. Abecele: ASCII (32-126)." << endl;

    for (int ilgis : ilgiai) {
        cout << "\nGeneruojama 100,000 poru ilgiui: " << ilgis << " baitu..." << endl;
        
        int porosKolizijos = 0;
        int bendrosKolizijos = 0;
        
        std::unordered_map<string, string> visiHashai; 
        visiHashai.reserve(poruSkaicius * 2);

        for (int i = 0; i < poruSkaicius; i++) {
            string strA = "", strB = "";
            for (int j = 0; j < ilgis; j++) {
                strA += static_cast<char>(distribucija(generatorius));
                strB += static_cast<char>(distribucija(generatorius));
            }
            
            vector<unsigned char> vecA(strA.begin(), strA.end());
            vector<unsigned char> vecB(strB.begin(), strB.end());

            string hashA = skaiciuotiMaisa(vecA);
            string hashB = skaiciuotiMaisa(vecB);

            if (hashA == hashB && strA != strB) {
                porosKolizijos++;
            }

            if (visiHashai.find(hashA) != visiHashai.end()) {
                if (visiHashai[hashA] != strA) bendrosKolizijos++;
            } else {
                visiHashai[hashA] = strA;
            }

            if (visiHashai.find(hashB) != visiHashai.end()) {
                if (visiHashai[hashB] != strB) bendrosKolizijos++;
            } else {
                visiHashai[hashB] = strB;
            }
        }
        
        cout << "> Porose rastu koliziju: " << porosKolizijos << endl;
        cout << "> Visame rinkinyje rastu koliziju (skirtingoms ivestims): " << bendrosKolizijos << endl;
        cout << "> Skirtingu ivesciu grupiu skaicius: " << visiHashai.size() << endl;
    }

    cout << "\n--- STRUKTURUOTI ATVEJAI ---" << endl;
    vector<string> strukturuoti = {
        "ABABABABABABABAB",
        "BচুBচুBচুBচুBচু", 
        "AAAAAAAAAAAAAAAB",
        "BAAAAAAAAAAAAAAA",
        "0000000000000000",
        "1111111111111111"
    };

    std::unordered_map<string, string> strukturuotiHashai;
    int strukturuotosKolizijos = 0;
    
    for (const string& s : strukturuoti) {
        vector<unsigned char> vec(s.begin(), s.end());
        string h = skaiciuotiMaisa(vec);
        cout << "Tekstas: " << std::setw(16) << s << " | Hash: " << h << endl;
        
        if (strukturuotiHashai.find(h) != strukturuotiHashai.end() && strukturuotiHashai[h] != s) {
            strukturuotosKolizijos++;
        }
        strukturuotiHashai[h] = s;
    }
    cout << "Strukturiniu koliziju rasta: " << strukturuotosKolizijos << endl;

    return 0;
}