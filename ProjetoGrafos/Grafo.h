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
struct ArestasPeso{
    int v1;
    int v2;
    int peso;
};

class Grafo {
public:
    
    Grafo(int tipoDeGrafo); //feito
    Grafo(const Grafo& orig);
    virtual     ~Grafo();//feito
   
    //manipulação de arquivos JSON
    void        criaGrafo(int vertices, int mAresta, int ** arestas);//feito
    void        criaGrafoJSON(int nVertices, int mArestas, Json::Value raiz);
    
    //Info do grafo
    string      imprimeGrafo();//feito
    vector<int> vertVizinhos(int v);
    
     int        getNumAresta();
    friend ostream& operator<<(ostream& strm,  Grafo& g); 
    int tipoGrafo;
    
    //manipulacao do grafo    
    void        RemoveAresta(int v1, int v2);
    void        RemoveVertice(int v);
    void        insereAresta(int a, int b);//feito
    int         insereVertice();//feito
//algoritmos de busca    
    void        buscaEmLargura(int raiz);
    void        BuscaEmProfundidade(int raiz);
   
    void        buscaCompleta(int r);
    bool        conectividade();
    bool        temCiclo();
    bool        isFloresta();
    bool        isArvore();
    void        ObterFlorestaGeradora();    
    void        BuscaEmProfundidadeRec(int raiz,bool flag=true);   
    vector<int>* determinarDistancias(int raiz);
    
//caminhos minimos       
    void         dijkstra(int orig);
    int procuraPeso(int v1, int v2);
    //void Floyd(int **arestas, int n, int mArestas);
    
    // estrutura de dados publica.
    vector<bool>* visitados; //vertices visitados
    list<int *> arestasExploradas ;//arestas exploradas
    list<int *> arestasDescobertas; //arestas descobertas
 
private:
    
    int ** matriz;
    int nVertices, mArestas;
    list<int> *listaAdj;
    ArestasPeso *pesoArestas;// apontará para um vetor com ponteiro pro inicio das listas 
    //lista de adjacencia
    
    
       
    
    
};

#endif /* GRAFO_H */

