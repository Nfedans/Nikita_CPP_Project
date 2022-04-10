//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"
#include <cmath>



bool Image::load(string filename)
{

    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) {
            throw("Can't open input file");
        }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w;
        this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3]; // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
        return false;
    }
    return true;
}
bool Image::loadRaw(string filename)
{
    return false;
}
bool Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return false; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail())
        {
            throw("Can't open output file");
            return false;
        }
        ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; ++i) {
            r = this->pixels[i].r;
            g = this->pixels[i].g;
            b = this->pixels[i].b;
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }
    return true;
}


void Image::filterRed()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }
}
void Image::filterGreen()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }
}
void Image::filterBlue()
{
    for(int i = 0; i < h*w; i++)
    {
        this->pixels[i].r = 0;
        this->pixels[i].g = 0;
    }
}
void Image::greyScale()
{
//    int x = 1;
//    int r = this->pixels[x].r;
//    int g = this->pixels[x].g;
//    int b = this->pixels[x].b;
//    cout << "r value : " << r << endl;
//    cout << "g value : " << g << endl;
//    cout << "b value : " << b << endl;
//    double greyedPre = (double)(r + g + b) / 3;
//    int answer = round(greyedPre);
//    cout << "greyed value : " << answer << endl;
    for(int i = 0; i < h*w; i++)
    {
        int r = this->pixels[i].r;
        int g = this->pixels[i].g;
        int b = this->pixels[i].b;

        double greyed_unrounded = (double)(r + g + b) / 3;
        int greyed_rounded = round(greyed_unrounded);

        this->pixels[i].r = greyed_rounded;
        this->pixels[i].g = greyed_rounded;
        this->pixels[i].b = greyed_rounded;
    }
}
void Image::flipHorizontal()
{
        int pixels[3];
        unsigned int temp1;
        unsigned int temp2;
        for(int y= 0; y < h; ++y)
        {
            for(int x =0; x <w/2; ++x)
            {
                temp1 = x + y * w;
                temp2 = (w - 1 - x) + y * w;

                pixels[0] = this->pixels[temp1].r;
                pixels[1] = this->pixels[temp1].g;
                pixels[2] = this->pixels[temp1].b;

                this->pixels[temp1] = this->pixels[temp2];

                this->pixels[temp2].r = pixels[0];
                this->pixels[temp2].g = pixels[1];
                this->pixels[temp2].b = pixels[2];
            }
        }
}
void Image::flipVertically()
{
    int pixels[3];
    unsigned int temp1;
    unsigned int temp2;
    for(int x= 0; x < w; ++x)
    {
        for(int y =0; y <h/2; ++y)
        {
            temp1 = x + y * w;
            temp2 = x + (h - 1 - y) * w;

            pixels[0] = this->pixels[temp1].r;
            pixels[1] = this->pixels[temp1].g;
            pixels[2] = this->pixels[temp1].b;

            this->pixels[temp1] = this->pixels[temp2];

            this->pixels[temp2].r = pixels[0];
            this->pixels[temp2].g = pixels[1];
            this->pixels[temp2].b = pixels[2];
        }
    }
}
void Image::AdditionalFunction2()
{
    // 90 degree turn clockwise

    Image sideFlipped = Image(w,h);
    for(int x= 0; x < h; ++x)
    {
        for(int y = 0; y < w; ++y)
        {
            unsigned int destination = (y * h) + (h - x - 1);
            sideFlipped.pixels[destination] = pixels[(x * w) + y];
        }
    }
*this = sideFlipped;
}

Image& Image::operator=(const Image &reference)
{
    if(this != &reference) {
        w = reference.w;
        h = reference.h;
        pixels = new Rgb[w * h];
        for(int i = 0; i < w * h; ++i) {
            pixels[i] = reference.pixels[i];
        }
    }
    return *this;
}
void Image::AdditionalFunction3()
{
    //  8 shades of gray

    for(int i = 0; i < h*w; i++)
    {
        int r = this->pixels[i].r;
        int g = this->pixels[i].g;
        int b = this->pixels[i].b;

        double greyed_unrounded = (double)(r + g + b) / 3;
        int greyed_rounded = round(greyed_unrounded);

        if(greyed_rounded > 223) {
            this->pixels[i].r = 255;
            this->pixels[i].g = 255;
            this->pixels[i].b = 255;
        }
        else if (greyed_rounded > 191) {
            this->pixels[i].r = 223;
            this->pixels[i].g = 223;
            this->pixels[i].b = 223;
        }
        else if (greyed_rounded > 159) {
            this->pixels[i].r = 191;
            this->pixels[i].g = 191;
            this->pixels[i].b = 191;
        }
        else if (greyed_rounded > 127) {
            this->pixels[i].r = 159;
            this->pixels[i].g = 159;
            this->pixels[i].b = 159;
        }
        else if (greyed_rounded > 95) {
            this->pixels[i].r = 127;
            this->pixels[i].g = 127;
            this->pixels[i].b = 127;
        }
        else if (greyed_rounded > 63) {
            this->pixels[i].r = 95;
            this->pixels[i].g = 95;
            this->pixels[i].b = 95;
        }
        else if (greyed_rounded > 31) {
            this->pixels[i].r = 63;
            this->pixels[i].g = 63;
            this->pixels[i].b = 63;
        }
        else
        {
            this->pixels[i].r = 31;
            this->pixels[i].g = 31;
            this->pixels[i].b = 31;
        }
    }
}
void Image::AdditionalFunction1()
{
    //Rotate image 180
    //Idea is that the array of pixels is reversed

    int pixels[3];
    unsigned int mirror;
    unsigned int size = h*w;
    for(int i = 0; i < (size / 2); ++i)
    {
        mirror = size - i;

        pixels[0] = this->pixels[i].r;
        pixels[1] = this->pixels[i].g;
        pixels[2] = this->pixels[i].b;

        this->pixels[i] = this->pixels[mirror];

        this->pixels[mirror].r = pixels[0];
        this->pixels[mirror].g = pixels[1];
        this->pixels[mirror].b = pixels[2];
    }

}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
    return w;
}

int Image::getHeight()
{
    return h;
}

Rgb* Image::getImage()
{
    return pixels;
}