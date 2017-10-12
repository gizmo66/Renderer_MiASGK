//
// Created by Adam on 2017-10-12.
//

#include "Math.h"

double Math::abs(double number) {
    if (number >= 0) {
        return number;
    } else {
        return -number;
    }
}

double Math::sqrt(double number) {
    double result = number / 2;
    const double tolerance = 1.0e-7;
    do {
        result = (result + number / result) / 2;
    } while (abs(result * result - number) > tolerance);
    return result;
}