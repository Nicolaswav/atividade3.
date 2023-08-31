#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    float y;
} Ponto;

void readCSV(const char *filename, Ponto **pontos, int *numPontos) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int capacity = 10;  // Initial capacity of the array
    int size = 0;
    *pontos = (Ponto *)malloc(capacity * sizeof(Ponto));

    if (*pontos == NULL) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    int x;
    float y;

    while (fscanf(file, "%d,%f", &x, &y) == 2) {
        if (size == capacity) {
            capacity *= 2;
            *pontos = (Ponto *)realloc(*pontos, capacity * sizeof(Ponto));
            if (*pontos == NULL) {
                perror("Erro ao realocar memória");
                exit(EXIT_FAILURE);
            }
        }

        (*pontos)[size].x = x;
        (*pontos)[size].y = y;
        size++;
    }

    fclose(file);
    *numPontos = size;
}

void calculateRegression(const Ponto *pontos, int numPontos, float *slope, float *intercept) {
    float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < numPontos; i++) {
        sumX += pontos[i].x;
        sumY += pontos[i].y;
        sumXY += pontos[i].x * pontos[i].y;
        sumX2 += pontos[i].x * pontos[i].x;
    }

    *slope = (numPontos * sumXY - sumX * sumY) / (numPontos * sumX2 - sumX * sumX);
    *intercept = (sumY - *slope * sumX) / numPontos;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    Ponto *pontos;
    int numPontos;

    readCSV(filename, &pontos, &numPontos);

    float slope, intercept;
    calculateRegression(pontos, numPontos, &slope, &intercept);

    printf("Saída: y = %.2fx + %.2f\n", slope, intercept);

    free(pontos);

    return EXIT_SUCCESS;
}
