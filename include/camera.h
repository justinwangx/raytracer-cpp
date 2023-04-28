#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"

class camera {
    public:
        camera(
            point3 look_from,
            point3 look_at,
            vec3 vup,
            float vfov, // vertical field of view in degrees
            float aspect_ratio,
            float aperture,
            float focus_distance
        ) {
            float theta = degrees_to_radians(vfov);
            float h = tan(theta / 2);
            float viewport_height = 2.0 * h; 
            float viewport_width = viewport_height * aspect_ratio;

            auto w = unit_vector(look_from - look_at);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            origin = look_from;
            horizontal = focus_distance * viewport_width * u;
            vertical = focus_distance * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_distance*w;

            lens_radius = aperture / 2;
        }

        ray get_ray(float s, float t) const {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                origin + offset,
                lower_left_corner + s*horizontal + t*vertical - origin - offset
            );
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
};

#endif