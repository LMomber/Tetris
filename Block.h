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
		LightBlue,
		All
	};

	Block(Color color);

	~Block();

	const vec GetPosition() const;
	void SetPosition(const vec pos);

	const int GetSize() const;
	const int GetSizeOne() const;
	const int GetSizeOfRed() const;
	const int GetSizeOfPurple() const;

	const int GetCoreWidth() const;
	const int GetCoreHeight() const;
	const vec GetCorePos() const;

	const int GetExtraWidth() const;
	const int GetExtraHeight() const;
	const vec GetExtraPos() const;

	const int GetFrame() const;

	void SetColor(const Block::Color color);
	const Color GetColor() const;

	int FramePlus();

	void Collider();
	void WallCollision(const int& left, const int& right, const int& bottom);

	bool onGround(const int& bottom);

private:
	Color color{ Blue };
	Shape::Rectangle core{ 0 };
	Shape::Rectangle extra{ 0 };

	int frame = 0;
	int size = 90;

	int one = size / 3;
	int two = one * 2; 
	int three = size;
	int four = one * 4;
	int five = one * 5;

	vec origin{ 0, size }; //bottom left of 3x3 grid
	vec position{ 0 };
};