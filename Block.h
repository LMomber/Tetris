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

	const int GetCoreWidth() const;
	const int GetCoreHeight() const;
	const vec GetCorePos() const;
	const int GetExtraWidth() const;
	const int GetExtraHeight() const;
	const vec GetExtraPos() const;

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

	int one = size * 0.33;
	int two = size * 0.66;
	int three = size;
	int four = size * 1.33;
	int five = size * 1.66;

	vec origin{ 0, size }; //bottom left of 3x3 grid
	vec position{ origin };
};