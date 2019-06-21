/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Grafo.h
 * Author: silver
 *
 * Created on 29 de Março de 2019, 23:40
 */
#include <queue>
#include<stack>
#include<vector>
#include <list>
#ifndef GRAFO_H
#define GRAFO_H
#define grafoMatriz 1
#define grafoLista  2
#include <iostream>
#include <jsoncpp/json/json.h>
using namespace std;

class Grafo {
public:
    
    Grafo(int tipoDeGrafo); //feito
    Grafo(const Grafo& orig);
    virtual     ~Grafo();//feito
    void        insereAresta(int a, int b);//feito
    int         insereVertice();//feito
    string      imprimeGrafo();//feito
    void        criaGrafo(int vertices, int mAresta, int ** arestas);//feito
    void        criaGrafoJSON(int nVertices, int mArestas, Json::Value raiz);
    
//algoritmos de busca    
    void        buscaEmLargura();
    vector<int> BuscaEmProfundidade();
    vector<int> vertVizinhos(int v);
    void        RemoveAresta(int v1, int v2);
    void        RemoveVertice(int v);



   friend ostream& operator<<(ostream& strm,  Grafo& g); 
    int tipoGrafo;
 
private:
    
    int ** matriz;
    int nVertices, mArestas;
    list<int> *listaAdj; // apontará para um vetor com ponteiro pro inicio das listas 
    //lista de adjacencia
    stack<int>pilhaDeBusca;
    queue<int>filaDeBusca;
    vector<int>visitados; //vertices visitados
    int **exploreadas;//arestas exploradas
    int **descobertas;//arestas descobertas    
    
    
};

#endif /* GRAFO_H */

