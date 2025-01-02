#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>


#define SIZE_CIRCLE 45

// Variables globales para la interfaz
extern HMENU hMenu;
extern HFONT hMenuFont;

void DrawNode(HDC hdc, int x, int y, int id);
void DrawMenu(HDC hdc, RECT rect, LPCSTR text);
void AddMenu(HWND hwnd);

#endif
