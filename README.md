# Merkle tree
## Palyginimas su v0.1 versija
Ankstesnėje versijoje transakcijų hash'ai buvo sudedami vienas su kitu ir taip gaunamas bendras hash. Pritaikius Merkle medį dabar transakcijų hash'ai yra sudedami poromis (jei nėra poros su kopija), kol gaunamas vienas root hash.

## Pakeitimų logikos paaiškinimas
1) Sukuriami 5 blokai kandidatai
2) Transakcijos sumaišomos, kad kandidatai skirtųsi
3) Atliekama, kol apdorojamos 100 transakcijų:
    - Teigiamos sumos validacija
    - Skirtingų siuntėjų/gavėjų validacija
    - Nepasikartojančių transakcijų validacija
    - Balanso validacija (ar siuntėjas turi pakankamai lėšų)
    Praėjus šias validacijas transakcija priimama ir apdorojama toliau
4) Sukuriama kandidatų antraštės
5) Ieškomas kandidatas laimėtojas:
    - Kiekvienam kandidatui skiriama 1000 bandymų
    - Kadangi sudėtingumas yra 3, tai ieškoma hash, kuris prasideda "000"
    - Kandidatu nugalėtoju tampa tas blokas, iš kurio pirmo randamas tinkamas hash
    - Jei vieno ciklo metu nerandamas laimėtojas, didinamas nonce
6) Iš transakcijų poolo panaikinamos transakcijos, kurios priklausė blokui laimėtojui
## Testai
### Testas 1
1) Šaknys, kai transakcijų kiekiai skirtingi

| Transakcijų kiekis | Šaknis |
|--------------------|--------|
| 1                  | c68e80c4b842d42983080271009ed19614dcb8a061375e8de3b2cea55a3362ee |
| 2                  | 3aa09f4b47a2dba89da28c2b47e23d099230a28cd566bbc5c43f2775e0cacc9c |
| 3                  | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| 5                  | 7e6107983562d9bd4fb227923097b63449215b6110a7a0a7606fcb313d120aa7 |

- Šaknys skiriasi viena nuo kitos - kolizijų nėra.

2) Šaknys, kai keičiamas turinys (3 transakcijos):
- Buvęs turinys: 300
- Naujas turinys: 9999

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Pakeista šaknis  | 2ba60835a1ccb7c5d02d07fc98467f8e7822cc80f06bb8919580e5e79f9b7af9 |

- Pakeitus turinį, šaknis sėkmingai pasikeitė! Čia kolizijos nepatinkame

3) Šaknys, kai keičiama transakcijų tvarka (3 transakcijos):
- Pradinė tvarka : 1 -> 2 -> 3
- Pakeista tvarka: 2 -> 1 -> 3

| Būsena           | Šaknis |
|------------------|--------|
| Originali šaknis | c0075d1751c76f517b0441587df6314d9ae1393f6b825f1bce54c464fd11691d |
| Sukeista šaknis  | 65c4d693ea01fe916a5970f971a887c20ec818ef86bc1f590c331f7dd4d497a6 |

- Tvarkos pakeitimas kardinaliai pakeičia šaknį, kolizijos čia taip pat neaptinkame.

### Teisingumo testai
#### Lėšų nepakankamumas, pasikartojantis transakcijos id
1) Buvo sukurti dalyviai Jonas ir Ona su balansais, bei transakcijos tarp jų:
```
    std::map<string, int64_t> balansai;
    balansai["Jonas"] = 100;
    balansai["Ona"] = 50;
    
    Transakcija tx1 = {"Jonas", "Ona", 70, 1, "id_1"};
    Transakcija tx2 = {"Jonas", "Ona", 50, 2, "id_2"};
    Transakcija tx3 = {"Jonas", "Ona", 30, 3, "id_1"};
```
2) Laukiami rezultatai:
    - tx1 turėtų praeiti.
    - tx2 turėtų nepraeiti, kadangi po tx1 transakcijos pas Joną liko balansas 30, o nori išsiųsti 50.
    - tx3 turėtų nepraeiti, kadangi jos id sutampa su tx1.
3) Gauti rezultatai:
```
Patvirtinta (Validus): 70 nuo Jonas (Likutis: 30)
Atmesta (Truksta lesu): Nori siusti 50, bet turi tik 30
Atmesta (Dubliuotas ID): id_1
Rezultatas: Jonas galutinis: 30, Ona galutinis: 120
```
* Gauti rezultatai atitiko, ko tikėjomės. Testas teisingas!

#### PoW pažeidimas
- Pradinis bloko hash: 000cfbab84fbd55a24be0036f9580eae3824738d026d85941484eabda5ed4a46
- Pradinis nonce: 1969
1) Prie jau įvykusių transakcijų pridedama netikra hakerio transakcija:
```
    Transakcija netikra_tx = {"Hakeris", "Hakeris", 10000, 0, "netikras_id"};
    originalus_test_blokas.transakcijos.push_back(netikra_tx);
```
2) Iš naujo randama antraštės root ir suskaičiuojamas naujas bloko hash (su senu nonce):
```
    originalus_test_blokas.antraste.tx_root = merkleRoot(originalus_test_blokas.transakcijos);
    string naujas_hash = custom_hashas_string(serializuotiAntraste(originalus_test_blokas.antraste));
```
- Po pridėtos naujos transakcijos naujas hash:  9a68162a3bddf8bf2766474dd621351180f7fd49c45244fedb9720bb7cabff77
- PoW reikalavimas, kad hash prasidėtų "000" sugadintas. Kadangi bloko turinys pasikeitė, senas nonce yra nebetinkamas. Taigi PoW veikia tinkamai :)


## Kodėl nėra P2P konsensusas?
Ši simuliacija nėra pilnavertis P2P konsensusas dėl šių priežasčių:
#### Nėra decentralizacijos
Visa simuliacija yra vykdoma lokaliai. Tikrame P2P tinkle tūkstančiai nepriklausomų mazgų (nodes) siunčia transakcijas ir blokus internetu.
#### Konkurencija tik imituojama
Realiame P2P blokų konkurencija yra vykdoma lygiagrečiai, o ne paskyrus tam tikrą limitą (laiko ar bandymų) kiekvienam blokui.
#### Nėra šakų persitvarkymo
Simuliacijoje grandinė visada auga tiesiai, tačiau tikrame P2P konsensuso tinkle du mazgai gali iškasti reikiamą bloką vienu metu, sukurdami dvi skirtingas šakas, ko pasekoje tinklas atrenka viena šaką pagal "ilgiausios grandinės taisyklę".