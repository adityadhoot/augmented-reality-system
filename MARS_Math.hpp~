#ifndef MARS_MATH_H_
#define MARS_MATH_H_

#include<math.h>

template<class T> class vector3{
	public:
		vector3<T>(T x, T y, T z) { this->x = x; this->y = y; this->z = z;}
		vector3<T>(){this->x = this->y = this->z = 0;}
		vector3<T>(const vector3<T>& rhs) { this->x = rhs.x; this->y = rhs.y; this->z = rhs.z;}
		vector3<T>& operator=(const vector3<T>& rhs) { this->x = rhs.x; this->y = rhs.y; this->z = rhs.z;}
		T x, y, z;
		T dot(const vector3<T>& rhs) { return this->x*rhs.x + this->y*rhs.y + this->z*rhs.z;}
		vector3<T> cross(const vector3<T>& rhs) { vector3<T> a(this->y*rhs.z - this->z*rhs.y,this->z*rhs.x - this->x*rhs.z, this->x*rhs.y - this->y*rhs.x ); return a;} 
		vector3<T> operator-(const vector3<T>& rhs) { vector3<T> a(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); return a;}
		vector3<T> operator+(const vector3<T>& rhs) { vector3<T> a(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); return a;}
		vector3<T> operator*(const T& rhs) { vector3<T> a(this->x*rhs, this->y*rhs, this->z*rhs); return a;}
		vector3<T> operator/(const T& rhs) { vector3<T> a(this->x/rhs, this->y/rhs, this->z/rhs); return a;}
		void makeVertex(){ glVertex3f(this->x, this->y, this->z);}
		T norm() {return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);}
		void set(T x, T y, T z) { this->x = x; this->y = y; this->z = z;}
};
#endif
