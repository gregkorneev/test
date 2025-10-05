#include <iostream>
#include <cstdlib>
#include <ctime>

struct color {
    int red;
    int green;
    int blue;
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    color palettes[5][4];

    for (int i = 0; i < 5; i++) {
        // базовый цвет
        palettes[i][0].red   = std::rand() % 256;
        palettes[i][0].green = std::rand() % 256;
        palettes[i][0].blue  = std::rand() % 256;

        // оттенки/контраст
        for (int j = 1; j < 4; j++) {
            switch (j) {
                case 1:
                    palettes[i][j].red   = palettes[i][0].red   + (std::rand() % 40) - 20;
                    palettes[i][j].green = palettes[i][0].green + (std::rand() % 40) - 20;
                    palettes[i][j].blue  = palettes[i][0].blue  + (std::rand() % 40) - 20;
                    break;
                case 2:
                    palettes[i][j].red   = palettes[i][0].red   + (std::rand() % 40) - 20;
                    palettes[i][j].green = palettes[i][0].green + (std::rand() % 40) - 20;
                    palettes[i][j].blue  = palettes[i][0].blue  + (std::rand() % 40) - 20;
                    break;
                case 3:
                    palettes[i][j].red   = 255 - palettes[i][0].red;
                    palettes[i][j].green = 255 - palettes[i][0].green;
                    palettes[i][j].blue  = 255 - palettes[i][0].blue;
                    break;
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        std::cout << "Палитра " << (i + 1) << ":\n";
        for (int j = 0; j < 4; j++) {
            std::cout << "Цвет " << (j + 1) << ": ("
                      << palettes[i][j].red << ", "
                      << palettes[i][j].green << ", "
                      << palettes[i][j].blue << ")\n";
        }
        std::cout << "\n";
    }

    return 0;
}
