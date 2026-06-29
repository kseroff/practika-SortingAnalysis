#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "core/SortRegistry.h"
#include "sorts/AllSorts.h"
#include "utils/ArrayUtils.h"
#include "utils/Console.h"
#include "utils/FileUtils.h"
#include "utils/Input.h"

class SortingApplication {
private:
    std::vector<int> currentArray;
    std::string currentBaseFileName;

    bool hasArray() const {
        return !currentArray.empty();
    }

    void printHeader() const {
        Console::printLine('=');
        std::cout << "SortingAnalysis — программа для сортировки массивов\n";
        Console::printLine('=');

        if (hasArray()) {
            std::cout << "Текущий массив: " << currentArray.size() << " элементов";
            if (!currentBaseFileName.empty()) {
                std::cout << " | файл: " << FileUtils::makeTxtFileName(currentBaseFileName);
            }
            std::cout << "\n";
        } else {
            std::cout << "Текущий массив: не загружен\n";
        }

        Console::printLine();
    }

    void printMenu() const {
        std::cout << "1. Сгенерировать случайный массив\n";
        std::cout << "2. Ввести массив вручную\n";
        std::cout << "3. Загрузить массив из файла\n";
        std::cout << "4. Показать текущий массив\n";
        std::cout << "5. Отсортировать текущий массив\n";
        std::cout << "6. Показать доступные сортировки\n";
        std::cout << "0. Выход\n";
        Console::printLine();
    }

    void saveCurrentArray(const std::string& baseName) {
        std::string fileName = FileUtils::makeTxtFileName(baseName);
        FileUtils::saveArrayToFile(currentArray, fileName);
        currentBaseFileName = baseName;
        std::cout << "Массив сохранён в файл: " << fileName << "\n";
    }

    void generateRandomArray() {
        Console::clear();
        printHeader();
        std::cout << "Генерация случайного массива\n\n";

        int size = Input::readIntInRange("Введите размер массива от 1 до 1000000: ", 1, 1000000);
        int minValue = Input::readInt("Введите минимальное значение: ");
        int maxValue = Input::readInt("Введите максимальное значение: ");

        if (minValue > maxValue) {
            std::cout << "Минимальное значение больше максимального. Значения будут поменяны местами.\n";
            std::swap(minValue, maxValue);
        }

        std::string baseName = Input::readFileBaseName("Введите имя файла без .txt: ");

        currentArray = ArrayUtils::generateRandomArray(size, minValue, maxValue);
        saveCurrentArray(baseName);

        std::cout << "Новый массив стал текущим рабочим массивом.\n";
        Console::pause();
    }

    void inputArrayManually() {
        Console::clear();
        printHeader();
        std::cout << "Ручной ввод массива\n\n";

        currentArray = Input::readArrayFromLine();
        std::string baseName = Input::readFileBaseName("Введите имя файла без .txt: ");

        saveCurrentArray(baseName);
        std::cout << "Введённый массив стал текущим рабочим массивом.\n";
        Console::pause();
    }

    void loadArrayFromFile() {
        Console::clear();
        printHeader();
        std::cout << "Загрузка массива из файла\n\n";

        std::string baseName = Input::readFileBaseName("Введите имя файла без .txt: ");
        std::string fileName = FileUtils::makeTxtFileName(baseName);

        currentArray = FileUtils::loadArrayFromFile(fileName);
        currentBaseFileName = baseName;

        std::cout << "Массив загружен из файла: " << fileName << "\n";
        std::cout << "Загруженный массив стал текущим рабочим массивом.\n";
        Console::pause();
    }

    void showCurrentArray() const {
        Console::clear();
        printHeader();
        std::cout << "Просмотр текущего массива\n\n";
        ArrayUtils::printArray(currentArray);
        Console::pause();
    }

    void showSorts() const {
        Console::clear();
        printHeader();
        std::cout << "Доступные сортировки\n\n";

        const auto& algorithms = SortRegistry::instance().all();
        if (algorithms.empty()) {
            std::cout << "Сортировки не зарегистрированы.\n";
            Console::pause();
            return;
        }

        for (std::size_t i = 0; i < algorithms.size(); ++i) {
            std::cout << i + 1 << ". " << algorithms[i]->name() << "\n";
            std::cout << "   " << algorithms[i]->description() << "\n";
        }

        Console::pause();
    }

    void sortCurrentArray() {
        Console::clear();
        printHeader();
        std::cout << "Сортировка текущего массива\n\n";

        if (!hasArray()) {
            std::cout << "Ошибка: сначала нужно создать или загрузить массив.\n";
            Console::pause();
            return;
        }

        const auto& algorithms = SortRegistry::instance().all();
        if (algorithms.empty()) {
            std::cout << "Ошибка: нет доступных сортировок.\n";
            Console::pause();
            return;
        }

        std::cout << "Выберите сортировку:\n";
        for (std::size_t i = 0; i < algorithms.size(); ++i) {
            std::cout << i + 1 << ". " << algorithms[i]->name() << "\n";
        }

        int choice = Input::readIntInRange("Ваш выбор: ", 1, static_cast<int>(algorithms.size()));
        const ISortAlgorithm& algorithm = SortRegistry::instance().getByIndex(static_cast<std::size_t>(choice - 1));

        std::vector<int> sortedArray = currentArray;
        SortStatistics stats;

        std::cout << "\nВыполняется сортировка: " << algorithm.name() << "\n";

        auto start = std::chrono::high_resolution_clock::now();
        algorithm.sort(sortedArray, stats);
        auto finish = std::chrono::high_resolution_clock::now();

        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
        double milliseconds = static_cast<double>(microseconds) / 1000.0;

        std::string outputFile = FileUtils::makeSortedFileName(currentBaseFileName, algorithm.fileName());
        FileUtils::saveArrayToFile(sortedArray, outputFile);

        std::cout << "\nСортировка завершена.\n";
        std::cout << "Результат сохранён в файл: " << outputFile << "\n";
        std::cout << "Время сортировки: " << microseconds << " мкс (" << milliseconds << " мс)\n";
        std::cout << "Сравнения: " << stats.comparisons << "\n";
        std::cout << "Обмены: " << stats.swaps << "\n";
        std::cout << "Перемещения/записи: " << stats.moves << "\n";
        std::cout << "\nОсновной массив не изменён, чтобы можно было сравнивать разные сортировки на одинаковых данных.\n";

        Console::pause();
    }

public:
    void run() {
        while (true) {
            Console::clear();
            printHeader();
            printMenu();

            int command = Input::readIntInRange("Выберите действие: ", 0, 6);

            try {
                switch (command) {
                case 1:
                    generateRandomArray();
                    break;
                case 2:
                    inputArrayManually();
                    break;
                case 3:
                    loadArrayFromFile();
                    break;
                case 4:
                    showCurrentArray();
                    break;
                case 5:
                    sortCurrentArray();
                    break;
                case 6:
                    showSorts();
                    break;
                case 0:
                    Console::clear();
                    std::cout << "Работа программы завершена.\n";
                    return;
                default:
                    std::cout << "Неизвестная команда.\n";
                    Console::pause();
                    break;
                }
            } catch (const std::exception& ex) {
                std::cout << "\nОшибка: " << ex.what() << "\n";
                Console::pause();
            }
        }
    }
};

int main() {
    Console::setupRussianConsole();

    try {
        SortingApplication app;
        app.run();
    } catch (const std::exception& ex) {
        std::cout << "Критическая ошибка: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
