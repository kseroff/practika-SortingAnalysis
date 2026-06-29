#pragma once

#include <cstdlib>
#include <clocale>
#include <iostream>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace Console {
    inline void setupRussianConsole() {
#ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
#endif
        std::setlocale(LC_ALL, "");
    }

    inline void clear() {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    inline void pause() {
        std::cout << "\nНажмите Enter, чтобы продолжить...";
        std::string temp;
        std::getline(std::cin, temp);
    }

    inline void printLine(char symbol = '-', int count = 60) {
        for (int i = 0; i < count; ++i) {
            std::cout << symbol;
        }
        std::cout << '\n';
    }
}
