#pragma once

template<typename T>
struct Vec3
{
	Vec3(const T& x_, const T& y_, const T& z_) : x(x_), y(y_), z(z_) {}

	Vec3<T> getNormalized() const
	{
		T norm2 = *this * *this;
		return std::sqrt(1. / norm2) * *this;
	}

	T x;
	T y;
	T z;
};

template<typename T>
Vec3<T> operator+(const Vec3<T>& op1, const Vec3<T>& op2)
{
	return Vec3<T>{op1.x + op2.x, op1.y + op2.y, op1.z + op2.z};
}

template<typename T>
Vec3<T> operator-(const Vec3<T>&op1, const Vec3<T>&op2)
{
	return Vec3<T>{op1.x - op2.x, op1.y - op2.y, op1.z - op2.z};
}

template<typename T>
Vec3<T> operator*(const T& op1, const Vec3<T>& op2)
{
	return Vec3<T>{op1 * op2.x, op1 * op2.y, op1 * op2.z};
}

template<typename T>
T operator*(const Vec3<T>& op1, const Vec3<T>& op2)
{
	return T{op1.x * op2.x + op1.y * op2.y + op1.z * op2.z};
}

template<typename T>
Vec3<T> operator^(const Vec3<T>& op1, const Vec3<T>& op2)
{
	return Vec3<T>{
		op1.y* op2.z - op1.z * op2.y
	  , op1.z* op2.x - op1.x * op2.z
	  , op1.x* op2.y - op1.y * op2.x
	};
}

template<typename T>
using Point3 = Vec3<T>;

template<typename T>
struct Ray
{
	Point3<T> origin;
	Vec3<T> direction;
};

template<typename T>
struct Sphere
{
	Point3<T> center;
	T radius;
};

template<typename T>
constexpr T NO_INTERSECTION() { return -1.; }

template<typename T>
T intersect(const Sphere<T>& sphere, const Ray<T>& ray)
{
	T a = ray.direction * ray.direction;
	T b = (ray.origin - sphere.center) * ray.direction * 2;
	T c = (ray.origin - sphere.center) * (ray.origin - sphere.center) - sphere.radius * sphere.radius;

	T delta = b * b - 4. * a * c;
	if (delta < 0)
		return NO_INTERSECTION<T>();

	T k1 = -b - std::sqrt(delta) / 2. / a;
	T k2 = -b + std::sqrt(delta) / 2. / a;

	if (k1 >= 0.)
		return k1;

	if (k2 >= 0.)
		return k2;

	return NO_INTERSECTION<T>();
}

