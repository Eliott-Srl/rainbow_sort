#include "image.h"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

Image::Image(const char* fname) {
    if ((data = stbi_load(fname, &width, &height, &channels, 0)) != NULL) {
        //std::cout << fname << " " << width << " " << height << " " << channels << std::endl;
        size = width * height * channels;
        allocation_ = STB_ALLOCATED;
    }
    else {
        ON_ERROR_EXIT(true, "Couldn't open the image");
    }
}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

int Image::getChannels() {
    return channels;
}

size_t Image::getSize() {
    return size;
}

uint8_t* Image::getDatas() {
    return data;
}

void Image::save_as(const char* fname) {
    if (str_ends_in(fname, ".jpg") || str_ends_in(fname, ".JPG") || str_ends_in(fname, ".jpeg") || str_ends_in(fname, ".JPEG")) {
        stbi_write_jpg(fname, width, height, channels, data, 100);
    }
    else if (str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG")) {
        stbi_write_png(fname, width, height, channels, data, width * channels);
    }
    else {
        ON_ERROR_EXIT(true, "");
    }
}

void Image::freeImage() {
    if (allocation_ != NO_ALLOCATION && data != NULL) {
        if (allocation_ == STB_ALLOCATED) {
            stbi_image_free(data);
        }
        data = NULL;
        width = 0;
        height = 0;
        size = 0;
        allocation_ = NO_ALLOCATION;
    }
}

AveragedChannels::AveragedChannels(Image* img, char* fname) {
    original = img;
    filename = fname;
}

Image* AveragedChannels::getImage() {
    return original;
}

char* AveragedChannels::getFileName() {
    return filename;
}

int AveragedChannels::getRed() {
    return red;
}

int AveragedChannels::getGreen() {
    return green;
}

int AveragedChannels::getBlue() {
    return blue;
}

int AveragedChannels::getHue() {
    return hue;
}

int AveragedChannels::getSaturation() {
    return saturation;
}

int AveragedChannels::getValue() {
    return value;
}

bool AveragedChannels::operator<(AveragedChannels& _o) {
    return hue < _o.getHue();
}

void AveragedChannels::average() {
    long long lred = 0;
    long long lgreen = 0;
    long long lblue = 0;

    for (uint8_t* p = original->getDatas(); p <= original->getDatas() + original->getSize(); p += original->getChannels()) {
        lred += p[0];
        lgreen += p[1];
        lblue += p[2];
    }
    red = lred / (original->getSize() / original->getChannels());
    green = lgreen / (original->getSize() / original->getChannels());
    blue = lblue / (original->getSize() / original->getChannels());
}

void AveragedChannels::rgbtohsv() {
    double r = red / 255.0;
    double g = green / 255.0;
    double b = blue / 255.0;

    double cmax = std::max(r, std::max(g, b));
    double cmin = std::min(r, std::min(g, b));

    double delta = cmax - cmin;

    value = cmax * 100;

    if (delta > 0) {
        if (r >= cmax) {
            hue = 60 * (fmod(((g - b) / delta), 6));
        }
        else if (g >= cmax) {
            hue = 60 * (((b - r) / delta) + 2.0);
        }
        else if (b >= cmax) {
            hue = 60 * (((r - g) / delta) + 4.0);
        }

        hue = (hue < 0.0 ? hue + 360 : hue);

        saturation = (cmax > 0.0 ? (delta / cmax) * 100 : 0);
    }
    else {
        hue = 0;
        saturation = 0;
    }
}

std::string current_path() { 
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    return (std::string)buff;
}