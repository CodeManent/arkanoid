#ifndef __POINT3_HPP__
#define __POINT3_HPP__

#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>

template <class T> class point3{
public:
	T x;
	T y;
	T z;

	point3():x(0), y(0), z(0){}
	point3(const T &x): x(x), y(0), z(0) {}
	point3(const T &x, const T &y):x(x), y(y), z(0) {}
	point3(const T &x, const T &y, const T &z):x(x), y(y), z(z){}
	point3(const T arr[3]):x(arr[0]), y(arr[1]), z(arr[2]){}
	point3(const point3<T> &p2):x(p2.x), y(p2.y), z(p2.z){}
	
	float length() const						{	return sqrt(x*x + y*y + z*z);	}
	float distance(const point3<T> &p2) const	{	return (*this - p2).length();	}

	point3<T>& normalise(){
		*this /= length();
		return *this;
	}

	point3<T>& fast_normalise(){
		const float &isqrt = InvSqrt(x*x + y*y + z*z);
		*this *= isqrt;
		return *this;
	}

	T dot(const point3<T> &p2) const				{	return (x*p2.x + y*p2.y + z*p2.z);									}
	point3<T> cross(const point3<T> &p2) const		{	return point3<T>(y*p2.z - z*p2.y, z*p2.x - x*p2.z, x*p2.y - y*p2.x);}

	point3<T>& operator+=(const point3<T> &p2)		{	x += p2.x; y += p2.y; z += p2.z; return *this;						}
	point3<T>& operator-=(const point3<T> &p2)		{	x -= p2.x; y -= p2.y; z -= p2.z; return *this;						}
	point3<T>& operator*=(const T &d)				{	x *= d; y *=d; z *=d; return *this;									}
	point3<T> operator+(const point3<T> &p2) const	{	return point3<T>(x+p2.x, y+p2.y, z+p2.z);							}
	point3<T> operator-(const point3<T> &p2) const	{	return point3<T>(x-p2.x, y-p2.y, z-p2.z);							}
	point3<T> operator-() const						{	return point3<T>(-x, -y, -z);										}
	point3<T> operator*(const T &d) const			{	return point3<T>(x*d, y*d, z*d);									}
	point3<T> operator*(const point3<T> &p2) const	{	return point3<T>(x*p2.x, y*p2.y, z*p2.z);							}
	point3<T>& operator=(const point3<T> &p2)		{	x = p2.x; y = p2.y; z = p2.z; return *this;							}
	bool operator==(const point3<T> &p2) const		{	return ((x == p2.x) && (y == p2.y) && (z == p2.z))? true : false;	}
	bool operator!=(const point3<T> &p2) const		{	return !(*this == p2);												}
	bool operator< (const point3<T> &p2) const		{	return ((x < p2.x)  && (y < p2.y)  && (z < p2.z)) ? true : false;	}
	bool operator< (const T &d) const				{	return ((x < d)     && (y < d)     && (z < d))    ? true : false;	}
	bool operator<= (const point3<T> &p2) const		{	return ((x <= p2.x) && (y <= p2.y) && (z <= p2.z))? true : false;	}
	bool operator<= (const T &d) const				{	return ((x <= d)    && (y <= d)    && (z <= d))   ? true : false;	}
	bool operator> (const point3<T> &p2) const		{	return ((x > p2.x)  && (y > p2.y)  && (z > p2.z)) ? true : false;	}
	bool operator> (const T &d) const				{	return ((x > d)     && (y > d)     && (z > d))    ? true : false;	}
	bool operator>= (const point3<T> &p2) const		{	return ((x >= p2.x) && (y >= p2.y) && (z >= p2.z))? true : false;	}
	bool operator>= (const T &d) const				{	return ((x >= d)    && (y >= d)    && (z >= d))   ? true : false;	}
	point3<T> operator/(const point3<T> &p2) const
	{
		if(!(p2.x && p2.y && p2.z)) throw(std::runtime_error("Division By Zero"));
		return point3<T>(x/p2.x, y/p2.y, z/p2.z);
	}
	point3<T> operator/(const T &d) const
	{
		if(!d) throw(std::runtime_error("Division By Zero"));
		return point3<T>(x/d, y/d, z/d);
	}
	point3<T>& operator/=(const T &d)
	{
		if(!d) throw(std::runtime_error("Division By Zero"));
		x /= d; y /=d; z /=d; return *this;
	}
	std::string toString() const{
		std::stringstream ss;
		ss << '(' << x << ", " << y << ", " << z << ')';
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

typedef point3<double>	point3d;
typedef point3<float>	point3f;
typedef point3<int>		point3i;
typedef point3<unsigned int>	point3ui;

static const point3d zeroPoint3d;
static const point3d  onePoint3d(1.0f, 1.0f, 1.0f);

static const point3f zeroPoint3f;
static const point3f  onePoint3f(1.0f, 1.0f, 1.0f);

static const point3i zeroPoint3i;
static const point3i  onePoint3i(1, 1, 1);

static const point3ui zeroPoint3ui;
static const point3ui  onePoint3ui(1, 1, 1);

#endif
