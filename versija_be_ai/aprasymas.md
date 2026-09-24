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
- Hashas su vieno baito ivestimi 'a': 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2
- Hashas su vieno baito ivestimi 'b': 6c27592be2e15b21a83a25ffa3dbb6dddb104c4439700d1998a27ee16b617fc2
- Hashas su >1000 baitu ascii teksto ivestimi: 43727c100990f2df2d420aff3ffe73a624fc1b7ed73df0aa64165c89ced1e284
- Hashas su >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas: 3e7e2716692d78df94b928ff2d90b4a60c95d97853a488a430575487d8c5558a
- Hashas su kita >1000 baitu ascii teksto ivestimi: 081d449e5d10d180dc87624f2d9b9b752e4bcec241fcafb600f84541edc2fd03
- Hashas su kita >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas: d3ecad9e25d6718090b30dc55cfa5b75b953bdc22c30d98e3cbf7c797f2c0d3b
- Hashas su dar kita >1000 baitu ascii teksto ivestimi: 09614641a1468231760b18586856d2bdf6880b6ee9d5df78600cb2418868bd46
- Hashas su dar kita >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas: 8ed2824103688db11d0f295bcdb1a3be005ec26d2da8a07bafcfa0426c859e45
- Hashas su strukturuota pasikartojancios raides ivestimi (AAA...AAA): c742492b1db3c32101f11dff5af416dd235e8444859e3d116b2736e92e33b7ca
- Hashas su strukturuota ivestimi su '/n' simboliu (ABC / abc) a2a9452b557c862121c341ffa9b915dd7332ac443fb1ca29c4983fdb046ec178
- Hashas su ne ascii simboliu ivestimi: d25a383144332efb3492d85ecae4e149dee65b1da825576b84c9b54db1df7e72

### Eksperimentas 2
Hashas su vieno baito ivestimi 'a' is failo: 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2
Hashas su vieno baito ivestimi 'a' per terminala: 

Įveskite tekstą: a
2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2

- Mano maiša yra 256 bitų ilgio, 256 / 4 yra 64. Gautas hash yra būtent 64 simbolių ilgio, hex formatu.

### Eksperimentas 3
A, B, A Testas
1 (A): 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2
2 (B): 6c27592be2e15b21a83a25ffa3dbb6dddb104c4439700d1998a27ee16b617fc2
3 (A): 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2

Paleidžiant programą iš naujo:
Pirma kartą iš failas1.txt: 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2
Antra kartą iš failas1.txt: 2227592bcce15b21ca3a25ffd1dbb6ddfd104c4463700d1927a27ee18c617fc2
- Atlikus A-B-A sekos testą vienos sesijos metu bei testuojant programą atskirais paleidimais, rezultatų neatitikimų nebuvo rasta. Hash reikšmės tam pačiam failui visada sutapo. Taip yra todėl, nes funkcija hash() kiekvieno iškvietimo metu iš naujo inicializuoja pradinę būseną (state masyvą) bei sukuria naują, tuščią text vektorių, todėl atmintyje nelieka jokių praėjusių skaičiavimų pėdsakų.

### Eksperimentas 4
Failas nuskaitytas. Viso eilučių: 789

Eilutės Baitai  Min(ms) Max(ms) Vidurkis(ms)
1       70      0.000597144     0.000861559     0.00071298
2       123     0.000755823     0.000810019     0.000776666
4       205     0.000615249     0.000890526     0.000707684
8       362     0.000885155     0.00092812      0.000898904
16      996     0.00205203      0.00211661      0.00207924
32      1841    0.00358121      0.00362758      0.00359847
64      3712    0.00701232      0.00707894      0.0070491
128     9155    0.0169211       0.0169606       0.0169403
256     20409   0.0378242       0.0381168       0.0379359
512     47434   0.0860224       0.0886161       0.0869255

- Tiesinė priklausomybė (Algoritmo sudėtingumas $O(n)$):
    Duomenų apdorojimo laikas auga tiesiogiai proporcingai įvesties dydžiui. Tai ypač gerai matoma nuo 16 eilučių (~1 KB) ribos. Pavyzdžiui, failo dydžiui padidėjus maždaug dvigubai (nuo 1841 B iki 3712 B), vidutinis vykdymo laikas taip pat padidėja beveik lygiai dvigubai (nuo 0.0036 ms iki 0.0070 ms). Tai įrodo, kad algoritmas teisingai apdoroja duomenis nuosekliais blokais (Merkle-Damgård struktūra) – kuo daugiau blokų, tuo ilgiau užtrunka ciklas, bet apdorojimo laikas vienam blokui išlieka pastovus.
- Anomalija prie mažų įvesčių (Bazinės išlaidos):
    Matuojant labai mažas ištraukas (1, 2 ir 4 eilutes, atitinkamai 70 B, 123 B ir 205 B), vykdymo laikas beveik nesiskiria ir svyruoja apie 0.0007 ms. Matoma netgi nedidelė anomalija, kai 205 baitų apdorojimas (0.000707 ms) užtruko vos vos trumpiau nei 123 baitų (0.000776 ms).
    Paaiškinimas: Kai duomenų kiekis labai mažas, paties maišymo (matematikos) laikas yra toks trumpas, kad jį užgožia bazinės programos išlaidos (overhead) – atminties išskyrimas vektoriui (std::vector inicializacija), funkcijų iškvietimai ir pradinės būsenos užkrovimas. Be to, dėl padding funkcijos pridedamų nulių, labai trumpi tekstai vis tiek yra išplečiami iki to paties ar panašaus blokų skaičiaus (pvz., tiek 70, tiek 123 baitai galiausiai sudaro nedidelį skaičių pilnų 256 bitų blokų), todėl ir laikas išlieka identiškas.
- Algoritmo stabilumas:
    Skirtumas tarp minimalaus (Min) ir maksimalaus (Max) vykdymo laiko kiekvienoje eilutėje yra labai mažas (dažniausiai skiriasi tik dešimtatūkstantosiomis milisekundės dalimis). Tai rodo, kad:
        - Matavimai atlikti teisingai (pakankamas iteracijų kiekis neutralizavo operacinės sistemos foninių procesų triukšmą).
        - Algoritmo veikimas yra stabilus ir neturi jokių neplanuotų bottlenecks, atsirandančių dėl atminties fragmentacijos.

### Eksperimentas 5
Eksperimento metu sugeneravome po 100 000 atsitiktinių ASCII eilučių porų keturiems ilgiams: 10, 100, 500 ir 1000 baitų (naudotas std::mt19937 generatorius su seed = 2026). Kolizijų ieškojome trimis etapais.

--- 5 EKSPERIMENTAS: KOLIZIJU PAIESKA ---
Naudojamas seed: 2026. Abecele: ASCII (32-126).

Generuojama 100,000 poru ilgiui: 10 baitu...
Porose rastu koliziju: 0
Visame rinkinyje rastu koliziju (skirtingoms ivestims): 0
Skirtingu ivesciu grupiu skaicius: 200000

Generuojama 100,000 poru ilgiui: 100 baitu...
Porose rastu koliziju: 0
Visame rinkinyje rastu koliziju (skirtingoms ivestims): 0
Skirtingu ivesciu grupiu skaicius: 200000

Generuojama 100,000 poru ilgiui: 500 baitu...
Porose rastu koliziju: 0
Visame rinkinyje rastu koliziju (skirtingoms ivestims): 0
Skirtingu ivesciu grupiu skaicius: 200000

Generuojama 100,000 poru ilgiui: 1000 baitu...
Porose rastu koliziju: 0
Visame rinkinyje rastu koliziju (skirtingoms ivestims): 0
Skirtingu ivesciu grupiu skaicius: 200000

--- STRUKTURUOTI ATVEJAI ---
Tekstas: ABABABABABABABAB | Hash: d9286c6990395263757ea4ff8d5044ddc28533448cf5b1916a14862b82d5c34a
Tekstas: BচুBচুBচুBচুBচু | Hash: 11a76b589635bf7b123147c5a286298b0c9975e9152fc09052d88c5b1758c31a
Tekstas: AAAAAAAAAAAAAAAB | Hash: 8da8f0ea2d4d87638b9c54ffa90741ddb5e48b443fe9e2918f21d0a8e4608dca
Tekstas: BAAAAAAAAAAAAAAA | Hash: 602205ea09fd3c607fb4e3ff23181dddc4719b448918bb91b24714a812b7b7ca
Tekstas: 0000000000000000 | Hash: 616f3a1b2cb89711d1f734ff747964dd52f28944e1fb259126f21e595fe8ca4a
Tekstas: 1111111111111111 | Hash: b2a23d9a9dc37c10fea043fffab0cddde00f7b443f4a8b915457dcd8588a3fca
Strukturiniu koliziju rasta: 0
Rezultatai rodo, kad kolizijų nebuvo aptikta. Kaip minėta paskaitose, kolizijos tikimybė yra itin maža. Remiantis užduoties sąlygoje pateiktomis formulėmis, vienos konkrečios poros kolizijos tikimybė yra vos $2^{-256}$.Eksperimento metu vienam lygiui buvo sugeneruota 200 000 skirtingų įvesčių. Iš šio kiekio galima sudaryti maždaug $2 \times 10^{10}$ (20 milijardų) unikalių porų. Nors 20 milijardų atrodo didelis skaičius, bendras galimų maišos (hash) reikšmių kiekis yra $2^{256}$. Lyginant su šia neįsivaizduojamai didele aibe, mūsų sugeneruotas porų skaičius yra pernelyg mažas, todėl tikimybė eksperimento metu rasti koliziją išlieka praktiškai nulinė.

### Eksperimentas 6
Lavinos efektui patikrinti parašiau kodą, kuris sugeneruoja 100000 porų, kuris po lygiai paskirsto keturiems prieš tai nurodytiems ilgiams. Gavau štai tokius rezultatus: 
--- 6 EKSPERIMENTAS: LAVINOS EFEKTAS ---
Viso poru: 100 000 (po 25 000 ilgiams 10, 100, 500, 1000).

Ilgis: 10 baitu
  Bitu skirtumas (%): Min = 0.78%, Max = 50.00%, Vidurkis = 24.25%
  Hex skirtumas (%):  Min = 3.12%, Max = 81.25%, Vidurkis = 47.48%

Ilgis: 100 baitu
  Bitu skirtumas (%): Min = 1.17%, Max = 61.33%, Vidurkis = 27.90%
  Hex skirtumas (%):  Min = 4.69%, Max = 100.00%, Vidurkis = 53.93%

Ilgis: 500 baitu
  Bitu skirtumas (%): Min = 1.56%, Max = 64.06%, Vidurkis = 28.34%
  Hex skirtumas (%):  Min = 6.25%, Max = 100.00%, Vidurkis = 54.67%

Ilgis: 1000 baitu
  Bitu skirtumas (%): Min = 1.17%, Max = 59.38%, Vidurkis = 27.86%
  Hex skirtumas (%):  Min = 3.12%, Max = 100.00%, Vidurkis = 53.86%

--- BENDRI REZULTATAI (100 000 poru) ---
Bitu skirtumas (%): Min = 0.78%, Max = 64.06%, Vidurkis = 27.09%
Hex skirtumas (%):  Min = 3.12%, Max = 100.00%, Vidurkis = 52.48%

--- HISTOGRAMOS DUOMENYS (Bitu skirtumas) ---
Besiskiriantys_bitai,Poru_skaicius
2,1
3,8
4,16
5,34
6,32
7,61
8,68
9,99
10,102
11,90
12,164
13,200
14,211
15,251
16,310
17,384
18,454
19,528
20,629
21,763
22,914
23,1119
24,1234
25,1467
26,1583
27,1710
28,1779
29,1792
30,1788
31,1654
32,1563
33,1343
34,1113
35,903
36,713
37,552
38,380
39,285
40,225
41,194
42,171
43,187
44,184
45,254
46,280
47,319
48,419
49,452
50,563
51,603
52,821
53,812
54,976
55,1112
56,1234
57,1283
58,1316
59,1462
60,1489
61,1443
62,1455
63,1390
64,1277
65,1184
66,1029
67,951
68,803
69,677
70,568
71,528
72,440
73,343
74,367
75,363
76,339
77,375
78,385
79,487
80,504
81,608
82,653
83,805
84,845
85,911
86,1075
87,1142
88,1293
89,1368
90,1438
91,1490
92,1597
93,1674
94,1666
95,1674
96,1652
97,1636
98,1542
99,1601
100,1491
101,1406
102,1306
103,1247
104,1162
105,1087
106,961
107,891
108,860
109,780
110,787
111,722
112,656
113,626
114,530
115,483
116,475
117,448
118,369
119,356
120,324
121,302
122,287
123,291
124,229
125,210
126,189
127,167
128,150
129,129
130,103
131,101
132,87
133,74
134,80
135,62
136,60
137,40
138,36
139,35
140,22
141,25
142,21
143,18
144,22
145,8
146,11
147,8
148,5
149,8
150,2
151,5
152,5
153,4
154,1
155,2
156,1
157,1
164,1

Išanalizavus rezultatus matyti, kad algoritmas neužtikrina pakankamo bitų sklaidos (lavinos) efekto. Remiantis standartinėmis maišos (hash) funkcijomis, pakeitus įvestį, vidutiniškai turėtų pasikeisti apie 50 % maišos bitų, tačiau mano realizacijoje šis pokytis siekia tik apie 27 %. Dėl šios priežasties atitinkamai per mažas yra ir šešioliktainių (HEX) simbolių skirtumas. Idealiu atveju besiskiriančių HEX simbolių proporcija turėtų siekti apie 97 %, bet šiuo atveju ji tesiekė 50 %. Detalesnė rezultatų analizė bei vizuali duomenų reprezentacija (histograma) bus pateikta vėliau, papildžius aprasymas.md failą.

### Eksperimentas 7
--- 7 EKSPERIMENTAS: SPEJIMAS ---

Kandidatu erdve: 0000 - 9999 (10,000 variantu)

[1] ATAKA BE DRUSKOS
Uzpuolikui pateikta maisa: 1096092ba9e1dc21779e4aff8acff8dd51ad55449330e1315b0dcbf8812ddddb
Rasta ivestis: 7391
Atlikta bandymu: 7392
Veikimo laikas: 19794 mikrosekundziu.

[2] ATAKA SU VIESA DRUSKA
Zinoma druska (salt): 'X9qP'
Uzpuolikui pateikta maisa: 30276f2bb3801821a8c380ff283de0dd6ebf9544dc61a751b68d699858dc30eb
Rasta ivestis: 7391
Atlikta bandymu: 7392
Veikimo laikas: 17782 mikrosekundziu.

Eksperimento rezultatai rodo, kad nulaužti 4 skaitmenų PIN kodą abiem atvejais užtruko labai panašiai – apie 18–20 milisekundžių. Nežymus laiko skirtumas (19.7 ms vs 17.7 ms) yra atsitiktinis ir priklauso nuo operacinės sistemos foninių procesų, o ne nuo druskos naudojimo. Tai patvirtina, kad vykdant tiesioginės paieškos (angl. *brute-force*) ataką prieš vieną konkretų taikinį, **viešos druskos pridėjimas paties spėliojimo nepasunkina** – užpuolikui vis tiek tenka patikrinti tuos pačius 10 000 variantų.

Nors bendruoju atveju rasta sutampanti maiša (hash) nebūtinai garantuoja, kad rasta originali įvestis (dėl teorinės kolizijų galimybės), 256 bitų maišai ir tokiai mažai paieškos erdvei kolizijos tikimybė yra praktiškai nulinė. Todėl šiuo atveju rastas atitikmuo neabejotinai yra tikrasis PIN kodas.

Nors vieša druska nesulėtina tiesioginės atakos, ji yra **esminė apsauga nuo iš anksto apskaičiuotų lentelių** (angl. *Rainbow tables*). Be druskos užpuolikas galėtų vieną kartą suskaičiuoti visų 10 000 PIN kodų maišas ir jas naudoti tūkstančiams nulaužtų paskyrų vienu metu. Su unikalia vieša druska šias lenteles tenka generuoti iš naujo kiekvienam vartotojui atskirai.

# Šaltiniai
- https://dev.to/alen_pythonista_bb/binary-file-handling-in-c-a-beginners-guide-148o
- https://www.youtube.com/watch?v=gTfNtop9vzM
- https://youtu.be/2BldESGZKB8?si=tCAiYJbK9HOVaoQJ
- https://www.youtube.com/watch?v=kF_h9gl-vyw
- https://rosettacode.org/wiki/MD5/Implementation#C++
- https://www.geeksforgeeks.org/cpp/left-shift-right-shift-operators-c-cpp/
- https://www.researchgate.net/publication/400346320_A_Step-by-Step_Explanation_of_Hash_Functions_From_Naive_Digestion_to_SHA-2