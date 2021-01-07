# raytracer

This is an offline 3D graphics ray tracer implemented in C++11. No external graphics libraries were used, only standard C++ libraries. It is still a work in progress but so far includes the following features:

- global illumination (recursive ray-trace)
- antialiasing
- diffuse lambertian material
- specular metal material and imperfect metal (fuzzy metal)
- dielectric non-metal material
- camera properties
    - positionable
    - depth of field
- shapes: spheres

Planned future features:

- acceleration structure (bounding volume hierarchies)
- textures and texture mapping
- emmisive material (lights)
- constant density mediums (fog, smoke, etc.)
- shapes: rectangles

## Metal Materials Scene
![Alt text](images/metal_scene.png?raw=true "Metal Materials Scene")

## Glass Materials Scene
![Alt text](images/glass_scene_side.png?raw=true "Glass Materials Scene Side")
![Alt text](images/glass_scene_front.png?raw=true "Gass Materials Scene Front")
