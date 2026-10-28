#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <map>
#include <chrono>
#include <ctime>
#include <random>

using std::cout;
using std::vector;
using std::cin;
using std::endl;
using std::string;

// Išankstinė deklaracija, kad custom_hashas_string žinotų apie šią funkciją
string custom_hashas(vector<uint8_t> ivestis);

struct Transakcija {
    string siuntejas;
    string gavejas;
    std::int64_t suma; 
    std::uint64_t seka;
    string txId; 
};

struct BlokoAnt {
    string praeitas_hash;
    uint64_t timestamp;
    int versija;
    string tx_root;
    uint64_t nonce;
    int sudetingumas;
};

struct Blokas {
    BlokoAnt antraste;
    vector<Transakcija> transakcijos;
    string blokoHash;
};

string custom_hashas_string(const string& tekstas) {
    vector<uint8_t> duomenys(tekstas.begin(), tekstas.end());
    return custom_hashas(duomenys);
}

string serializuotiTransakcija(const Transakcija& tx) {
    return tx.siuntejas + "|" + 
           tx.gavejas + "|" + 
           std::to_string(tx.suma) + "|" + 
           std::to_string(tx.seka);
}

string serializuotiAntraste(const BlokoAnt& ant) {
    return ant.praeitas_hash + "|" + 
           std::to_string(ant.timestamp) + "|" + 
           std::to_string(ant.versija) + "|" + 
           ant.tx_root + "|" + 
           std::to_string(ant.nonce) + "|" + 
           std::to_string(ant.sudetingumas);
}

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

int main() {
    std::mt19937 gen(12345); //generatorius
    std::uniform_int_distribution<int64_t> balanso_gen(100, 1000000); //riba
    
    std::map<string, int64_t> balansai; //mape laikom
    vector<string> vartotoju_id;
    int64_t pradine_bendra_suma = 0;

    for (int i = 0; i < 1000; i++) { //vartotojus generuojam
        string id = "user_" + std::to_string(i);
        int64_t pradinis_balansas = balanso_gen(gen);
        
        balansai[id] = pradinis_balansas;
        vartotoju_id.push_back(id);
        pradine_bendra_suma += pradinis_balansas;
    }
    cout << "Sugeneruota vartotoju: " << balansai.size() << endl;
    cout << "Bendra pradine suma tinkle: " << pradine_bendra_suma << endl;

    vector<Transakcija> transakciju_poolas;  //daba 10k transakciju generuojam
    std::uniform_int_distribution<int> vartotojo_indekso_gen(0, 999);
    std::uniform_int_distribution<int64_t> sumos_gen(1, 10000); // Suma nuo 1 iki 10 000

    for (uint64_t i = 0; i < 10000; i++) { //pats generavimas
        string siuntejas = vartotoju_id[vartotojo_indekso_gen(gen)];
        string gavejas = vartotoju_id[vartotojo_indekso_gen(gen)];
        
        while (siuntejas == gavejas) {      //siuntejas negali but tuo pat metu ir gaveju vienoj transakcijoj
            gavejas = vartotoju_id[vartotojo_indekso_gen(gen)];
        }
        Transakcija tx;
        tx.siuntejas = siuntejas;
        tx.gavejas = gavejas;
        tx.suma = sumos_gen(gen);
        tx.seka = i; //unikalus nuoseklus numeris
        
        //serializacija
        string serializuota = serializuotiTransakcija(tx);
        tx.txId = custom_hashas_string(serializuota);
        
        transakciju_poolas.push_back(tx);
    }
    
    cout << "Sugeneruota transakciju: " << transakciju_poolas.size() << endl;
    
    //small test
    cout << "\nPirmos transakcijos pavyzdys:" << endl;
    cout << "Siuntejas: " << transakciju_poolas[0].siuntejas << endl;
    cout << "Gavejas: " << transakciju_poolas[0].gavejas << endl;
    cout << "Suma: " << transakciju_poolas[0].suma << endl;
    cout << "Seka: " << transakciju_poolas[0].seka << endl;
    cout << "Tx ID (maisa): " << transakciju_poolas[0].txId << endl;

    //genesis
    vector<Blokas> blockchain;

    Blokas genesis;
    genesis.antraste.praeitas_hash = string(64, '0'); //64 nuliai
    genesis.antraste.timestamp = std::time(nullptr); //dabartinis laikas
    genesis.antraste.versija = 1;
    genesis.antraste.tx_root = custom_hashas_string(""); //maisa nuo nieko
    genesis.antraste.nonce = 0;
    genesis.antraste.sudetingumas = 3;
    
    genesis.blokoHash = custom_hashas_string(serializuotiAntraste(genesis.antraste));
    blockchain.push_back(genesis);
    
    cout << "\nGenesis maisa: " << genesis.blokoHash << endl;

    cout << "\nMatavimas bloko kasimo:" << endl;
    auto start_time = std::chrono::high_resolution_clock::now();

    int atmestos_transakcijos = 0;
    size_t dabartinis_tx_indeksas = 0; 

    //sukame cikla kol patikrinsim transakcijas
    while (dabartinis_tx_indeksas < transakciju_poolas.size()) {
        vector<Transakcija> bloko_transakcijos;
        std::map<string, int64_t> laikinieji_balansai = balansai; //kopija busenos patikrai
        string visi_tx_id_kartu = ""; //txruoot reikes visu id

        int paimta_i_kandidata = 0;
        
        //iki 100 transakciju salyga
        while (paimta_i_kandidata < 100 && dabartinis_tx_indeksas < transakciju_poolas.size()) {
            Transakcija tx = transakciju_poolas[dabartinis_tx_indeksas];
            dabartinis_tx_indeksas++;
            
            //ar uztenka lesu siuntejui
            if (laikinieji_balansai[tx.siuntejas] >= tx.suma) {
                laikinieji_balansai[tx.siuntejas] -= tx.suma; //pinigai kopijoje
                laikinieji_balansai[tx.gavejas] += tx.suma;   //gavejui pinigai kopijoje
                bloko_transakcijos.push_back(tx);
                visi_tx_id_kartu += tx.txId; //id bloko maisai
            } else {
                atmestos_transakcijos++; //jei neuztenka lesu skaitliukui pridedam
            }
            paimta_i_kandidata++;
        }

        if (bloko_transakcijos.empty()) continue;

        //kandidatinis blok
        Blokas naujas_blokas;
        naujas_blokas.transakcijos = bloko_transakcijos;
        
        //antraste uzpildom
        naujas_blokas.antraste.praeitas_hash = blockchain.back().blokoHash; //grandinės susiejimas
        naujas_blokas.antraste.timestamp = std::time(nullptr);
        naujas_blokas.antraste.versija = 1;
        naujas_blokas.antraste.tx_root = custom_hashas_string(visi_tx_id_kartu); 
        naujas_blokas.antraste.sudetingumas = 3; 
        naujas_blokas.antraste.nonce = 0; //pradedame speliot nuo 0

        string kandidato_maisa;
        auto block_start = std::chrono::high_resolution_clock::now();

        //kasimas
        while (true) {
            kandidato_maisa = custom_hashas_string(serializuotiAntraste(naujas_blokas.antraste));
            
            //randame 000""""""""" pavyko
            if (kandidato_maisa.substr(0, 3) == "000") {
                naujas_blokas.blokoHash = kandidato_maisa;
                break;
            }
            //jei netiko didinam nonca
            naujas_blokas.antraste.nonce++; 
        }

        auto block_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> block_duration = block_end - block_start;
        ...
        
    return 0;
}