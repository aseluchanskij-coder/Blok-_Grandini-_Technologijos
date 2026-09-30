# Pradžios etapas
Kadangi mano tikslas buvo sukurti hash funkciją, nenaudojant AI pasiūlymų, savo darbą pradėjau nuo research apie hash funkcijas, kriptografiją (mano naudoti šaltiniai pateikti apačioje). Išsityrus pagrindinius hash algoritmo etapus bandžiau juos pritaikyti savo algoritmui. Pasirinkau sugeneruoti 264 bitų hash, kadangi jis plačiai naudojamas. Seed'am parinkau atsitiktines reikšmes.

# Tyrimai
### Seed
```
uint32_t state[8] = {
        0x12345678, // seed 1
        0x8abcd123, // seed 2
        0xabcabcab, // seed 3
        0x87654321, // seed 4
        0xdefdefff, // seed 5
        0xaabbccdd, // seed 6
        0x11223344, // seed 7
        0xfedbc111 // seed 8
    };
```
### Metodologija
- Pradinė informacija skaitoma iš failo (ios::binary)
- Tyrimams naudota imtis: Tuščias failas, 2 skirtingi vieno baito dydžio failai; Keli atsitiktinio ASCII turinio failai; Jų kopijos su vienu pakeistu baitu, pradžioje, viduryje, pabaigoje; Keli struktūrizuoti atvejai (pasikartojantys simboliai, tarpai pradžioje / pabaigoje ir pnš.); Vienas UTF-8 tekstas
- CPU: AMD Ryzen™ 5 8645HS w/ Radeon™ 760M Graphics × 12
- RAM: 16GB
- OS: Ubuntu 24.04.4 LTS
- Kompiliatoriaus versija: g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
- Kompiliavimo parinktys: g++ -O3 main.cpp -o main

### Eksperimentas 1
| Nr. | Įvestis | Maiša (256 bitų, hex) |
|---:|---|---|
| 1 | Vieno baito įvestis `a` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
| 2 | Vieno baito įvestis `b` | `6c27592be2e15b21a83a25ffa3dbb6dddb104c4439700d1998a27ee16b617fc2` |
| 3 | \>1000 baitų ASCII tekstas | `43727c100990f2df2d420aff3ffe73a624fc1b7ed73df0aa64165c89ced1e284` |
| 4 | Tas pats tekstas, pakeistas vienas baitas | `3e7e2716692d78df94b928ff2d90b4a60c95d97853a488a430575487d8c5558a` |
| 5 | Kitas \>1000 baitų ASCII tekstas | `081d449e5d10d180dc87624f2d9b9b752e4bcec241fcafb600f84541edc2fd03` |
| 6 | Tas pats tekstas, pakeistas vienas baitas | `d3ecad9e25d6718090b30dc55cfa5b75b953bdc22c30d98e3cbf7c797f2c0d3b` |
| 7 | Dar kitas \>1000 baitų ASCII tekstas | `09614641a1468231760b18586856d2bdf6880b6ee9d5df78600cb2418868bd46` |
| 8 | Tas pats tekstas, pakeistas vienas baitas | `8ed2824103688db11d0f295bcdb1a3be005ec26d2da8a07bafcfa0426c859e45` |
| 9 | Struktūruota pasikartojanti raidė (`AAA...AAA`) | `c742492b1db3c32101f11dff5af416dd235e8444859e3d116b2736e92e33b7ca` |
| 10 | Struktūruota įvestis su `\n` simboliu (`ABC` / `abc`) | `a2a9452b557c862121c341ffa9b915dd7332ac443fb1ca29c4983fdb046ec178` |
| 11 | Ne ASCII simboliai | `d25a383144332efb3492d85ecae4e149dee65b1da825576b84c9b54db1df7e72` |

Visų maišų ilgis: 64 hex simboliai (256 bitai).

### Eksperimentas 2
Ta pati įvestis `a` hashinama dviem būdais: nuskaitant iš failo ir įvedant per terminalą.
 
| Nr. | Įvesties šaltinis | Įvestis | Maiša (256 bitų, hex) |
|---:|---|---|---|
| 1 | Failas (`failas1.txt`) | `a` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
| 2 | Terminalas (`std::getline`) | `a` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
 
**Palyginimas:** maišos **sutampa** (64 iš 64 hex simbolių).
 
**Išvada:** maiša priklauso tik nuo įvesties baitų, o ne nuo jų šaltinio. `getline` neįtraukia eilutės pabaigos simbolio, todėl terminalo įvestis `a` yra tiksliai 1 baitas, kaip ir failas.


- Mano maiša yra 256 bitų ilgio, 256 / 4 yra 64. Gautas hash yra būtent 64 simbolių ilgio, hex formatu.

### Eksperimentas 3
A, B, A testas (vienos programos paleidimo metu)
 
| Nr. | Failas | Įvestis | Maiša (256 bitų, hex) |
|---:|---|---|---|
| 1 (A) | `failas1.txt` | `a` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
| 2 (B) | `failas2.txt` | `b` | `6c27592be2e15b21a83a25ffa3dbb6dddb104c4439700d1998a27ee16b617fc2` |
| 3 (A) | `failas1.txt` | `a` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
 
- 1 ir 3 maišos **sutampa**; 2 maiša **skiriasi**.
 
Programos paleidimas iš naujo
 
| Paleidimas | Failas | Maiša (256 bitų, hex) |
|---|---|---|
| Pirmas | `failas1.txt` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
| Antras | `failas1.txt` | `2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2` |
 
- **Palyginimas:** maišos **sutampa** ir tarp skirtingų programos paleidimų.
 
- Funkcija yra **deterministinė**: ta pati įvestis visada duoda tą pačią maišą, nepriklausomai nuo to, kiek kartų ir kokia tvarka ji hashinama, ar programa paleista iš naujo. Tai rodo, kad tarp kvietimų nelieka jokios paslėptos būsenos (`state` kiekvieną kartą inicializuojamas iš tų pačių fiksuotų pradinių reikšmių, seed).


### Eksperimentas 4
Failas nuskaitytas. Viso eilučių: 789

| Eilutės | Baitai | Min (ms) | Max (ms) | Vidurkis (ms) | Vidurkis (ns / baitą) |
|---:|---:|---:|---:|---:|---:|
| 1 | 70 | 0.0004551 | 0.0005615 | 0.0004911 | 7.02 |
| 2 | 123 | 0.0005539 | 0.0005936 | 0.00057614 | 4.68 |
| 4 | 205 | 0.0006881 | 0.0022755 | 0.00106014 | 5.17 |
| 8 | 362 | 0.0009486 | 0.0010875 | 0.00100642 | 2.78 |
| 16 | 996 | 0.0020323 | 0.0024606 | 0.0022603 | 2.27 |
| 32 | 1841 | 0.0034232 | 0.0038892 | 0.00358276 | 1.95 |
| 64 | 3712 | 0.0065897 | 0.0070674 | 0.00673082 | 1.81 |
| 128 | 9155 | 0.0158637 | 0.0167162 | 0.0161995 | 1.77 |
| 256 | 20409 | 0.0343394 | 0.035833 | 0.0350514 | 1.72 |
| 512 | 47434 | 0.0863487 | 0.113639 | 0.0956014 | 2.02 |
| 789 (visas failas) | 75595 | 0.133922 | 0.146361 | 0.137835 | 1.82 |

- **Laikas auga maždaug tiesiškai** nuo baitų skaičiaus: nuo ~1 000 baitų apie 1,7–2,0 ns vienam baitui (kai įvestis dvigubėja, laikas ~dvigubėja).


### Eksperimentas 5
Eksperimento metu sugeneravome po 100 000 atsitiktinių ASCII eilučių porų keturiems ilgiams: 10, 100, 500 ir 1000 baitų (naudotas std::mt19937 generatorius su seed = 2026). Kolizijų ieškojome trimis etapais.

--- 5 EKSPERIMENTAS: KOLIZIJU PAIESKA ---
Naudojamas seed: 2026. Abecele: ASCII (32-126).

| Ilgis (baitai) | Porų skaičius | Kolizijos porose | Kolizijos visame rinkinyje | Skirtingų maišų skaičius |
|---:|---:|---:|---:|---:|
| 10 | 100 000 | 0 | 0 | 200 000 |
| 100 | 100 000 | 0 | 0 | 200 000 |
| 500 | 100 000 | 0 | 0 | 200 000 |
| 1000 | 100 000 | 0 | 0 | 200 000 |
 
**Rezultatas:** nė viename ilgyje kolizijų nerasta; visos 200 000 įvesčių davė skirtingas maišas.

| Nr. | Įvestis | Baitai | Maiša (256 bitų, hex) |
|---:|---|---:|---|
| 1 | `ABABABABABABABAB` | 16 | `d9286c6990395263757ea4ff8d5044ddc28533448cf5b1916a14862b82d5c34a` |
| 2 | `BচুBচুBচুBচুBচু` | 35 | `11a76b589635bf7b123147c5a286298b0c9975e9152fc09052d88c5b1758c31a` |
| 3 | `AAAAAAAAAAAAAAAB` | 16 | `8da8f0ea2d4d87638b9c54ffa90741ddb5e48b443fe9e2918f21d0a8e4608dca` |
| 4 | `BAAAAAAAAAAAAAAA` | 16 | `602205ea09fd3c607fb4e3ff23181dddc4719b448918bb91b24714a812b7b7ca` |
| 5 | `0000000000000000` | 16 | `616f3a1b2cb89711d1f734ff747964dd52f28944e1fb259126f21e595fe8ca4a` |
| 6 | `1111111111111111` | 16 | `b2a23d9a9dc37c10fea043fffab0cddde00f7b443f4a8b915457dcd8588a3fca` |
 
**Rezultatas:** visos 6 maišos skirtingos, struktūrinių kolizijų nėra. Pastaba: `BচুBচু...` yra 35 baitų, nes kiekvienas bengalų simbolis UTF-8 užima 3 baitus.
- **Nulis kolizijų yra tikėtinas rezultatas.** Idealiai 256 bitų maišai kolizijos tikimybė tarp 200 000 eilučių yra nepalyginamai maža (kolizijos pasirodo tik apie 2^128 bandymų). Todėl šis testas sunkiai gali aptikti silpną funkciją; jis tik patvirtina, kad nėra akivaizdžių defektų.

### Eksperimentas 6
Lavinos efektui patikrinti parašiau kodą, kuris sugeneruoja 100000 porų, kuris po lygiai paskirsto keturiems prieš tai nurodytiems ilgiams. Gavau štai tokius rezultatus: 
--- 6 EKSPERIMENTAS: LAVINOS EFEKTAS ---
- Viso poru: 100 000 (po 25 000 ilgiams 10, 100, 500, 1000).

Bitų skirtumas
| Ilgis (baitai) | Min (%) | Max (%) | Vidurkis (%) |
|---:|---:|---:|---:|
| 10 | 0.78 | 50.00 | 24.25 |
| 100 | 1.17 | 61.33 | 27.90 |
| 500 | 1.56 | 64.06 | 28.34 |
| 1000 | 1.17 | 59.38 | 27.86 |
| **Bendrai (100 000 porų)** | **0.78** | **64.06** | **27.09** |

Hex simbolių skirtumas
| Ilgis (baitai) | Min (%) | Max (%) | Vidurkis (%) |
|---:|---:|---:|---:|
| 10 | 3.12 | 81.25 | 47.48 |
| 100 | 4.69 | 100.00 | 53.93 |
| 500 | 6.25 | 100.00 | 54.67 |
| 1000 | 3.12 | 100.00 | 53.86 |
| **Bendrai (100 000 porų)** | **3.12** | **100.00** | **52.48** |

Palyginimas su maišos funkcija
| Metrika | Idealus vidurkis | Gautas vidurkis | Skirtumas |
|---|---:|---:|---:|
| Bitų skirtumas | ~50 % | 27.09 % | ~23 procentiniai punktai mažiau |
| Hex skirtumas | ~93.75 % | 52.48 % | ~41 procentinis punktas mažiau |

![Histograma](image.png)
- Idealios maišos atveju matytume vieną siaurą varpą aplink 128 bitus, o čia pasiskirstymas platus, apimantis ~2–164 bitus, o vidurkis tik ~69 bitai (27,09 %).

- **Lavinos efektas silpnas.** Pakeitus vieną simbolį, vidutiniškai pasikeičia tik ~27 % bitų, o ne ~50 %.
- **Minimumas labai mažas:** 0.78 % = 2 bitai iš 256, o 3.12 % = 2 hex simboliai iš 64. Vadinasi, kai kurių vieno simbolio pokyčių poveikis beveik nesimato.
- **Trumpos įvestys (10 baitų) blogesnės** (24.25 %) nei ilgesnės (~28 %). Ilgesnėse įvestyse pokytis turi daugiau blokų ir raundų išsisklaidyti.


### Eksperimentas 7
| Rodiklis | [1] Be druskos | [2] Su vieša druska |
|---|---|---|
| Hashinamas tekstas | `7391` | `7391X9qP` |
| Užpuolikui pateikta maiša | `1096092ba9e1dc21779e4aff8acff8dd51ad55449330e1315b0dcbf8812ddddb` | `30276f2bb3801821a8c380ff283de0dd6ebf9544dc61a751b68d699858dc30eb` |
| Rasta įvestis | `7391` | `7391` |
| Atlikta bandymų | 7392 | 7392 |
| Veikimo laikas (µs) | 19 794 | 17 782 |
| Laikas vienam bandymui (µs) | ~2.68 | ~2.41 |

- **Druska neapsaugo nuo šios atakos.** Abiem atvejais slaptažodis rastas po tiek pat bandymų (7392, nes `7391` yra 7392-as kandidatas nuo `0000`). Užpuolikas žino druską, todėl tiesiog prijungia ją prie kiekvieno kandidato.
- **Laikų skirtumas (~10 %) nėra druskos efektas.** Abi įvestys telpa į vieną 32 baitų bloką (4 ir 8 baitai), todėl maišos skaičiavimo kaina vienoda. Greičiausiai pirmasis ciklas lėtesnis dėl "šalto starto" (procesoriaus talpyklos, dinaminės atminties paskirstymas pirmą kartą), o ne dėl algoritmo.
- **Visas perrinkimas užtrunka ~20–25 ms** (10 000 × ~2,5 µs), t. y. maža kandidatų erdvė (4 skaitmenys) visiškai neatspari brute force atakai, nepriklausomai nuo maišos funkcijos.
- **Druska turi prasmę kitose situacijose:** prieš iš anksto paskaičiuotas lenteles (rainbow tables) ir kad vienodi slaptažodžiai turėtų skirtingas maišas. Jai nereikia būti slaptai, bet ji negali kompensuoti per mažos slaptažodžio erdvės.


# Šaltiniai
- https://dev.to/alen_pythonista_bb/binary-file-handling-in-c-a-beginners-guide-148o
- https://www.youtube.com/watch?v=gTfNtop9vzM
- https://youtu.be/2BldESGZKB8?si=tCAiYJbK9HOVaoQJ
- https://www.youtube.com/watch?v=kF_h9gl-vyw
- https://rosettacode.org/wiki/MD5/Implementation#C++
- https://www.geeksforgeeks.org/cpp/left-shift-right-shift-operators-c-cpp/
- https://www.researchgate.net/publication/400346320_A_Step-by-Step_Explanation_of_Hash_Functions_From_Naive_Digestion_to_SHA-2