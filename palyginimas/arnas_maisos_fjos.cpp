#include "arnas_maisos_fjos.h"
#include <bit> // dėl std::rotr (rotate bits to right)
#include <array>
#include <cstring> // dėl std::memcpy()
#include "arnas_pagalb_fjos.h"

std::vector<std::uint8_t> gaut_256bit_hasha(std::vector<std::uint32_t> &&blokai)
{
    // parinktos randomiškos konstantos
    constexpr std::array<std::uint32_t, 8> konstantos = {
        0xA3D71B29u,
        0x6C8E93F5u,
        0xD4B21763u,
        0x39F15AC7u,
        0x82E64D1Bu,
        0x57C93AF1u,
        0xE1B46D85u,
        0x2F73C9D3u,
    };

    for (size_t i = 0; i < blokai.size(); ++i)
    {
        std::uint32_t x = blokai.at(i);
        for (size_t r = 0; r < 8; ++r)
        {
            x = x ^ konstantos[(i + r) % 8] ^ static_cast<std::uint32_t>((i + r) * konstantos[(i + 3) % 8]);
            x = std::rotr(x, static_cast<std::uint32_t>((i + 1) * (x >> 27) * konstantos[(r * x) % 8]) % 31 + 1);
            x *= konstantos[(i + r) % 8];
        }
        blokai.at(i) = x;
    }

    // ===== blokų skaičiaus suvienodinimas (8 4-baičiai blokai)

    if (blokai.size() < 8)
    {
        int idx = blokai.size() - 1;
        while (blokai.size() < 8)
        {
            if (idx < 0)
                idx = blokai.size() - 1;
            std::uint32_t naujas_blokas = blokai.at(idx);
            naujas_blokas = std::rotr(naujas_blokas, idx % 2 + 1);
            blokai.push_back(naujas_blokas);
            --idx;
        }
    }
    else if (blokai.size() > 8)
    {
        for (int i = 0; i < 8; ++i)
        {
            int idx = i + 8;
            while (idx < blokai.size())
            {
                blokai.at(i) = blokai.at(i) ^ blokai.at(idx);
                idx += 8;
            }
            blokai.at(i) = std::rotr(blokai.at(i), idx % 2 + 1);
        }
        blokai.resize(8);
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            blokai.at(i) = std::rotr(blokai.at(i), i % 2 + 1);
        }
    }

    // ===== papildoma blokų maiša

    for (int i = 0; i < blokai.size(); ++i)
    {
        for (int j = 0; j < blokai.size(); ++j)
        {
            blokai[i] += blokai[j];
        }
    }

    // ===== blokų atskaidymas baituos

    std::vector<std::uint8_t> isvestis;
    for (auto blokas : blokai)
    {
        isvestis.push_back(static_cast<std::uint8_t>(blokas >> 24)); // didžiausias baitas
        isvestis.push_back(static_cast<std::uint8_t>(blokas >> 16));
        isvestis.push_back(static_cast<std::uint8_t>(blokas >> 8));
        isvestis.push_back(static_cast<std::uint8_t>(blokas)); // mažiausias baitas
    }

    // ===== paskutinė maiša atskirais baitais (dėl lavinos efekto)
    for (int i = 0; i < isvestis.size(); ++i)
    {
        for (int j = 0; j < isvestis.size(); ++j)
        {
            if (i != j)
            {
                isvestis.at(i) += isvestis.at(j);
            }
        }
    }

    return isvestis;
}

std::vector<std::uint8_t> str_i_hasha(const std::string &str)
{
    std::vector<std::uint8_t> ivestis(str.begin(), str.end());
    return gaut_256bit_hasha(baitus_sujungt_po_4(std::move(ivestis)));
}