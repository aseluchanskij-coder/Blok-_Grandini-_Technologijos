#include "mylib.h"
#include "tests.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <windows.h>

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    try {
        // Pasirinkimai
        std::cout << "Pasirinkite įvedimo būdą:\n[1] - Įvedimas ranka\n[2] - Skaitymas iš failo\n[3] - Testai (eksperimentai)\nPasirinkimas: ";
        int ivedimas = getInt(1, 3);

        if (ivedimas == 3) {
            std::cout << "\nKurį eksperimentą paleisti?\n[0] - Visus\n[1-7] - Pasirinktą eksperimentą\nPasirinkimas: ";
            int exp = getInt(0, 7);
            runTests(exp == 0 ? "all" : std::to_string(exp));
            return 0;
        }

        std::cout << "\nPasirinkite išvedimo būdą:\n[1] - Išvedimas į ekraną\n[2] - Išvedimas į failą\nPasirinkimas: ";
        int isvedimas = getInt(1, 2);

        // Įvedimas
        std::vector<std::string> A;
        if (ivedimas == 1) {
            A = readManual();
        } else {
            std::string failas = getFile();
            A = readFile(failas);
        }

        // Skaičiavimas
        std::vector<std::string> H = hashAll(A);

        // Išvedimas
        switch (isvedimas) {
            case 1: {
                printRez(std::cout, A, H);
                break;
            }
            case 2: {
                std::ofstream fout("isvedimas.txt");
                printRez(fout, A, H);
                fout.close();
                std::cout << "Rezultatai įrašyti į isvedimas.txt\n";
                break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Klaida: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
