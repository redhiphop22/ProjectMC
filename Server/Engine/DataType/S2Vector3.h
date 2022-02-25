#pragma once

#define FLT_SUB(x,y) (x>y?x-y:y-x)

class S2Vector3
{
public:
	S2Vector3();
	S2Vector3(float x, float y, float z);
	S2Vector3(float pos[3]);
	S2Vector3(S2Vector3& vector);
	S2Vector3(const S2Vector3& vector);

	float X() { return m_pos[0]; }
	float Y() { return m_pos[1]; }
	float Z() { return m_pos[2]; }
	void Reset();
	void Set(float x, float y, float z);

	friend bool operator==(const S2Vector3& v1, const S2Vector3& v2)
	{
		return (FLT_SUB(v1.m_pos[0], v2.m_pos[0]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[1], v2.m_pos[1]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[2], v2.m_pos[2]) < FLT_EPSILON);
	}
	friend bool operator!=(const S2Vector3& v1, const S2Vector3& v2)
	{
		return !(FLT_SUB(v1.m_pos[0], v2.m_pos[0]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[1], v2.m_pos[1]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[2], v2.m_pos[2]) < FLT_EPSILON);
	}
	friend S2Vector3 operator+(const S2Vector3& v1, const S2Vector3& v2)
	{
		float x = v1.m_pos[0] + v2.m_pos[0];
		float y = v1.m_pos[1] + v2.m_pos[1];
		float z = v1.m_pos[2] + v2.m_pos[2];
		return S2Vector3(x, y, z);
	}
	friend S2Vector3 operator-(const S2Vector3& v1, const S2Vector3& v2)
	{
		float x = v1.m_pos[0] - v2.m_pos[0];
		float y = v1.m_pos[1] - v2.m_pos[1];
		float z = v1.m_pos[2] - v2.m_pos[2];
		return S2Vector3(x, y, z);
	}
	friend float operator*(const S2Vector3& v1, const S2Vector3& v2)
	{
		float x = v1.m_pos[0] * v2.m_pos[0];
		float y = v1.m_pos[1] * v2.m_pos[1];
		float z = v1.m_pos[2] * v2.m_pos[2];
		return x + y + z;
	}

	// 길이
	static float MagnitudePow(const S2Vector3& v)
	{ 
		return static_cast<float>(pow(v.m_pos[0], 2) + pow(v.m_pos[1], 2) + pow(v.m_pos[2], 2));
	}
	static float Magnitude(const S2Vector3& v)
	{ 
		return sqrt(MagnitudePow(v));
	}
	float MagnitudePow()
	{ 
		return S2Vector3::MagnitudePow(*this);
	}
	float Magnitude()
	{ 
		return S2Vector3::Magnitude(*this);
	}
	
	// 단위백터
	static S2Vector3 Normalrize(const S2Vector3& v)
	{ 
		S2Vector3 normal; 
		float magnitude = S2Vector3::Magnitude(v); 
		normal.m_pos[0] = v.m_pos[0] / magnitude;
		normal.m_pos[1] = v.m_pos[1] / magnitude;
		normal.m_pos[2] = v.m_pos[2] / magnitude;
		return normal; 
	} 
	void Normalrize()
	{ 
		*this = S2Vector3::Normalrize(*this);
	}

	// 외적
	static S2Vector3 CrossProduct(const S2Vector3& v1, const S2Vector3& v2)
	{ 
		S2Vector3 cross;
		cross.m_pos[0] = v1.m_pos[1]*v2.m_pos[2] - v1.m_pos[2]*v2.m_pos[1];
		cross.m_pos[1] = v1.m_pos[2]*v2.m_pos[0] - v1.m_pos[0]*v2.m_pos[2];
		cross.m_pos[2] = v1.m_pos[0]*v2.m_pos[1] - v1.m_pos[1]*v2.m_pos[0]; 
		return cross; 
	}

	// 각도
	static float InnerCos(const S2Vector3& v1, const S2Vector3& v2)
	{ 
		float cos = (v1 * v2) / ((S2Vector3::Magnitude(v1)*S2Vector3::Magnitude(v2))); 
		return cos; 
	}

private:
	float m_pos[3];
};

typedef S2Vector3				vector3_t;