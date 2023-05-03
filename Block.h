#pragma once

#include "Rectangle.h"

class Block
{
public:
	enum Color
	{
		Blue,
		Green, 
		Red,
		Orange,
		Yellow,
		Purple,
		LightBlue
	};

	Block();

	~Block();

	const vec GetPosition() const;
	void SetPosition(const vec pos);

	const int GetSize() const;

	int FramePlus();

	void Collider();

private:
	Color color;
	Shape::Rectangle core;
	Shape::Rectangle extra;

	int frame{ 0 };
	int size = 100;
	int sizeOfFive = size + size * (2 / 3);
	vec origin{ 0, 100 }; //bottom left of 3x3 grid
};