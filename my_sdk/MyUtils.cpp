#include "stdafx.h" /*put the line at the top of all your .cpp files or disable precompiled headers in your project.*/
#include "MyUtils.h"

MyUtils::MyUtils(const TCHAR *pPath):m_pFile(NULL)
{
	beginWriting(pPath);
}


MyUtils::~MyUtils(void)
{
	endWriting();
}

// Writing utility functions

void MyUtils::beginWriting(const TCHAR *pPath)
{
	// Check we are not currently writing
	assert(!m_pFile);

	//errno_t lErr = fopen_s(&m_pFile, pPath, "wb");
	//m_pFile = _tfopen(pPath,_T("wb")); 
	_wfopen_s(&m_pFile, pPath, _T("wb")); 
}

void MyUtils::endWriting()
{
	assert(m_pFile);

	fclose(m_pFile);
	m_pFile = NULL;
}

void MyUtils::writeFun(int indent, const char* pMsg, ...)
{
	assert(m_pFile);

	// first, write in the indent
	for (int i = 0; i < indent; i++) fprintf_s(m_pFile, "\t");

	// Write the message, passing in our variable
	// parameters to a function that wraps printf
	va_list vargs;
	va_start(vargs, pMsg);
	vfprintf_s(m_pFile, pMsg, vargs);
	va_end(vargs);

	// Finish the line
	fprintf_s(m_pFile, "\n");
}

void MyUtils::writeFun(int indent, const wchar_t* pMsg, ...)
{
	assert(m_pFile);

	// first, write in the indent
	for (int i = 0; i < indent; i++) fprintf_s(m_pFile, "\t");

	// Write the message, passing in our variable
	// parameters to a function that wraps printf
	va_list vargs;
	va_start(vargs, pMsg);
	vfwprintf(m_pFile, pMsg, vargs);
	va_end(vargs);

	// Finish the line
	fprintf_s(m_pFile, "\n");
}


#define BUFFER_SIZE 100
std::string
MyUtils::widechar2str(const wchar_t* pWchar)
{
	char* pBuffer = (char*)malloc(BUFFER_SIZE);
	size_t   i;
	memset(pBuffer, '\0', sizeof pBuffer);
	//std::wcstombs(buffer, pWchar, sizeof(buffer));
	wcstombs_s(&i, pBuffer, (size_t)BUFFER_SIZE, pWchar, (size_t)BUFFER_SIZE );

	std::string str(pBuffer);
	return str;
}

wchar_t * 
	MyUtils::UTF8ToUnicode( const char* str )
{
     int textlen ;
     wchar_t * result;
     textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
     MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen ); 
     return result; 
}

char * 
	MyUtils::UnicodeToUTF8( const wchar_t* str )
{
     char* result;
     int textlen;
     textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
     result =(char *)malloc((textlen+1)*sizeof(char));
     memset(result, 0, sizeof(char) * ( textlen + 1 ) );
     WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
     return result;
}