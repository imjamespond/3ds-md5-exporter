#pragma once
#include <windows.h>/* va_start...*/
#include <assert.h>/* assert */
//#include <wchar.h>/* FILE, */
#include <tchar.h>/* _T, TCHAR */
#include <string>/* std::string */
//#include <stdlib.h>     /* wcstombs, wchar_t(C) */
class MyUtils
{
public:
	MyUtils(const TCHAR *pPath);
	~MyUtils(void);

	// Our output section.  Because we are a simple text writer,
	// we are simply going to pipe our messages to a text file.
	FILE* m_pFile;

	void beginWriting(const TCHAR *pPath);
	void writeFun(int indent, const char* pMsg, ...);
	void writeFun(int indent, const wchar_t* pMsg, ...);

	static std::string widechar2str(const wchar_t*); //mind this function returns r-value, the reference will be destroied outside...
	static wchar_t * UTF8ToUnicode( const char* str );
	static char * UnicodeToUTF8( const wchar_t* str );

private:
	void endWriting();
};

extern float gPrecision;

inline float truncf(float f)
{
	if(gPrecision>1000000.0f)
		return f;
	return static_cast<float>(static_cast<int>(f*gPrecision))/gPrecision;
}
inline float truncf(double f)
{
	return truncf(static_cast<float>(f));
}