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
        ~Image();

        int getWidth();
        int getHeight();
        int getChannels();
        size_t getSize();
        uint8_t* getDatas();
};

class AveragedChannels {
    private:
        Image* original;
        char* filename;
        int red;
        int green;
        int blue;
        double hue;
        double saturation;
        double value;

    public:
        AveragedChannels(Image* img, char* fname);
        ~AveragedChannels();

        Image* getImage();
        char* getFileName();
        int getRed();
        int getGreen();
        int getBlue();
        double getHue();

        bool operator<(AveragedChannels& _o);

        void average();
        void rgbtohsv();
};