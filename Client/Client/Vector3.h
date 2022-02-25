#pragma once

class Vector3
{
	Vector3(float x, float y, float z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}
public:
	float					X()	{	return m_pos[0];	}
	float					Y()	{	return m_pos[1];	}
	float					Z()	{	return m_pos[2];	}
	void					Set(float x, float y, float z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}

private:
	float					m_pos[3];
};