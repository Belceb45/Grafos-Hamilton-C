#ifndef GRAFO_H
#define GRAFO_H

#include<windows.h>

#define MAX_NODES 100
#define MAX_EDGES 200
#define SIZE_CIRCLE 45
#define MIN_DISTANCE (SIZE_CIRCLE * 2)// Distancia mínima para evitar la superposición de nodos
// Estructura para representar un nodo
typedef struct
{
    int x, y; // Coordenadas del nodo
} Node;

// Estructura para representar una arista
typedef struct
{
    int start, end; // Índices de los nodos conectados
    COLORREF color;
} Edge;

// Variables globales del grafo
extern Node nodes[MAX_NODES];
extern Edge edges[MAX_EDGES];
extern int node_count;
extern int edge_count;
extern int route_nodes[MAX_NODES];
extern int route_node_count;
// Declaraciones de funciones
int GetNodeAt(int x, int y);
int CanCreateNode(int x, int y);
void AddNode(int x, int y);
void AddEdge(int start, int end);
int verificarHamilton(int ruta[], int size);
void ShowConnections();
void ShowRoute();
void clearNodes();



#endif
