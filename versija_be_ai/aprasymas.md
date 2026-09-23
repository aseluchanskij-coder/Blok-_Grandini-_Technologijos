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

# Šaltiniai
- https://dev.to/alen_pythonista_bb/binary-file-handling-in-c-a-beginners-guide-148o
- https://www.youtube.com/watch?v=gTfNtop9vzM
- https://youtu.be/2BldESGZKB8?si=tCAiYJbK9HOVaoQJ
- https://www.youtube.com/watch?v=kF_h9gl-vyw
- https://rosettacode.org/wiki/MD5/Implementation#C++
- https://www.geeksforgeeks.org/cpp/left-shift-right-shift-operators-c-cpp/
- https://www.researchgate.net/publication/400346320_A_Step-by-Step_Explanation_of_Hash_Functions_From_Naive_Digestion_to_SHA-2