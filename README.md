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