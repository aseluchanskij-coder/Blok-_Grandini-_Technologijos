# UTXO modelis

Prieš pradedant kurti UTXO modelį, nuspręsta buvo sukurti naują objektą, kuris turės ID, indekso numerį, savininką ir sumą.
Nuo to ir pradėjau pradedant rašyti kodą.

Visų pirma teko pašalinti seną struktūrą Transakcijos, ir ja pakeisti naujomis, t. y. įvestis, išvestis ir transakcijos tačiau jau be siuntėjo, gavėjo, sumos. Nes mes imsime senų įvesčių ir keisime naujomis išvestimis.
Pakeitus struktūras kode atsirado daug vietų kurias reikia pakeisti nes naudosime naujus kintamuosius.