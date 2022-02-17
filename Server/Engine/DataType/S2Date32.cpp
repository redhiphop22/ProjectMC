#include "S2Engine.h"
#include "S2Date32.h"

S2Date32::S2Date32()
{
	m_ui32Date = 0;
}

S2Date32::S2Date32(uint32_t ui32Date)
{
	m_ui32Date = ui32Date;
}

S2Date32::~S2Date32()
{
}

//S2Date32& S2Date32::operator=(const SYSTEMTIME& pTime)
//{
//	int32_t i32Year = pTime.wYear;
//	if (S2DATE32_YEAR_MAX < i32Year)
//		i32Year = S2DATE32_YEAR_MAX;
//
//	m_ui32Date = ((i32Year - S2DATE32_YEAR_MAX) << 26) |
//		(pTime.wMonth << 22) |
//		(pTime.wDay << 17) |
//		(pTime.wHour << 12) |
//		(pTime.wMinute << 6) |
//		pTime.wSecond;
//	return *this;
//}

S2Date32& S2Date32::operator=(const tm& pTime)
{
	int32_t i32Year = pTime.tm_year;
	if (S2DATE32_YEAR_MAX < i32Year)
		i32Year = S2DATE32_YEAR_MAX;

	m_ui32Date = (((i32Year + 1900) - S2DATE32_YEAR_INIT) << 26) |
		((pTime.tm_mon + 1) << 22) |
		(pTime.tm_mday << 17) |
		(pTime.tm_hour << 12) |
		(pTime.tm_min << 6) |
		pTime.tm_sec;
	return *this;
}

S2Date32& S2Date32::operator=(const DATE& pTime)
{
	COleDateTime oleDate(pTime);

	int32_t i32Year = oleDate.GetYear();
	if (S2DATE32_YEAR_MAX < i32Year)
		i32Year = S2DATE32_YEAR_MAX;

	m_ui32Date = ((i32Year - S2DATE32_YEAR_INIT) << 26) |
		(oleDate.GetMonth() << 22) |
		(oleDate.GetDay() << 17) |
		(oleDate.GetHour() << 12) |
		(oleDate.GetMinute() << 6) |
		oleDate.GetSecond();

	return *this;
}

S2Date32& S2Date32::operator=(const uint32_t pTime)
{
	m_ui32Date = pTime;
	return *this;
}

S2Date32& S2Date32::operator=(const S2Date32& pTime)
{
	m_ui32Date = pTime.m_ui32Date;
	return *this;
}

uint32_t S2Date32::GetDateTimeBin(uint32_t ui32DateType)
{
	return (m_ui32Date & ui32DateType);
}

uint32_t S2Date32::GetDateTimeYYYYMMDD()
{
	return GetYear() * 10000 + GetMonth() * 100 + GetDay();
}

uint32_t S2Date32::GetDateTimeYYMMDDHHMI()
{
	return (GetYear() % 100) * 100000000 + GetMonth() * 1000000 + GetDay() * 10000 + GetHour() * 100 + GetMinute();
}

uint32_t S2Date32::GetDateTimeMMDDHHMISS()
{
	return GetMonth() * 100000000 + GetDay() * 1000000 + GetHour() * 10000 + GetMinute() * 100 + GetSecond();
}

void S2Date32::GetSmallDate(char* strDate, int32_t i32Length)
{
	//S2String::Format(strDate, i32Length, "%04d-%02d-%02d %02d:%02d:%02d",
//		GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
}

void S2Date32::GetSmallDate(wchar_t* wstrDate, int32_t i32Length)
{
	//S2String::Format(wstrDate, i32Length, L"%04d-%02d-%02d %02d:%02d:%02d",
	//	GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
}

void S2Date32::AddTime(S2_DATE_ADD_TYPE addType, INT32 i32Time)
{
	struct tm oldTime;
	oldTime.tm_year = (GetYear() - 1900) + (S2_DATE_ADD_TYPE::YEAR == addType ? i32Time : 0);
	oldTime.tm_mon = GetMonth() - 1 + (S2_DATE_ADD_TYPE::MONTH == addType ? i32Time : 0);
	oldTime.tm_mday = GetDay() + (S2_DATE_ADD_TYPE::DAY == addType ? i32Time : 0);
	oldTime.tm_hour = GetHour() + (S2_DATE_ADD_TYPE::HOUR == addType ? i32Time : 0);
	oldTime.tm_min = GetMinute() + (S2_DATE_ADD_TYPE::MINUTE == addType ? i32Time : 0);
	oldTime.tm_sec = GetSecond() + (S2_DATE_ADD_TYPE::SECOND == addType ? i32Time : 0);
	oldTime.tm_isdst = SUMMER_TIME_FOLLOW_SYSTEM;
	mktime(&oldTime);
	this->operator=(oldTime);
}

int32_t S2Date32::DiffTime(S2Date32 dt32Date)
{
	struct tm tmTime1;

	tmTime1.tm_year = dt32Date.GetYear() - 1900;
	tmTime1.tm_mon = dt32Date.GetMonth() - 1;
	tmTime1.tm_mday = dt32Date.GetDay();
	tmTime1.tm_hour = dt32Date.GetHour();
	tmTime1.tm_min = dt32Date.GetMinute();
	tmTime1.tm_sec = dt32Date.GetSecond();
	tmTime1.tm_isdst = SUMMER_TIME_FOLLOW_SYSTEM;

	time_t tm_tTime1 = mktime(&tmTime1);

	struct tm tmTime2;

	tmTime2.tm_year = GetYear() - 1900;
	tmTime2.tm_mon = GetMonth() - 1;
	tmTime2.tm_mday = GetDay();
	tmTime2.tm_hour = GetHour();
	tmTime2.tm_min = GetMinute();
	tmTime2.tm_sec = GetSecond();
	tmTime2.tm_isdst = SUMMER_TIME_FOLLOW_SYSTEM;

	time_t tm_tTime2 = mktime(&tmTime2);

	return (int32_t)(difftime(tm_tTime2, tm_tTime1));
}