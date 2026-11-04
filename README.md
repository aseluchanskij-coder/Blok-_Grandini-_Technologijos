# Blockchain v0.1

## Supaprastintos blokų grandinės kūrimas

### Pasiruošimas

Visų pirma reikėjo išsirinkti kurią iš mūsų dviejų maišos funkcijų naudoti. Nusprendėme, kad labiau mums tiktų naudoti Alano sukurtą maišos funkciją (ji buvo kuriama naudojant AI), kadangi šiai funkcijai nereikėtų korekcijų, kadangi 3 eksperimente 'difficulty' antraštės maišos hex užrašas turi prasidėti 3 nuliais.

### Kodo paruošimas

Kad būtų galimybė kurti vartotojus reikėjo pasirinkti kokio tipo objektus kursime. Pasirinkome 'struct' metodą, kadangi jis lengvesnis ir paprastesnis realizacijai, o 'class' metodas yra sudėtingesnis realizuoti. Tačiau ne tai lėmė mūsų pasirinkimą, o tai, kad mums nerūpi koks bus duomenų pasiekiamumo lygis (private/public), nes duomenims nereikės apsaugos nuo savęs pačių nes yra sakykime "paprasti".

Vartotojų būsenos lieka pastovios viso programos veikimo metu, kadangi naudojamas tas pats seed generavimui.
Tad sukūrus 3 struktūras, teko papildyti kodą 3 papildomomis funkcijomis:
1. Custom_hashas_string (kadangi mano prieme uint8, tai pridėjau kuri ir string priima).
2. serializuotiTransakcija (paverčia transakcijos struktūros laukus į vientisą tekstą, atskirtą "|", kad būtų galima apskaičiuoti transakcijos ID/maišą).
3. serializuotiAntraste (paverčia bloko antraštės duomenis į vientisą tekstą, kuris vėliau naudojamas skaičiuojant bloko maišą ir ieškant PoW).

### Genesis Blokas

"praeitas_hash" sukūrėme su 64 nuliais. Nustatome pradinį laiką ir kvieciame maišos funkciją su tusčia įvestimi.
Genesis blokas yra PoW išimtis (pagal užduoties sąlyga), todėl maišą priskiriame be kasimo ciklo.

### Transakcijų atranka ir tikrinimas

Prieš formuojant bloką, sukuriama laikina balansų kopija, kad kandidato kūrimas nekeistų tikrųjų balansų[cite: 2]. Iš transakcijų fondo ciklo pagalba atrenkama iki 100 transakcijų[cite: 2]. Kiekvienai transakcijai tikrinama, ar siuntėjas turi pakankamai lėšų (naudojant laikinąją balansų map kopiją). Jei lėšų užtenka, jos atimamos iš siuntėjo ir pridedamos gavėjui, o transakcija įtraukiama į bloką. Jei lėšų trūksta, atmetamų transakcijų skaitiklis padidinamas. Šioje v0.1 versijoje bloko `tx_root` skaičiuojamas pagal bloko tvarką sujungus įtrauktų transakcijų ID į vieną bendrą teksto eilutę ir iš jos išvedus maišą[cite: 2].

### PoW

Darbo įrodymas (PoW) ieškomas pasitelkiant nesibaigiantį `while (true)` ciklą. Bloko antraštė yra serializuojama ir apskaičiuojama jos maiša. Tada tikrinama sąlyga – ar gautos maišos hex užrašas prasideda trimis nuliais ("000")[cite: 3]. Jeigu maiša netenkina sąlygos, bloko antraštės `nonce` (paieškos skaitiklio) reikšmė padidinama vienetu ir maiša skaičiuojama iš naujo[cite: 2]. Kai tinkama maiša pagaliau randama, ciklas nutraukiamas, blokas išsaugomas prie grandinės, o tikrieji balansai atnaujinami pritaikant laikinojoje kopijoje padarytus pakeitimus[cite: 2].

## Paleidimas terminale

g++ -std=c++11 main.cpp -o blockchain_v01
./blockchain_v01