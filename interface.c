#include "interface.h"
#include "grafo.h"
#include <stdio.h>


HMENU hMenu;
HFONT hMenuFont;
// Función para dibujar un nodo
void DrawNode(HDC hdc, int x, int y, int id)
{
    // Crear un pincel vacío para el relleno (transparente)
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // Color blanco para el relleno, puede ser transparente si lo prefieres

    // Crear un pincel para el borde (por ejemplo, color negro)
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(227, 72, 245)); // Borde negro, con grosor de 2 px

    // Guardar el pincel y la pluma actuales
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    // Dibujar la elipse (solo el borde)
    Ellipse(hdc, x - SIZE_CIRCLE, y - SIZE_CIRCLE, x + SIZE_CIRCLE, y + SIZE_CIRCLE);

    // Restaurar los objetos originales
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);

    // Liberar los recursos creados
    DeleteObject(hBrush);
    DeleteObject(hPen);

    SetTextColor(hdc, RGB(227, 72, 245)); // Color del texto (blanco)
    SetBkMode(hdc, TRANSPARENT);          // Fondo transparente

    // Cambiar la fuente si lo deseas
    HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT oldFont = SelectObject(hdc, hFont); // Seleccionar la nueva fuente

    // Dibujar el texto (número del nodo)
    char label[5];
    sprintf(label, "%d", id);

    SIZE textSize;
    GetTextExtentPoint32A(hdc, label, strlen(label), &textSize);

    // Calcular la posición para centrar el texto
    int textX = x - textSize.cx / 2; // Centrado horizontal
    int textY = y - textSize.cy / 2; // Centrado vertical

    // Dibujar el texto centrado
    TextOut(hdc, textX, textY, label, strlen(label));

    // Restaurar la fuente original
    SelectObject(hdc, oldFont);
    DeleteObject(hFont); // Liberar la fuente creada
}

// Funcion que dibuja el menu
void DrawMenu(HDC hdc, RECT rect, LPCSTR text)
{
    SetBkMode(hdc, TRANSPARENT);     // Fondo transparente
    SelectObject(hdc, hMenuFont);    // Usar la fuente personalizada
    SetTextColor(hdc, RGB(0, 0, 0)); // Color del texto

    // Dibujar el texto centrado en el rectángulo
    DrawText(hdc, text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

// Funcion que crea los menus
void AddMenu(HWND hwnd)
{
    hMenu = CreateMenu();

    AppendMenu(hMenu, MF_OWNERDRAW, 1, "Limpiar");
    AppendMenu(hMenu, MF_OWNERDRAW, 2, "Bloquear");
    AppendMenu(hMenu, MF_OWNERDRAW, 3, "Desbloquear");
    AppendMenu(hMenu, MF_OWNERDRAW, 4, "Marcar Rutas");
    AppendMenu(hMenu, MF_OWNERDRAW, 5, "Ejecutar Hamilton");
    AppendMenu(hMenu, MF_OWNERDRAW, 6, "Mostrar Conexiones");
    AppendMenu(hMenu, MF_OWNERDRAW, 7, "RUBIO HARO DIEGO 3CM3");

    SetMenu(hwnd, hMenu);

    // Crear una fuente más grande para el menú
    LOGFONT lf = {0};
    lf.lfHeight = 15;                  // Tamaño de la fuente
    lf.lfWeight = FW_NORMAL;           // Grosor de la fuente
    strcpy(lf.lfFaceName, "Segoe UI"); // Familia de fuente

    hMenuFont = CreateFontIndirect(&lf);
}
