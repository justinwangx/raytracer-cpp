#ifndef TEXTURE_H
#define TEXTURE_H

#include "rt.h"

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

public:
    color color_value;
};

#endif
