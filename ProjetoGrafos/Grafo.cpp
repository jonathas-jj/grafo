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
#include <jsoncpp/json/value.h>
#include <algorithm>

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
    a--;b--;
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
    cout << nVertices;
    this->nVertices++;
    cout << nVertices;
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
        for(int i = 0; i < this->nVertices-1; i++){
            listAux[i] = this->listaAdj[i];

        }
        
        this->listaAdj = listAux;
        
    }
    return this->nVertices;
}


//cria grafo a partir de uma matriz de arestas
// verifica o tipo de grafo , aloca a matriz e depois prenche a matriz com as arestas passadas por 
// parametro(um array n x 2, onde os vertices estão aos pares indicando ligação).
void Grafo::criaGrafo(int nVertices, int mArestas, int ** arestas ){
    
    this->nVertices = nVertices;
    this->mArestas = mArestas;
    
    if(tipoGrafo == grafoMatriz ){
         this->matriz = new int *[nVertices];
        for(int i = 0; i < nVertices; i++){
            this->matriz[i] = new int[nVertices];
        }
         //preenhe matriz cm arestas.
        for (int i = 0; i < mArestas; i++){
            int verticeA = arestas[i][0]-1, verticeB = arestas[i][1]-1;
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
            int verticeA = arestas[i][0]-1, verticeB = arestas[i][1]-1;
            this->listaAdj[verticeA].push_back(verticeB) ;
            this->listaAdj[verticeB].push_back(verticeA) ;
            
        }
    }
    
}


//cria grafo a partir de um arquivo JSON.
void Grafo::criaGrafoJSON(int nVertices, int mArestas, Json::Value raiz){
    int *arestas[mArestas];
    
    for(int i=0;i< mArestas;i++){
        arestas[i] = new int[2];
        arestas[i][0] =stoi(raiz["arestas"][i][0].asString());
        arestas[i][1] =stoi(raiz["arestas"][i][1].asString());
    }
    criaGrafo(nVertices,mArestas,arestas);
    
    
        
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
    s= s + "\n vértices: \n";
    for(int i = 0 ; i < this->nVertices ;i++){
        if ((this->tipoGrafo == grafoMatriz && 
                this->matriz[i][i]==-1) || (this->tipoGrafo == grafoLista)
                && this->listaAdj[i].front()==-1) continue;
         s= s + " "+to_string(i+1)+", "; 
    }
    
    if (this->tipoGrafo == grafoMatriz){
        s= s + "\nArestas: \n"; 
        for(int i = 0 ; i < this->nVertices ;i++){
            //pula vertices excluidos
               if(matriz[i][i]==-1 )continue;
            for(int j = 0 ; j < this->nVertices ;j++){
                //pula vertices iguais e excluidos
                if(i==j || matriz[j][j]==-1 )continue;
                if(this->matriz[i][j] == 1)
                 s += " (" +to_string(i+1) + " ," + to_string(j+1) + "), "; //consertar
            }
        }
    }else if(this->tipoGrafo == grafoLista){
        s= s + "\nArestas: \n"; 
        for(int i = 0 ; i < this->nVertices; i++){
           for(list<int>::iterator it = this->listaAdj[i].begin(); it != this->listaAdj[i].end(); it++ ){
               if( *it==-1 )continue;
               s += " ("  + to_string(i+1)  + ", " + to_string((*it)+1) + "), ";
           } 
        }
    }
    return s+"\n";
}


ostream& operator<<(ostream& strm,  Grafo &g) {
    strm << g.imprimeGrafo();
    return strm ;
}

vector<int> Grafo::vertVizinhos(int v){
    vector<int> vizinhos(nVertices);
    int indice = v -1;
    int count = 0;
    
    if (this->tipoGrafo == grafoMatriz && this->matriz[indice][indice]==-1) 
        return vector<int>();
    
    if(tipoGrafo == grafoMatriz ){
        for(int i = 0; i < this->nVertices; i++){
            
            if(this->matriz[indice][i] == 1 || this->matriz[i][indice] == 1){
                vizinhos[count]=i+1;
                count++;
            }
           
           
        } 
    }
   
    else {
        for (list<int>::iterator i = this->listaAdj[indice].begin(); i != this->listaAdj[indice].end(); ++i){
            vizinhos[count] = *i;
            count++;
        }
    }
    //flag pra parar um loop 
    vizinhos[count]=-1;
    return vizinhos;

}
 void  Grafo::RemoveAresta(int v1, int v2 ){
     v1--;v2--;
     if(tipoGrafo == grafoMatriz ){
        this->matriz[v1][v2]=0;  
        this->matriz[v2][v1]=0; 
        
    }else if(tipoGrafo == grafoLista){
          this->listaAdj[v1].remove(v2);
          this->listaAdj[v2].remove(v1);        
    }
    this->mArestas--;
 }
 void  Grafo::RemoveVertice(int v){
    v--;
    if(tipoGrafo == grafoMatriz ){
       this->matriz[v][v]=-1;  
       
          
            
    }else if(tipoGrafo == grafoLista){ 
       while ( !this->listaAdj[v].empty()){
           this->listaAdj[v].pop_back();
        }
        
        for(int i = 0; i < this->nVertices-1; i++){
            this->listaAdj[i].remove(v);
        }
       this->listaAdj[v].push_back(-1);
    }
     
 }