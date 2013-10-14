/*
HellTrace by Przemys³aw Pastuszka
University of Wroc³aw, Poland

3D vector and useful operations
*/

#ifndef VECTOR3
#define VECTOR3

struct Vector3
{
	union
	{
		struct {float x, y, z;};
		struct {float r,g,b;};
	};
	
	//constructors
	Vector3() {}
	Vector3(float init)
	{
		x=y=z=init;
	}
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	//translation
	const Vector3 Translate(const Vector3& a) const;
	const Vector3 Translate(float x,float y,float z) const;
	const Vector3 operator+(const Vector3& a) const;
	const Vector3 operator-() const;
	const Vector3 operator-(const Vector3& a) const;
	Vector3& operator+=(const Vector3& a);
	Vector3& operator-=(const Vector3& a);

	//scaling
	const Vector3 Scale(float s) const;
	const Vector3 Scale(float x,float y,float z) const;
	const Vector3 Scale(const Vector3& a) const;
	Vector3& operator*=(float b);
	Vector3& operator/=(float b);
	friend const Vector3 operator*(const Vector3& a,float b);
	friend const Vector3 operator/(const Vector3& a,float b);
	friend const Vector3 operator*(float b,const Vector3& a);
	friend const Vector3 operator/(float b,const Vector3& a);

	//rotation
	const Vector3 RotateY(float angle) const; //obrót póki co tylko wokó³ osi Y

	//normalizing and calculating length
	const Vector3 Normalize() const;
	float Length() const;
	float SqrLength() const;
	
	//logic operators
	bool operator==(const Vector3& v) const
	{	return x==v.x&&y==v.y&&z==v.z;}
	bool operator!=(const Vector3& v) const
	{	return !(*this==v);}

	//static methods
	static float Dot(const Vector3& a,const Vector3& b); //dot product
	static const Vector3 Cross(const Vector3& a,const Vector3& b); //cross product

	//clamping
	static const Vector3 GetMin(const Vector3& a, const Vector3& b);
	static const Vector3 GetMax(const Vector3& a, const Vector3& b);
	static const Vector3 Clamp(const Vector3& a,float min,float max);
};

const Vector3 operator*(const Vector3& a,float b);
const Vector3 operator/(const Vector3& a,float b);
const Vector3 operator*(float b,const Vector3& a);
const Vector3 operator/(float b,const Vector3& a);

#endif