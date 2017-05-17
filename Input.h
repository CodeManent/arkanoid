#ifndef Input_H
#define Input_H

#include "point2.h"

struct Input
{
	enum Type{
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		A,
		B,
		X,
		Y,
		Character,
		MouseClick
	} type;

	struct Value{
		enum ButtonValue{
			ButtonDown,
			ButtonUp
		}buttonValue;
		float rangeValue;
		int charValue;
		point2i pointerPosition;
	}value;
};


#endif
