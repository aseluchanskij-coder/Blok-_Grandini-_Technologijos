#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <algorithm>

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

void readInput(vector<unsigned char>& data) {
    string input;
    std::cout << "Įveskite tekstą: ";
    std::getline(std::cin, input);
    if(input.empty()) {
        throw std::runtime_error("Įvestas tekstas negali būti tuščias");
    }
    data.assign(input.begin(), input.end());
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

string hash_algo(vector<unsigned char> text) { // Perduodame kopiją, nes text bus modifikuotas paddingo
    uint32_t state[8] = { 0x12345678, 0x8abcd123, 0xabcabcab, 0x87654321, 
                          0xdefdefff, 0xaabbccdd, 0x11223344, 0xfedbc111 };
    
    padding(text);
    for(int i = 0; i < text.size(); i += 32){
        uint32_t zodziai[8];
        vertimasZodziais(text, zodziai, i);
        for(int j = 0; j < 8; ++j) state[j] ^= zodziai[j];
        for(int k = 0; k < 16; ++k) lavina(state);
    }

    std::stringstream hash_result;
    for (int i = 0; i < 8; ++i) {
        hash_result << std::hex << std::setfill('0') << std::setw(8) << state[i];             
    }   
    return hash_result.str();
}

void vykdytiSpartosTesta(const string& filename) {
    // 1. Nuskaitome VISĄ failą iš anksto (prieš matuojant laiką)
    std::ifstream in(filename);
    if (!in) { std::cerr << "Failas nerastas!\n"; return; }
    
    std::vector<std::string> visos_eilutes;
    std::string eilute;
    while (std::getline(in, eilute)) {
        // Išsaugome eilučių skirtuką, kaip reikalaujama
        visos_eilutes.push_back(eilute + "\n"); 
    }
    
    int max_eiluciu = visos_eilutes.size();
    std::cout << "Failas nuskaitytas. Viso eilučių: " << max_eiluciu << "\n\n";

    // Apsaugome nuo kompiliatoriaus optimizacijų (-O3 gali ištrinti nevykdomą kodą)
    volatile int dummy = 0; 
    int iteracijos = 1000; // Jei hashinasi per greitai, didiname šį skaičių

    std::cout << "Eilutės\tBaitai\tMin(ms)\tMax(ms)\tVidurkis(ms)\n";

    std::vector<int> matuojami_kiekiai;
for (int n = 1; n <= max_eiluciu; n *= 2) {
    matuojami_kiekiai.push_back(n);
}
// Jei paskutinis dvejeto laipsnis neapėmė viso failo, pridedame visą failą
if (matuojami_kiekiai.back() != max_eiluciu) {
    matuojami_kiekiai.push_back(max_eiluciu);
}

// 3. Ciklas per sudarytą sąrašą
for (int n : matuojami_kiekiai) {
    // A. Sudarome ištrauką iš n eilučių
    std::vector<unsigned char> istrauka;
    for (int i = 0; i < n; ++i) {
        istrauka.insert(istrauka.end(), visos_eilutes[i].begin(), visos_eilutes[i].end());
    }
    size_t baitu_skaicius = istrauka.size();

    // B. Apšilimas (Warm-up)
    for(int i = 0; i < 5; ++i) {
        string h = hash_algo(istrauka);
        dummy += h[0];
    }

    // C. Atliekame 5 matavimus
    double min_t = 9999999.0, max_t = 0.0, suma_t = 0.0;
    
    for (int m = 0; m < 5; ++m) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for(int i = 0; i < iteracijos; ++i) {
            string h = hash_algo(istrauka);
            dummy += h[0]; 
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> trukme = end - start;
        
        double vieno_hash_laikas = trukme.count() / iteracijos;
        
        if (vieno_hash_laikas < min_t) min_t = vieno_hash_laikas;
        if (vieno_hash_laikas > max_t) max_t = vieno_hash_laikas;
        suma_t += vieno_hash_laikas;
    }

    double vidurkis = suma_t / 5.0;

        // D. Spausdiname rezultatus lentelės formatu
        std::cout << n << "\t" << baitu_skaicius << "\t" 
                  << min_t << "\t" << max_t << "\t" << vidurkis << "\n";
    }
    
    // Kad kompiliatorius visiškai neišmestų dummy kintamojo
    if (dummy == 123456789) std::cout << " "; 
}

int main() {
    vykdytiSpartosTesta("konstitucija.txt");
    return 0;
}