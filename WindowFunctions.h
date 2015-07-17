#ifndef WINDOWFUNCTIONS_H
#define WINDOWFUNCTIONS_H

#include "WindowFunctions.h"
#include <SDL_syswm.h>
#include <string>

string ExePath();

void AddStringToClipboard(const std::string &s);

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

#endif