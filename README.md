# v0.3 (v0.2 programos tobulinimas su AI)
## Kasimo ciklo efektyvumas
Pasikonsultavus su Gemini, jis atrado, kad mano kasimo cikle kiekvieną kartą pakeitus nonce, bloko antraštė yra per naują paverčiama į tekstą ir atgal į baitų vektorių. Todėl nusprendžiau priimti šį pasiūlymą atsisakyti std::string objektų kūrimo.

Prompt:
```
 Šią vietą:

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

Pakeisk taip, kad antraštė į baitus būtų serializuojama tik vieną kartą, atsisakant std::string objektų kūrimo. Mano tikslas yra šią vietą kiek įmanoma optimizuoti efektyvumui.

```

Gavau atsakymą, su šiuo nauju kodo fragmentu:
```
// PARUOŠIMAS (Prieš 1000 bandymų ciklą)
// 1. Suklijuojame statines dalis, kurios nesikeičia viso kasimo metu
const auto& ant = kandidatai[k].antraste;
string pradzia = ant.praeitas_hash + "|" + 
                 std::to_string(ant.timestamp) + "|" + 
                 std::to_string(ant.versija) + "|" + 
                 ant.tx_root + "|";
string pabaiga = "|" + std::to_string(ant.sudetingumas);

// 2. Iš anksto išskiriame atmintį baitų vektoriui (išvengiame reallocation cikle)
vector<uint8_t> hash_input;
hash_input.reserve(pradzia.size() + 30 + pabaiga.size()); 
char nonce_buffer[24]; // Pakankamai vietos bet kokiam uint64_t skaičiui

for (int bandymas = 0; bandymas < 1000; bandymas++) {
    // Išvalome vektorių (talpa išlieka, tad atmintis neperskirstoma!)
    hash_input.clear(); 
    
    // A) Pridedame pradinę antraštės dalį
    hash_input.insert(hash_input.end(), pradzia.begin(), pradzia.end());
    
    // B) Paverčiame nonce į tekstą vietoje, nesukuriant std::string objekto
    int nonce_len = snprintf(nonce_buffer, sizeof(nonce_buffer), "%llu", kandidatai[k].antraste.nonce);
    hash_input.insert(hash_input.end(), nonce_buffer, nonce_buffer + nonce_len);
    
    // C) Pridedame pabaigos dalį
    hash_input.insert(hash_input.end(), pabaiga.begin(), pabaiga.end());
    
    // Siunčiame tiesiai į tavo custom_hashas funkciją (aplenkiame custom_hashas_string,
    // kuris darė dar vieną nereikalingą atminties kopiją)
    string hash = custom_hashas(hash_input);
    
    // Patikriname PoW
    if (hash.substr(0, 3) == "000") {
        kandidatai[k].blokoHash = hash;
        laimetojas = k;
        break;
    }
    kandidatai[k].antraste.nonce++;
}
```
#### Spartos palyginimas
| Bandymas | v0.2      | v0.3      |
|----------|-----------|-----------|
| 1        | 4.81738 s | 4.18734 s |
| 2        | 4.93145 s | 4.22933 s |
| 3        | 4.44011 s | 4.15384 s |
| 4        | 4.66835 s | 4.07431 s |
| 5        | 4.83354 s | 4.10661 s |
| Vidurkis | 4.73817 s | 4.15029 s |
- Pritaikius pasiūlytus pakeitimus programa vidutiniškai pagreitėjo beveik 0,6s. Kadangi buvo pakeistas labai mažas kodo fragmentas, toks greičio pokytis yra įspūdingas.

## Atminties naudojimas balansų kopijavime
Kitas Gemini pasiūlymas buvo std::map pakeisti std::unordered_map, taip paaukojant rikiavimą dėl spartos. Taip pat vietoj pilnos kopijos sukurti naują laikiną map, į kurį įrašomi tik transakcijų dalyviai (taip būtų išvengiama bereikalingo kopijavimo).

Prompt:
```
 Šioje vietoje:

vector<std::map<string, int64_t>> kandidatu_balansai(5);// ...for(int k = 0; k < 5; k++){

kandidatu_balansai[k] = balansai;

std::map pakeisk į std::unordered_map bei vietoj visos kopijos daryk "delta"(sukurk naują laikiną map, į kurį įrašyk tik tuos vartotojus, kurių balansai pasikeičia bloke, o tikrininant - jei vartotojo nėra laikinoje kopijoje, balansą imk iš pagrindinio) 
```

Gavau atsakymą kandidatų ciklo vidų pakeisti į:
```
vector<Blokas> kandidatai(5);
// Dabar tai saugos TIK PASIKEITIMUS (deltas), o ne visą būseną!
vector<std::unordered_map<string, int64_t>> kandidatu_balansai(5); 
bool sukurta_validziu = false;

for(int k = 0; k < 5; k++){
    // IŠTRINAME pilną kopijavimą! Laikinas žemėlapis startuoja tuščias.
    // kandidatu_balansai[k] = balansai; 
    
    vector<Transakcija> sumaisytos = laukiancios_tx;
    std::shuffle(sumaisytos.begin(), sumaisytos.end(), gen);
    
    int paimta = 0;
    // Čia irgi rekomenduoju unordered_map greitesnei paieškai
    std::unordered_map<string, bool> id_dublikatai; 
    
    for (const auto& tx : sumaisytos) {
        if (paimta >= 100) break;
        
        // Validacijos
        if (tx.suma <= 0) continue; 
        if (tx.siuntejas == tx.gavejas) continue;
        if (id_dublikatai[tx.txId]) continue;
        
        // ==========================================
        // DELTA (SKIRTUMŲ) LOGIKA
        // ==========================================
        
        // 1. SIUNTĖJAS: Jei jo dar nėra laikinoje (delta) būsenoje, balanso ieškome pagrindinėje
        int64_t siuntejo_esamas_balansas = kandidatu_balansai[k].count(tx.siuntejas) 
                                           ? kandidatu_balansai[k][tx.siuntejas] 
                                           : balansai[tx.siuntejas];
        
        // 2. Tikriname, ar užtenka pinigų
        if (siuntejo_esamas_balansas >= tx.suma) {
            
            // 3. GAVĖJAS: Tas pats principas
            int64_t gavejo_esamas_balansas = kandidatu_balansai[k].count(tx.gavejas) 
                                             ? kandidatu_balansai[k][tx.gavejas] 
                                             : balansai[tx.gavejas];
            
            // 4. Įrašome / atnaujiname reikšmes TIK laikinoje DELTA atmintyje
            kandidatu_balansai[k][tx.siuntejas] = siuntejo_esamas_balansas - tx.suma;
            kandidatu_balansai[k][tx.gavejas] = gavejo_esamas_balansas + tx.suma;
            
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
```
#### Spartos palyginimas
| Bandymas | v0.2      | v0.3      |
|----------|-----------|-----------|
| 1        | 4.81738 s | 3.74287 s |
| 2        | 4.93145 s | 3.82915 s |
| 3        | 4.44011 s | 3.73729 s |
| 4        | 4.66835 s | 3.89167 s |
| 5        | 4.83354 s | 3.97564 s |
| Vidurkis | 4.73817 s | 3.83532 s |
- Dar pridėjus šį pasiūlymą DI patobulinta versija jau greitesnė beveik pilna sekunde. Toks pokytis jau žymiai pastebimas ir pačiam žmogui. Dabar programa yra daug efektyvesnė ir eikvoja mažiau nebūtinų resursų.

## Testai
### Merkle medis
Kaip galime matyti žemiau, tiek v0.2 tiek v0.3 testo rezultatai yra identiški. Reiškiasi įgyvendinti patobulinimai išlaikė teisingą programos veikimą, tuo pačiu pagerindami spartą.
#### v0.2
1) Šaknys, kai keičiasi transakcijų kiekis
|Transakcijų skaičius | Šaknis |
|---------------------|--------|
| 1                   | c68e80c4b842d42983080271009ed19614dcb8a061375e8de3b2cea55a3362ee |
| 2                   | 3aa09f4b47a2dba89da28c2b47e23d099230a28cd566bbc5c43f2775e0cacc9c |
| 3                   | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| 5                   | 7e6107983562d9bd4fb227923097b63449215b6110a7a0a7606fcb313d120aa7 |

2) Šaknys, kai keičiamas turinys (3 transakcijos):
- Buvęs turinys: 300
- Naujas turinys: 9999

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Pakeista šaknis  | 2ba60835a1ccb7c5d02d07fc98467f8e7822cc80f06bb8919580e5e79f9b7af9 |

3) Šaknys, kai keičiama transakcijų tvarka (3 transakcijos):
- Pradinė tvarka : 1 -> 2 -> 3
- Pakeista tvarka: 2 -> 1 -> 3

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Sukeista šaknis  | 65c4d693ea01fe916a5970f971a887c20ec818ef86bc1f590c331f7dd4d497a6 |

#### v0.3
1) Šaknys, kai keičiasi transakcijų kiekis
|Transakcijų skaičius | Šaknis |
|---------------------|--------|
| 1                   | c68e80c4b842d42983080271009ed19614dcb8a061375e8de3b2cea55a3362ee |
| 2                   | 3aa09f4b47a2dba89da28c2b47e23d099230a28cd566bbc5c43f2775e0cacc9c |
| 3                   | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| 5                   | 7e6107983562d9bd4fb227923097b63449215b6110a7a0a7606fcb313d120aa7 |

2) Šaknys, kai keičiamas turinys (3 transakcijos):
- Buvęs turinys: 300
- Naujas turinys: 9999

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Pakeista šaknis  | 2ba60835a1ccb7c5d02d07fc98467f8e7822cc80f06bb8919580e5e79f9b7af9 |

3) Šaknys, kai keičiama transakcijų tvarka (3 transakcijos):
- Pradinė tvarka : 1 -> 2 -> 3
- Pakeista tvarka: 2 -> 1 -> 3

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Sukeista šaknis  | 65c4d693ea01fe916a5970f971a887c20ec818ef86bc1f590c331f7dd4d497a6 |

### Teisingumo testai
Detalesnį testo aprašymą galima rasti v0.2 versijoje
```
--- Pradine busena: Jonas = 100, Ona = 50 ---

[SCENARIJAI 1, 3, 4] Galiojimas, eiliškumas ir dublikatai
 -> Patvirtinta (Validus): 70 nuo Jonas (Likutis: 30)
 -> Atmesta (Truksta lesu): Nori siusti 50, bet turi tik 30
 -> Atmesta (Dubliuotas ID): id_1
 * Rezultatas: Jonas galutinis: 30, Ona galutinis: 120
 * Laukta: Jonas 30, Ona 120. Testas Išlaikytas!

[SCENARIJUS 5] Pakeistas bloko turinys (PoW pažeidimas)
Buvęs teisingas bloko hash: 000fbe1280b9b10df80a18dac01afc35cc7371d56383ec74fe2ef08cc8890738 (Rastas su nonce: 403)
 Hash po hakerio transakcijos: 711a610f65754eeca7f62331b8ebac8f10c46edf868d973fdbb3ad94c220f58d
 -> Rezultatas: Bloko turinys pasikeitė, todėl senas 'nonce' nebetinka. 
    PoW reikalavimas (000) sugadintas, blokas atmetamas!
 * Testas Išlaikytas!
```

- Kaip matome visi testai praeiti sėkmingai!
