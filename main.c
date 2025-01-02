/*INSTITUTO POLITECNICO NACIONAL
ESCUELA SUPERIOR DE COMPUTO

Analisis y Diseno de Algoritmos
Grupo: 3CM3

Alumnos:
    Rubio Haro Diego

Profesor:
    LUNA BENOSO BEJAMIN

Fecha: 29-Diciembre-2024

Practica 7: Verifiacion en tiempo polinomial
    - Verficación Hamilton   */

// Librerias

#include "grafo.h"
#include "interface.h"

int selected_node = -1;
int rutaActiva = 0;
int grafoFijo = 0; // Parametro que verifica si está bloqueada la ventana
int width = 1280;
int height = 820;

void bloqGrafo(HWND hwnd);
void unlockGrafo(HWND hwnd);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Función principal main
int main()
{
    const char CLASS_NAME[] = "GraphEditor";
    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, "Error al registrar la clase de ventana", "Error", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindow(CLASS_NAME, "Rubio Haro Diego  3CM3", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                             CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                             NULL, NULL, wc.hInstance, NULL);

    if (!hwnd)
    {
        MessageBox(NULL, "Error al crear la ventana", "Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Procesador de mensajes de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    MEASUREITEMSTRUCT *measureItem;
    DRAWITEMSTRUCT *drawItem;

    switch (uMsg)
    {

    case WM_ERASEBKGND:
    {

        HDC hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);

        HBRUSH hBrush = CreateSolidBrush(RGB(240, 240, 240)); // Color gris claro
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);

        return 1; // Indica que el fondo ya se ha dibujado
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:                               // Opción "Limpiar"
            clearNodes();                     // Limpiar nodos y aristas
            InvalidateRect(hwnd, NULL, TRUE); // Solicitar redibujar la ventana
            UpdateWindow(hwnd);               // Asegurarse de que se actualice inmediatamente
            rutaActiva = 0;
            memset(route_nodes, 0, sizeof(route_nodes));
            node_count = 0;
            MessageBeep(MB_OK);
            break;
        case 2: // Opción "Bloquear"
            bloqGrafo(hwnd);
            MessageBeep(MB_OK);
            break;
        case 3: // Opción "Desbloquear"
            unlockGrafo(hwnd);
            MessageBeep(MB_OK);
            break;
        case 4: // Opción "Marcar rutas"
            rutaActiva = 1;

            MessageBeep(MB_OK);
            break;
        case 5: // Hamilton
            if (verificarHamilton(route_nodes, route_node_count))
            {
                MessageBox(hwnd, "Es Hamiltoniano", "Hamilton", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hwnd, "No es Hamiltoniano", "NO Hamiltoniano", MB_OK | MB_ICONINFORMATION);
            }

            MessageBeep(MB_OK);
            break;
        case 6: // Opción "Mostrar conexiones terminal"
            // ShowConnections();
            ShowRoute();
            ShowConnections();
            MessageBeep(MB_OK);
            break;
        case 7: // Opción "DSF"
            MessageBeep(MB_OK);
            break;
        default:
            break;
        }
        break;
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);

        int node_id = GetNodeAt(x, y);
        if (node_id == -1)
        {
            // Si estamos en "marcar ruta", no permitimos la creación de nuevos nodos
            if (node_count < MAX_NODES && CanCreateNode(x, y) && rutaActiva == 0)
            {
                nodes[node_count].x = x;
                nodes[node_count].y = y;
                node_count++;
                InvalidateRect(hwnd, NULL, TRUE);
            }
        }
        else
        {
            if (rutaActiva == 1) // Modo de "marcar ruta"
            {
                if (selected_node == -1)
                {
                    selected_node = node_id; // Seleccionar el primer nodo
                }
                else if (selected_node != node_id)
                {
                    // Buscar si ya existe una arista entre estos nodos
                    for (int i = 0; i < edge_count; i++)
                    {
                        if ((edges[i].start == selected_node && edges[i].end == node_id) ||
                            (edges[i].start == node_id && edges[i].end == selected_node))
                        {
                            // Cambiar el color de la arista para marcarla como parte de la ruta
                            edges[i].color = RGB(255, 0, 0); // Rojo para marcar la ruta

                            // Almacenar los nodos de la ruta en orden
                            if (route_node_count < MAX_NODES)
                            {
                                route_nodes[route_node_count] = selected_node;
                                route_node_count++;
                            }

                            break;
                        }
                    }

                    // Actualizar el nodo seleccionado
                    selected_node = -1;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else // No estamos en modo de marcar ruta
            {
                if (selected_node == -1)
                {
                    selected_node = node_id; // Seleccionar el primer nodo
                }
                else if (selected_node != node_id)
                {
                    // Crear una nueva arista si no estamos en modo de marcar ruta
                    edges[edge_count].start = selected_node;
                    edges[edge_count].end = node_id;
                    edges[edge_count].color = RGB(0, 0, 0); // Color predeterminado (negro)
                    edge_count++;

                    selected_node = -1;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
        }
        break;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);

        // Crear una pluma de color verde
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(137, 47, 254)); // 1 es el grosor de la línea
        HPEN oldPen = (HPEN)SelectObject(hdc, hPen);           // Seleccionar la pluma en el HDC

        // Dibujar aristas
        for (int i = 0; i < edge_count; i++)
        {
            int x1 = nodes[edges[i].start].x;
            int y1 = nodes[edges[i].start].y;
            int x2 = nodes[edges[i].end].x;
            int y2 = nodes[edges[i].end].y;

            // Seleccionar el color de la pluma dependiendo si la arista es parte de la ruta
            HPEN hPen;
            if (edges[i].color == RGB(255, 0, 0)) // Si es rojo, marcarla como parte de la ruta
            {
                hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Rojo para la ruta
            }
            else
            {
                hPen = CreatePen(PS_SOLID, 1, RGB(137, 47, 254)); // Otro color para las aristas normales
            }

            HPEN oldPen = (HPEN)SelectObject(hdc, hPen); // Seleccionar la pluma en el HDC

            // Mover al punto de inicio de la línea
            MoveToEx(hdc, x1, y1, NULL);
            // Dibujar la línea hacia el punto final
            LineTo(hdc, x2, y2);
        }

        // Dibujar nodos
        for (int i = 0; i < node_count; i++)
        {
            DrawNode(hdc, nodes[i].x, nodes[i].y, i);
        }

        // Restaurar la pluma original
        SelectObject(hdc, oldPen);
        DeleteObject(hPen); // Eliminar la pluma creada

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_CREATE:
        HRGN hRgn = CreateRoundRectRgn(0, 0, 1280, 720, 20, 20); // Región rectangular con esquinas redondeadas
        SetWindowRgn(hwnd, hRgn, TRUE);
        AddMenu(hwnd);
        break;

    case WM_MEASUREITEM:
        measureItem = (MEASUREITEMSTRUCT *)lParam;

        if (measureItem->CtlType == ODT_MENU)
        {
            measureItem->itemWidth = 150; // Ancho del menú
            measureItem->itemHeight = 50; // Alto del menú
        }
        break;

    case WM_DRAWITEM:
        drawItem = (DRAWITEMSTRUCT *)lParam;

        if (drawItem->CtlType == ODT_MENU)
        {
            // Dibujar fondo
            FillRect(drawItem->hDC, &drawItem->rcItem, (HBRUSH)(COLOR_MENU + 1));

            // Dibujar borde
            HPEN hPen = CreatePen(PS_INSIDEFRAME, 0, RGB(0, 0, 0)); // Borde negro
            HPEN oldPen = (HPEN)SelectObject(drawItem->hDC, hPen);
            HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
            HBRUSH oldBrush = (HBRUSH)SelectObject(drawItem->hDC, hBrush);

            Rectangle(drawItem->hDC, drawItem->rcItem.left, drawItem->rcItem.top, drawItem->rcItem.right, drawItem->rcItem.bottom);

            // Restaurar objetos
            SelectObject(drawItem->hDC, oldPen);
            DeleteObject(hPen);
            SelectObject(drawItem->hDC, oldBrush);

            // Dibujar texto
            DrawMenu(drawItem->hDC, drawItem->rcItem, (LPCSTR)drawItem->itemData);
        }
        break;

    case WM_DESTROY:
        if (hMenuFont)
        {
            DeleteObject(hMenuFont);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void bloqGrafo(HWND hwnd)
{

    grafoFijo = 1; // Cambiar el estado a bloqueado
    MessageBox(hwnd, "El grafo ha sido bloqueado y no se puede modificar.", "Bloquear", MB_OK | MB_ICONINFORMATION);
}
void unlockGrafo(HWND hwnd)
{
    rutaActiva = 0;
    grafoFijo = 0; // Cambiar el estado a bloqueado
    MessageBox(hwnd, "El grafo ha sido bloqueado y no se puede modificar.", "Bloquear", MB_OK | MB_ICONINFORMATION);
}
