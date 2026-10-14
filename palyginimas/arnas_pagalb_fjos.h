#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <random>

void spausd_baitais(std::vector<std::uint8_t> &baitai);

void spausd_raidem(std::vector<std::uint8_t> &baitai);

std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> &baitai);
std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> &&baitai); // PERDENGIMAS, KAD PRIIMTŲ RVALUE (efektyvumui)

// ==========

void istatyt_utf8();

std::string failo_pasirinkimas() noexcept(false);

// ------- eksperimentų funkcijos

std::string generuotRandomStr(std::size_t ilgis, const std::string raidynas, std::mt19937_64 &generatorius, std::uniform_int_distribution<std::size_t> &pasiskirstymas);