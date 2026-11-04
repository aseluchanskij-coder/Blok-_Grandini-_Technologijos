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
#include <algorithm>

using std::cout;
using std::vector;
using std::cin;
using std::endl;
using std::string;

string custom_hashas(vector<uint8_t> ivestis);

struct TXInput {
    string ankstesnis_txId; 
    int isvesties_indeksas; 
    string savininkas;      
};

struct TXOutput {
    int64_t suma;           
    string gavejas;         
};

struct Transakcija {
    vector<TXInput> ivestys;   
    vector<TXOutput> isvestys; 
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
    string rezultatas = "";
    //prijungiame ivestis
    for (const auto& ivestis : tx.ivestys) {
        rezultatas += ivestis.ankstesnis_txId + "|" + 
                      std::to_string(ivestis.isvesties_indeksas) + "|" + 
                      ivestis.savininkas + "|";
    }
    
    //prijungiame išvestis
    for (const auto& isvestis : tx.isvestys) {
        rezultatas += std::to_string(isvestis.suma) + "|" + 
                      isvestis.gavejas + "|";
    }
    
    //prijungiame sekos numeri
    rezultatas += std::to_string(tx.seka);
    
    return rezultatas;
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

string merkleRoot(const vector<Transakcija>& transakcijos) {
    // Transakciju hashus sudedame i bendra vektoriu
    vector<string> txHashes;
    for (const auto& tx : transakcijos) {
        txHashes.push_back(tx.txId);
    }
    if (txHashes.empty()) {
        return string(64, '0'); 
    }

    while(txHashes.size() > 1) {
        vector<string> suporuotiHashai;
        for(int i = 0; i < txHashes.size(); i += 2){
            if(i + 1 < txHashes.size()){
                suporuotiHashai.push_back(custom_hashas_string(txHashes[i] + txHashes[i + 1]));
            }
            else {
                suporuotiHashai.push_back(custom_hashas_string(txHashes[i] + txHashes[i]));
            }
        }
        txHashes = suporuotiHashai;
    }
    return txHashes[0];
}

void testMerkleTree() {
    cout << "\n=== UZDUOTIS: MERKLE MEDZIO TESTAVIMAS ===" << endl;

    // 1. Sukuriame 5 bazines transakcijas testams
    vector<Transakcija> txs(5);
    for (int i = 0; i < 5; i++) {
        txs[i].siuntejas = "Alice";
        txs[i].gavejas = "Bob";
        txs[i].suma = 100 * (i + 1);
        txs[i].seka = i;
        txs[i].txId = custom_hashas_string(serializuotiTransakcija(txs[i]));
    }

    // 2. Patikriname atvejus su 1, 2, 3 ir 5 transakcijomis
    cout << "1) Skirtingu kiekiu saknys:" << endl;
    vector<int> kiekiai = {1, 2, 3, 5};
    for (int kiekis : kiekiai) {
        vector<Transakcija> dalis(txs.begin(), txs.begin() + kiekis);
        cout << kiekis << " transakciju saknis: " << merkleRoot(dalis) << endl;
    }

    // 3. Pakeisto turinio testas (naudosime 3 transakcijas)
    cout << "\n2) Turinio keitimo testas (3 transakcijos):" << endl;
    vector<Transakcija> originalios = {txs[0], txs[1], txs[2]};
    string originaliSaknis = merkleRoot(originalios);
    cout << "Originali saknis: " << originaliSaknis << endl;

    vector<Transakcija> pakeistos = originalios;
    pakeistos[0].suma = 9999; // Keiciame suma (turini)
    pakeistos[0].txId = custom_hashas_string(serializuotiTransakcija(pakeistos[0])); // Perskaiciuojame ID!
    string pakeistaSaknis = merkleRoot(pakeistos);
    cout << "Pakeista saknis:  " << pakeistaSaknis << endl;
    
    if (originaliSaknis != pakeistaSaknis) {
        cout << "-> Rezultatas: Pakeitus turini, saknis sekmingai pasikeite!" << endl;
    }

    // 4. Sukeistos tvarkos testas (naudosime tas pacias 3 originalias)
    cout << "\n3) Tvarkos keitimo testas (3 transakcijos):" << endl;
    vector<Transakcija> sukeistos = originalios;
    
    // Sukeiciame pirma su antra
    Transakcija laikina = sukeistos[0];
    sukeistos[0] = sukeistos[1];
    sukeistos[1] = laikina;
    
    string sukeistaSaknis = merkleRoot(sukeistos);
    cout << "Sukeista saknis:  " << sukeistaSaknis << endl;
    
    if (originaliSaknis != sukeistaSaknis) {
        cout << "-> Rezultatas: Sukeitus tvarka, saknis sekmingai pasikeite!" << endl;
    }
    cout << "==========================================\n" << endl;
}

int main() {
    cout << "1 - merkle testas, 2 - blockchain sim\n";
    int pasirinkimas;
    cin >> pasirinkimas;
    
    if (pasirinkimas == 1) {
        testMerkleTree(); //paleidziam testus
        return 0; //baigiame programa po testu
    }
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
    genesis.antraste.tx_root = merkleRoot(genesis.transakcijos); //maisa nuo nieko
    genesis.antraste.nonce = 0;
    genesis.antraste.sudetingumas = 3;
    
    genesis.blokoHash = custom_hashas_string(serializuotiAntraste(genesis.antraste));
    blockchain.push_back(genesis);
    
    cout << "\nGenesis maisa: " << genesis.blokoHash << endl;

    cout << "\nMatavimas bloko kasimo:" << endl;
    auto start_time = std::chrono::high_resolution_clock::now();

    // pakeitimai pagal v0.2 reikalavimus
    int atmestos_transakcijos = 0;
    vector<Transakcija> laukiancios_tx = transakciju_poolas;
    while(!laukiancios_tx.empty()) {
        vector<Blokas> kandidatai(5); //sukuriami 5 kandidatai
        vector<std::map<string, int64_t>> kandidatu_balansai(5);
        bool sukurta_validziu = false;

        for(int k = 0; k < 5; k++){
            kandidatu_balansai[k] = balansai; 
            
            // transakcijos sumaisomas, kad blokai butu skirtingi
            vector<Transakcija> sumaisytos = laukiancios_tx;
            std::shuffle(sumaisytos.begin(), sumaisytos.end(), gen);
            
            int paimta = 0;
            std::map<string, bool> id_dublikatai; // Apsauga nuo vienodų ID
            
            for (const auto& tx : sumaisytos) {
                if (paimta >= 100) break;
                
                // Validacijos
                if (tx.suma <= 0) continue; 
                if (tx.siuntejas == tx.gavejas) continue;
                if (id_dublikatai[tx.txId]) continue;
                
                // Balando tikrinimas
                if (kandidatu_balansai[k][tx.siuntejas] >= tx.suma) {
                    kandidatu_balansai[k][tx.siuntejas] -= tx.suma;
                    kandidatu_balansai[k][tx.gavejas] += tx.suma;
                    
                    kandidatai[k].transakcijos.push_back(tx);
                    id_dublikatai[tx.txId] = true;
                    paimta++;
                    sukurta_validziu = true;
                }
            }
            kandidatai[k].antraste.praeitas_hash = blockchain.back().blokoHash;
            kandidatai[k].antraste.timestamp = std::time(nullptr);
            kandidatai[k].antraste.versija = 1;
            kandidatai[k].antraste.tx_root = merkleRoot(kandidatai[k].transakcijos);
            kandidatai[k].antraste.sudetingumas = 3;
            kandidatai[k].antraste.nonce = 0;
        }

        // Jei nei vienam kandidatui nepavyko rasti validžių transakcijų, baigiam
        if (!sukurta_validziu) {
            atmestos_transakcijos += laukiancios_tx.size();
            break;
        }

        // Kasimas
        int laimetojas = -1;
        
        while (laimetojas == -1) {
            for (int k = 0; k < 5; k++) {
                if (kandidatai[k].transakcijos.empty()) continue; 
                
                // Kiekvienas kandidatas gauna 1000 bandymų
                for (int bandymas = 0; bandymas < 1000; bandymas++) {
                    string hash = custom_hashas_string(serializuotiAntraste(kandidatai[k].antraste));
                    //jei hash prasideda 000 mums tinka
                    if (hash.substr(0, 3) == "000") {
                        kandidatai[k].blokoHash = hash;
                        laimetojas = k;
                        break;
                    }
                    kandidatai[k].antraste.nonce++;
                }
                if (laimetojas != -1) break;
            }
        }
        Blokas nugaletojas = kandidatai[laimetojas];
        balansai = kandidatu_balansai[laimetojas]; // Priskyrimas tikros būsenos
        blockchain.push_back(nugaletojas);

        // Išvalome nugalėtojo transakcijas iš pagrindinio pool'o
        std::map<string, bool> ismestos_tx;
        for (const auto& tx : nugaletojas.transakcijos) {
            ismestos_tx[tx.txId] = true;
        }
        
        vector<Transakcija> naujas_poolas;
        for (const auto& tx : laukiancios_tx) {
            if (!ismestos_tx[tx.txId]) {
                naujas_poolas.push_back(tx);
            }
        }
        laukiancios_tx = naujas_poolas; // Atnaujinam laukiančias kitam raundui

        cout << "Iškastas Blokas #" << blockchain.size() - 1 
             << " | Laimėjo Kand: " << laimetojas + 1 
             << " | Tx: " << nugaletojas.transakcijos.size() 
             << " | Nonce: " << nugaletojas.antraste.nonce 
             << " | Hash: " << nugaletojas.blokoHash << endl;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_duration = end_time - start_time;

    //skaiciuojam pinigus
    int64_t galutine_bendra_suma = 0;
    for (const auto& pora : balansai) {
        galutine_bendra_suma += pora.second;
    }
    cout << "Priimtu bloku (su Genesis): " << blockchain.size() << endl;
    cout << "Atmestos transakcijos: " << atmestos_transakcijos << endl;
    cout << "Pradine suma: " << pradine_bendra_suma << endl;
    cout << "Galutine suma: " << galutine_bendra_suma << endl;
    
    if (pradine_bendra_suma == galutine_bendra_suma) {
        cout << "Pinigai nedingo" << endl;
    } else {
        cout << "Pinigai dingo" << endl;
    }
    cout << "Visas kasimo laikas: " << total_duration.count() << " s" << endl;
        
    return 0;
}