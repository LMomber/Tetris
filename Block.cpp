#include "Block.h"
#include <iostream>

Block::Block(Color color): 
	color{color}
{
}

Block::~Block()
{}

const vec Block::GetPosition() const
{
	return position;
}

void Block::SetPosition(const vec pos)
{
	position = pos;
}

const int Block::GetSize() const
{
	return size;
}

const int Block::GetSizeOne() const
{
	return one;
}

const int Block::GetSizeOfRed() const
{
	return two; 
}

const int Block::GetSizeOfPurple() const
{
	return five; 
}

const int Block::GetCoreWidth() const
{
	return core.width;
}

const int Block::GetCoreHeight() const
{
	return core.height;
}

const vec Block::GetCorePos() const
{
	return core.pos;
}

const int Block::GetExtraWidth() const
{
	return extra.width;
}

const int Block::GetExtraHeight() const
{
	return extra.height;
}

const vec Block::GetExtraPos() const
{
	return extra.pos;
}

const int Block::GetFrame() const
{
	return frame; 
}

void Block::SetColor(const Block::Color color)
{
	this->color = color;
}

const Block::Color Block::GetColor() const
{
	return color;
}

void Block::SetGridPos(vec gridPos)
{
	this->gridPos = gridPos;
}

const vec Block::GetGridPos() const
{
	return gridPos;
}

int Block::FramePlus()
{
	if (color == Red) frame = 0;
	else
	{
		if (frame < 3) frame++; 
		else frame = 0; 
	}

	return frame; 
}

void Block::Collider()
{
	//Relies on the fact that the frame will always increment by 1. (core.pos)
	//core.pos and extra.pos are always the bottomleft corner of the colliders.
	//core is always the collider closest to the origin, with the origin being the leftbottom corner. 
	//If core & extra are equally as far from the origin, most left is core.
	//extra.pos always relies on core.pos.
	switch (color) {
	case Color::Blue:
	/*
		         ///////  ///////
		         ///////  ///////
		         ///////  ///////

		///////  ///////
		///////  ///////
		///////  ///////
	*/
		switch (frame) {
		case 0:
			core.width = one;
			core.height = two;
			core.pos.y = origin.y - one;

			extra.width = one;
			extra.height = two;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y + one;
			break;
		case 1:
			core.width = two;
			core.height = one;

			extra.width = two;
			extra.height = one;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y - one;
			break;
		case 2:
			core.width = one;
			core.height = two;
			core.pos.x = core.pos.x + one;
			core.pos.y = core.pos.y;

			extra.width = one;
			extra.height = two;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y + one;
			break;
		case 3:
			core.width = two;
			core.height = one;
			core.pos.x -= one;

			extra.width = two;
			extra.height = one;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y - one;
			break;
		}
		break;
	case Color::Green:
	/*
		         ///////
		         ///////
		         ///////

		         ///////
		         ///////
		         ///////

		///////  ///////
		///////  ///////
		///////  ///////
	*/
		switch (frame) {
		case 0:
			core.width = one;	
			core.height = one;	
			core.pos = origin;		
									
			extra.width = one;	
			extra.height = three;	
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y;
			break;					
		case 1:
			core.width = three;
			core.height = one;
			core.pos.y -= one;

			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x;
			extra.pos.y = core.pos.y - one;
			break;
		case 2:
			core.width = one;
			core.height = three;
			core.pos.x += one; 
			core.pos.y += one; 

			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y - two;
			break;
		case 3:
			core.width = three; 
			core.height = one;
			core.pos.x -= one; 
			core.pos.y -= two; 

			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x + two;
			extra.pos.y = core.pos.y + one;
			break;
		}
		break;
	case Color::Red:
	/*
		///////  ///////
		///////  ///////
		///////  ///////

		///////  ///////
		///////  ///////
		///////  ///////
	*/
		core.width = two; 
		core.height = two; 
		core.pos.x = origin.x;
		core.pos.y = origin.y - one;			
									
		extra.width = 0;			
		extra.height = 0;			
		extra.pos = core.pos; 
		break;

	case Color::Orange: 
	/*
		///////  ///////  ///////
		///////  ///////  ///////
		///////  ///////  ///////

		         ///////
		         ///////
		         ///////
	*/
		switch (frame) {
		case 0:
			core.width = one;		
			core.height = one;	
			core.pos.x = origin.x;	
			core.pos.y = origin.y - one;
												
			extra.width = one;				
			extra.height = three; 
			extra.pos.x = origin.x + one;
			extra.pos.y = origin.y;
			break;
		case 1:
			core.width = three;
			core.height = one;  

			extra.width = one; 
			extra.height = one;
			extra.pos.y = core.pos.y - one;
			break;
		case 2: 
			core.width = one; 
			core.height = three; 
			core.pos.x += one;
			core.pos.y += one;

			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x + one; 
			extra.pos.y = core.pos.y - one;
			break;
		case 3:
			core.width = three; 
			core.height = one; 
			core.pos.x -= one; 
			core.pos.y -= two; 
			 
			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x + one; 
			extra.pos.y = core.pos.y + one; 
			break;
		}
		break;
	case Color::Yellow: 
	/*
		///////  ///////
		///////  ///////
		///////  ///////

		         ///////  ///////
		         ///////  ///////
		         ///////  ///////
	*/
		switch (frame) {
		case 0:
			core.width = one;
			core.height = two;		
			core.pos.x = origin.x;		
			core.pos.y = origin.y;	
												
			extra.width = one;		
			extra.height = two;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y - one;
			break;
		case 1:
			core.width = two;
			core.height = one;
			core.pos.y -= two;

			extra.width = two;
			extra.height = one;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y + one;
			break;
		case 2:
			core.width = one;
			core.height = two;
			core.pos.x += one;
			core.pos.y += two;

			extra.width = one;
			extra.height = two;
			extra.pos.x = core.pos.x + one;
			break;
		case 3:
			core.width = two;
			core.height = one;
			core.pos.x -= one;
			core.pos.y -= two;

			extra.width = two;
			extra.height = one;
			extra.pos.x = core.pos.x + one;
			extra.pos.y = core.pos.y + one;
			break;
		}
		break;
	case Color::Purple:
	/*
		///////
		///////
		///////

		///////
		///////
		///////

		///////
		///////
		///////

		///////
		///////
		///////
	*/
		switch (frame) {
		case 0:
			core.width = one;
			core.height = four;
			core.pos.x = origin.x + two;
			core.pos.y = static_cast<int>(origin.y * 1.66);

			extra.width = 0;
			extra.height = 0;
			extra.pos = core.pos;
			break;
		case 1:
			core.width = four;
			core.height = one;
			core.pos.x -= one;
			core.pos.y -= two;

			extra.pos = core.pos;
			break;
		case 2:
			core.width = one;
			core.height = four;
			core.pos.x += one;
			core.pos.y += two;

			extra.pos = core.pos;
			break;
		case 3:
			core.width = four;
			core.height = one;
			core.pos.x -= two; 
			core.pos.y -= two;

			extra.pos = core.pos;
			break;
		}
		break;
	case Color::LightBlue: 
	/*
		///////
		///////
		///////

		///////
		///////
		///////

		///////  ///////
		///////  ///////
		///////  ///////
	*/
		switch (frame) {
		case 0:
			core.width = one;										
			core.height = three;								
			core.pos.x = origin.x + one;				
			core.pos.y = origin.y;							
									 						
			extra.width = one;	 						
			extra.height = one;	 												
			extra.pos.x = core.pos.x + one;			 							
			extra.pos.y = core.pos.y;													
			break;														
		case 1:
			core.width = one; 
			core.height = one; 
			core.pos.x = core.pos.x - one;
			core.pos.y -= one;

			extra.width = three; 
			extra.height = one; 
			extra.pos.x = core.pos.x; 
			extra.pos.y = core.pos.y - one; 
			break;
		case 2:
			core.width = one; 
			core.height = one; 
			core.pos.y -= one; 

			extra.width = one; 
			extra.height = three; 
			extra.pos.x = core.pos.x + one; 
			extra.pos.y = core.pos.y + two; 
			break;
		case 3:
			core.width = three; 
			core.height = one; 
			core.pos.y += one;

			extra.width = one; 
			extra.height = one; 
			extra.pos.x = core.pos.x + two; 
			extra.pos.y = core.pos.y - one; 
			break;
		}
		break;
	}
}

void Block::WallCollision(const int& left, const int& right, const int& bottom)
{
	//Left collision
	if ((position.x + core.pos.x) <= left) position.x = left - core.pos.x;

	//Right collision
	if ((position.x + extra.pos.x + extra.width) >= right) position.x = (right - extra.pos.x - extra.width);
	if ((position.x + core.pos.x + core.width) >= right) position.x = (right - core.pos.x - core.width);

	//Bottom collision
	if (position.y + extra.pos.y >= bottom) position.y = bottom - extra.pos.y;
	if (position.y + core.pos.y >= bottom) position.y = bottom - core.pos.y;
}

bool Block::onGround(const int& bottom)
{
	//Bottom collision
	if (position.y + extra.pos.y >= bottom)
		return true;
	if (position.y + core.pos.y >= bottom)
		return true;

	return false;
}
