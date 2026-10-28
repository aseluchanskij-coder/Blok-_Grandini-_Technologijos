# Blockchain v0.1

## Supaprastintos blokų grandinės kūrimas

### Pasiruošimas

Visų pirma reikėjo išsirinkti kurią iš mūsų dviejų maišos funkcijų naudoti. Nusprendėme, kad labiau mums tiktų naudoti Alano sukurtą maišos funkciją (ji buvo kuriama naudojant AI), kadangi šiai funkcijai nereikėtų korekcijų, kadangi 3 eksperimente 'difficulty' antraštės maišos hex užrašas turi prasidėti 3 nuliais.

### Kodo paruošimas

Kad būtų galimybė kurti vartotojus reikėjo pasirinkti kokio tipo objektus kursime. Pasirinkome 'struct' metodą, kadangi jis lengvesnis ir paprastesnis realizacijai, o 'class' metodas yra sudėtingesnis realizuoti. Tačiau ne tai lėmė mūsų pasirinkimą, o tai, kad mums nerūpi koks bus duomenų pasiekiamumo lygis (private/public), nes duomenims nereikės apsaugos nuo savęs pačių nes yra sakykime "paprasti".

Tad sukūrus 3 struktūras, teko papildyti kodą 3 papildomomis funkcijomis:
1. Custom_hashas_string (kadangi mano prieme uint8, tai pridėjau kuri ir string priima).
2. 
3. 


## Paleidimas terminale

g++ -std=c++11 main.cpp -o blockchain_v01
./blockchain_v01

# PILDOMA...