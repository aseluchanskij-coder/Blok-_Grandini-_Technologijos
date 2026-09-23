Pradines busena sukuriu skirstant 256 bitu i keturis gabalus po 64, pradinė bubena uzpildidamais tribonacio sekos skaiciais. Issiaiskinau kad tribonaci sekos skaičiai ženkliai skiriasi pvz 70 ir 74 narys, teko sugalvoti apribojime kad juos galima butu sutalpinti i konteinerius ir parasyti sesioliktainiu formatu. Todel pasirinkau 70,71,72,73 sekos narius, ju atitikmenys pavaizduoti zemiau. 0xD6D12E7B5A03A401ULL = 15479308092729377793 0x8A59B51F41029312ULL = 9969146807112667922 0x32A398246E20349AULL = 3648012790626366618 0x7158932402138901ULL = 8167402682752305409 // Naudojame ULL (Unsigned Long Long), kad kompiliatorius tiksliai žinotų, jog tai 64 bitų skaičiai

Toliau medaus daryti pirmini pildyma (padding). Jis reikalingas tam kad nesvarbu ka mes ivesin viena raide, nieko, ar daug raidziu, musu blokas dalintusi is 32 (nes musu mano versijos blokai bus po 32 baitus). Visa likusia vieta kuria atskirsime pabaiga bus pripildyta pirminiais skaiciais 2,3,5,7,11 vietoj standartinio nuliu. Pripildome masyve is hard coded saraso (kadangi neapsimoka tikrinti ar skaičius pirminis kai ju pripildymas niekaip nevirsiu 32 baitu).

Pirmojoje lentelėje buvo patikrinti hash su skirtingu failu ivestimi:
| Failo tipas                                                                        | Gautas hash                                                      |
|------------------------------------------------------------------------------------|------------------------------------------------------------------|
| Hashas su vieno baito ivestimi 'a':                                                | d417829f4b73db6cfce0f37fa44cad1163fa1376935903c0ff668df9701541c7 |
| Hashas su vieno baito ivestimi 'b':                                                | 6e26a9a388d4fab2e3810326cf6656ed88e4f920d807b06157f62e0fb38bdeb3 |
| Hashas su >1000 baitu ascii teksto ivestimi:                                       | 5aa4473a828c5d106ce2e6873fed63cb070d9de24253dc07940b93a753d42cce |
| Hashas su >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas:          | 5054f047bf43a80966ca6107dffaeb1b0c4cdf22f291d4e038c7184d08dbe69d |
| Hashas su kita >1000 baitu ascii teksto ivestimi:                                  | 5c81812f1de0ea83e32fff9b77ec1a6f343cef82dbec1bd08eb7439b35ce3502 |
| Hashas su kita >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas:     | e737e630a4ab1534369bb517e8eef389d4b567ff87e8e0deec80cec0d7ee9d56 |
| Hashas su dar kita >1000 baitu ascii teksto ivestimi:                              | 73812a02e20f44b0096a4b62fb9a216fde4a45a085263e5a5118d3136d0c3f42 |
| Hashas su dar kita >1000 baitu ascii teksto ivestimi, kai pakeistas vienas baitas: | 44ea090362c7ea28e4ae0dc35269dd9d94cc9d4d9cfeea5d05c12736d6ba0b8c |
| Hashas su strukturuota pasikartojancios raides ivestimi (AAA...AAA):               | 822b45ab8494346c88c8ee6ca11d191bd27ce05c145dc9abd380e847918e1f29 |
| Hashas su strukturuota ivestimi su '/n' simboliu (ABC / abc)                       | cc15da605c19a348650b9f538df4723e6b55a1c689a25306667b0e6f2b4f90e6 |
| Hashas su ne ascii simboliu ivestimi:                                              | 32e7f014bda6619ab29ea388f6a7198de3882818f56f7a656f412f189599afe7 |

Išvados: vienodu hash nėra, iš to galime spręsti, jog skaitymas vyksta taisyklingai.
