#pragma once

#include "template.h"

/************************************
	Structure for making a rectangle.
*************************************/

struct vec
{
	int x{ 0 };
	int y{ 0 };
};

namespace Shape
{
	struct Rectangle
	{
		int width{ 0 };
		int height{ 0 };
		vec pos{ 0 };
	};
}