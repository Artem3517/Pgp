#include "calculate.h"

int det(int **matrix, const int size) {
    int result = 0;
    if (size == 1) {
        return matrix[0][0];
    } else if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    int **tmp = (int **)malloc((size - 1) * sizeof(int *));
    for (int i = 0; i < size - 1; i++) {
        tmp[i] = (int *)malloc((size - 1) * sizeof(int));
    }
    int sign = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 1; j < size; j++) {
            for (int k = 0; k < size; k++) {
                if (k < i) {
                    tmp[j - 1][k] = matrix[j][k];
                } else if (k > i) {
                    tmp[j - 1][k - 1] = matrix[j][k];
                }
            }
        }
        result += sign * det(tmp, size - 1) * matrix[0][i];
        sign *= -1;
    }
    for (int i = 0; i < size - 1; i++) {
        free(tmp[i]);
    }
    free(tmp);
    return result;
}