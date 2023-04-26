#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"
#include "hittable.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
    
        virtual ~material() = default;
};

class lambertian : public material {
    public:
        lambertian(const color& c) : albedo(c) {} 

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            // catch bad scatter direction
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    
    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& c, float f) : albedo(c), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        float fuzz;
};

class dielectric : public material {
    public: 
        dielectric() {}
        dielectric(float idx) : ir(idx) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            // refraction based on snell's law
            float refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());

            float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            // account for problems with internal refraction
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float()) {
                direction = reflect(unit_direction, rec.normal);
            }
            else {
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        float ir; // index of refraction

    private:
            static float reflectance(float cosine, float ref_idx) {
                // use schlick's approximation for reflectance.
                auto r0 = (1-ref_idx) / (1+ref_idx);
                r0 = r0*r0;
                return r0 + (1-r0)*pow((1 - cosine),5);
            }
};

#endif