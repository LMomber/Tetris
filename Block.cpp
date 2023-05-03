#include "Block.h"

Block::Block(Color color):
	color{color}
{}


Block::~Block()
{
}

inline void Block::Collider()
{
	//Relies on the fact that the frame will always increment by 1. (core.pos)
	//core.pos and extra.pos are always the bottomleft corner of the colliders.
	//core is always the collider closest to the origin, with the origin being the leftbottom corner.
	switch (color) {
	case Color::Blue:
		switch (frame) {
		case 0:
			core.width = size * (2 / 3);
			core.height = size / 3;
			core.pos = origin;

			extra.width = size * (2 / 3);
			extra.height = size / 3;
			extra.pos.x = core.pos.x + (size / 3);
			extra.pos.y = core.pos.y - (size / 3);
			break;
		case 1:
			core.width = size / 3;
			core.height = size * (2 / 3);
			core.pos.y -= size / 3;

			extra.width = size / 3;
			extra.height = size * (2 / 3);
			extra.pos.x = core.pos.x + (size / 3);
			extra.pos.y = core.pos.y + (size / 3);
			break;
		case 2:
			core.width = size * (2 / 3); 
			core.height = size / 3; 

			extra.width = size * (2 / 3); 
			extra.height = size / 3;  
			extra.pos.y = core.pos.y - (size * (2 / 3)); 
			break;
		case 3:
			core.width = size / 3; 
			core.height = size * (2 / 3); 
			core.pos.x += size / 3; 

			extra.width = size / 3; 
			extra.height = size * (2 / 3); 
			extra.pos.x = core.pos.x + (size / 3); 
			extra.pos.y = core.pos.y + (size * (2 / 3)); 
			break;
		}
		break;
	case Color::Green:
		switch (frame) {
		case 0:
			core.width = size / 3;
			core.height = size / 3;
			core.pos = origin;

			extra.width = size / 3;
			extra.height = size;
			extra.pos.x = core.pos.x + (size / 3);
			extra.pos.y = core.pos.y;
			break;
		case 1:
			core.width = size;
			core.height = size / 3;
			core.pos.y -= size / 3;

			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x;
			extra.pos.y = core.pos.y - (size / 3);
			break;
		case 2:
			core.width = size / 3;
			core.height = size;
			core.pos.x += size / 3; 
			core.pos.y += size / 3; 

			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x + (size / 3);
			extra.pos.y = core.pos.y - (size * (2 / 3));
			break;
		case 3:
			core.width = size; 
			core.height = size / 3;
			core.pos.x -= size / 3; 
			core.pos.y -= size / 3; 

			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x + (size * (2 / 3));
			extra.pos.y = core.pos.y + (size / 3);
			break;
		}
		break;
	case Color::Red:
		core.width = size * (2 / 3);
		core.height = size * (2 / 3);
		core.pos = origin;
		break;
	case Color::Orange: 
		break;
	case Color::Yellow: 
		break;
	case Color::LightBlue: 
		break;
	}
}
