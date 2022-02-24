#pragma once

#if defined(CONSOLE_DEBUG) || defined(_DEBUG)
int DebugWrite(LPCSTR Format, ...);
int DebugLog(LPCSTR Id, LPCSTR Format, ...);
#else
inline int DebugWrite(LPCSTR Format, ...) { return 0; }
inline int DebugLog(LPCSTR Id, LPCSTR Format, ...) { return 0; }
#endif