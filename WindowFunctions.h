#ifndef WINDOWFUNCTIONS_H
#define WINDOWFUNCTIONS_H


void AddStringToClipboard(const std::string &s);

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

#endif