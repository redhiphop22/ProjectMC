#pragma once

#include <time.h>
#include <ATLComTime.h>

#define SMALLDATE_STRING_SIZE		20
#define S2DATE32_YEAR_INIT			2000
#define S2DATE32_YEAR_MAX			(S2DATE32_YEAR_INIT + 63)
#define SUMMER_TIME_FOLLOW_SYSTEM	-1			// A value less than zero to have the C run-time library code compute whether standard time or daylight saving time is in effect.

enum S2_DATE_TYPE : uint32_t
{
	YEAR = 0xFC000000,
	MONTH = 0x3C00000,
	DAY = 0x3E0000,
	HOUR = 0x1F000,
	MINUTE = 0xFC0,
	SECOND = 0x3F,
};

enum class S2_DATE_ADD_TYPE : uint8_t
{
	SECOND,
	MINUTE,
	HOUR,
	DAY,
	MONTH,
	YEAR,
};

class S2Date32
{
public:
	S2Date32();
	S2Date32(uint32_t ui32Date);
	~S2Date32();

	int32_t					GetYear() { return ((m_ui32Date & S2_DATE_TYPE::YEAR) >> 26) + S2DATE32_YEAR_INIT; }
	int32_t					GetMonth() { return (m_ui32Date & S2_DATE_TYPE::MONTH) >> 22; }
	int32_t					GetDay() { return (m_ui32Date & S2_DATE_TYPE::DAY) >> 17; }
	int32_t					GetHour() { return (m_ui32Date & S2_DATE_TYPE::HOUR) >> 12; }
	int32_t					GetMinute() { return (m_ui32Date & S2_DATE_TYPE::MINUTE) >> 6; }
	int32_t					GetSecond() { return (m_ui32Date & S2_DATE_TYPE::SECOND); }
	uint32_t				GetDate() { return m_ui32Date; }

	inline void				GetSmallDate(char* strDate, int32_t i32Length);
	inline void				GetSmallDate(wchar_t* wstrDate, int32_t i32Length);
	inline uint32_t			GetDateTimeBin(uint32_t ui32DateType = (S2_DATE_TYPE::YEAR | S2_DATE_TYPE::MONTH | S2_DATE_TYPE::DAY | S2_DATE_TYPE::HOUR | S2_DATE_TYPE::MINUTE | S2_DATE_TYPE::SECOND));
	inline uint32_t			GetDateTimeYYYYMMDD();
	inline uint32_t			GetDateTimeYYMMDDHHMI();
	inline uint32_t			GetDateTimeMMDDHHMISS();

	void					Reset() { m_ui32Date = 0; }
	void					AddTime(S2_DATE_ADD_TYPE addType, int32_t i32Time);
	int32_t					DiffTime(S2Date32 dt32Date);

	//inline S2Date32& operator=(const SYSTEMTIME& pTime);
	inline S2Date32& operator=(const tm& pTime);
	inline S2Date32& operator=(const DATE& pTime);
	inline S2Date32& operator=(const uint32_t pTime);
	inline S2Date32& operator=(const S2Date32& pTime);

	friend inline BOOL		operator==(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date == dt32Date2.m_ui32Date);
	}
	friend inline BOOL		operator!=(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date != dt32Date2.m_ui32Date);
	}
	friend inline BOOL		operator<(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date < dt32Date2.m_ui32Date);
	}
	friend inline BOOL		operator<=(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date <= dt32Date2.m_ui32Date);
	}
	friend inline BOOL		operator>(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date > dt32Date2.m_ui32Date);
	}
	friend inline BOOL		operator>=(const S2Date32& dt32Date1, const S2Date32& dt32Date2)
	{
		return (dt32Date1.m_ui32Date >= dt32Date2.m_ui32Date);
	}

private:
	// 32 비트를 사용합니다.
	// 111111 1111 11111 11111 111111 111111
	// (년도) (월)  (일)  (시)   (분)   (초)
	uint32_t					m_ui32Date;
};

typedef S2Date32				date32_t;