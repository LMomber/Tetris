#pragma once

#include "Rectangle.h"

class Block
{
public:
	Block();
	~Block();

private:
	Shape::Rectangle blockCore;
	Shape::Rectangle blockExtra;
};

Block::Block()
{
}

Block::~Block()
{
}
