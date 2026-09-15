#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::vector;
using std::cin;
using std::endl;
using std::string;

//busima maisos funkcija
string custom_hashas(vector<uint8_t> ivestis) {
    return "Tuscia...";
}

int main() {
    vector<uint8_t> duomenys;
    string tekstas;

    cout << "Iveskite teksta (viena zodi): ";
    cin >> tekstas;

    for (size_t i = 0; i < tekstas.length(); i++) { //dedame raides i baitu vektoriu
        duomenys.push_back(tekstas[i]);
    }

    cout << "Nuskaityta baitu: " << duomenys.size() << endl; //kad ateityje suzinot kiek baitu reik pripildyti pading,
    // kad pripildyt iki 32 baitu, kolkas tai darysiu pasitelkiant pirminiais skaiciais 
    
    string rezultatas = custom_hashas(duomenys);
    cout << "Gauta maisa (Hash): " << rezultatas << endl;

    return 0;
}

