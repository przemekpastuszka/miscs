#ifndef VECTOR3
#define VECTOR3

/* Struktura trzymaj¹ca trójwymiarowy wektor i pozwalaj¹ca
wykonywaæ na nim ró¿noraki operacje.
Oby siê przyda³o */

struct Vector3
{
	union
	{
		struct {float x, y, z;};
		struct {float r,g,b;};
	};
	
	//konstruktory
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

	//translacja
	const Vector3 Translate(const Vector3& a) const;
	const Vector3 Translate(float x,float y,float z) const;
	const Vector3 operator+(const Vector3& a) const;
	const Vector3 operator-() const;
	const Vector3 operator-(const Vector3& a) const;
	Vector3& operator+=(const Vector3& a);
	Vector3& operator-=(const Vector3& a);

	//skalowanie
	const Vector3 Scale(float s) const;
	const Vector3 Scale(float x,float y,float z) const;
	const Vector3 Scale(const Vector3& a) const;
	Vector3& operator*=(float b);
	Vector3& operator/=(float b);
	friend const Vector3 operator*(const Vector3& a,float b);
	friend const Vector3 operator/(const Vector3& a,float b);
	friend const Vector3 operator*(float b,const Vector3& a);
	friend const Vector3 operator/(float b,const Vector3& a);

	//rotacja
	const Vector3 RotateY(float angle) const; //obrót póki co tylko wokó³ osi Y

	//normalizacja i d³ugoœæ
	const Vector3 Normalize() const;
	float Length() const;
	float SqrLength() const;
	
	//statyczne
	static float Dot(const Vector3& a,const Vector3& b); //iloczyn skalarny
	static const Vector3 Cross(const Vector3& a,const Vector3& b); //iloczyn wektorowy

	static const Vector3 GetMin(const Vector3& a, const Vector3& b);
	static const Vector3 GetMax(const Vector3& a, const Vector3& b);
	static const Vector3 Clamp(const Vector3& a,float min,float max);
};

const Vector3 operator*(const Vector3& a,float b);
const Vector3 operator/(const Vector3& a,float b);
const Vector3 operator*(float b,const Vector3& a);
const Vector3 operator/(float b,const Vector3& a);

#endif