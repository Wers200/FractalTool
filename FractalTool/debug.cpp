#include "framework.h"

#if defined(CONSOLE_DEBUG) || defined(_DEBUG)
int DebugWrite(LPCSTR Format, ...) {
	va_list vl;
	va_start(vl, Format);

	int result = vprintf_s(Format, vl);
	if (result > 0) {
		char* buf = new char[result + 1];
		vsprintf_s(buf, result + 1, Format, vl);
		OutputDebugStringA(buf);
		delete[] buf;
	}

	va_end(vl);

	return result;
}

int DebugLog(LPCSTR Id, LPCSTR Format, ...) {
	va_list vl;
	va_start(vl, Format);

	std::string Result = "[" + std::string(Id) + "]: " + std::string(Format) + "\n";

	int result = vprintf_s(Result.c_str(), vl);
	if (result > 0) {
		char* buf = new char[result + 1];
		vsprintf_s(buf, result + 1, Result.c_str(), vl);
		OutputDebugStringA(buf);
		delete[] buf;
	}

	va_end(vl);

	return result;
}
#endif