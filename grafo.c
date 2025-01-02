#include "grafo.h"
#include <stdio.h>
#include <math.h>
#include <windows.h>

Node nodes[MAX_NODES];
Edge edges[MAX_EDGES];
int node_count;
int edge_count;
int route_nodes[MAX_NODES];
int route_node_count = 0;


int GetNodeAt(int x, int y)
{
    for (int i = 0; i < node_count; i++)
    {
        int dx = x - nodes[i].x;
        int dy = y - nodes[i].y;
        if (sqrt(dx * dx + dy * dy) <= SIZE_CIRCLE)
        { // Dentro del radio del nodo
            return i;
        }
    }
    return -1;
}

int CanCreateNode(int x, int y)
{
    for (int i = 0; i < node_count; i++)
    {
        int dx = x - nodes[i].x;
        int dy = y - nodes[i].y;
        if (sqrt(dx * dx + dy * dy) < MIN_DISTANCE)
        {             // Si la distancia es menor a la mínima permitida
            return 0; // No se puede crear el nodo
        }
    }
    return 1; // Se puede crear el nodo
}

void AddNode(int x, int y)
{
    if (node_count < MAX_NODES)
    {
        nodes[node_count].x = x;
        nodes[node_count].y = y;
        node_count++;
    }
}

void AddEdge(int start, int end)
{
    if (edge_count < MAX_EDGES)
    {
        edges[edge_count].start = start;
        edges[edge_count].end = end;
        edge_count++;
    }
}

void ShowConnections()
{
    printf("\n\n");
    // Recorremos todas las aristas
    for (int i = 0; i < edge_count; i++)
    {
        int start = edges[i].start;
        int end = edges[i].end;

        // Imprimimos las conexiones entre nodos
        printf("Conexion entre Nodo %d (%d, %d) y Nodo %d (%d, %d)\n",
               start, nodes[start].x, nodes[start].y,
               end, nodes[end].x, nodes[end].y);
    }
}

void ShowRoute()
{
    printf("\nRuta marcada:\n");
    for (int i = 0; i < route_node_count; i++)
    {
        printf("%d", route_nodes[i]);
        if (i < route_node_count - 1)
        {
            printf(" -> ");
        }
    }
    printf("\n");
    printf("\nn: %d",route_node_count);
}


void clearNodes()
{
    node_count = 0;
    edge_count = 0;

    for (int i = 0; i < MAX_NODES; i++)
    {
        nodes[i].x = 0;
        nodes[i].y = 0;
    }

    for (int i = 0; i < MAX_EDGES; i++)
    {
        edges[i].start = 0;
        edges[i].end = 0;
    }
}

// Función para verificar si el certificado es un ciclo Hamiltoniano
int verificarHamilton(int ruta[], int size)
{
    // Verificar que no haya nodos repetidos en la ruta
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (ruta[i] == ruta[j]) // Si un nodo se repite
            {
                printf("Nodo repetido: %d\n", ruta[i]);
                return 0; // No es un ciclo Hamiltoniano
            }
        }
    }

    // Verificar que haya una arista entre cada par de nodos consecutivos en la ruta
    for (int i = 0; i < size - 1; i++)
    {
        int start = ruta[i];
        int end = ruta[i + 1];
        int existeArista = 0;

        // Comprobar si hay una arista entre start y end
        for (int j = 0; j < edge_count; j++)
        {
            if ((edges[j].start == start && edges[j].end == end) ||
                (edges[j].start == end && edges[j].end == start))
            {
                existeArista = 1; // Existe una arista entre los nodos
                break;
            }
        }

        if (!existeArista) // Si no hay arista entre los nodos consecutivos
        {
            printf("No existe arista entre %d y %d\n", start, end);
            return 0; // No es un ciclo Hamiltoniano
        }
    }

    // Verificar que haya una arista entre el último y el primer nodo (para cerrar el ciclo)
    int start = ruta[size - 1];
    int end = ruta[0];
    int existeArista = 0;
    for (int j = 0; j < edge_count; j++)
    {
        if ((edges[j].start == start && edges[j].end == end) ||
            (edges[j].start == end && edges[j].end == start))
        {
            existeArista = 1; // Existe una arista entre el último y el primer nodo
            break;
        }
    }

    if (!existeArista) // Si no hay arista entre el último y el primer nodo
    {
        printf("No existe arista entre %d y %d (último y primer nodo)\n", start, end);
        return 0; // No es un ciclo Hamiltoniano
    }

    return 1; // Es un ciclo Hamiltoniano
}