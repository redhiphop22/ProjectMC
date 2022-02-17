#include "S2Engine.h"
#include "S2Vector3.h"

S2Vector3::S2Vector3()
{
	m_pos[0] = 0.f;
	m_pos[1] = 0.f;
	m_pos[2] = 0.f;
}

S2Vector3::S2Vector3(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

S2Vector3::S2Vector3(float pos[3])
{
	m_pos[0] = pos[0];
	m_pos[1] = pos[1];
	m_pos[2] = pos[2];
}

S2Vector3::S2Vector3(S2Vector3& vector)
{
	m_pos[0] = vector.m_pos[0];
	m_pos[1] = vector.m_pos[1];
	m_pos[2] = vector.m_pos[2];
}