#include "maisos_fjos.h"
#include "pagalb_fjos.h"
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <bit> // dėl std::popcount()
#include <map>

struct StrPora
{
    std::string str1;
    std::string str2;
};

int main()
{
    istatyt_utf8(); // windows aplinkos terminalui padaro UTF-8 koduotę (Linux ir macOS ją ir taip naudoja)

    std::cout << "Maišos generavimo programa.\nNorėdami baigti programą, spauskite Ctrl+C.\n\n";
    for (;;)
    {
        std::string eiga;
        std::cout << "\nPasirinkite programos eigą:\n1 - Rankinis teksto įvedimas\n2 - Failo turinio įvestis\n3 - Efektyvumo matavimas (su failu)\n4 - Kolizijos tyrimas\n5 - Lavinos efekto tyrimas\n6 - Spėjimo tyrimas\nPasirinkimas: ";
        if (!std::getline(std::cin, eiga))
        {
            std::cout << "\n\nPrograma baigiama...";
            break;
        }
        if (eiga != "1" && eiga != "2" && eiga != "3" && eiga != "4" && eiga != "5" && eiga != "6")
        {
            std::cout << "Tokio pasirinkimo nėra.\n";
            continue;
        }

        // vvvvv ĮVESTIES RUOŠIMAS vvvvv

        std::string investis_str;
        if (eiga == "1")
        {
            std::cout << "\nPasirinktas teksto įvesties būdas: RANKINIS\n\nĮveskite tekstą:\n";
            if (!std::getline(std::cin, investis_str))
            {
                std::cout << "\n\nPrograma baigiama...";
                break;
            }
            if (investis_str.length() == 0)
                continue;
        }
        else if (eiga == "2" || eiga == "3") // 2 - failo įvestis arba 3 - matavimai (vis tiek failo reik)
        {
            std::cout << "\nPasirinktas teksto įvesties būdas: FAILAS\n\nPasirinkite failą pasirinkimo lange...\n";
            std::string failo_kelias;
            try
            {
                failo_kelias = failo_pasirinkimas(); // failo_pasirinkimas() meta runtime_error, jeigu nepavyko failo pasirinkt
                std::ifstream failas(failo_kelias);
                if (!failas.is_open())
                {
                    throw std::runtime_error("Nepavyko nuskaityti failo.");
                }
                std::cout << "Pasirinktas failas: " << failo_kelias << "\n\n";
                std::stringstream buferis;
                buferis << failas.rdbuf();
                investis_str = buferis.str();
                if (investis_str.size() == 0)
                {
                    throw std::runtime_error("Pasirinktas failas yra tuščias.");
                }
                failas.close();
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << '\n';
                continue;
            }
        }

        // spausd_baitais(investis); // JEIGU DIDELIS FAILAS, TAI ŠAKĖS
        std::cout << '\n';

        // vvvvv DOROJIMAS vvvvv

        if (eiga == "3")
        {
            // rast eilučių galus ('\n' pozicijas/indeksus)
            std::vector<size_t> eiluciu_galai; // eiluciuGalai[k] parodys, kiek baitų sudaro pirmos k eilučių (0..k). Realiai tai eiluciuGalai[k] laikys eilutės k+1 pradinio baito INDEKSĄ.
            size_t pos = 0;
            while (pos < investis_str.size())
            {
                size_t nauj_eil = investis_str.find('\n', pos); // ras indeksą artimiausio simbolio '\n', ieškant nuo indekso pos
                if (nauj_eil == std::string::npos)              // jeigu simbolio \n neberanda (== nebėr daugiau eilučių), tai .find() grąžina std::string::npos
                {
                    eiluciu_galai.push_back(investis_str.size());
                    break;
                }
                eiluciu_galai.push_back(nauj_eil + 1); // '\n' įeina eilutėn, tai eilutės paskutinis simbolis
                pos = nauj_eil + 1;
            }

            // pats algoritmas
            size_t eiliu_sk = eiluciu_galai.size();
            size_t hashuotinu_eiliu_sk = 1; // bus 1, 2, 4, 8, ..., o paskutinėj iteracijoj iš tikro hashuosim ne šį sk. eilių, o kiek faktiškai jų yra (tam atskirt naudosim std::min())
            for (;;)
            {
                size_t hashuojamu_eiliu_sk = std::min(hashuotinu_eiliu_sk, eiliu_sk);
                size_t paskutinio_baito_idx = eiluciu_galai.at(hashuojamu_eiliu_sk - 1); // -1, nes indeksavimas nuo 0 gi
                std::cout << "\nIštraukos eilių sk.: " << std::dec << hashuojamu_eiliu_sk << "\nIštraukos baitų sk.: " << paskutinio_baito_idx + 1 << '\n';
                std::vector<std::uint8_t> eiluciu_gabalas(investis_str.begin(), investis_str.begin() + paskutinio_baito_idx);
                std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(std::move(eiluciu_gabalas));
                auto pr = std::chrono::high_resolution_clock::now();
                std::vector<std::uint8_t> hashas = gaut_256bit_hasha(std::move(blokai)); // efektyviau taip negu daryt atskirą tarpinį kintamąjį baitus_sujungt_po_4(investis) išvesčiai
                auto pab = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> laikas = pab - pr;
                std::cout << "Maišos trukmė (s): " << laikas.count() << "\nMaiša: "; // laikas sekundėm, double tipo; ČIA vis dar bus std::hex, bet tai neveikia laikas.count(), nes tai double!
                spausd_baitais(hashas);

                if (hashuojamu_eiliu_sk == eiliu_sk)
                    break;
                hashuotinu_eiliu_sk *= 2;
            }
        }
        else if (eiga == "1" || eiga == "2")
        {
            std::vector<std::uint8_t> investis(investis_str.begin(), investis_str.end());
            std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(std::move(investis));
            std::vector<std::uint8_t> hashas = gaut_256bit_hasha(std::move(blokai));

            std::cout << "Maiša:\n";
            spausd_baitais(hashas);
            // spausd_raidem(hashas);
        }

        // vvvv KOLIZIJOS TYRIMO KODAS BENDRAI vvvv
        if (eiga == "4")
        {
            // ASCII charai 32-126 (visi kiti yra sisteminiai simboliai ir pan., nepanaudojami)
            const std::string raidynas =
                " !\"#$%&'()*+,-./"
                "0123456789"
                ":;<=>?@"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "[\\]^_`"
                "abcdefghijklmnopqrstuvwxyz"
                "{|}~";

            unsigned int sekla = 123456789;
            std::mt19937_64 generatorius(sekla);
            std::uniform_int_distribution<std::size_t> pasiskirstymas(0, raidynas.size() - 1); // duos tolygiai paskirstytą random indeksą raidyno simboliui parinkt
            std::cout << "\nEilučių generatorius: std::mt19937_64\nSėkla: " << sekla << '\n';

            std::vector<size_t> str_dydziai = {10, 100, 500, 1000};
            std::vector<StrPora> kolizijos;

            // TIKRINIMAS POROMIS

            const size_t poruSk = 100000;
            for (size_t str_dydis : str_dydziai)
            {
                for (size_t i = 0; i < poruSk; ++i)
                {
                    std::string str1 = generuotRandomStr(str_dydis, raidynas, generatorius, pasiskirstymas);
                    std::string str2 = generuotRandomStr(str_dydis, raidynas, generatorius, pasiskirstymas);
                    while (str1 == str2) // beveik neįmanoma, bet atsargai
                    {
                        str2 = generuotRandomStr(str_dydis, raidynas, generatorius, pasiskirstymas);
                    }
                    auto hash1 = str_i_hasha(str1);
                    auto hash2 = str_i_hasha(str2);
                    if (hash1 == hash2)
                    {
                        kolizijos.push_back({str1, str2});
                    }
                }
                std::cout << std::dec << "Tarp " << poruSk << " eilučių (" << str_dydis << " ilgio) porų, kolizijų rasta: " << kolizijos.size() << "\n";
                if (kolizijos.size() > 0)
                {
                    std::cout << "Kolizijos:\n";
                    for (StrPora kolizija : kolizijos)
                    {
                        std::cout << kolizija.str1 << " : " << kolizija.str2 << '\n';
                    }
                }
            }

            // TIKRINIMAS VISŲ SU VISAIS

            kolizijos.clear(); // porų tikrinimo kolizijas jau tikrinom, tai kad nekartot išvalom vektorių
            const size_t str_sk = 100000;
            for (size_t str_dydis : str_dydziai)
            {
                std::vector<std::string> eilutes;
                eilutes.reserve(str_sk);
                // eilučių generavimas
                for (size_t i = 0; i < str_sk; ++i)
                {
                    eilutes.push_back(generuotRandomStr(str_dydis, raidynas, generatorius, pasiskirstymas));
                }
                // kolizijų tikrinimas (visi su visais)
                for (size_t i = 0; i < str_sk; ++i)
                {
                    for (size_t j = 0; i < str_sk; ++i)
                    {
                        if (i != j && eilutes[i] != eilutes[j])
                        {
                            if (str_i_hasha(eilutes[i]) == str_i_hasha(eilutes[j]))
                            {
                                kolizijos.push_back({eilutes[i], eilutes[j]});
                            }
                        }
                    }
                }
                std::cout << "Tarp " << poruSk << " eilučių (" << str_dydis << " ilgio), kolizijų rasta: " << kolizijos.size() << "\n";
                if (kolizijos.size() > 0)
                {
                    std::cout << "Kolizijos:\n";
                    for (StrPora kolizija : kolizijos)
                    {
                        std::cout << kolizija.str1 << " : " << kolizija.str2 << '\n';
                    }
                }
            }

            // struktūruotų įvesčių rinkinio tikrinimas

            std::vector<std::string> ivestys_perstatymas1 = {
                "ABCDEFGH",
                "HGFEDCBA",
                "DCBAHGFE",
                "EFGHABCD",
                "HGFEDCBA",
                "ABEFCDGH",
                "ABGHCDEF",
                "GHEFCDAB",
            };
            std::vector<std::string> ivestys_perstatymas2 = {
                "BAAAAAAA",
                "ABAAAAAA",
                "AABAAAAA",
                "AAABAAAA",
                "AAAABAAA",
                "AAAAABAA",
                "AAAAAABA",
                "AAAAAAAB",
            };
            std::vector<std::string> ivestys_pasikartojimai = {
                "AAAAAAAA",
                "ABABABAB",
                "ABCABCAB",
                "ABCDABCD",
                "11111111",
                "12121212",
                "12312312",
                "12341234",
            };
            std::vector<std::vector<std::string>> tiriami_rinkiniai = {ivestys_pasikartojimai, ivestys_perstatymas1, ivestys_perstatymas2};

            kolizijos.clear();
            for (auto rinkinys : tiriami_rinkiniai)
            {
                for (int i = 0; i < rinkinys.size(); ++i)
                {
                    for (int j = 0; j < rinkinys.size(); ++j)
                    {
                        if (i != j && rinkinys[i] != rinkinys[j])
                        {
                            if (str_i_hasha(rinkinys[i]) == str_i_hasha(rinkinys[j]))
                            {
                                kolizijos.push_back({rinkinys[i], rinkinys[j]});
                            }
                        }
                    }
                }
            }
            std::cout << "Tarp struktūruotų atvejų rinkinių, kolizijų rasta: " << kolizijos.size() << "\n";
            if (kolizijos.size() > 0)
            {
                std::cout << "Kolizijos:\n";
                for (StrPora kolizija : kolizijos)
                {
                    std::cout << kolizija.str1 << " : " << kolizija.str2 << '\n';
                }
            }
        }
        else if (eiga == "5")
        {
            // ASCII charai 32-126 (visi kiti yra sisteminiai simboliai ir pan., nepanaudojami)
            const std::string raidynas =
                " !\"#$%&'()*+,-./"
                "0123456789"
                ":;<=>?@"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "[\\]^_`"
                "abcdefghijklmnopqrstuvwxyz"
                "{|}~";

            unsigned int sekla = 123456789;
            std::mt19937_64 generatorius(sekla);
            std::uniform_int_distribution<std::size_t> raidyno_pasiskirstymas(0, raidynas.size() - 1); // duos tolygiai paskirstytą random indeksą raidyno simboliui parinkt
            std::cout << "\nEilučių generatorius: std::mt19937_64\nSėkla: " << sekla << '\n';

            std::vector<size_t> str_dydziai = {10, 100, 500, 1000};

            unsigned int bendr_hex_skirtumai = 0;
            unsigned int bendr_visi_hex_skaitmenys = 0;
            unsigned int bendr_bitu_skirtumai = 0;
            unsigned int bendr_visi_bitu_skaitmenys = 0;

            const size_t poruSk = 25000; // kiekvienam str_dydziui
            for (size_t str_dydis : str_dydziai)
            {
                std::uniform_int_distribution<size_t> pozicijos_pasiskirstymas(0, str_dydis - 1);
                unsigned int hex_skirtumai = 0;
                unsigned int visi_hex_skaitmenys = 0;
                unsigned int bitu_skirtumai = 0;
                unsigned int visi_bitu_skaitmenys = 0;

                for (size_t i = 0; i < poruSk; ++i)
                {
                    std::string str1 = generuotRandomStr(str_dydis, raidynas, generatorius, raidyno_pasiskirstymas);
                    std::string str2 = str1;
                    size_t pos = pozicijos_pasiskirstymas(generatorius);
                    char sena_raide = str2.at(pos);
                    char nauja_raide;
                    do
                    {
                        nauja_raide = raidynas.at(raidyno_pasiskirstymas(generatorius));
                    } while (nauja_raide == sena_raide);
                    str2.at(pos) = nauja_raide;

                    auto hash1 = str_i_hasha(str1);
                    auto hash2 = str_i_hasha(str2);

                    // hex skirtumas
                    for (size_t i = 0; i < hash1.size(); ++i)
                    {
                        std::uint8_t aukst_hex_skaitmuo_1 = (hash1[i] >> 4) & 0x0F; // 4 aukstesnius bitus pastumiam zemesniu 4 vieton, ir su & 0x0F juosius pasirenkam (1 & x = 1)
                        std::uint8_t zem_hex_skaitmuo_1 = hash1[i] & 0x0F;
                        std::uint8_t aukst_hex_skaitmuo_2 = (hash2[i] >> 4) & 0x0F; // Xx
                        std::uint8_t zem_hex_skaitmuo_2 = hash2[i] & 0x0F;          // xX
                        if (aukst_hex_skaitmuo_1 != aukst_hex_skaitmuo_2)
                            ++hex_skirtumai;
                        if (zem_hex_skaitmuo_1 != zem_hex_skaitmuo_2)
                            ++hex_skirtumai;
                        visi_hex_skaitmenys += 2;
                    }

                    // bitu skirtumas
                    for (size_t i = 0; i < hash1.size(); ++i)
                    {
                        std::uint8_t skirtumas = hash1[i] ^ hash2[i];
                        bitu_skirtumai += std::popcount(skirtumas);
                        visi_bitu_skaitmenys += 8;
                    }
                }

                std::cout << std::dec << "\n"
                          << str_dydis << " ILGIO EILUTĖS:\nBitų skirtumai: " << bitu_skirtumai << "\nViso bitų: " << visi_bitu_skaitmenys << "\nSkirtumas (bitų): " << 100 * (bitu_skirtumai * 1.0) / (visi_bitu_skaitmenys * 1.0) << "%\nHex skirtumai: " << hex_skirtumai << "\nViso hexų: " << visi_hex_skaitmenys << "\nSkirtumas (hex): " << 100 * (hex_skirtumai * 1.0) / (visi_hex_skaitmenys * 1.0) << "%\n";

                // sumuojam ir bendrai, kad būtų ir bendros statistikos
                bendr_hex_skirtumai += hex_skirtumai;
                bendr_visi_hex_skaitmenys += visi_hex_skaitmenys;
                bendr_bitu_skirtumai += bitu_skirtumai;
                bendr_visi_bitu_skaitmenys += visi_bitu_skaitmenys;
            }

            std::cout << "\nBENDRAI:\nBitų skirtumai: " << bendr_bitu_skirtumai << "\nViso bitų: " << bendr_visi_bitu_skaitmenys << "\nSkirtumas (bitų): " << 100 * (bendr_bitu_skirtumai * 1.0) / (bendr_visi_bitu_skaitmenys * 1.0) << "%\nHex skirtumai: " << bendr_hex_skirtumai << "\nViso hexų: " << bendr_visi_hex_skaitmenys << "\nSkirtumas (hex): " << 100 * (bendr_hex_skirtumai * 1.0) / (bendr_visi_hex_skaitmenys * 1.0) << "%\n";
        }
        else if (eiga == "6")
        {
            // kandidatų rinkinys
            std::vector<std::string> kandidatai;
            kandidatai.reserve(10000);
            for (int i = 0; i < 10000; ++i)
            {
                std::string str = std::to_string(i);
                while (str.size() < 4)
                {
                    str.insert(0, "0");
                }
                kandidatai.push_back(str);
            }

            // random generatoriaus paruošimas
            unsigned int sekla = 123456789;
            std::mt19937_64 generatorius(sekla);

            // įvesties generavimas
            std::uniform_int_distribution<std::size_t> nuo_0_lig_9999(0, 9999);
            std::string ivestis = std::to_string(nuo_0_lig_9999(generatorius));
            while (ivestis.size() < 4)
            {
                ivestis.insert(0, "0");
            }

            // spėjimas be druskos

            auto maisa = str_i_hasha(ivestis);

            {
                std::vector<std::string> sutapimai;
                bool ar_jau_rastas_sutapimas = false;
                int bandymu_sk = 0;
                auto pr = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < kandidatai.size(); ++i)
                {
                    auto kand = kandidatai.at(i);
                    auto hash = str_i_hasha(kand);
                    if (hash == maisa)
                    {
                        sutapimai.push_back(kand);
                        if (!ar_jau_rastas_sutapimas)
                        {
                            bandymu_sk = i + 1;
                            ar_jau_rastas_sutapimas = true;
                            // toliau vis tiek pravarom visus, maž bus dar kandidatas (kolizija td)
                        }
                    }
                }
                auto pab = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> laikas = pab - pr;
                std::cout << std::dec << "\nĮvestis: " << ivestis << "\nMaiša: ";
                spausd_baitais(maisa);
                std::cout << "\nSpėjimas:\nRasti kandidatai: ";
                for (auto sutap : sutapimai)
                {
                    std::cout << sutap << " ";
                }
                std::cout << "\nBandymų sk.: " << std::dec << bandymu_sk << "\nVeikimo laikas: " << laikas.count() << " s\n\n";
            }

            // spėjimas su druska

            // ASCII charai 32-126 (visi kiti yra sisteminiai simboliai ir pan., nepanaudojami)
            const std::string raidynas =
                " !\"#$%&'()*+,-./"
                "0123456789"
                ":;<=>?@"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "[\\]^_`"
                "abcdefghijklmnopqrstuvwxyz"
                "{|}~";
            std::uniform_int_distribution<std::size_t> raidyno_pasiskirstymas(0, raidynas.size() - 1); // duos tolygiai paskirstytą random indeksą raidyno simboliui parinkt
            const size_t druskos_ilgis = 4;
            std::string druska;
            while (druska.size() < 4)
            {
                druska.push_back(raidynas[raidyno_pasiskirstymas(generatorius)]);
            }

            const std::string ivestis_drusk = ivestis + druska;
            auto maisa_drusk = str_i_hasha(ivestis_drusk);

            // ANTRINNASIN PALAIPSĀI...
            std::vector<std::string> sutapimai;

            bool ar_jau_rastas_sutapimas = false;
            int bandymu_sk = 0;
            auto pr = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < kandidatai.size(); ++i)
            {
                auto kand = kandidatai.at(i);
                auto hash = str_i_hasha(kand + druska);
                if (hash == maisa_drusk)
                {
                    sutapimai.push_back(kand);
                    if (!ar_jau_rastas_sutapimas)
                    {
                        bandymu_sk = i + 1;
                        ar_jau_rastas_sutapimas = true;
                        // toliau vis tiek pravarom visus, maž bus dar kandidatas (kolizija td)
                    }
                }
            }
            auto pab = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> laikas = pab - pr;
            std::cout << "\nĮvestis: " << ivestis_drusk << "\nMaiša: ";
            spausd_baitais(maisa_drusk);
            std::cout << "\nSpėjimas:\nRasti kandidatai: ";
            for (auto sutap : sutapimai)
            {
                std::cout << sutap << " ";
            }
            std::cout << "\nBandymų sk.: " << std::dec << bandymu_sk << "\nVeikimo laikas: " << laikas.count() << " s\n\n";
        }
    }
    return 0;
}