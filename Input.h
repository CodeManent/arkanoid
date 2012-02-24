#ifndef Input_H
#define Input_H

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
		Y
	} type;

	union Value{
		enum ButtonValue{
			ButtonDown,
			ButtonUp
		}buttonValue;
		float rangeValue;
	}value;
};


#endif
