#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(const point3& c, float r, shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {}

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
        virtual ~sphere() {}
    
    public:
        point3 center;
        float radius;
        shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray&r, float t_min, float t_max, hit_record& rec) const {
    // The math behind calculating the intersection of a ray and sphere:
    // A point P = (P_x, P_y, P_z) intersects with a sphere at center (C_x, C_y, C_z) if
    // (P_x - C_x)^2 + (P_y - C_y)^2 + (P_z - C_z)^2 = r^2, where r is the radius of the sphere.
    // We can simply this equation to be (P - C) * (P - C) = r^2, where * is the dot product.
    // When dealing with points on a ray, P = A + tb, where A is the origin of the ray, t is a scalar,
    // and b is the direction of the ray.
    // The intersection equation can then be written as (A + tb - C) * (A + tb - C) == r^2.
    // Let u = A - C and v = tb. Then we have
    // (u + v) * (u + v) = u*u + 2u*v + v*v = (A - C)*(A - C) + 2tb*(A - C) + t^2b*b = r^2.
    // Rewriting, we have a quadratic: t^2b*b + 2tb*(A - C) + (A - C)*(A - C) - r^2 = 0.
    // The ray intersects the sphere at the solutions to this equation.
    // If the discriminant is greater than or equal to zero, we have at least one solution.

    vec3 oc = r.origin() - center;
    // note that the dot product of a vector with itself is the square of the vector's length
    auto a = r.direction().length_squared();
    // half_b simplification to remove constants (essentially multiply num and denom of quadratric equation by 1/2)
    auto half_b = dot(r.direction(), oc);
    auto c =  oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return false;
    }

    auto sqrt_d = sqrt(discriminant);
    auto t = (-half_b - sqrt_d) / a;
    if (t < t_min || t_max < t) {
        t = (-half_b + sqrt_d) / a;
        if (t < t_min || t_max < t) {
            return false;
        }
    }

    rec.t = t;
    rec.p = r.at(t);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
