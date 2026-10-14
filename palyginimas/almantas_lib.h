#pragma once
#include <string>
#include <vector>
#include <ostream>

int getInt(int min, int max);
std::string getFile();

std::vector<std::string> readManual();
std::vector<std::string> readFile(const std::string& failas);
std::string readFileBytes(const std::string& failas);

std::string hash(const std::string& in);
std::vector<std::string> hashAll(const std::vector<std::string>& in);

void printRez(std::ostream& out, const std::vector<std::string>& in, const std::vector<std::string>& hashes);
