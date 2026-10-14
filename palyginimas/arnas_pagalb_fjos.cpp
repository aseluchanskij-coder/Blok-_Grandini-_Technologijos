#include "arnas_pagalb_fjos.h"
#include <iostream>
#include <iomanip>
#include <cstring> // dėl std::memcpy()
#include <bit>     // dėl std::byteswap()
#include <exception>
#include <random>
#include "portable-file-dialogs.h"

#ifdef _WIN32 // jei programa veikia Windowsuos:
#include <windows.h>
#endif

void spausd_baitais(std::vector<std::uint8_t> &baitai)
{
    std::cout << std::hex; // tai lieka kaip nustatymas
    for (std::uint8_t baitas : baitai)
    {
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(baitas);
    }
    std::cout << '\n';
}

void spausd_raidem(std::vector<std::uint8_t> &baitai)
{
    for (std::uint8_t baitas : baitai)
    {
        std::cout << baitas;
    }
    std::cout << '\n';
}

std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> &baitai)
{
    auto inv_ilgis = baitai.size();
    size_t paddingas = (4 - (inv_ilgis % 4)) % 4; // tam, kad vektoriaus "baitai" ilgis būtų 4-ių kartotinis
    baitai.resize(inv_ilgis + paddingas);
    std::vector<std::uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < baitai.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        std::uint32_t blokas =
            (static_cast<std::uint32_t>(baitai[i]) << 24) |
            (static_cast<std::uint32_t>(baitai[i + 1]) << 16) |
            (static_cast<std::uint32_t>(baitai[i + 2]) << 8) |
            (static_cast<std::uint32_t>(baitai[i + 3]));
        blokai.push_back(blokas);
    }
    return blokai;
}

// PERDENGIMAS, KAD PRIIMTŲ RVALUE (std::move(x)), dėl efektyvumo
std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> &&baitai)
{
    auto inv_ilgis = baitai.size();
    size_t paddingas = (4 - (inv_ilgis % 4)) % 4; // tam, kad vektoriaus "baitai" ilgis būtų 4-ių kartotinis
    baitai.resize(inv_ilgis + paddingas);
    std::vector<std::uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < baitai.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        std::uint32_t blokas =
            (static_cast<std::uint32_t>(baitai[i]) << 24) |
            (static_cast<std::uint32_t>(baitai[i + 1]) << 16) |
            (static_cast<std::uint32_t>(baitai[i + 2]) << 8) |
            (static_cast<std::uint32_t>(baitai[i + 3]));
        blokai.push_back(blokas);
    }
    return blokai;
}

// ==========

void istatyt_utf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

std::string failo_pasirinkimas() noexcept(false)
{
    auto pasirinkimas_vektoriuj = pfd::open_file(
                                      "Pasirinkite failą",
                                      pfd::path::home(),
                                      {"Tekstiniai failai (.txt .text)", "*.txt *.text", "Visi failai", "*"})
                                      .result();
    if (pasirinkimas_vektoriuj.empty())
    {
        throw std::runtime_error("Nepavyko pasirinkti failo.");
    }
    return pasirinkimas_vektoriuj[0]; // neleidžiam rinktis kelių failų, tai tik vienas elementas bus vektoriuj
}

// -------- EKSPERIMENTŲ PAGALB. FUNKCIJOS

std::string generuotRandomStr(std::size_t ilgis, const std::string raidynas, std::mt19937_64 &generatorius, std::uniform_int_distribution<std::size_t> &pasiskirstymas)
{
    // raidyną, generatorių ir pasiskirstymą priimam argumentais, kad nereiktų jų kiekvienąkart funkcijoj kurt per naują
    std::string randomStr;
    randomStr.reserve(ilgis);

    for (std::size_t i = 0; i < ilgis; ++i)
    {
        randomStr.push_back(raidynas[pasiskirstymas(generatorius)]);
    }

    return randomStr;
}