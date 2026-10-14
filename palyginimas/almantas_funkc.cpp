#include "almantas_lib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <random>

int getInt(int min, int max) {
    int value;
    while (true) {
        try {
            if (!(std::cin>>value)) {
                if (std::cin.eof()) throw std::ios_base::failure("Įvestis nutraukta");
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                throw std::runtime_error("Neteisinga įvestis");
            }
            char leftover;
            if (std::cin.get(leftover)&&leftover!='\n') {
                std::cin.ignore(10000, '\n');
                throw std::runtime_error("Neteisinga įvestis");
            }
            if (value<min||value>max)
                throw std::runtime_error("Pasirinkimas turi būti tarp "+std::to_string(min)+" ir "+std::to_string(max));
            return value;
        }
        catch (const std::runtime_error& e) {
            std::cout<<e.what()<<", pabandykite dar kartą: ";
        }
    }
}

std::string getFile() {
    std::string failas;
    std::cout << "Įveskite failo pavadinimą: ";
    while (std::getline(std::cin, failas)) {
        std::ifstream fin(failas);
        if (fin.is_open()) return failas;
        std::cout << "Failas \"" << failas << "\" nerastas, pabandykite dar kartą: ";
    }
    throw std::runtime_error("Įvestis nutraukta");
}

std::vector<std::string> readManual() {
    std::vector<std::string> A;
    std::string eil;
    std::cout << "Įveskite eilutes (tuščia eilutė - pabaiga):\n";
    while (std::getline(std::cin, eil) && !eil.empty())
        A.push_back(eil);
    return A;
}

std::vector<std::string> readFile(const std::string& failas) {
    std::vector<std::string> A;
    std::ifstream fin(failas);
    if (!fin.is_open())
        throw std::runtime_error("Nepavyko atidaryti failo \"" + failas + "\"");
    std::string eil;
    while (std::getline(fin, eil)) {
        if (!eil.empty() && eil.back()=='\r') eil.pop_back();
        A.push_back(eil);
    }
    return A;
}

namespace {
    constexpr uint64_t SETUP_SEED = 0x5EED0F1A5C0DE001ULL;

    struct Params {
        uint64_t a[4];
        uint64_t b[4];
    };

    Params makeParams() {
        std::mt19937_64 gen(SETUP_SEED);
        Params p;
        for (int i = 0; i < 4; ++i) p.a[i] = gen() | 1ULL;
        for (int i = 0; i < 4; ++i) p.b[i] = gen() | 1ULL;
        return p;
    }

    const Params& params() {
        static const Params p = makeParams();
        return p;
    }
}

namespace {
    inline uint64_t rotl(uint64_t x, int r) { return (x << r) | (x >> (64 - r)); }

    constexpr int ROT[4] = {13, 29, 41, 53};

    inline void mixLanes(uint64_t a[4]) {
        for (int i = 0; i < 4; ++i)
            a[i] += rotl(a[(i + 1) & 3], ROT[i]);
    }

    inline uint64_t load64(const unsigned char* p) {
        return  static_cast<uint64_t>(p[0])        | static_cast<uint64_t>(p[1]) << 8  |
                static_cast<uint64_t>(p[2]) << 16  | static_cast<uint64_t>(p[3]) << 24 |
                static_cast<uint64_t>(p[4]) << 32  | static_cast<uint64_t>(p[5]) << 40 |
                static_cast<uint64_t>(p[6]) << 48  | static_cast<uint64_t>(p[7]) << 56;
    }

    inline uint64_t loadTail(const unsigned char* p, size_t n) {
        uint64_t w = 0;
        for (size_t i = 0; i < n; ++i)
            w |= static_cast<uint64_t>(p[i]) << (8 * i);
        return w;
    }

    inline void absorb(uint64_t a[4], const uint64_t b[4], uint64_t w) {
        for (int i = 0; i < 4; ++i) {
            a[i] = (a[i] + w) * b[i];
            a[i] ^= a[i] >> 32;
        }
        mixLanes(a);
    }
}

std::string hash_almantas(const std::string& in) {
    const Params& p = params();
    uint64_t a[4] = {p.a[0], p.a[1], p.a[2], p.a[3]};
    const uint64_t* b = p.b;

    const unsigned char* d = reinterpret_cast<const unsigned char*>(in.data());
    const size_t n = in.size();

    size_t i = 0;
    for (; i + 8 <= n; i += 8)
        absorb(a, b, load64(d + i));
    if (i < n)
        absorb(a, b, loadTail(d + i, n - i));

    const uint64_t len = static_cast<uint64_t>(n);
    for (int k = 0; k < 4; ++k)
        a[k] += len * b[k];
    for (int r = 0; r < 4; ++r) {
        for (int k = 0; k < 4; ++k) {
            a[k] ^= a[k] >> 29;
            a[k] *= b[k];
            a[k] ^= a[k] >> 32;
        }
        mixLanes(a);
    }

    static const char HEX[] = "0123456789abcdef";
    std::string out(64, '0');
    for (int k = 0; k < 4; ++k)
        for (int j = 0; j < 16; ++j)
            out[k * 16 + j] = HEX[(a[k] >> (60 - 4 * j)) & 0xF];
    return out;
}

std::string readFileBytes(const std::string& failas) {
    std::ifstream fin(failas, std::ios::binary);
    if (!fin.is_open())
        throw std::runtime_error("Nepavyko atidaryti failo \"" + failas + "\"");
    std::ostringstream ss;
    ss << fin.rdbuf();
    if (fin.bad())
        throw std::runtime_error("Klaida skaitant failą \"" + failas + "\"");
    return ss.str();
}

std::vector<std::string> hashAll(const std::vector<std::string>& in) {
    std::vector<std::string> H;
    H.reserve(in.size());
    for (const std::string& s : in)
        H.push_back(hash_almantas(s));
    return H;
}

void printRez(std::ostream& out, const std::vector<std::string>& in, const std::vector<std::string>& hashes) {
    if (in.empty()) {
        out << "Nėra duomenų.\n";
        return;
    }
    for (size_t i=0; i<in.size(); ++i)
        out << hashes[i] << "  " << in[i] << " " << in[i].size() << '\n';
}
