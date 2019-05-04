/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: silver
 *
 * Created on 29 de Março de 2019, 23:30
 */
#include <iostream>
#include <cstdlib>
#include <string>
#include <jsoncpp/json/json.h> //biblioteca de manipulação de arquivos json
#include "Grafo.h"
#include <fstream>  

using namespace std;

/*
 * 
 */
void abreArq(string s, Grafo *g){
     int n=0,m=0;
    fstream arq(s);
    Json::Value JSONRaiz;

    arq >> JSONRaiz;
    cout << JSONRaiz["nome"];
    n = JSONRaiz["vertices"].size(); 
    m = JSONRaiz["arestas"].size();
    cout<<m;
 
    g->criaGrafoJSON(n,m,JSONRaiz);
    
    arq.close();
}

int main() {
    Grafo *g1 = new Grafo(grafoMatriz);// grafo do tipo matriz
    Grafo *g2 = new Grafo(grafoLista);// grafo do tipo lista
    int v1=0,v2=0; // numero de vertices . Vértice 1 e vértice 2 da aresta
  
    //abre arquivo JSON e joga no objeto grafo
    abreArq("dados/grafoN5.json",  g1);
    
    //imprime o grafo
    cout << *g1 ;
   
    
//    cout << "digite o numero de vértices " << endl ;
//    cin >> n;
//    g1->criaGrafo(n,0,NULL); // cria grafo com 0 arestas
//    cout << "digite as arestas separadas por espaços.Quando acabar, ou se não houver aresta, digite -1 ." << endl ;
//    cin >> v1 ;
//    while(v1!=-1){
//        cin >> v2;
//
//        g1->insereAresta(v1,v2);
//                
//        cin >> v1 ;
//    }
     
    
    return 0;
}   