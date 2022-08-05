#include "Render.hpp"

Render::Render() :
	shouldRender(true)
{

}

Render::~Render() = default;

void Render::setShouldRender(bool newShouldRender)
{
	shouldRender = newShouldRender;
}

bool Render::getShouldRender() const
{
	return shouldRender;
}