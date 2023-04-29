#include "image.hpp"
#include "utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.hpp"

//Constructor of the class Image
Image::Image(const char* fname) {
    if ((data = stbi_load(fname, &width, &height, &channels, 0)) != NULL) { //load the datas from the image
        size = width * height * channels;
    } else {
        ON_ERROR_EXIT(true, "Couldn't open the image");
    }
}

//Destrcutor of the class Image that free the image
Image::~Image() {
    stbi_image_free(data);
    data = NULL;
    width, height, size = 0;
}

//All the useful getters
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
//End of the useful getters


//Constructor of the class AveragedChannels
AveragedChannels::AveragedChannels(Image* img, char* fname) {
    original = img;
    filename = fname;
}

//All the useful getters
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

double AveragedChannels::getHue() {
    return hue;
}
//End of the useful getters

//Operator overloading so I can compare and sort the images by hue
bool AveragedChannels::operator<(AveragedChannels& _o) {
    return hue < _o.getHue();
}

//function to average the color of original
void AveragedChannels::average() {
    //r, g, b
    long long lred = 0, lgreen = 0, lblue = 0;

    //for loop which add to lred, lgreen, lblue, the rgb values of every pixel of the image
    for (uint8_t* p = original->getDatas(); p <= original->getDatas() + original->getSize(); p += original->getChannels()) {
        lred += p[0];
        lgreen += p[1];
        lblue += p[2];
    }

    //do the average of every channels
    size_t size = (original->getSize() / original->getChannels());
    red = lred / size;
    green = lgreen / size;
    blue = lblue / size;
}

void AveragedChannels::rgbtohsv() {
    //Implementation of this: https://www.geeksforgeeks.org/program-change-rgb-color-model-hsv-color-model/
    //For now, only the hue and the saturation is accurate
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

    //std::cout << red << " " << green << " " << blue << " " << hue << " " << saturation << " " << value << std::endl;
}