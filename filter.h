#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "dotmesh.h"

// Функция для применения медианного фильтра к вектору
bool applyMedianFilter(std::vector<Dot>& indata, int windowSize) {
    std::vector<double> data;
    for (auto i : indata) data.push_back(i.R);
    if (windowSize % 2 == 0 || windowSize < 1) {
        throw std::invalid_argument("Размер окна должен быть нечетным и положительным числом.");
    }

    std::vector<double> filteredData(data.size());
    int halfWindow = windowSize / 2;

    for (size_t i = 0; i < data.size(); ++i) {
        // Определяем границы окна (циклический массив)
        std::vector<double> window;
        for (int j = -halfWindow; j <= halfWindow; ++j) {
            int index = (i + j + data.size()) % data.size(); // Зацикливание индекса
            window.push_back(data[index]);
        }

        // Сортируем окно для нахождения медианы
        std::sort(window.begin(), window.end());

        // Записываем медиану в результирующий вектор
        filteredData[i] = window[window.size() / 2];
    }
    for (int i = 0 ; i < filteredData.size(); i++) indata[i].R = filteredData[i];
    return true;
}

// Функция экспоненциального сглаживания
bool applyExponentialSmoothing(std::vector<Dot>& indata, double alpha) {
    std::vector<double> data;
    for (auto i : indata) data.push_back(i.R);
    if (alpha <= 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Коэффициент сглаживания alpha должен быть в пределах (0, 1].");
    }

    std::vector<double> smoothedData(data.size());

    // Инициализация первого значения
    smoothedData[0] = data[0];

    // Применение формулы экспоненциального сглаживания
    for (size_t i = 1; i < data.size(); ++i) {
        smoothedData[i] = alpha * data[i] + (1.0 - alpha) * smoothedData[i - 1];
    }

    for (int i = 0 ; i < smoothedData.size(); i++) indata[i].R = smoothedData[i];
    return true;
}

bool minimizeData(std::vector<Dot>& data, int step){
    std::vector<Dot> res;
    int count = 0;
    for (int i = 0; i < data.size(); i++){
        if (count < step){
            res.push_back(data[i]);
            count++;
        } else count = 0;
    }
    data = res;
    return true;
}

#endif // FILTER_H
