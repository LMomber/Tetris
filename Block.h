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

	Block(Color color);

	~Block();

	const vec GetPosition() const;
	void SetPosition(const vec pos);

	const int GetSize() const;
	const int GetSizeOfRed() const;
	const int GetSizeOfPurple() const;

	const int GetFrame() const;

	int FramePlus();

	void Collider();
	void WallCollision();

private:
	Color color{ Blue };
	Shape::Rectangle core{ 0 };
	Shape::Rectangle extra{ 0 };

	int frame{ 0 };
	int size = 100;
	int sizeOfRed = size * 0.66;
	int sizeOfPurple = size * 1.66;
	vec origin{ 0, size }; //bottom left of 3x3 grid
	vec position{ origin };
};