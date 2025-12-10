#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "Renderer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Scene.h"
#include "Sphere.h"
#include "Random.h"
#include "Material.h"
#include "Plane.h"
#include <array>
#include <memory>

int main() {
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;


	// create renderer
	Renderer renderer;
	Scene scene;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", SCREEN_WIDTH, SCREEN_HEIGHT);

	Framebuffer framebuffer(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	float aspectRatio = (float)framebuffer.width / (float)framebuffer.height;
	Camera camera(80.0f, aspectRatio);
	camera.SetView({ 0, 2, 5 }, { 0, 0, 0 });
	scene.SetSky({ 1.0f, 0.4f, 0.3f }, { 0.1f, 0.2f, 0.8f });

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f, 0.5f, 0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ { 0.0f, 0.0f, 0.0f } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			glm::vec3 position(a + 0.9f * random::getReal(), 0.2f, b + 0.9f * random::getReal());

			if ((position - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
				std::shared_ptr<Material> sphere_material;

				auto choose_mat = random::getReal();
				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = HSVtoRGB({ 360.0f * random::getReal(), 1.0f, 1.0f });
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo = color3_t{ random::getReal(0.5f, 1.0f) };
					auto fuzz = random::getReal(0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(HSVtoRGB(360.0f * random::getReal(), 1.0f, 1.0f), 1.0f);
					scene.AddObject(std::make_unique<Sphere>(Transform{ position }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1.0f, 1.0f, 1.0f }, 1.5f);
	scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ 0.0f, 1.0f, 0.0f } }, 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
	scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ -4.0f, 1.0f, 0.0f } }, 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
	scene.AddObject(make_unique<Sphere>(Transform{ glm::vec3{ 4.0f, 1.0f, 0.0f } }, 1.0f, material3));
	
	//auto red = std::make_shared<Lambertian>(color3_t{ 1.0f, 0.0f, 0.0f });
	//auto green = std::make_shared<Lambertian>(color3_t{ 0.0f, 1.0f, 0.0f });
	//auto blue = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.0f, 1.0f });
	//auto light = std::make_shared<Emissive>(color3_t{ 1.0f, 1.0f, 1.0f }, 3.0f);
	//auto metal = std::make_shared<Metal>(color3_t{ 1.0f, 1.0f, 1.0f }, 0.0f);

	//auto sphere = std::make_unique<Sphere>(
	//	Transform{ glm::vec3 {0,0,0} },
	//	0.5f,
	//	red
	//);
	//scene.AddObject(std::move(sphere));
	//
	//std::array<std::shared_ptr<Material>, 5> materials = { red, green, blue, light, metal };

	//for (int i = 0; i < 15; i++) {
	//	// randomize mize size and position, place spheres on plane
	//	float radius = random::getReal(0.2f, 0.5f);
	//	glm::vec3 position = random::getReal(glm::vec3{ -3.0f, radius, -3.0f }, glm::vec3{ 3.0f, radius, 3.0f });

	//	std::unique_ptr<Object> sphere = std::make_unique<Sphere>(Transform{ position }, radius, materials[random::getInt(4)]);
	//	scene.AddObject(std::move(sphere));
	//}
	//
	//auto gray = std::make_shared<Lambertian>(color3_t{ 0.2f, 0.2f, 0.2f });
	//std::unique_ptr<Plane> plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0.0f, 0.0f, 0.0f } }, gray);
	//scene.AddObject(std::move(plane));

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		// check for exit events
		while (SDL_PollEvent(&event)) {
			// window (X) quit
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			// escape key quit
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
				quit = true;
			}
		}

		// draw to frame buffer
		framebuffer.Clear({ 0, 0, 0, 255 });
		/*for (int i = 0; i < 300; i++) framebuffer.DrawPoint(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, { 255, 255, 255, 255 });*/
		scene.Render(framebuffer, camera, 150);
		// update frame buffer, copy buffer pixels to texture
		framebuffer.Update();

		// copy frame buffer texture to renderer to display
		renderer.CopyFramebuffer(framebuffer);
		renderer.Show();
	}
}