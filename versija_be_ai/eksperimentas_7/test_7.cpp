#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

string formatuotiKandidata(int skaicius) {
    stringstream ss;
    ss << setw(4) << setfill('0') << skaicius;
    return ss.str();
}

int main() {
    string tikslinis_tekstas = "7391"; 
    string druska = "X9qP"; 

    cout << "--- 7 EKSPERIMENTAS: SPEJIMAS ---" << endl;
    cout << "Kandidatu erdve: 0000 - 9999 (10,000 variantu)" << endl;
    
    vector<unsigned char> t_vec_be_druskos(tikslinis_tekstas.begin(), tikslinis_tekstas.end());
    string tikslinis_hash_be_druskos = skaiciuotiMaisa(t_vec_be_druskos);
    
    cout << "\n[1] ATAKA BE DRUSKOS" << endl;
    cout << "Uzpuolikui pateikta maisa: " << tikslinis_hash_be_druskos << endl;
    
    int bandymai_1 = 0;
    string rastas_sutapimas_1 = "";
    
    auto start_1 = high_resolution_clock::now();
    for (int i = 0; i <= 9999; i++) {
        bandymai_1++;
        string kandidatas = formatuotiKandidata(i);
        vector<unsigned char> k_vec(kandidatas.begin(), kandidatas.end());
        
        if (skaiciuotiMaisa(k_vec) == tikslinis_hash_be_druskos) {
            rastas_sutapimas_1 = kandidatas;
            break; 
        }
    }
    auto end_1 = high_resolution_clock::now();
    auto trukme_1 = duration_cast<microseconds>(end_1 - start_1).count();

    cout << "> Rasta ivestis: " << rastas_sutapimas_1 << endl;
    cout << "> Atlikta bandymu: " << bandymai_1 << endl;
    cout << "> Veikimo laikas: " << trukme_1 << " mikrosekundziu." << endl;

    string tekstas_su_druska = tikslinis_tekstas + druska;
    vector<unsigned char> t_vec_su_druska(tekstas_su_druska.begin(), tekstas_su_druska.end());
    string tikslinis_hash_su_druska = skaiciuotiMaisa(t_vec_su_druska);
    
    cout << "\n[2] ATAKA SU VIESA DRUSKA" << endl;
    cout << "Zinoma druska (salt): '" << druska << "'" << endl;
    cout << "Uzpuolikui pateikta maisa: " << tikslinis_hash_su_druska << endl;
    
    int bandymai_2 = 0;
    string rastas_sutapimas_2 = "";
    
    auto start_2 = high_resolution_clock::now();
    for (int i = 0; i <= 9999; i++) {
        bandymai_2++;
        string kandidatas = formatuotiKandidata(i) + druska;
        vector<unsigned char> k_vec(kandidatas.begin(), kandidatas.end());
        
        if (skaiciuotiMaisa(k_vec) == tikslinis_hash_su_druska) {
            rastas_sutapimas_2 = kandidatas.substr(0, 4); 
            break;
        }
    }
    auto end_2 = high_resolution_clock::now();
    auto trukme_2 = duration_cast<microseconds>(end_2 - start_2).count();

    cout << "> Rasta ivestis: " << rastas_sutapimas_2 << endl;
    cout << "> Atlikta bandymu: " << bandymai_2 << endl;
    cout << "> Veikimo laikas: " << trukme_2 << " mikrosekundziu." << endl;

    return 0;
}