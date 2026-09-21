Pradines busena sukuriu skirstant 256 bitu i keturis gabalus po 64, pradinė bubena uzpildidamais tribonacio sekos skaiciais.
Issiaiskinau kad tribonaci sekos skaičiai ženkliai skiriasi pvz 70 ir 74 narys, teko sugalvoti apribojime kad juos galima butu sutalpinti i konteinerius ir parasyti sesioliktainiu formatu. Todel pasirinkau 70,71,72,73 sekos narius, ju atitikmenys pavaizduoti zemiau.
0xD6D12E7B5A03A401ULL = 15479308092729377793 
0x8A59B51F41029312ULL = 9969146807112667922 
0x32A398246E20349AULL = 3648012790626366618 
0x7158932402138901ULL = 8167402682752305409
    // Naudojame ULL (Unsigned Long Long), kad kompiliatorius tiksliai žinotų, jog tai 64 bitų skaičiai

Toliau medaus daryti pirmini pildyma (padding). Jis reikalingas tam kad nesvarbu ka mes ivesin viena raide, nieko, ar daug raidziu, musu blokas dalintusi is 32 (nes musu mano versijos blokai bus po 32 baitus). Visa likusia vieta kuria atskirsime pabaiga bus pripildyta pirminiais skaiciais 2,3,5,7,11 vietoj standartinio nuliu. Pripildome masyve is hard coded saraso (kadangi neapsimoka tikrinti ar skaičius pirminis kai ju pripildymas niekaip nevirsiu 32 baitu.
Kaip veikia pading code: // 2 ŽINGSNIS: Pirminis pildymas (Padding)

toliau readme dar pildomas ir bus graziai sustrukturuotas.....