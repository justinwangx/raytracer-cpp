Let's create a basic camera class and then gradually add features to it.

```cpp
class camera {
	public:
		camera() {
			float aspect_ratio = 16.0 / 9.0;
			float viewport_height = 2.0;
			float viewport_width = 2.0 * aspect_ratio;
			float focal_length = 1.0;	

			origin = point3(0, 0, 0);
			horizontal = vec3(viewport_width, 0, 0);
			vertical = vec3(0, viewport_height, 0);
			lower_left_corner = origin - horizontal/2 -vertical/2 - vec3(0,0,focal_length);
		}

		ray get_ray(float u, float v) const {
			return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);	
		}

	private:
		point3 origin;
		point3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
}
```

## Adjustable Field of View

Let's now add an adjustable field of view. Below is the modified constructor. TODO: further explain this.

```cpp
camera(
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio
) {
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0.0, 0.0);
    vertical = vec3(0.0, viewport_height, 0.0);
    lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
}
```

## Position and Orientation

We can also add the ability to position and orient the camera. We do this by adding the `lookfrom` and `lookat` properties, as well as `vup`.  Using these vectors and taking cross products, we can define a coordinate system by `w`, `u`, and `v`. TODO: change this to use a viewing direction instead of `lookat`.

```cpp
camera(
	point3 lookfrom,
	point3 lookat,
	vec3 vup,
    float vfov, // vertical field-of-view in degrees
    float aspect_ratio
) {
    float theta = degrees_to_radians(vfov);
    float h = tan(theta/2);
    float viewport_height = 2.0 * h;
    float viewport_width = aspect_ratio * viewport_height;

    auto w = unit_vector(lookfrom - lookat);
    auto u = unit_vector(cross(vup, w));
    auto v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal/2 - vertical/2 - w;
}
```

Note that the `get_ray` method has not changed from the original basic camera!

## Defocus Blur

To implement defocus blur (a depth of field effect) we need to create a virtual aperture and a virtual focus distance. We can first scale the `horizontal` and `vertical` vectors by the focus distance. Then, in `get_rays`, rather than shooting rays from a single `lookfrom` point (which equates to having a lens of radius 0), we shoot rays from a virtual disk centered at the `lookfrom` point.

```cpp
vec3 random_in_unit_disk() {
    while (true) {
	    auto p = vec3(random_float(-1,1), random_float(-1,1), 0);
	    if (p.length_squared() >= 1) continue;
        return p;
    }
}

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            float vfov, // vertical field-of-view in degrees
            float aspect_ratio,
	        float aperture,
            float focus_dist
        ) {
            float theta = degrees_to_radians(vfov);
            float h = tan(theta/2);
            float viewport_height = 2.0 * h;
            float viewport_width = aspect_ratio * viewport_height;


            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

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
```
