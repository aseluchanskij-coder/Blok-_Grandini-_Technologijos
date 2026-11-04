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

## Kodėl nėra P2P konsensusas?
Ši simuliacija nėra pilnavertis P2P konsensusas dėl šių priežasčių:
#### Nėra decentralizacijos
Visa simuliacija yra vykdoma lokaliai. Tikrame P2P tinkle tūkstančiai nepriklausomų mazgų (nodes) siunčia transakcijas ir blokus internetu.
#### Konkurencija tik imituojama
Realiame P2P blokų konkurencija yra vykdoma lygiagrečiai, o ne paskyrus tam tikrą limitą (laiko ar bandymų) kiekvienam blokui.
#### Nėra šakų persitvarkymo
Simuliacijoje grandinė visada auga tiesiai, tačiau tikrame P2P konsensuso tinkle du mazgai gali iškasti reikiamą bloką vienu metu, sukurdami dvi skirtingas šakas, ko pasekoje tinklas atrenka viena šaką pagal "ilgiausios grandinės taisyklę".