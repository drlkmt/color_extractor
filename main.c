#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

using namespace cv;
using namespace std;

void printColor(int r, int g, int b, int index) {
    printf("Цвет #%d: \033[48;2;%d;%d;%dm  0x%02X%02X%02X  \033[0m\n", 
           index, r, g, b, r, g, b);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Использование: %s --image <путь до изображения> [--num-colors <кол-во цветов>]\n", argv[0]);
        return -1;
    }

    const char *imagePath = NULL;
    int numColors = 5;

    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i + 1 < argc) {
            imagePath = argv[++i];
        } else if (strcmp(argv[i], "--num-colors") == 0 && i + 1 < argc) {
            numColors = atoi(argv[++i]);
        }
    }

    if (imagePath == NULL) {
        printf("Ошибка: путь до изображения не указан.\n");
        return -1;
    }

    // Загрузка изображения
    Mat image = imread(imagePath);
    if (image.empty()) {
        printf("Ошибка: не удалось загрузить изображение по пути: %s\n", imagePath);
        return -1;
    }

    // Преобразование изображения в RGB
    cvtColor(image, image, COLOR_BGR2RGB);

    // Преобразование в двумерный массив пикселей
    Mat pixels = image.reshape(1, image.rows * image.cols);

    // Преобразование типа данных в float для k-means
    pixels.convertTo(pixels, CV_32F);

    // K-means кластеризация
    Mat labels, centers;
    kmeans(pixels, numColors, labels,
           TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
           3, KMEANS_PP_CENTERS, centers);

    // Округление центров
    centers.convertTo(centers, CV_8U);

    printf("\nИзвлечённые цвета (всего %d):\n\n", numColors);

    // Вывод цветов
    for (int i = 0; i < numColors; i++) {
        int r = centers.at<uchar>(i, 0);
        int g = centers.at<uchar>(i, 1);
        int b = centers.at<uchar>(i, 2);
        printColor(r, g, b, i + 1);
    }

    printf("\n");

    return 0;
}
