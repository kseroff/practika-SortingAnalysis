#include <algorithm>
#include <chrono>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/SortRegistry.h"
#include "sorts/AllSorts.h"
#include "utils/ArrayUtils.h"
#include "utils/Console.h"
#include "utils/FileUtils.h"
#include "utils/Input.h"

namespace fs = std::filesystem;

class SortingApplication {
private:
    std::vector<int> currentArray;
    std::string currentBaseFileName;
    std::string currentSourcePath;
    std::string lastSortInfo;
    std::vector<std::string> sessionHistory;

    const fs::path inputDirectory = fs::path("data") / "input";
    const fs::path outputDirectory = fs::path("data") / "output";
    const fs::path reportDirectory = fs::path("data") / "reports";
    fs::path sessionLogFile;

    struct SortRunResult {
        bool success = false;
        bool sortedCorrectly = false;
        long long microseconds = 0;
        double milliseconds = 0.0;
        SortStatistics stats;
        std::string outputFile;
        std::string reportFile;
        std::string errorMessage;
    };

    bool hasArray() const {
        return !currentArray.empty();
    }

    static std::tm safeLocalTime(std::time_t value) {
        std::tm result{};
#ifdef _WIN32
        localtime_s(&result, &value);
#else
        localtime_r(&value, &result);
#endif
        return result;
    }

    static std::string currentDateTime() {
        std::time_t now = std::time(nullptr);
        std::tm local = safeLocalTime(now);

        std::ostringstream stream;
        stream << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
        return stream.str();
    }

    static std::string timestampForFileName() {
        std::time_t now = std::time(nullptr);
        std::tm local = safeLocalTime(now);

        std::ostringstream stream;
        stream << std::put_time(&local, "%Y%m%d-%H%M%S");
        return stream.str();
    }

    static std::string pathToString(const fs::path& path) {
        return path.generic_string();
    }

    fs::path inputPath(const std::string& baseName) const {
        return inputDirectory / FileUtils::makeTxtFileName(baseName);
    }

    fs::path outputPath(const std::string& baseName, const std::string& sortFileName) const {
        return outputDirectory / FileUtils::makeSortedFileName(baseName, sortFileName);
    }

    fs::path reportPath(const std::string& baseName, const std::string& sortFileName) const {
        return reportDirectory / (baseName + "-report-" + sortFileName + ".txt");
    }

    void ensureDataDirectories() {
        fs::create_directories(inputDirectory);
        fs::create_directories(outputDirectory);
        fs::create_directories(reportDirectory);

        sessionLogFile = reportDirectory / ("session-log-" + timestampForFileName() + ".txt");
    }

    void addHistory(const std::string& message) {
        sessionHistory.push_back("[" + currentDateTime() + "] " + message);
    }

    void saveSessionHistory() const {
        if (sessionLogFile.empty()) {
            return;
        }

        std::ofstream file(sessionLogFile);
        if (!file.is_open()) {
            return;
        }

        file << "История текущего запуска SortingAnalysis\n";
        file << "Дата сохранения: " << currentDateTime() << "\n";
        file << "\n";

        if (sessionHistory.empty()) {
            file << "История пуста.\n";
            return;
        }

        for (const std::string& item : sessionHistory) {
            file << item << "\n";
        }
    }

    void printHeader() const {
        Console::printLine('=');
        std::cout << "SortingAnalysis — тестирование и сравнение сортировок\n";
        Console::printLine('=');

        std::cout << "Состояние программы:\n";
        if (hasArray()) {
            std::cout << "  Текущий массив: загружен\n";
            std::cout << "  Размер: " << currentArray.size() << " элементов\n";
            std::cout << "  Источник: " << (currentSourcePath.empty() ? "не указан" : currentSourcePath) << "\n";
            if (!lastSortInfo.empty()) {
                std::cout << "  Последняя сортировка: " << lastSortInfo << "\n";
            }
        } else {
            std::cout << "  Текущий массив: не загружен\n";
            std::cout << "  Подсказка: создайте, введите или загрузите массив в меню работы с массивом.\n";
        }

        std::cout << "\nПапки программы:\n";
        std::cout << "  Входные массивы: " << pathToString(inputDirectory) << "\n";
        std::cout << "  Результаты сортировки: " << pathToString(outputDirectory) << "\n";
        std::cout << "  Отчёты: " << pathToString(reportDirectory) << "\n";
        Console::printLine();
    }

    void printMainMenu() const {
        std::cout << "Главное меню\n";
        std::cout << "1. Работа с массивом\n";
        std::cout << "2. Сортировка и отчёты\n";
        std::cout << "3. Тестовые данные\n";
        std::cout << "4. Информация о сортировках\n";
        std::cout << "5. История текущего запуска\n";
        std::cout << "0. Выход\n";
        Console::printLine();
    }

    void saveCurrentArray(const std::string& baseName) {
        fs::path filePath = inputPath(baseName);
        FileUtils::saveArrayToFile(currentArray, pathToString(filePath));
        currentBaseFileName = baseName;
        currentSourcePath = pathToString(filePath);
        std::cout << "Массив сохранён в файл: " << currentSourcePath << "\n";
    }

    void saveInputArray(const std::vector<int>& array, const std::string& baseName) const {
        FileUtils::saveArrayToFile(array, pathToString(inputPath(baseName)));
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
        addHistory("Сгенерирован случайный массив " + currentSourcePath + ", размер: " + std::to_string(currentArray.size()));

        std::cout << "Новый массив стал текущим рабочим массивом.\n";
        Console::pause();
    }

    std::vector<int> makeTemplateArray(int templateChoice, int size) const {
        switch (templateChoice) {
        case 1:
            return ArrayUtils::generateRandomArray(size, 0, 10000);
        case 2:
            return ArrayUtils::generateSortedArray(size);
        case 3:
            return ArrayUtils::generateLimitedValuesArray(size, 2);
        case 4:
            return ArrayUtils::generateLimitedValuesArray(size, 3);
        case 5:
            return ArrayUtils::generateLimitedValuesArray(size, 5);
        case 6:
            return ArrayUtils::generateLimitedValuesArray(size, 10);
        case 7:
            return ArrayUtils::generateRandomArray(size, -10000, 0);
        case 8:
            return ArrayUtils::generateRandomArray(size, -10000, 10000);
        default:
            throw std::runtime_error("Неизвестный шаблон массива.");
        }
    }

    std::string templateName(int templateChoice) const {
        switch (templateChoice) {
        case 1: return "обычный набор: числа от 0 и выше";
        case 2: return "уже отсортированный массив";
        case 3: return "массив из 2 уникальных значений";
        case 4: return "массив из 3 уникальных значений";
        case 5: return "массив из 5 уникальных значений";
        case 6: return "массив из 10 уникальных значений";
        case 7: return "только отрицательные числа до 0";
        case 8: return "отрицательные и положительные числа";
        default: return "неизвестный шаблон";
        }
    }

    void printTemplateMenu() const {
        std::cout << "Шаблоны массивов\n";
        std::cout << "1. Обычный набор: случайные числа от 0 до 10000\n";
        std::cout << "2. Уже отсортированный массив\n";
        std::cout << "3. Массив из 2 уникальных значений\n";
        std::cout << "4. Массив из 3 уникальных значений\n";
        std::cout << "5. Массив из 5 уникальных значений\n";
        std::cout << "6. Массив из 10 уникальных значений\n";
        std::cout << "7. Только отрицательные числа до 0\n";
        std::cout << "8. Отрицательные и положительные числа\n";
        std::cout << "0. Назад\n";
        Console::printLine();
    }

    void generateTemplateArray() {
        Console::clear();
        printHeader();
        printTemplateMenu();

        int templateChoice = Input::readIntInRange("Выберите шаблон: ", 0, 8);
        if (templateChoice == 0) {
            return;
        }

        int size = Input::readIntInRange("Введите размер массива от 1 до 1000000: ", 1, 1000000);
        std::string baseName = Input::readFileBaseName("Введите имя файла без .txt: ");

        currentArray = makeTemplateArray(templateChoice, size);
        saveCurrentArray(baseName);
        addHistory("Создан шаблонный массив: " + templateName(templateChoice) + ", файл: " + currentSourcePath);

        std::cout << "Шаблон: " << templateName(templateChoice) << "\n";
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
        addHistory("Массив введён вручную и сохранён в " + currentSourcePath);
        std::cout << "Введённый массив стал текущим рабочим массивом.\n";
        Console::pause();
    }

    void loadArrayFromFile() {
        Console::clear();
        printHeader();
        std::cout << "Загрузка массива из файла\n\n";
        std::cout << "Файл должен лежать в папке: " << pathToString(inputDirectory) << "\n";

        std::string baseName = Input::readFileBaseName("Введите имя файла без .txt: ");
        fs::path filePath = inputPath(baseName);

        currentArray = FileUtils::loadArrayFromFile(pathToString(filePath));
        currentBaseFileName = baseName;
        currentSourcePath = pathToString(filePath);
        lastSortInfo.clear();

        addHistory("Загружен массив из файла " + currentSourcePath + ", размер: " + std::to_string(currentArray.size()));
        std::cout << "Массив загружен из файла: " << currentSourcePath << "\n";
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

    bool isPotentiallySlow(const ISortAlgorithm& algorithm) const {
        const std::string fileName = algorithm.fileName();
        const std::size_t size = currentArray.size();

        bool quadraticSort =
            fileName == "bubble-sort" ||
            fileName == "bubble-sort-swap-flag" ||
            fileName == "cocktail-sort" ||
            fileName == "selection-sort" ||
            fileName == "insertion-sort" ||
            fileName == "binary-insertion-sort";

        return quadraticSort && size >= 5000;
    }

    bool confirmSlowSortIfNeeded(const ISortAlgorithm& algorithm) const {
        if (!isPotentiallySlow(algorithm)) {
            return true;
        }

        std::cout << "\nВнимание! Вы выбрали " << algorithm.name() << " для массива из " << currentArray.size() << " элементов.\n";
        std::cout << "Эта сортировка может выполняться долго, особенно на 10000 и 25000 элементах.\n";
        std::cout << "Для больших массивов обычно быстрее Merge Sort, Quick Sort, Heap Sort или Shell Sort.\n";
        return Input::readYesNo("Продолжить сортировку");
    }

    void saveSortingReport(
        const ISortAlgorithm& algorithm,
        const SortRunResult& result
    ) const {
        std::ofstream file(result.reportFile);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл отчёта для записи: " + result.reportFile);
        }

        file << "Отчёт о сортировке SortingAnalysis\n";
        file << "====================================\n\n";
        file << "Дата запуска: " << currentDateTime() << "\n";
        file << "Алгоритм: " << algorithm.name() << "\n";
        file << "Описание: " << algorithm.description() << "\n";
        file << "Размер массива: " << currentArray.size() << "\n";
        file << "Исходный файл: " << currentSourcePath << "\n";
        file << "Файл результата: " << (result.outputFile.empty() ? "не создан" : result.outputFile) << "\n";
        file << "Статус: " << (result.success ? "успешно" : "ошибка") << "\n";

        if (!result.success) {
            file << "Ошибка: " << result.errorMessage << "\n";
        }

        file << "Результат отсортирован корректно: " << (result.sortedCorrectly ? "да" : "нет") << "\n";
        file << "Время сортировки, мкс: " << result.microseconds << "\n";
        file << "Время сортировки, мс: " << std::fixed << std::setprecision(3) << result.milliseconds << "\n";
        file << "Сравнения: " << result.stats.comparisons << "\n";
        file << "Обмены: " << result.stats.swaps << "\n";
        file << "Перемещения/записи: " << result.stats.moves << "\n";

        if (!file.good()) {
            throw std::runtime_error("Ошибка записи отчёта: " + result.reportFile);
        }
    }

    SortRunResult runSortingAlgorithm(const ISortAlgorithm& algorithm) {
        SortRunResult result;
        result.outputFile = pathToString(outputPath(currentBaseFileName, algorithm.fileName()));
        result.reportFile = pathToString(reportPath(currentBaseFileName, algorithm.fileName()));

        std::vector<int> sortedArray = currentArray;

        auto start = std::chrono::high_resolution_clock::now();
        try {
            algorithm.sort(sortedArray, result.stats);
            auto finish = std::chrono::high_resolution_clock::now();

            result.microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
            result.milliseconds = static_cast<double>(result.microseconds) / 1000.0;
            result.sortedCorrectly = std::is_sorted(sortedArray.begin(), sortedArray.end());
            result.success = result.sortedCorrectly;

            if (result.success) {
                FileUtils::saveArrayToFile(sortedArray, result.outputFile);
            } else {
                result.errorMessage = "Алгоритм завершился, но массив не отсортирован корректно.";
            }
        } catch (const std::exception& ex) {
            auto finish = std::chrono::high_resolution_clock::now();
            result.microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
            result.milliseconds = static_cast<double>(result.microseconds) / 1000.0;
            result.success = false;
            result.sortedCorrectly = false;
            result.outputFile.clear();
            result.errorMessage = ex.what();
        }

        saveSortingReport(algorithm, result);
        return result;
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
            std::cout << i + 1 << ". " << algorithms[i]->name();
            if (isPotentiallySlow(*algorithms[i])) {
                std::cout << "  [может быть долго]";
            }
            std::cout << "\n";
        }
        std::cout << "0. Назад\n";

        int choice = Input::readIntInRange("Ваш выбор: ", 0, static_cast<int>(algorithms.size()));
        if (choice == 0) {
            return;
        }

        const ISortAlgorithm& algorithm = SortRegistry::instance().getByIndex(static_cast<std::size_t>(choice - 1));
        if (!confirmSlowSortIfNeeded(algorithm)) {
            addHistory("Сортировка " + algorithm.name() + " отменена пользователем из-за предупреждения о долгом выполнении.");
            std::cout << "Сортировка отменена.\n";
            Console::pause();
            return;
        }

        std::cout << "\nВыполняется сортировка: " << algorithm.name() << "\n";
        SortRunResult result = runSortingAlgorithm(algorithm);

        if (result.success) {
            lastSortInfo = algorithm.name() + " — " + std::to_string(result.milliseconds) + " мс";
            addHistory("Выполнена сортировка " + algorithm.name() + ", файл результата: " + result.outputFile + ", отчёт: " + result.reportFile);

            std::cout << "\nСортировка завершена.\n";
            std::cout << "Корректность результата: массив отсортирован.\n";
            std::cout << "Результат сохранён в файл: " << result.outputFile << "\n";
        } else {
            lastSortInfo = algorithm.name() + " — ошибка";
            addHistory("Ошибка сортировки " + algorithm.name() + ": " + result.errorMessage + ", отчёт: " + result.reportFile);

            std::cout << "\nСортировка завершилась с ошибкой.\n";
            std::cout << "Ошибка: " << result.errorMessage << "\n";
        }

        std::cout << "Отчёт сохранён в файл: " << result.reportFile << "\n";
        std::cout << "Время сортировки: " << result.microseconds << " мкс (" << std::fixed << std::setprecision(3) << result.milliseconds << " мс)\n";
        std::cout << "Сравнения: " << result.stats.comparisons << "\n";
        std::cout << "Обмены: " << result.stats.swaps << "\n";
        std::cout << "Перемещения/записи: " << result.stats.moves << "\n";
        std::cout << "\nОсновной массив не изменён, чтобы можно было сравнивать разные сортировки на одинаковых данных.\n";

        saveSessionHistory();
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

        std::cout << "Простые сортировки:\n";
        for (std::size_t i = 0; i < algorithms.size(); ++i) {
            const std::string fileName = algorithms[i]->fileName();
            if (fileName == "bubble-sort" || fileName == "bubble-sort-swap-flag" || fileName == "cocktail-sort" ||
                fileName == "selection-sort" || fileName == "insertion-sort" || fileName == "binary-insertion-sort") {
                std::cout << "  " << i + 1 << ". " << algorithms[i]->name() << " — " << algorithms[i]->description() << "\n";
            }
        }

        std::cout << "\nБыстрые универсальные сортировки:\n";
        for (std::size_t i = 0; i < algorithms.size(); ++i) {
            const std::string fileName = algorithms[i]->fileName();
            if (fileName == "merge-sort" || fileName == "quick-sort-lomuto" || fileName == "quick-sort-hoare" ||
                fileName == "heap-sort" || fileName == "shell-sort") {
                std::cout << "  " << i + 1 << ". " << algorithms[i]->name() << " — " << algorithms[i]->description() << "\n";
            }
        }

        std::cout << "\nСортировки для целых чисел:\n";
        for (std::size_t i = 0; i < algorithms.size(); ++i) {
            const std::string fileName = algorithms[i]->fileName();
            if (fileName == "counting-sort" || fileName == "radix-sort") {
                std::cout << "  " << i + 1 << ". " << algorithms[i]->name() << " — " << algorithms[i]->description() << "\n";
            }
        }

        std::cout << "\nПримечание:\n";
        std::cout << "  Radix Sort работает только с неотрицательными числами.\n";
        std::cout << "  Counting Sort зависит от диапазона значений.\n";
        std::cout << "  Простые сортировки могут быть очень медленными на больших массивах.\n";

        Console::pause();
    }

    void generateFullTestDataSet() {
        Console::clear();
        printHeader();
        std::cout << "Генерация полного набора тестовых файлов\n\n";
        std::cout << "Будут созданы файлы в папке " << pathToString(inputDirectory) << ".\n";
        std::cout << "Существующие файлы с такими же именами будут перезаписаны.\n\n";

        if (!Input::readYesNo("Продолжить")) {
            std::cout << "Генерация отменена.\n";
            Console::pause();
            return;
        }

        const std::vector<int> normalSizes = { 10, 100, 1000, 2000, 5000, 10000, 25000 };
        const std::vector<int> unusualSizes = { 100, 1000, 5000 };

        int createdFiles = 0;

        for (int size : normalSizes) {
            saveInputArray(ArrayUtils::generateRandomArray(size, 0, 10000), "normal_" + std::to_string(size));
            ++createdFiles;
        }

        for (int size : unusualSizes) {
            saveInputArray(ArrayUtils::generateSortedArray(size), "sorted_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateLimitedValuesArray(size, 2), "two_values_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateLimitedValuesArray(size, 3), "three_values_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateLimitedValuesArray(size, 5), "five_values_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateLimitedValuesArray(size, 10), "ten_values_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateRandomArray(size, -10000, 0), "negative_" + std::to_string(size));
            saveInputArray(ArrayUtils::generateRandomArray(size, -10000, 10000), "mixed_" + std::to_string(size));
            createdFiles += 7;
        }

        addHistory("Сгенерирован полный набор тестовых данных. Количество файлов: " + std::to_string(createdFiles));
        saveSessionHistory();

        std::cout << "Готово. Создано файлов: " << createdFiles << "\n";
        std::cout << "Папка: " << pathToString(inputDirectory) << "\n";
        Console::pause();
    }

    void showHistory() {
        Console::clear();
        printHeader();
        std::cout << "История текущего запуска\n\n";

        if (sessionHistory.empty()) {
            std::cout << "История пока пуста.\n";
        } else {
            for (std::size_t i = 0; i < sessionHistory.size(); ++i) {
                std::cout << i + 1 << ". " << sessionHistory[i] << "\n";
            }
        }

        saveSessionHistory();
        std::cout << "\nИстория сохранена в файл: " << pathToString(sessionLogFile) << "\n";
        Console::pause();
    }

    void dataMenu() {
        while (true) {
            Console::clear();
            printHeader();
            std::cout << "Работа с массивом\n";
            std::cout << "1. Сгенерировать случайный массив\n";
            std::cout << "2. Сгенерировать массив по шаблону\n";
            std::cout << "3. Ввести массив вручную\n";
            std::cout << "4. Загрузить массив из файла\n";
            std::cout << "5. Показать текущий массив\n";
            std::cout << "0. Назад\n";
            Console::printLine();

            int command = Input::readIntInRange("Выберите действие: ", 0, 5);
            switch (command) {
            case 1: generateRandomArray(); break;
            case 2: generateTemplateArray(); break;
            case 3: inputArrayManually(); break;
            case 4: loadArrayFromFile(); break;
            case 5: showCurrentArray(); break;
            case 0: return;
            default: break;
            }
        }
    }

    void sortingMenu() {
        while (true) {
            Console::clear();
            printHeader();
            std::cout << "Сортировка и отчёты\n";
            std::cout << "1. Отсортировать текущий массив одной сортировкой\n";
            std::cout << "0. Назад\n";
            Console::printLine();

            int command = Input::readIntInRange("Выберите действие: ", 0, 1);
            switch (command) {
            case 1: sortCurrentArray(); break;
            case 0: return;
            default: break;
            }
        }
    }

    void testDataMenu() {
        while (true) {
            Console::clear();
            printHeader();
            std::cout << "Тестовые данные\n";
            std::cout << "1. Сгенерировать полный набор тестовых файлов\n";
            std::cout << "0. Назад\n";
            Console::printLine();

            std::cout << "Обычные размеры: 10, 100, 1000, 2000, 5000, 10000, 25000\n";
            std::cout << "Необычные размеры: 100, 1000, 5000\n";
            std::cout << "Необычные типы: отсортированный, 2/3/5/10 значений, отрицательные, смешанные.\n\n";

            int command = Input::readIntInRange("Выберите действие: ", 0, 1);
            switch (command) {
            case 1: generateFullTestDataSet(); break;
            case 0: return;
            default: break;
            }
        }
    }

public:
    void run() {
        ensureDataDirectories();
        addHistory("Программа запущена.");
        saveSessionHistory();

        while (true) {
            Console::clear();
            printHeader();
            printMainMenu();

            int command = Input::readIntInRange("Выберите действие: ", 0, 5);

            try {
                switch (command) {
                case 1:
                    dataMenu();
                    break;
                case 2:
                    sortingMenu();
                    break;
                case 3:
                    testDataMenu();
                    break;
                case 4:
                    showSorts();
                    break;
                case 5:
                    showHistory();
                    break;
                case 0:
                    addHistory("Программа завершена.");
                    saveSessionHistory();
                    Console::clear();
                    std::cout << "Работа программы завершена.\n";
                    std::cout << "История запуска сохранена в файл: " << pathToString(sessionLogFile) << "\n";
                    return;
                default:
                    std::cout << "Неизвестная команда.\n";
                    Console::pause();
                    break;
                }
            } catch (const std::exception& ex) {
                addHistory(std::string("Ошибка: ") + ex.what());
                saveSessionHistory();
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
