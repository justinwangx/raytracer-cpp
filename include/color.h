#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

color gamma_correct_and_scale(const color& pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();   
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // scale color values by number of samples and gamma correct for gamma=2
    float scale = 1.0 / samples_per_pixel;
    r = sqrt(r * scale) * 255.999;
    g = sqrt(g * scale) * 255.999;
    b = sqrt(b * scale) * 255.999;

    return color(
        static_cast<int>(clamp(r, 0.0, 255.0)),
        static_cast<int>(clamp(g, 0.0, 255.0)),
        static_cast<int>(clamp(b, 0.0, 255.0))
    );
}

void write_color(std::ostream &out, const color& pixel_color, int samples_per_pixel) {
    color corrected_color = gamma_correct_and_scale(pixel_color, samples_per_pixel);

    out << corrected_color.x() << ' '
        << corrected_color.y() << ' '
        << corrected_color.z() << '\n';
}

#endif