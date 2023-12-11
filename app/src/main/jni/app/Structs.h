#ifndef Structs_h
#define Structs_h

#include "Memory.h"

struct Vector2 
{
    float X;
    float Y;

    Vector2() {
        this->X = 0;
        this->Y = 0;
    }

    Vector2(float x, float y) {
        this->X = x;
        this->Y = y;
    }

    static Vector2 Zero() {
        return Vector2(0.0f, 0.0f);
    }

    static float Distance(Vector2 a, Vector2 b) {
        Vector2 vector = Vector2(a.X - b.X, a.Y - b.Y);
        return sqrt((vector.X * vector.X) + (vector.Y * vector.Y));
    }

    bool operator!=(const Vector2 &src) const {
        return (src.X != X) || (src.Y != Y);
    }

    Vector2 &operator+=(const Vector2 &v) {
        X += v.X;
        Y += v.Y;
        return *this;
    }

    Vector2 &operator-=(const Vector2 &v) {
        X -= v.X;
        Y -= v.Y;
        return *this;
    }
};

struct Vector3 
{
    float X;
    float Y;
    float Z;

    Vector3() {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    Vector3(float x, float y, float z) {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

    Vector3 operator+(const Vector3 &v) const {
        return Vector3(X + v.X, Y + v.Y, Z + v.Z);
    }

    Vector3 operator-(const Vector3 &v) const {
        return Vector3(X - v.X, Y - v.Y, Z - v.Z);
    }

    bool operator==(const Vector3 &v) {
        return X == v.X && Y == v.Y && Z == v.Z;
    }

    bool operator!=(const Vector3 &v) {
        return !(X == v.X && Y == v.Y && Z == v.Z);
    }

    static Vector3 Zero() {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    static float Dot(Vector3 lhs, Vector3 rhs) {
        return (((lhs.X * rhs.X) + (lhs.Y * rhs.Y)) + (lhs.Z * rhs.Z));
    }

    static Vector3 OneVector() {
        return Vector3(1.0f, 1.0f, 1.0f);
    }

    /*static float Distance(Vector3 a) {
        Vector3 vector = Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        return sqrt(((vector.X * vector.X) + (vector.Y * vector.Y)) + (vector.Z * vector.Z));
    }*/

    float Distance(Vector3 b) {
        Vector3 vector = Vector3(this->X - b.X, this->Y - b.Y, this->Z - b.Z);
        return sqrt(((vector.X * vector.X) + (vector.Y * vector.Y)) + (vector.Z * vector.Z));
    }
};

struct FRotator 
{
    float Pitch;
    float Yaw;
    float Roll;
};

struct FMatrix 
{
    float M[4][4];

    float *operator[](int index) {
        return M[index];
    }

    const float* operator[](int index) const {
        return M[index];
    }
};

struct Quat
{
	float X;
	float Y;
	float Z;
	float W;
};


struct FTransform
{
	Quat Rotation;
	Vector3 Translation;
	float chunk;
	Vector3 Scale3D;
};

typedef struct ObjectName
{
    char Data[64];
};

struct MinimalViewInfo
{
     Vector3 Location;
     FRotator Rotation;
     float FOV;
};

struct SetValue {
	int mode;
	int type;
    float px;
    float py;
};


#endif
