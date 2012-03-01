#ifndef __POINT2_HPP__
#define __POINT2_HPP__

#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>

template <class T> class point2{
public:
	T x;
	T y;

	point2():x(0), y(0){}
	point2(const T &x): x(x), y(0){}
	point2(const T &x, const T &y):x(x), y(y){}
	point2(const T arr[2]):x(arr[0]), y(arr[1]){}
	point2(const point2<T> &p2):x(p2.x), y(p2.y){}
	
	float length() const							{	return sqrt(x*x + y*y);								}
	float distance(const point2<T> &p2) const		{	return (*this - p2).length();						}
	point2<T> normal()const							{	return *this/length();								}
	point2<T> fast_normal()const					{	return *this*InvSqrt(x*x + y*y);					}
	point2<T>& normalise()							{	return *this /= length();							}
	point2<T>& fast_normalise()						{	return *this *= InvSqrt(x*x + y*y);					}
	T dot(const point2<T> &p2) const				{	return x*p2.x + y*p2.y;								}

	point2<T>& operator+=(const point2<T> &p2)		{	x += p2.x; y += p2.y; return *this;					}
	point2<T>& operator-=(const point2<T> &p2)		{	x -= p2.x; y -= p2.y; return *this;					}
	point2<T>& operator*=(const T &d)				{	x *= d; y *=d; return *this;						}
	point2<T> operator+(const point2<T> &p2) const	{	return point2<T>(x+p2.x, y+p2.y);					}
	point2<T> operator-(const point2<T> &p2) const	{	return point2<T>(x-p2.x, y-p2.y);					}
	point2<T> operator-() const						{	return point2<T>(-x, -y);							}
	point2<T> operator*(const T &d) const			{	return point2<T>(x*d, y*d);							}
	point2<T> operator*(const point2<T> &p2) const	{	return point2<T>(x*p2.x, y*p2.y);					}
	point2<T>& operator=(const point2<T> &p2)		{	x = p2.x; y = p2.y; return *this;					}
	bool operator==(const point2<T> &p2) const		{	return ((x == p2.x) && (y == p2.y))? true : false;	}
	bool operator!=(const point2<T> &p2) const		{	return !(*this == p2);								}
	bool operator< (const point2<T> &p2) const		{	return ((x < p2.x)  && (y < p2.y)) ? true : false;	}
	bool operator< (const T &d) const				{	return ((x < d)     && (y < d))    ? true : false;	}
	bool operator<= (const point2<T> &p2) const		{	return ((x <= p2.x) && (y <= p2.y))? true : false;	}
	bool operator<= (const T &d) const				{	return ((x <= d)    && (y <= d))   ? true : false;	}
	bool operator> (const point2<T> &p2) const		{	return ((x > p2.x)  && (y > p2.y)) ? true : false;	}
	bool operator> (const T &d) const				{	return ((x > d)     && (y > d))    ? true : false;	}
	bool operator>= (const point2<T> &p2) const		{	return ((x >= p2.x) && (y >= p2.y))? true : false;	}
	bool operator>= (const T &d) const				{	return ((x >= d)    && (y >= d))   ? true : false;	}
	point2<T> operator/(const point2<T> &p2) const
	{
		if(!(p2.x && p2.y)) throw(std::runtime_error("Division by zero"));
		return point2<T>(x/p2.x, y/p2.y);
	}
	point2<T> operator/(const T &d) const{
		if(!d) throw(std::runtime_error("Division by zero"));
		return point2<T>(x/d, y/d);
	}
	point2<T>& operator/=(const T &d)
	{
		if(!d) throw(std::runtime_error("Division by zero"));
		x /= d; y /=d; return *this;
	}
	std::string toString() const{
		std::stringstream ss;
		ss << '(' << x << ", " << y << ')';
		return ss.str();
	}

private:
	/*
		Fast invert square square root
		http://en.wikipedia.org/wiki/Fast_inverse_square_root
	*/
	static float InvSqrt (float x){
		float xhalf = 0.5f*x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i>>1);
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x);
		return x;
	}
};

typedef point2<double>	point2d;
typedef point2<float>	point2f;
typedef point2<int>		point2i;
typedef point2<unsigned int>	point2ui;

static const point2d zeroPoint2d;
static const point2d  onePoint2d(1.0f, 1.0f);

static const point2f zeroPoint2f;
static const point2f  onePoint2f(1.0f, 1.0f);

static const point2i zeroPoint2i;
static const point2i  onePoint2i(1, 1);

static const point2ui zeroPoint2ui;
static const point2ui  onePoint2ui(1, 1);

#endif
