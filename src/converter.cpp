#include "converter.h"
#include <iostream>

// Функция для конвертации температуры
double convertTemperature(double value, TemperatureUnit from, TemperatureUnit to) {
    if (from == to) return value;

    double celsiusValue = 0.0;
    if (from == FAHRENHEIT) {
        celsiusValue = (value - 32) * 5.0 / 9.0;
    } else if (from == KELVIN) {
        celsiusValue = value - 273.15;
    } else {
        celsiusValue = value;
    }

    if (to == FAHRENHEIT) {
        return (celsiusValue * 9.0 / 5.0) + 32;
    } else if (to == KELVIN) {
        return celsiusValue + 273.15;
    } else {
        return celsiusValue;
    }
}

// ===== Новая реализация длины: через метры =====

// Вспомогательная функция: коэффициент перевода единицы в метры
static double factorToMeters(LengthUnit u) {
    switch (u) {
        case METERS:         return 1.0;        // 1 м
        case KILOMETERS:     return 1000.0;     // 1 км = 1000 м
        case MILES:          return 1609.344;   // 1 ми = 1609.344 м
        case CENTIMETERS:    return 0.01;       // 1 см = 0.01 м
        case MILLIMETERS:    return 0.001;      // 1 мм = 0.001 м
        case FEET:           return 0.3048;     // 1 фут = 0.3048 м
        case INCHES:         return 0.0254;     // 1 дюйм = 0.0254 м
        case YARDS:          return 0.9144;     // 1 ярд = 0.9144 м
        case NAUTICAL_MILES: return 1852.0;     // 1 морская миля = 1852 м
        default:             return 0.0;        // защита от некорректных значений
    }
}

// Конвертация длины через нормализацию к метрам
double convertLength(double value, LengthUnit from, LengthUnit to) {
    if (from == to) return value;

    double fFrom = factorToMeters(from);
    double fTo   = factorToMeters(to);

    if (fFrom <= 0.0 || fTo <= 0.0) return 0.0; // некорректные единицы

    // Переводим исходное значение в метры, затем в целевую единицу
    double inMeters = value * fFrom;
    return inMeters / fTo;
}

// Функция для конвертации валют (фиксированный курс)
double convertCurrency(double value, Currency from, Currency to) {
    if (from == to) return value;

    std::map<std::pair<Currency, Currency>, double> rates;
    rates[{USD, RUB}] = 90.0;
    rates[{RUB, USD}] = 1.0 / 90.0;
    rates[{EUR, RUB}] = 100.0;
    rates[{RUB, EUR}] = 1.0 / 100.0;
    rates[{EUR, USD}] = 1.1;
    rates[{USD, EUR}] = 1.0 / 1.1;

    if (rates.count({from, to})) {
        return value * rates[{from, to}];
    }
    return 0.0;
}

// Функция для взаимодействия с пользователем
void runConverter() {
    int choice;
    double value;

    std::cout << "Выберите тип конвертации:\n";
    std::cout << "1. Температура\n";
    std::cout << "2. Длина\n";
    std::cout << "3. Валюта\n";
    std::cout << "Введите ваш выбор: ";
    std::cin >> choice;

    switch (choice) {
        case 1: {
            int from, to;
            std::cout << "Введите значение: ";
            std::cin >> value;
            std::cout << "Из какой единицы (0-Цельсий, 1-Фаренгейт, 2-Кельвин): ";
            std::cin >> from;
            std::cout << "В какую единицу (0-Цельсий, 1-Фаренгейт, 2-Кельвин): ";
            std::cin >> to;
            double result = convertTemperature(value, static_cast<TemperatureUnit>(from),
                                               static_cast<TemperatureUnit>(to));
            std::cout << "Результат: " << result << std::endl;
            std::cout << "Пояснение: переводы идут через градусы Цельсия.\n";
            break;
        }
        case 2: {
            int from, to;
            std::cout << "Введите значение: ";
            std::cin >> value;
            std::cout << "Из какой единицы "
                         "(0-Метры, 1-Километры, 2-Мили, 3-Сантиметры, 4-Миллиметры, "
                         "5-Футы, 6-Дюймы, 7-Ярды, 8-Морские мили): ";
            std::cin >> from;
            std::cout << "В какую единицу "
                         "(0-Метры, 1-Километры, 2-Мили, 3-Сантиметры, 4-Миллиметры, "
                         "5-Футы, 6-Дюймы, 7-Ярды, 8-Морские мили): ";
            std::cin >> to;

            double result = convertLength(value, static_cast<LengthUnit>(from),
                                          static_cast<LengthUnit>(to));
            std::cout << "Результат: " << result << std::endl;
            std::cout << "Пояснение: все переводы выполняются через метры.\n";
            break;
        }
        case 3: {
            int from, to;
            std::cout << "Введите значение: ";
            std::cin >> value;
            std::cout << "Из какой валюты (0-USD, 1-EUR, 2-RUB): ";
            std::cin >> from;
            std::cout << "В какую валюту (0-USD, 1-EUR, 2-RUB): ";
            std::cin >> to;
            double result = convertCurrency(value, static_cast<Currency>(from),
                                            static_cast<Currency>(to));
            std::cout << "Результат: " << result << std::endl;
            std::cout << "Пояснение: используется фиксированный учебный курс.\n";
            break;
        }
        default:
            std::cout << "Неверный выбор." << std::endl;
            break;
    }
}
