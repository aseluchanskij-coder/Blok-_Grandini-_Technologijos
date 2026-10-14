#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <bit>
#include <iomanip>
#include <unordered_set>

// 1. ĮTRAUKIAME IŠORINIUS ALGORITMUS
extern std::vector<std::uint8_t> str_i_hasha(const std::string &str);  // Arnas
extern std::string hash_almantas(const std::string& in);               // Almantas
extern std::vector<uint8_t> joringis_hash_global(const uint8_t* data, size_t size); // Joringis
extern std::string hash_algo(std::vector<unsigned char> text);         // JŪSŲ

// 2. APVALKALAI (Wrappers)

std::string test_arnas(const std::string& in) {
    auto baitai = str_i_hasha(in);
    std::stringstream ss;
    for(auto b : baitai) ss << std::hex << std::setfill('0') << std::setw(2) << (int)b;
    return ss.str();
}

std::string test_almantas(const std::string& in) {
    return hash_almantas(in);
}

std::string test_joringis(const std::string& in) {
    auto baitai = joringis_hash_global(reinterpret_cast<const uint8_t*>(in.data()), in.size());
    std::stringstream ss;
    for(auto b : baitai) ss << std::hex << std::setfill('0') << std::setw(2) << (int)b;
    return ss.str();
}

std::string test_mano(const std::string& in) {
    std::vector<unsigned char> data(in.begin(), in.end());
    return hash_algo(data);
}

// 3. EKSPERIMENTŲ FUNKCIJOS

using HashFunc = std::string(*)(const std::string&);

void run_exp4_collisions(HashFunc f, const std::string& name) {
    std::cout << "--- 4. Koliziju Testas ---\n";
    std::unordered_set<std::string> hashes;
    int collisions = 0;
    for(int i = 0; i < 100000; i++) {
        std::string h = f("test_" + std::to_string(i));
        if(!hashes.insert(h).second) collisions++;
    }
    std::cout << "Rasta koliziju: " << collisions << " is 100,000\n\n";
}

void run_exp5_avalanche(HashFunc f, const std::string& name) {
    std::cout << "--- 5. Lavinos Efektas ---\n";
    std::string in1 = "Lietuvos Respublika";
    std::string in2 = "Lietuvos respublika"; // pakeista 1 raidė ('R' -> 'r')
    
    std::string h1 = f(in1);
    std::string h2 = f(in2);
    
    int bit_diff = 0;
    // Skaičiuojame tik jeigu abu hash'ai yra vienodo ilgio (turėtų būti 64 HEX simboliai)
    size_t len = std::min(h1.size(), h2.size());
    for(size_t i = 0; i < len; i++) {
        int val1 = std::stoi(std::string(1, h1[i]), nullptr, 16);
        int val2 = std::stoi(std::string(1, h2[i]), nullptr, 16);
        bit_diff += std::popcount(static_cast<unsigned int>(val1 ^ val2));
    }
    
    int total_bits = len * 4; // Kiekvienas HEX simbolis = 4 bitai
    std::cout << "Bitu skirtumas: " << bit_diff << " / " << total_bits 
              << " (" << (bit_diff * 100.0 / total_bits) << "%)\n\n";
}

void run_exp6_speed(HashFunc f, const std::string& name) {
    std::cout << "--- 6. Spartos Testas ---\n";
    std::string data(1024 * 1024 * 10, 'A'); // 10 MB duomenų failas atmintyje
    
    auto start = std::chrono::high_resolution_clock::now();
    f(data);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    std::cout << "Laikas 10 MB apdorojimui: " << diff.count() << " s\n";
    std::cout << "Greitis: " << 10.0 / diff.count() << " MB/s\n\n";
}

int main() {
    std::vector<std::pair<HashFunc, std::string>> algoritmai = {
        {test_mano, "Mano Algoritmas"},
        {test_arnas, "Arnas"},
        {test_almantas, "Almantas"},
        {test_joringis, "Joringis"}
    };

    std::cout << "Pradedamas algoritmu palyginimas...\n\n";

    for(auto& algo : algoritmai) {
        std::cout << ">>> TESTUOJAMAS ALGORITMAS: " << algo.second << " <<<\n";
        run_exp4_collisions(algo.first, algo.second);
        run_exp5_avalanche(algo.first, algo.second);
        run_exp6_speed(algo.first, algo.second);
        std::cout << "====================================\n\n";
    }
    
    return 0;
}