Diffuse materials are materials that do not emit light. They appear matte or non-shiny. Diffuse materials scatter light uniformly in all directions. In ray tracing, we can achieve this effect by sending scattered rays in random directions at hit points.

To implement this for spheres, first consider an intersection point P between a ray and a sphere. There are two unit circles tangent to P, one centered at P + N and one centered at P - N, where N is the normal vector at P. We can find a unit vector within the circle centered at P + N, and add this to P + N to get some point within the unit sphere tangent to P; let's call it S. Then to find the direction of the scattered ray, we compute S - P. (See the guide for a visual.)

# Lambertian Reflection

Lambertian reflection is a specific kind of diffuse reflection that follow's Lambert's cosine law. Lambertian reflection is typically used to represent diffuse objects, so "Lambertian" and "diffuse" are often used interchangeably. Lambert's cosine law states that the intensity of light that reflects from a surface is proportional to the cosine of the angel between the surface normal and the outgoing light direction.

We can create a Lambertian class that inherits from an abstract Material class.

```cpp
class lambertian : class material {
	public:
		lambertian(const color& a) : albedo(a) {}	

		virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
			auto scatter_direction = rec.normal + random_unit_vector();	
			// Catch degenerate scatter direction
			if (scatter_direction.near_zero()) {
				scatter_direction = rec.normal;
			}
			scattered = ray(rec.p, scatter_direction);
			attenuation = albedo;
			return true;
		}

	public:
		color albedo;
}
```
