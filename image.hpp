#pragma once

#include <iostream>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <algorithm>
#include <cmath>


class Image {
    private:
        int width;
        int height;
        int channels;
        size_t size;
        uint8_t* data;

    public:
        Image(const char* fname);

        int getWidth();
        int getHeight();
        int getChannels();
        size_t getSize();
        uint8_t* getDatas();

        void freeImage();
};

class AveragedChannels {
    private:
        Image* original;
        char* filename;
        int red;
        int green;
        int blue;
        int hue;
        int saturation;
        int value;

    public:
        AveragedChannels(Image* img, char* fname);

        Image* getImage();
        char* getFileName();
        int getRed();
        int getGreen();
        int getBlue();
        int getHue();

        bool operator<(AveragedChannels& _o);

        void average();
        void rgbtohsv();
};