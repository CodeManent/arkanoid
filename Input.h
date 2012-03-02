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
		Y,
		Character
	} type;

	struct Value{
		enum ButtonValue{
			ButtonDown,
			ButtonUp
		}buttonValue;
		float rangeValue;
		int charValue;
	}value;
};


#endif
