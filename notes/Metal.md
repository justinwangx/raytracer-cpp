Metal reflects light. We can calculate the reflection of a vector off a surface using vector math. Below, `n` is the normal vector at the point that `v` intersects the object.

```cpp
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2*dot(v, n)*n;
}
```

This is all we need to create the metal class.

```cpp
class metal : class material {
	public:
		metal(color& a) : albedo(a) {}

		virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
			vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = ray(rec.p, reflected);
			attenuation = albedo;
			return true;	
		}

	public:
		color albedo;
}
```
