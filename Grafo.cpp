/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Grafo.cpp
 * Author: silver
 * 
 * Created on 29 de Março de 2019, 23:40
 */
#include <string>
#include "Grafo.h"
#include <list>
#include<vector>
#include <iostream>
#include <iterator>



#define grafoMatriz 1
#define grafoLista  2
using namespace std;

Grafo::Grafo(int tipoDeGrafo) {
    this->tipoGrafo = tipoDeGrafo;
}

Grafo::Grafo(const Grafo& orig) {
    
}
// limpa a memória
Grafo::~Grafo() {
    if(this->tipoGrafo == grafoMatriz ){
        for (int i = 0; i < nVertices; i++){
           delete[] this->matriz[i];
        }
        delete[] this->matriz;
    }
    if(this->tipoGrafo == grafoLista ){
        for (int i = 0; i < nVertices; i++){
           delete[] this->matriz[i];
        }
        delete[] this->matriz;
    }
}

void Grafo::insereAresta(int a, int b){
     if(tipoGrafo == grafoMatriz ){
         //preenche apenas a parte de cima da matriz
            if(a <= b){ 
                this->matriz[a][b] = 1 ;
            }else{
                this->matriz[b][a] = 1 ;
            }
     }else if (tipoGrafo == grafoLista){
         this->listaAdj[a].push_back(b) ;
         this->listaAdj[b].push_back(a) ;
     }
}
// realoca a matriz usando uma auxiliar e depois  
int Grafo::insereVertice(){
    this->nVertices++;
    if(tipoGrafo == grafoMatriz ){
        int ** matrizAux = new int *[this->nVertices];
            for(int i = 0; i < this->nVertices; i++){
                matrizAux[i] = new int[this->nVertices];
            }
        for(int i = 0; i < this->nVertices-1; i++){
           for(int j = 0; j < this->nVertices-1; j++){
               matrizAux[i][j] = this->matriz[i][j];  
            }
           delete[] matriz[i] ;
        }
      delete[] this->matriz;  
      this->matriz = matrizAux;
    }
    else if(tipoGrafo == grafoLista){
        list<int> *listAux = new list<int>[this->nVertices];
        for(int i = 0; i < nVertices-1; i++){
            listAux[i] = this->listaAdj[i];
        }
        delete this->listaAdj; // obs !!!!!!!!!!!!!!!!!1
        this->listaAdj = listAux;
        
    }
}



// verifica o tipo de grafo , aloca a matriz e depois prenche a matriz com as arestas passadas por 
// parametro(um array n x 2, onde os vertices estão aos pares indicando ligação).
void Grafo::criaGrafo(int nVertices, int mArestas, int ** arestas){
    
    this->nVertices = nVertices;
    this->mArestas = mArestas;
    
    if(tipoGrafo == grafoMatriz ){
         this->matriz = new int *[nVertices];
        for(int i = 0; i < nVertices; i++){
            this->matriz[i] = new int[nVertices];
        }
         //preenhe matriz cm arestas.
        for (int i = 0; i < mArestas; i++){
            int verticeA = arestas[i][1], verticeB = arestas[i][2];
            //preenche apenas a parte de cima da matriz
            if(verticeA <= verticeB){ 
                this->matriz[verticeA][verticeB] = 1 ;
            }else{
                this->matriz[verticeB][verticeA] = 1 ;
            }
        }
        
    }else if (tipoGrafo == grafoLista){
        this->listaAdj = new list<int>[this->nVertices];
        
        //cada indice da lista é um vertice e todos os nós da lista sao outros vertices
        //conectados a eles.
        //preenhe matriz cm arestas.
        for(int i = 0; i < mArestas; i++ ){
             int verticeA = arestas[i][1], verticeB = arestas[i][2];
            this->listaAdj[verticeA].push_back(verticeB) ;
            this->listaAdj[verticeB].push_back(verticeA) ;
        }
    }
    
}

vector<int> Grafo::buscaEmLargura(){
    vector<int> saida;
    visitados[0]=1;
    saida[0]=1;
//    this->filaDeBusca();
    
    
    return saida;
}
vector<int> Grafo::BuscaEmProfundidade(){
    
}

string Grafo::imprimeGrafo(){
    string s(" ");
    
    s= s + "Arestas: /n"; 
    if (this->tipoGrafo == grafoMatriz){
        for(int i = 0 ; i < this->nVertices ;i++){
            for(int j = 0 ; j < this->nVertices ;j++){
                if(i==j)continue;
                if(this->matriz[i][j] == 1)
                 s += " (" +to_string(i) + " ," + to_string(j) + ") /n"; //consertar
            }
        }
    }else if(this->tipoGrafo == grafoLista){
        
        for(int i = 0 ; i < this->nVertices; i++){
           for(list<int>::iterator it = this->listaAdj->begin(); it != this->listaAdj[i].end(); it++ ){
               s += " ("  + to_string(i)  + ", " + to_string(*it) + ") /n";
           } 
        }
    }
    return s;
}

ostream& operator<<(ostream& strm,  Grafo &g) {
    strm << g.imprimeGrafo();
    return strm ;
}