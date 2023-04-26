Because of the discrete nature of rendered images, undesirable jagged edges can be a problem. We can use antialising techniques to remedy this. In ray tracing, we can perform antialiiasing by sending multiple rays per pixel and averaging their resulting colors. This is called "stochastic sampling" or "jittered sampling."

```cpp
// Render
std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

for (int j = image_height-1; j >= 0; --j) {
	std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
        color pixel_color(0, 0, 0);
        for (int s = 0; s < samples_per_pixel; ++s) {
            auto u = (i + random_float()) / (image_width-1);
            auto v = (j + random_float()) / (image_height-1);
            ray r = cam.get_ray(u, v);
            pixel_color += ray_color(r, world);
        }
        write_color(std::cout, pixel_color, samples_per_pixel);
    }
}
```
