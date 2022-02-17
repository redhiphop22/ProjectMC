#pragma once

#define FLT_SUB(x,y) (x>y?x-y:y-x)

class S2Vector3
{
public:
	S2Vector3();
	S2Vector3(float x, float y, float z);
	S2Vector3(float pos[3]);
	S2Vector3(S2Vector3& vector);

	float X() { return m_pos[0]; }
	float Y() { return m_pos[1]; }
	float Z() { return m_pos[2]; }

	friend inline bool operator==(const S2Vector3& v1, const S2Vector3& v2)
	{
		return (FLT_SUB(v1.m_pos[0], v2.m_pos[0]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[1], v2.m_pos[1]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[2], v2.m_pos[2]) < FLT_EPSILON);
	}
	friend inline bool operator!=(const S2Vector3& v1, const S2Vector3& v2)
	{
		return !(FLT_SUB(v1.m_pos[0], v2.m_pos[0]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[1], v2.m_pos[1]) < FLT_EPSILON &&
			FLT_SUB(v1.m_pos[2], v2.m_pos[2]) < FLT_EPSILON);
	}
private:
	float m_pos[3];
};

typedef S2Vector3				vector3_t;