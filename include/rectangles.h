#ifndef RECTANGLES_H
#define RECTANGLES_H

#include "hittable.h"
#include "material.h"

class xy_rect : public hittable {
public:
    xy_rect() {}
    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, shared_ptr<material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float t = (k - r.origin().z()) / r.direction().z();
        if (t < t_min || t > t_max)
            return false;
        float x = r.origin().x() + t * r.direction().x();
        float y = r.origin().y() + t * r.direction().y();
        if (x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (y - y0) / (y1 - y0);
        rec.t = t;
        rec.mat_ptr = mat_ptr;
        rec.set_face_normal(r, vec3(0, 0, 1));
        rec.p = r.at(t);
        return true;
    }

public:
    float x0, x1, y0, y1, k;
    shared_ptr<material> mat_ptr;
};

class xz_rect : public hittable {
public:
    xz_rect() {}
    xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, shared_ptr<material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float t = (k - r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max)
            return false;
        float x = r.origin().x() + t * r.direction().x();
        float z = r.origin().z() + t * r.direction().z();
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        rec.mat_ptr = mat_ptr;
        rec.set_face_normal(r, vec3(0, 1, 0));
        rec.p = r.at(t);
        return true;
    }

public:
    float x0, x1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};

class yz_rect : public hittable {
public:
    yz_rect() {}
    yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, shared_ptr<material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float t = (k - r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max)
            return false;
        float y = r.origin().y() + t * r.direction().y();
        float z = r.origin().z() + t * r.direction().z();
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        rec.u = (y - y0) / (y1 - y0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        rec.mat_ptr = mat_ptr;
        rec.set_face_normal(r, vec3(1, 0, 0));
        rec.p = r.at(t);
        return true;
    }

public:
    float y0, y1, z0, z1, k;
    shared_ptr<material> mat_ptr;
};

#endif