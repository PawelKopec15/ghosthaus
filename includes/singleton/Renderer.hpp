#pragma once

#include "../curseswrapper/Window.hpp"

class Renderer
{
public:
	static Renderer &Get()
	{
		static Renderer INSTANCE;
		return INSTANCE;
	}
	Renderer(Renderer &&)      = delete;
	Renderer(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) = delete;
	Renderer &operator=(const Renderer &) = delete;

private:
	Renderer();
};
