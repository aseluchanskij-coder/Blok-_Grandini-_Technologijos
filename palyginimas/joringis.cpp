#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#endif

using namespace std;

namespace {

constexpr size_t kSpokes = 8;
constexpr size_t kBlockBytes = 16;
constexpr size_t kDigestBytes = 32;

constexpr int kRoundsPerBlock = 3;
constexpr int kRoundsFinal = 4;
constexpr int kRoundsSqueeze = 2;

constexpr char kSeedPhrase[kSpokes * 4 + 1] = "Vilniaus universitetas, BGT 2026";

using State = uint32_t[kSpokes];

uint32_t rotl32(uint32_t x, unsigned n) {
    n &= 31u;
    if (n == 0) return x;
    return (x << n) | (x >> (32u - n));
}

uint32_t load_le32(const uint8_t* b) {
    return static_cast<uint32_t>(b[0]) |
           (static_cast<uint32_t>(b[1]) << 8) |
           (static_cast<uint32_t>(b[2]) << 16) |
           (static_cast<uint32_t>(b[3]) << 24);
}

void store_le32(uint32_t w, uint8_t* b) {
    b[0] = static_cast<uint8_t>(w);
    b[1] = static_cast<uint8_t>(w >> 8);
    b[2] = static_cast<uint8_t>(w >> 16);
    b[3] = static_cast<uint8_t>(w >> 24);
}

constexpr uint32_t seed_word(size_t i) {
    return static_cast<uint32_t>(static_cast<unsigned char>(kSeedPhrase[4 * i])) |
           (static_cast<uint32_t>(static_cast<unsigned char>(kSeedPhrase[4 * i + 1])) << 8) |
           (static_cast<uint32_t>(static_cast<unsigned char>(kSeedPhrase[4 * i + 2])) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(kSeedPhrase[4 * i + 3])) << 24);
}

constexpr uint32_t kIV[kSpokes] = {
    seed_word(0), seed_word(1), seed_word(2), seed_word(3),
    seed_word(4), seed_word(5), seed_word(6), seed_word(7),
};

uint32_t round_const(unsigned r) {
    return kIV[r % kSpokes] * (2u * r + 1u) + r;
}

void turn(State& s, unsigned r) {
    const uint32_t rc = round_const(r);
    for (size_t i = 0; i < kSpokes; ++i) {
        const uint32_t b = s[(i + 1) % kSpokes];
        const uint32_t c = s[(i + 3) % kSpokes];
        const uint32_t d = s[(i + 6) % kSpokes];
        uint32_t a = s[i];
        a += (b ^ c);
        const unsigned rot = d & 31u;
        a = rotl32(a, rot + (rot == 0u));
        a ^= (d + rc);
        s[i] = a;
        s[(i + 1) % kSpokes] += rotl32(a, 9);
    }
}

void turns(State& s, int count, unsigned& counter) {
    for (int k = 0; k < count; ++k) turn(s, counter++);
}

void absorb_block(State& s, const uint8_t* block, uint64_t block_index,
                  unsigned& counter) {
    State h;
    for (size_t i = 0; i < kSpokes; ++i) h[i] = s[i];
    s[0] ^= load_le32(block);
    s[1] ^= load_le32(block + 4);
    s[2] ^= load_le32(block + 8);
    s[3] ^= load_le32(block + 12);
    s[6] += static_cast<uint32_t>(block_index >> 32);
    s[7] += static_cast<uint32_t>(block_index);
    turns(s, kRoundsPerBlock, counter);
    for (size_t i = 0; i < kSpokes; ++i) s[i] += h[i];
}

class Ratas256 {
public:
    Ratas256() {
        for (size_t i = 0; i < kSpokes; ++i) s_[i] = kIV[i];
    }

    void update(const uint8_t* data, size_t size) {
        if (size == 0) return;
        total_ += size;
        if (buffered_ > 0) {
            const size_t room = kBlockBytes - buffered_;
            const size_t take = size < room ? size : room;
            memcpy(buf_ + buffered_, data, take);
            buffered_ += take;
            data += take;
            size -= take;
            if (buffered_ < kBlockBytes) return;
            absorb_block(s_, buf_, index_++, counter_);
            buffered_ = 0;
        }
        for (; size >= kBlockBytes; data += kBlockBytes, size -= kBlockBytes)
            absorb_block(s_, data, index_++, counter_);
        if (size > 0) memcpy(buf_, data, size);
        buffered_ = size;
    }

    vector<uint8_t> finish() {
        const uint8_t pad = static_cast<uint8_t>(kBlockBytes - buffered_);
        for (size_t i = buffered_; i < kBlockBytes; ++i) buf_[i] = pad;
        absorb_block(s_, buf_, index_, counter_);

        s_[4] ^= static_cast<uint32_t>(total_);
        s_[5] ^= static_cast<uint32_t>(total_ >> 32);
        s_[6] ^= 0xFFFFFFFFu;
        State f;
        for (size_t i = 0; i < kSpokes; ++i) f[i] = s_[i];
        turns(s_, kRoundsFinal, counter_);
        for (size_t i = 0; i < kSpokes; ++i) s_[i] += f[i];

        vector<uint8_t> out(kDigestBytes);
        for (size_t i = 0; i < 4; ++i) store_le32(s_[i], &out[4 * i]);
        turns(s_, kRoundsSqueeze, counter_);
        for (size_t i = 0; i < 4; ++i) store_le32(s_[i], &out[16 + 4 * i]);
        return out;
    }

    uint64_t total() const { return total_; }

private:
    State s_;
    uint8_t buf_[kBlockBytes];
    size_t buffered_ = 0;
    uint64_t index_ = 1;
    uint64_t total_ = 0;
    unsigned counter_ = 0;
};

[[maybe_unused]] vector<uint8_t> ratas256(const uint8_t* data, size_t size) {
    Ratas256 h;
    h.update(data, size);
    return h.finish();
}

string to_hex(const vector<uint8_t>& d) {
    static const char* digits = "0123456789abcdef";
    string s;
    s.reserve(d.size() * 2);
    for (uint8_t b : d) {
        s.push_back(digits[b >> 4]);
        s.push_back(digits[b & 0x0F]);
    }
    return s;
}

#ifdef _WIN32
string utf8_from_wide(const wstring& w) {
    if (w.empty()) return string();
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()),
                                      nullptr, 0, nullptr, nullptr);
    string s(static_cast<size_t>(n), 0);
    WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()),
                        &s[0], n, nullptr, nullptr);
    return s;
}

wstring wide_from_utf8(const string& s) {
    if (s.empty()) return wstring();
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()),
                                      nullptr, 0);
    wstring w(static_cast<size_t>(n), 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), &w[0], n);
    return w;
}
#endif

filesystem::path to_path(const string& utf8) {
#ifdef _WIN32
    return filesystem::path(wide_from_utf8(utf8));
#else
    return filesystem::path(utf8);
#endif
}

bool hash_file(const string& path, Ratas256& h) {
    ifstream f(to_path(path), ios::binary);
    if (!f) return false;
    vector<char> buf(1 << 16);
    while (f.read(buf.data(), static_cast<streamsize>(buf.size())) || f.gcount() > 0)
        h.update(reinterpret_cast<const uint8_t*>(buf.data()), static_cast<size_t>(f.gcount()));
    return !f.bad();
}

bool read_line(string& line) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode)) {
        wstring w;
        wchar_t buf[512];
        DWORD got = 0;
        bool ended = false;
        while (ReadConsoleW(h, buf, 512, &got, nullptr) && got > 0) {
            w.append(buf, got);
            if (w.back() == L'\n') {
                ended = true;
                break;
            }
        }
        if (!ended || w.find(L'\x1a') != wstring::npos) return false;
        while (!w.empty() && (w.back() == L'\n' || w.back() == L'\r')) w.pop_back();
        line = utf8_from_wide(w);
        return true;
    }
#endif
    if (!getline(cin, line) || cin.eof()) return false;
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return true;
}

void hold_console() {
#ifdef _WIN32
    DWORD pids[2];
    if (GetConsoleProcessList(pids, 2) == 1) {
        cout << "\nSpauskite Enter, kad uzdarytumete langa...";
        cin.get();
    }
#endif
}

int run(int argc, char** argv) {
    Ratas256 h;
    string mode;

    if (argc == 2) {
        string path = argv[1];
#ifdef _WIN32
        int wargc = 0;
        if (LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &wargc)) {
            if (wargc == 2) path = utf8_from_wide(wargv[1]);
            LocalFree(wargv);
        }
#endif
        mode = "failas: " + path;
        if (!hash_file(path, h)) {
            cerr << "Klaida: nepavyko perskaityti failo: " << path << '\n';
            return 2;
        }
    } else if (argc == 1) {
        cout << "Pasirinkite rezima:\n"
                "  1 - ivesti teksta ranka\n"
                "  2 - maisyti faila\n"
                "Pasirinkimas: ";
        string choice;
        if (!read_line(choice)) {
            cerr << "Klaida: ivestis baigesi, rezimas nepasirinktas\n";
            return 1;
        }

        if (choice == "1") {
            mode = "rankinis ivedimas";
            cout << "Iveskite teksta ir spauskite Enter (Enter neitraukiamas):\n";
            string line;
            if (!read_line(line)) {
                cerr << "Klaida: ivestis baigesi pries Enter, tekstas nemaisomas\n";
                return 1;
            }
            h.update(reinterpret_cast<const uint8_t*>(line.data()), line.size());
        } else if (choice == "2") {
            cout << "Failo kelias: ";
            string path;
            if (!read_line(path)) {
                cerr << "Klaida: ivestis baigesi, failo kelias neivestas\n";
                return 1;
            }
            if (path.size() >= 2 && path.front() == '"' && path.back() == '"')
                path = path.substr(1, path.size() - 2);
            mode = "failas: " + path;
            if (!hash_file(path, h)) {
                cerr << "Klaida: nepavyko perskaityti failo: " << path << '\n';
                return 2;
            }
        } else {
            cerr << "Klaida: reikia pasirinkti 1 arba 2\n";
            return 1;
        }
    } else {
        cerr << "Naudojimas: ratas [failas]\n";
        return 1;
    }

    cout << "Rezimas: " << mode << '\n'
         << "Ivesties baitu: " << h.total() << '\n'
         << "Ratas-256: " << to_hex(h.finish()) << '\n';
    return 0;
}

}
/*
int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    const int code = run(argc, argv);
    hold_console();
    return code;
}
*/
// Ši funkcija turi būti failo pabaigoje, UŽ "namespace { ... }" ribų
std::vector<uint8_t> joringis_hash_global(const uint8_t* data, size_t size) {
    return ratas256(data, size); // Iškviečia Joringio privačią funkciją
}