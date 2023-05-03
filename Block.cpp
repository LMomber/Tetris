#include "Block.h"

Block::Block()
{}


Block::~Block()
{
}

const vec Block::GetPosition() const
{
	return core.pos;
}

void Block::SetPosition(const vec pos)
{
	core.pos = pos;
}

const int Block::GetSize() const
{
	return size;
}

int Block::FramePlus()
{
	if (frame < 3) frame++; 
	else frame = 0; 

	return frame; 
}

inline void Block::Collider()
{
	//Relies on the fact that the frame will always increment by 1. (core.pos)
	//core.pos and extra.pos are always the bottomleft corner of the colliders.
	//core is always the collider closest to the origin, with the origin being the leftbottom corner.
	//extra.pos always relies on core or origin.
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
			core.width = size * static_cast<int>(2 / 3);
			core.height = static_cast<int>(size / 3);
			core.pos = origin;		
									
			extra.width = size * static_cast<int>(2 / 3);
			extra.height = static_cast<int>(size / 3);
			extra.pos.x = core.pos.x + static_cast<int>(size / 3);
			extra.pos.y = core.pos.y - static_cast<int>(size / 3);
			break;
		case 1:
			core.width = static_cast<int>(size / 3);
			core.height = size * static_cast<int>(2 / 3);
			core.pos.y -= static_cast<int>(size / 3);

			extra.width = static_cast<int>(size / 3);
			extra.height = size * static_cast<int>(2 / 3);
			extra.pos.x = core.pos.x + static_cast<int>(size / 3);
			extra.pos.y = core.pos.y + static_cast<int>(size / 3);
			break;
		case 2:
			core.width = size * static_cast<int>(2 / 3);
			core.height = static_cast<int>(size / 3);

			extra.width = size * static_cast<int>(2 / 3);
			extra.height = static_cast<int>(size / 3);
			extra.pos.y = core.pos.y - (size * static_cast<int>(2 / 3));
			break;
		case 3:
			core.width = static_cast<int>(size / 3);
			core.height = size * static_cast<int>(2 / 3);
			core.pos.x += static_cast<int>(size / 3);

			extra.width = static_cast<int>(size / 3);
			extra.height = size * static_cast<int>(2 / 3);
			extra.pos.x = core.pos.x + static_cast<int>(size / 3);
			extra.pos.y = core.pos.y + (size * static_cast<int>(2 / 3));
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
	/*
		///////  ///////
		///////  ///////
		///////  ///////

		///////  ///////
		///////  ///////
		///////  ///////
	*/
		core.width = size * (2 / 3);
		core.height = size * (2 / 3);
		core.pos = origin;			
									
		extra.width = 0;			
		extra.height = 0;			
		extra.pos = origin;
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
			core.width = size;		
			core.height = size / 3;	
			core.pos.x = origin.x;	
			core.pos.y = origin.y - (size / 3);
												
			extra.width = size / 3;				
			extra.height = size / 3; 
			extra.pos.x = origin.x + (size / 3);
			extra.pos.y = origin.y;
			break;
		case 1:
			core.width = size / 3;
			core.height = size / 3;  

			extra.width = size / 3; 
			extra.height = size; 
			break;
		case 2: 
			core.width = size; 
			core.height = size / 3; 

			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x + (size / 3); 
			extra.pos.y = core.pos.y - (size / 3); 
			break;
		case 3:
			core.width = size / 3; 
			core.height = size; 
			core.pos.x += size / 3; 
			core.pos.y += size / 3; 
			 
			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x + (size / 3); 
			extra.pos.y = core.pos.y - (size / 3); 
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
			core.width = size * (2 / 3);
			core.height = size / 3;		
			core.pos.x = origin.x;		
			core.pos.y = origin.y - (size / 3);	
												
			extra.width = size * (2 / 3);		
			extra.height = size / 3;
			extra.pos.x = origin.x + (size / 3);
			extra.pos.y = origin.y;
			break;
		case 1:
			core.width = size / 3;
			core.height = size * (2 / 3);
			core.pos.y += size / 3;

			extra.width = size / 3;
			extra.height = size * (2 / 3);
			extra.pos.x = core.pos.x + (size / 3);
			extra.pos.y = core.pos.y - (size / 3);
			break;
		case 2:
			core.width = size * (2 / 3);
			core.height = size / 3;
			core.pos.y -= size * (2 / 3);

			extra.width = size * (2 / 3);
			extra.height = size / 3;
			break;
		case 3:
			core.width = size / 3;
			core.height = size * (2 / 3);
			core.pos.x += size / 3;
			core.pos.y += size * (2 / 3);

			extra.width = size / 3;
			extra.height = size * (2 / 3);
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
			core.width = sizeOfFive;				
			core.height = size / 3;					
			core.pos.x = origin.x;					
			core.pos.y = origin.y - (size * (2 / 3));
													
			extra.width = 0;						
			extra.height = 0;						
			extra.pos = origin;						
			break;									
		case 1:										
			core.width = size / 3;					
			core.height = sizeOfFive;				
			core.pos.x += size * (2 / 3);
			core.pos.y += size / 3;
			break;
		case 2:
			core.width = sizeOfFive; 
			core.height = size / 3;
			core.pos.x += size / 3;
			core.pos.y -= size / 3;
			break;
		case 3:
			core.width = size / 3;
			core.height = sizeOfFive; 
			core.pos.x += size / 3; 
			core.pos.y += size * (2 / 3); 
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
			core.width = size / 3;										
			core.height = size;								
			core.pos.x = origin.x + (size / 3);				
			core.pos.y = origin.y;							
									 						
			extra.width = size / 3;	 						
			extra.height = size / 3;	 												
			extra.pos.x = core.pos.x + (size / 3);			 							
			extra.pos.y = core.pos.y;													
			break;														
		case 1:
			core.width = size / 3; 
			core.height = size / 3; 
			extra.pos.x = core.pos.x - (size / 3);

			extra.width = size; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x; 
			extra.pos.y = core.pos.y - (size / 3); 
			break;
		case 2:
			core.width = size / 3; 
			core.height = size / 3; 
			core.pos.y -= size * (2 / 3); 

			extra.width = size / 3; 
			extra.height = size; 
			extra.pos.x = core.pos.x + (size / 3); 
			extra.pos.y = core.pos.y + (size * (2 / 3)); 
			break;
		case 3:
			core.width = size; 
			core.height = size / 3; 
			core.pos.y += size / 3;

			extra.width = size / 3; 
			extra.height = size / 3; 
			extra.pos.x = core.pos.x + (size * (2 / 3)); 
			extra.pos.y = core.pos.y - (size / 3); 
			break;
		}
		break;
	}
}
