#include <iostream>
#include <vector>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "gl_core_4_5.h"
#include "arcball_camera.h"
#include "shader.h"

// A cube with different colors on each face
const std::vector<glm::vec3> cube = {
	glm::vec3(1.0, 1.0, -1.0), glm::vec3(1.0, 0.0, 0.0),
	glm::vec3(-1.0, 1.0, -1.0), glm::vec3(1.0, 0.0, 0.0),
	glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0),
	glm::vec3(-1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, 0.0),
	glm::vec3(-1.0, -1.0, 1.0), glm::vec3(0.0, 1.0, 0.0),
	glm::vec3(-1.0, 1.0, -1.0), glm::vec3(0.0, 1.0, 0.0),
	glm::vec3(-1.0, -1.0, -1.0), glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(1.0, 1.0, -1.0), glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0), glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 0.0),
	glm::vec3(1.0, -1.0, 1.0), glm::vec3(1.0, 1.0, 0.0),
	glm::vec3(-1.0, -1.0, 1.0), glm::vec3(1.0, 1.0, 0.0),
	glm::vec3(1.0, -1.0, -1.0), glm::vec3(1.0, 0.0, 1.0),
	glm::vec3(-1.0, -1.0, -1.0), glm::vec3(1.0, 0.0, 1.0)
};

const std::string cube_vs = R"(
#version 330 core

layout(location=0) in vec3 pos;
layout(location=1) in vec3 color;

uniform mat4 proj_view;

out vec3 vs_color;

void main(void){
	gl_Position = proj_view * vec4(pos, 1.f);
	vs_color = color;
}
)";

const std::string cube_fs = R"(
#version 330 core

in vec3 vs_color;

out vec4 color;

void main(void){ 
	color = vec4(vs_color, 1.f);
})";

int win_width = 1280;
int win_height = 720;
SDL_Window *window = nullptr;

void run_app(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);

glm::vec2 transform_mouse(glm::vec2 in) {
	return glm::vec2(in.x * 2.f / win_width - 1.f, 1.f - 2.f * in.y / win_height);
}

int main(int argc, const char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Failed to init SDL: " << SDL_GetError() << "\n";
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("Arcball Example",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_MakeCurrent(window, gl_context);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		std::cerr << "Failed to initialize OpenGL\n";
		return 1;
	}

	glm::vec3 eye(0, 0, 5);
	glm::vec3 center(0);
	glm::vec3 up(0, 1, 0);

	for (int i = 1; i < argc; ++i) {
		if (std::strcmp(argv[i], "-eye") == 0) {
			eye.x = std::stof(argv[++i]);
			eye.y = std::stof(argv[++i]);
			eye.z = std::stof(argv[++i]);
		} else if (std::strcmp(argv[i], "-center") == 0) {
			center.x = std::stof(argv[++i]);
			center.y = std::stof(argv[++i]);
			center.z = std::stof(argv[++i]);
		} else if (std::strcmp(argv[i], "-up") == 0) {
			up.x = std::stof(argv[++i]);
			up.y = std::stof(argv[++i]);
			up.z = std::stof(argv[++i]);
		} else {
			std::cout << "Usage: " << argv[0]
				<< " (-eye x y z) (-center x y z) (-up x y z)\n";
			return 0;
		}
	}

	run_app(eye, center, up);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void run_app(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
	ArcballCamera camera(eye, center, up);

	Shader render_cube(cube_vs, cube_fs);
	glUseProgram(render_cube.program);

	glm::mat4 proj = glm::perspective(glm::radians(65.f),
			static_cast<float>(win_width) / win_height,
			0.1f, 100.f);

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * cube.size(), cube.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glm::vec2 prev_mouse(-2.f);
	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				done = true;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
					&& event.window.windowID == SDL_GetWindowID(window))
			{
				done = true;
			}
			if (event.type == SDL_MOUSEMOTION) {
				const glm::vec2 cur_mouse = transform_mouse(glm::vec2(event.motion.x, event.motion.y));
				if (prev_mouse != glm::vec2(-2.f)) {
					if (event.motion.state & SDL_BUTTON_LMASK) {
						camera.rotate(prev_mouse, cur_mouse);
					} else if (event.motion.state & SDL_BUTTON_RMASK) {
						camera.pan(cur_mouse - prev_mouse);
					}
				}
				prev_mouse = cur_mouse;
			}
			if (event.type == SDL_MOUSEWHEEL) {
				camera.zoom(event.wheel.y * 0.1);
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
				win_width = event.window.data1;
				win_height = event.window.data2;
				proj = glm::perspective(glm::radians(65.f),
						static_cast<float>(win_width) / win_height,
						0.1f, 500.f);
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		render_cube.uniform("proj_view", proj * camera.transform());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, cube.size() / 2);

		SDL_GL_SwapWindow(window);
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

