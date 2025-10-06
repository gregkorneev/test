#include <iostream>

int main() {
    int choice;
    double value;
    double result;

    std::cout << "Выберите тип конвертации:" << std::endl;
    std::cout << "1. Километры в мили" << std::endl;
    std::cout << "2. Градусы Цельсия в Фаренгейты" << std::endl;
    std::cout << "3. Килограммы в фунты" << std::endl;
    std::cout << "Введите номер выбора: ";
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "Введите значение в километрах: ";
        std::cin >> value;
        result = value * 0.621371;
        std::cout << "Результат: " << result << " миль" << std::endl;
        std::cout << "Чтобы перевести километры в мили, нужно умножить значение на 0.621371." << std::endl;
    } else if (choice == 2) {
        std::cout << "Введите значение в градусах Цельсия: ";
        std::cin >> value;
        result = (value * 9/5) + 32;
        std::cout << "Результат: " << result << " градусов Фаренгейта" << std::endl;
        std::cout << "Чтобы перевести градусы Цельсия в Фаренгейты, нужно умножить значение на 9/5 и прибавить 32." << std::endl;
    } else if (choice == 3) {
        std::cout << "Введите значение в килограммах: ";
        std::cin >> value;
        result = value * 2.20462;
        std::cout << "Результат: " << result << " фунтов" << std::endl;
        std::cout << "Чтобы перевести килограммы в фунты, нужно умножить значение на 2.20462." << std::endl;
    } else {
        std::cout << "Неверный выбор." << std::endl;
    }

    return 0;
}
