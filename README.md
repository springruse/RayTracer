# C++ Ray Tracer

A sophisticated software ray tracer built from scratch in C++, implementing physically-based rendering techniques to generate photorealistic images through light ray simulation.

## Introduction

This project is a feature-rich ray tracing engine that simulates the physics of light to create realistic images. Built entirely in C++ with minimal OpenGL usage for display purposes, this ray tracer demonstrates advanced rendering algorithms including recursive ray tracing, multiple material types, and global illumination effects. The project showcases fundamental computer graphics principles and serves as both a learning tool and a platform for exploring cutting-edge rendering techniques.

## Features

- **Path Tracing Engine**: Monte Carlo path tracing for physically accurate light transport
- **Advanced Materials**:
  - Diffuse (Lambertian) surfaces
  - Metallic/reflective materials with controllable roughness
  - Dielectric materials (glass, water) with refraction
  - Emissive materials for area lights
- **Realistic Lighting**:
  - Direct and indirect illumination
  - Soft shadows through area lights
  - Caustics and light dispersion
  - Global illumination via path tracing
- **Geometric Primitives**:
  - Spheres, planes, and triangles
  - Triangle mesh support for complex models
  - Bounding Volume Hierarchy (BVH) acceleration
- **Camera System**:
  - Configurable field of view
  - Depth of field with adjustable aperture
  - Antialiasing through supersampling
- **Performance Optimizations**:
  - Multi-threaded rendering
  - Spatial acceleration structures
  - Adaptive sampling
- **Modern C++ Architecture**: Clean, extensible codebase using C++17 features

## Installation

### Prerequisites

- C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.15 or higher
- OpenGL 3.3 or higher (for display only)
- SDL3
- GLM (OpenGL Mathematics)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/cpp-raytracer.git
cd cpp-raytracer

# Initialize submodules (if using)
git submodule update --init --recursive

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
cmake --build . --config Release

# Run the executable
./raytracer
```

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl3-dev libglm-dev libglew-dev
```

**macOS (using Homebrew):**
```bash
brew install sdl3 glm
```

**Windows:**
- Download SDL3 development libraries from [libsdl.org](https://www.libsdl.org/)
- Download GLM from [github.com/g-truc/glm](https://github.com/g-truc/glm)
- Configure paths in CMake or place in a `thirdparty/` directory

### Project Structure

```
cpp-raytracer/
├── src/              # Source files
├── include/          # Header files
├── shaders/          # Minimal GLSL shaders for display
├── scenes/           # Scene description files
├── thirdparty/       # Third-party libraries
│   ├── glm/
│   └── SDL3/
├── output/           # Rendered images
└── CMakeLists.txt
```

## Usage

### Basic Rendering

```cpp
#include "RayTracer.h"
#include "Scene.h"
#include "Camera.h"

int main() {
    // Create scene
    Scene scene;
    scene.addSphere(glm::vec3(0, 0, -5), 1.0f, Material::diffuse(glm::vec3(0.8, 0.3, 0.3)));
    scene.addSphere(glm::vec3(2, 0, -5), 1.0f, Material::metallic(glm::vec3(0.8, 0.8, 0.8), 0.1f));
    scene.addSphere(glm::vec3(-2, 0, -5), 1.0f, Material::dielectric(1.5f));
    
    // Setup camera
    Camera camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5), 60.0f);
    
    // Configure ray tracer
    RayTracer tracer(800, 600);
    tracer.setSamplesPerPixel(100);
    tracer.setMaxBounces(10);
    
    // Render
    tracer.render(scene, camera, "output.png");
    
    return 0;
}
```

### Scene Configuration

```cpp
// Load scene from file
Scene scene = Scene::loadFromFile("scenes/cornell_box.json");

// Or build programmatically
Scene scene;

// Add ground plane
scene.addPlane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0), 
               Material::diffuse(glm::vec3(0.5, 0.5, 0.5)));

// Add light source
scene.addSphere(glm::vec3(0, 5, 0), 1.0f, 
                Material::emissive(glm::vec3(10, 10, 10)));

// Add glass sphere
scene.addSphere(glm::vec3(0, 0, -3), 1.0f, 
                Material::dielectric(1.5f));
```

### Interactive Preview Mode

```cpp
// Real-time preview with SDL3 window
RayTracer tracer(800, 600);
tracer.enableInteractiveMode(true);
tracer.setPreviewSamples(4); // Low sample count for speed
tracer.render(scene, camera);
```

### Advanced Configuration

```cpp
// Configure depth of field
camera.setAperture(0.1f);
camera.setFocusDistance(5.0f);

// Adjust rendering quality
tracer.setSamplesPerPixel(1000);  // More samples = less noise
tracer.setMaxBounces(50);         // More bounces = better indirect lighting
tracer.setRussianRouletteDepth(5); // Enable Russian Roulette path termination

// Enable multi-threading
tracer.setThreadCount(std::thread::hardware_concurrency());
```

## Technical Details

### Ray Tracing Algorithm

The renderer implements a recursive path tracing algorithm that simulates light transport by following rays from the camera into the scene. For each ray intersection, the algorithm:

1. Determines the surface material properties
2. Calculates direct lighting from light sources
3. Samples indirect lighting through recursive ray bounces
4. Accumulates light contributions using Monte Carlo integration
5. Applies Russian Roulette for unbiased path termination

### Material System

**Diffuse (Lambertian)**: Implements perfect diffuse reflection with cosine-weighted hemisphere sampling. Light is scattered uniformly in all directions weighted by the cosine of the angle to the surface normal.

**Metallic**: Simulates conductive materials with controllable roughness. Uses microfacet theory with a GGX normal distribution function for physically accurate specular reflections.

**Dielectric**: Models transparent materials like glass and water with Fresnel equations for proper reflection/refraction mixing. Implements Snell's law for refraction calculations and handles total internal reflection.

**Emissive**: Represents area light sources that emit light. These materials contribute directly to the scene illumination and can be importance sampled for efficient light transport.

### Acceleration Structures

The ray tracer employs a Bounding Volume Hierarchy (BVH) to accelerate ray-object intersection tests. The BVH construction uses the Surface Area Heuristic (SAH) to minimize expected ray traversal cost, providing logarithmic intersection complexity for complex scenes.

### Sampling and Convergence

Antialiasing and soft shadows are achieved through stochastic sampling. The renderer uses:
- **Uniform sampling** for pixel coverage (antialiasing)
- **Cosine-weighted sampling** for diffuse materials
- **Importance sampling** for specular materials and light sources
- **Multiple importance sampling (MIS)** for combining sampling strategies

Progressive rendering allows users to watch the image converge in real-time, with noise decreasing as more samples are accumulated.

### OpenGL Integration

OpenGL is used minimally for display purposes only. The ray tracer generates images entirely on the CPU, then uploads the final framebuffer to an OpenGL texture for visualization in the SDL3 window. This approach maintains the purity of the software rendering implementation while providing efficient display capabilities.

### GLM Usage

This project makes extensive use of GLM for mathematical operations:
- Vector and matrix operations
- Quaternion rotations for camera orientation
- Experimental GLM features for advanced mathematical functions
- SIMD-optimized operations for performance-critical code paths

## Showcase

*Below are examples of scenes rendered with this ray tracer.*


### Glass and Metal Materials / Complex Scene
![Materials Showcase]<img width="903" height="726" alt="Screenshot 2025-12-10 143154" src="https://github.com/user-attachments/assets/96e53866-75b5-416f-8aeb-84f610c631ce" />

*Spheres demonstrating diffuse, metallic, and dielectric materials, and a fairly complex scene*


## Performance Benchmarks

Typical rendering times on an AMD Ryzen 9 5900X (12 cores, 24 threads):

| Scene | Resolution | Samples/Pixel | Render Time |
|-------|------------|---------------|-------------|
| Material Showcase | 1920x1080 | 500 | 8 minutes |
| Complex Scene | 3840x2160 | 1000 | 42 minutes |

*Note: Times vary based on scene complexity and maximum ray bounces*

## Attribution

This project utilizes the following third-party libraries:

### Core Dependencies
- **SDL3**: Simple DirectMedia Layer for window management and input handling
  - License: zlib License
  - Website: [www.libsdl.org](https://www.libsdl.org/)
  
- **GLM**: OpenGL Mathematics library for vector and matrix operations
  - License: MIT License
  - Repository: [github.com/g-truc/glm](https://github.com/g-truc/glm)
  - This project uses experimental GLM features for advanced mathematical operations

### OpenGL
- **OpenGL**: Used minimally for framebuffer display only
- **GLEW** (optional): OpenGL Extension Wrangler for extension management

### Acknowledgments

Special thanks to:
- Peter Shirley's "Ray Tracing in One Weekend" series for foundational concepts
- Physically Based Rendering (PBRT) book for advanced techniques
- The computer graphics research community for ongoing innovation

## Contributing

Contributions are welcome! Areas of interest include:

- Additional primitive types (cylinders, torii, CSG operations)
- Texture mapping and procedural textures
- Advanced material models (subsurface scattering, anisotropic materials)
- GPU acceleration with CUDA or Vulkan compute
- Scene file format improvements
- Denoising algorithms for faster convergence

Please open an issue to discuss major changes before submitting a pull request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Conclusion

This ray tracer represents a comprehensive exploration of physically-based rendering principles implemented from the ground up. By building a software renderer without relying on hardware acceleration for the core rendering pipeline, the project provides deep insights into light transport algorithms and material modeling. The use of modern C++ practices combined with efficient mathematical libraries results in a performant and extensible platform for graphics research and experimentation.

Whether you're a student learning computer graphics, a developer exploring rendering techniques, or an artist seeking to understand the mathematics behind photorealistic imagery, this ray tracer offers a transparent implementation of state-of-the-art algorithms with room for customization and enhancement.

The minimal use of OpenGL for display purposes keeps the focus on ray tracing fundamentals while still providing real-time feedback through progressive rendering. Future development could explore GPU acceleration, advanced sampling techniques, or integration with production rendering pipelines.

For questions, bug reports, or feature requests, please open an issue on GitHub.

---

**Author**: Alec Thao 
**Project Link**: [github.com/springruse/cpp-raytracer](https://github.com/springruse/cpp-raytracer)  
**Documentation**: [Full documentation available in the wiki](https://github.com/springruse/cpp-raytracer/wiki)
