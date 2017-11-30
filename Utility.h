#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <D3D11.h>
#include <assert.h>
#include "dxerr.h"
#include <vector>
#include <xnamath.h>
#include <d3dcompiler.h>
#include <D3DX11async.h>

inline void Print(char* str)
{
	std::cout << str << std::endl;
}

template <class Interface>
inline void SafeRelease(Interface** ppInter)
{
	if (*ppInter != NULL)
	{
		(*ppInter)->Release();

		(*ppInter) = NULL;
	}
}

template <class Pointer>
inline void SafeDelete(Pointer* p)
{
	if (p != NULL)
	{
		delete p;
		p = NULL;
	}
}

template <class Pointer>
inline void SafeDeleteArray(Pointer* p, int size)
{
	if (p != NULL)
	{
		delete[] p;
		for(int i = 0; i < size; ++i)
			p[i] = NULL;
	}
}

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(WFILE, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 