A raytracer is a computer program that renders realistic images by simulating the physical behavior of light.

I implemented a simple raytracer by following Peter Shirley's guide. Here I will keep notes on the code so that I can use this as a reference when I forget how some of it works.

# Simple Example

Here is a basic example of what can be extended to be a raytracer.

```cpp
#include "ray.h"
#include "vec3.h"

void write_color(std::ostream &out, color pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// write the translated color to the outstream
	out << static_cast<int>(255.999 * r) << ' '
		<< static_cast<int>(255.999 * g) << ' '
		<< static_cast<int>(255.999 * b) << '\n';
}

color ray_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {
	float aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = static_cast<int>(image_width / aspect_ratio);

	// Camera
	float viewport_height = 2.0;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0;

	auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

	// PPM image format
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int i = image_height-1; i >= 0; --i) {
		for (int j = 0; j < image_width; ++j) {
			float u = float(i) / image_height;	
			float v = float(j) / image_width;
			vec3 dir = lower_left_corner + u*vertical + v*horizontal;
			ray r(origin, dir);
            color c = ray_color(r);
            write_color(std::cout, c);
		}	
	}
}
```

This program outputs a linearly interpolated gradient between white and light blue.

When we create a raytracer, we need a virtual camera and a virtual viewport. The viewport is the 2D plane that the entire scene gets projected onto. The camera is the point from which we shoot rays into the scene. The camera and viewport exist inside a virtual world.

To generate an image, we iterate over the `image_height` x `image_width` grid. For each pixel in this grid, we define the ray's direction vector as an offset of the vector pointing from the camera to the lower left corner of the viewport. Based on the y component of this vector, we calculate the color of its corresponding pixel.

# Surface Normals

We use surface normals to shade. Surface normals are vectors that are perpendicular to the surfaces of objects. For a sphere, we can calculate the surface normal of an intersecting ray with the expression `intersection_point - center`.  It is convenient to normalize surface normals to unit length.

A decision to make with normals is whether to make them always point out (towards the camera). When we shoot a ray into a scene, if it intersects an object from the outside, the normal will point against the ray (toward the viewer). However, if the ray intersects an object from the inside, the normal will point with the ray (away from the viewer).

It is common to make normals always point out for consistency. However, in doing so, we lose information on whether hits occur on the outside or inside of objects. To retain this information, we can include the boolean `front_face` in each hit record.

# Hit Records and Hittable Objects

What should we do if we want to include multiple types of objects? One solution is to have an abstract `hittable` class.

Another design decision is whether to always compute normals when rays intersect with objects. There are cases when we might not want to compute normals--for example, when there is a second intersection along a ray on the backside of an object, which is blocked from the camera's view. For now, we'll just store normals along with other relevant values in a `hit_record` struct.

```cpp
struct hit_record {
	point3 p;
	vec3 normal;
	float t;
	bool front_face;
};

class hittable {
	public:
		virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};
```

To store multiple types of `hittable` objects, we can introduce the `hittable_list` class. This is essentially a C++ vector that contains logic for hitting objects. Below is the implementation of the `hit` method in the `hittable_list` class. Note that `hittable_list` also inherits from the `hittable` class in addition to storing `hittable` objects, since it too contains a `hit` method.

```cpp
bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	bool hit_anything = false;
	float closest = t_max;
	hit_record temp_rec;

	for (const auto& object : objects) {
		if object->hit(r, t_min, closest, temp_rec) {
			closest = temp_rec.t;
			rec = temp_rec;	
			hit_anything = true;
		}
	}

	return hit_anything;
}	
```
