#pragma once

// Чтобы добавить новую сортировку:
// 1. Создайте новый header в папке sorts/.
// 2. Наследуйте класс от ISortAlgorithm.
// 3. Внизу файла добавьте REGISTER_SORT_ALGORITHM(ИмяКласса);
// 4. Подключите header здесь. Меню программы обновится автоматически через SortRegistry.

#include "BubbleSort.h"
#include "BubbleSortSwapFlag.h"
#include "CocktailSort.h"
#include "SelectionSort.h"
#include "InsertionSort.h"
#include "BinaryInsertionSort.h"
#include "MergeSort.h"
#include "QuickSortLomuto.h"
#include "QuickSortHoare.h"
#include "HeapSort.h"
#include "ShellSort.h"
#include "CountingSort.h"
#include "RadixSort.h"
