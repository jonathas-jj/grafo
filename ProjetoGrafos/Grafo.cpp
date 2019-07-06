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
#include <stack>
#include <queue>

#define grafoMatriz 1
#define grafoLista  2


using namespace std;

Grafo::Grafo(int tipoDeGrafo) {
    this->tipoGrafo = tipoDeGrafo;
    this->nVertices = 0;
    this->mArestas = 0;
    this->matriz = NULL;
    this->listaAdj = NULL;
}

Grafo::Grafo(const Grafo& orig) {
   
    this->nVertices = 0;
    this->mArestas = 0;
    this->matriz = NULL;
    this->listaAdj = NULL;
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
        
           delete[]  this->listaAdj;
        
        
    }
    delete[] this->visitados; //vertices visitados
    
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
    
    this->nVertices++;
    
    if(tipoGrafo == grafoMatriz ){
        int ** matrizAux = new int *[this->nVertices];
            for(int i = 0; i < this->nVertices; i++){
                matrizAux[i] = new int[this->nVertices];
            }
        if(this->matriz){
            for(int i = 0; i < this->nVertices-1; i++){
               for(int j = 0; j < this->nVertices-1; j++){
                   matrizAux[i][j] = this->matriz[i][j];  
                }
               delete[] matriz[i] ;
            }
          delete[] this->matriz;  
        }  
      
      this->matriz = matrizAux;
    }
    else if(tipoGrafo == grafoLista){
        list<int> *listAux = new list<int>[this->nVertices];
        for(int i = 0; i < this->nVertices-1; i++){
            listAux[i] = this->listaAdj[i];

        }
        delete[] this->listaAdj;
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
         this->pesoArestas = new ArestasPeso [mArestas];
         
        for(int i = 0; i < nVertices; i++){
            this->matriz[i] = new int[nVertices];
        }
        
        //preenhe matriz cm arestas.
        for (int i = 0; i < mArestas; i++){
            int verticeA = arestas[i][0]-1, verticeB = arestas[i][1]-1;
            
            pesoArestas[i].v1 =  verticeA;
            pesoArestas[i].v2 =  verticeB;
            pesoArestas[i].peso =  arestas[i][2];
            
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


void Grafo::buscaEmLargura(int raiz){
    //cria fila para enfileiras os vértices filhos.
    //Cria um array para listar os vértices visitados    
    queue<int> fila;
     int v = raiz;
    int n= this->nVertices;
    bool visitados[n];
    
    list<int*>::iterator it2;
    
    cout<< "-------------Busca Em Largura-------------" << endl;    
        for(int i = 0; i < n; i++)
            visitados[i] = false;
        //marca o vertice v como visitado
        cout << "Visitando vertice " << v+1 << "\n";
        visitados[v] = true;
     
    if (tipoGrafo == grafoLista){
        while(true){
            
            if(this->listaAdj[v].front() == -1) {cout <<"Vertice inexitente" ;return;}
            list<int>::iterator it;
            //visita todos os filhos de v
            for (it = this->listaAdj[v].begin(); it != listaAdj[v].end(); it++)
            {
               
                //se a aresta ainda n foi explorada, explore-a
                //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == *it) ||  
                           ( (*it2)[0] == *it) && (*it2)[1] == v)  break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = *it;    
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                          
                       
                    
                        if(!visitados[*it]){
                            cout << "Visitando vertice " << *it+1 << "\n";
                            visitados[*it] = true;
                            //coloca os filhos de v na fila para serem explorados
                            fila.push(*it);
                            arestasDescobertas.push_back(aresta);
                            cout << "aresta de descoberta: ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                         }
                    }           
                
               
                
            }

            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                v = fila.front();
                fila.pop();
            } else {
                //se a fila tiver vazia é pq o algoritmo já percorreu todos os vértices
                break;
            }
        }
    }else if(tipoGrafo == grafoMatriz){
        while(true){
            if(this->matriz[v][v] == -1) {cout <<"Vertice inexitente" ;return;}
            for(int j =0;j< this->nVertices;j++ ){                
                if(this->matriz[j][j] == -1) continue;
                //verifica se há aresta ( como a tabela matriz está preenchida soh na parte de cima
                // foi preciso verificar onde está marcado a aresta)
                if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){
                    
                    //explora a aresta
                    
                    
                    //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == j) ||  
                           ( (*it2)[0] == j) && (*it2)[1] == v)  break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = j;    
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                                     
                    
                        if(!visitados[j]){
                            cout << "Visitando vertice " << j+1 << "\n";
                            visitados[j] = true;
                            //coloca os filhos de v na fila para serem explorados
                            fila.push(j);
                            arestasDescobertas.push_back(aresta);
                            cout << "aresta de descoberta: ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                         }
                    }  
                    
                }

            }
            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                v = fila.front();
                fila.pop();
            } else {
                //se a fila tiver vazia é pq o algoritmo já percorreu todos os vértices
                break;
            }
        }
    }
    delete this->visitados;
    this->visitados = new vector<bool>(visitados,visitados+n);
}

void Grafo::BuscaEmProfundidade(int raiz ){
    stack<int> pilha;
    int v = raiz;
    int n = this->nVertices;
    int m = this->mArestas;
    bool visitados[n];
    list<int*>::iterator it2;    
    for(int i = 0; i < n; i++)
        visitados[i] = false;
    
    bool achou;
    
     cout<< "-------------Busca Em Profundidade -------------"<< endl;  
    if (tipoGrafo == grafoLista){
        while(true){
            if(this->listaAdj[v].front() == -1) {cout <<"Vertice inexitente" ;return;}
            
            if(!visitados[v]){
                cout << "Visitando vertice " << v+1 << "...\n";
                visitados[v] = true;
                //empilha primeiro vértice.
                pilha.push(v);
            }

            achou= false;
            
            list<int>::iterator it;

            for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++)
            {
                int* aresta = new int[2];
                aresta[0] = v;
                aresta[1] = *it;
                
                //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == *it) ||  
                           ( (*it2)[0] == *it) && (*it2)[1] == v)  break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){  
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = *it;                          
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                
                        if(!visitados[*it]){           
                            cout << "aresta de descoberta: ( " << aresta[0]+1 << "," <<aresta[1]+1 << ")\n";
                            this->arestasDescobertas.push_back(aresta);                
                            achou = true;
                            break;
                        }
                    }    
                
                
                
                
            }

            if(achou)
                v = * it;
            else {
                pilha.pop();

                if(pilha.empty())
                    break;

                v = pilha.top();
            }

        }
    }else if(tipoGrafo == grafoMatriz){
        while(true){
            if(this->matriz[v][v] == -1) {cout <<"Vertice inexitente" ;return;}
            if(!visitados[v]){
                cout << "Visitando vertice " << v+1 << "...\n";
                visitados[v] = true;
                //empilha primeiro vértice.
                pilha.push(v);
            }

            achou = false;
           
           int j;
           for(j =0;j< this->nVertices;j++ )  {
                  
                 if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){
                         
                    
                    //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == j) ||  
                           ( (*it2)[0] == j && (*it2)[1] == v) ) break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){  
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = j;                          
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                        
                        
                        if(!visitados[j]){
                            cout << "Aresta de descoberta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                            this->arestasDescobertas.push_back(aresta);   
                            achou = true;
                            break;
                        }
                    }
                    
                    
                }
            }

            if(achou)
                v = j;
            else {
                pilha.pop();

                if(pilha.empty())
                    break;

                v = pilha.top();
            }

        }
    }
    //deleta os vértices visitados anteriormente e adiciona novos.  
    
        delete this->visitados;
        this->visitados = new vector<bool>(visitados,visitados+n);
    
}

//flag serve pra não entrar no primeiro if 
// que  limpa o vetor visitados.
void Grafo::BuscaEmProfundidadeRec(int raiz,bool flag){
    int v = raiz;
    
    if (tipoGrafo == grafoLista){
        if(this->listaAdj[v].front() == -1) {cout <<"Vertice inexitente" ;return;}
        if (flag==true){
            delete this->visitados;
            this->visitados = new vector<bool>(this->nVertices);
            cout<< "-------------Busca Em Profundidade Recursiva-------------"<< endl;
        }
        cout << "Visitando vertice " << v+1 << "...\n";   
        list<int>::iterator it;
        for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++){
            int* aresta = new int[2];
            aresta[0] = v;
            aresta[1] = *it;
            cout << "Explorando aresta ( " << aresta[0] << "," << aresta[1] << ")\n";
                this->arestasExploradas.push_back(aresta); 
            if((*this->visitados)[*it] == false){ 
                (*this->visitados)[*it] = true;
                aresta[0] = v;
                aresta[1] = *it;
                this->arestasDescobertas.push_back(aresta);
                cout << "Aresta de descoberta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                   
                BuscaEmProfundidadeRec(*it,false);
            }
        }
    }else if(tipoGrafo == grafoMatriz){
        
        if(this->matriz[v][v] == -1) {cout <<"Vertice inexitente" ;return;}
        if (flag==true){
            delete this->visitados;
            this->visitados = new vector<bool>(this->nVertices);
            cout<< "-------------Busca Em Profundidade Recursiva-------------"<< endl; 
        }
         cout << "Visitando vertice " << v+1 << "...\n";  
        
        for (int j =0; j< this->nVertices; j++){
            if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){                    
                int* aresta = new int[2];
                aresta[0] = v;
                aresta[1] = j;
                cout << "Explorando aresta ( " << aresta[0]+1 << ","<<aresta[1]+1 << ")\n";
                    this->arestasExploradas.push_back(aresta); 
                                     
                if((*this->visitados)[j]==false){       
                    aresta[0] = v;
                    aresta[1] = j;
                    this->arestasDescobertas.push_back(aresta);
                    cout << "Aresta de descoberta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                    (*this->visitados)[j] = true;
                    BuscaEmProfundidadeRec(j,false);
                    
                }
            }    
        }
    }
}





void  Grafo::buscaCompleta(int r){
    int raiz = r;
    bool restaAlgumVert =false;
    
    int v = raiz;
    int n = this->nVertices;
    int m = this->mArestas;
    bool visitados[n];
    list<int*>::iterator it2;    
    for(int i = 0; i < n; i++)
        visitados[i] = false;
    
    vector<bool>::iterator it;
    cout<< "------------------Busca Completa-------------------------" << endl;  
    do {
        
        
        
        
        
        stack<int> pilha;         

        bool achou;
        cout<< "------------- Raiz = "<< v+1 << endl<< "--------";
        if (tipoGrafo == grafoLista ){
            if (this->listaAdj[v].front() != -1){
                while(true){
                    if(!visitados[v]){

                        cout << "Visitando vertice " << v+1 << "...\n";
                        visitados[v] = true;
                        //empilha primeiro vértice.
                        pilha.push(v);
                    }

                    achou= false;

                    list<int>::iterator it;


                    for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++)
                    {
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = *it;

                        //verifica se a aresta já foi explorada
                            it2 = arestasExploradas.begin();
                            while(it2!=arestasExploradas.end()){
                               if ( ( (*it2)[0] == v && (*it2)[1] == *it) ||  
                                   ( (*it2)[0] == *it) && (*it2)[1] == v)  break; 
                               it2++; 
                            }

                            if(it2==arestasExploradas.end()){  
                                int* aresta = new int[2];
                                aresta[0] = v;
                                aresta[1] = *it;                          
                                arestasExploradas.push_back(aresta);
                                cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";

                                if(!visitados[*it]){           
                                    cout << "aresta de descoberta: ( " << aresta[0]+1 << "," <<aresta[1]+1 << ")\n";
                                    this->arestasDescobertas.push_back(aresta);                
                                    achou = true;
                                    break;
                                }
                            }    




                    }

                    if(achou)
                        v = * it;
                    else {
                        pilha.pop();

                        if(pilha.empty())
                            break;

                        v = pilha.top();
                    }
                
                 }
                
            }else{  cout <<"Vertice "<< v+1 <<"inexitente" <<endl; visitados[v]=true;}   
        }else if(tipoGrafo == grafoMatriz ){
            //se o vértice raiz existir
            if(this->matriz[v][v] != -1){
                while(true){
                    
                    if(!visitados[v]){
                        cout << "Visitando vertice " << v+1 << "...\n";
                        visitados[v] = true;
                        //empilha primeiro vértice.
                        pilha.push(v);
                    }

                    achou = false;

                   int j;
                   for(j =0;j< this->nVertices;j++ )  {

                         if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){


                            //verifica se a aresta já foi explorada
                            it2 = arestasExploradas.begin();
                            while(it2!=arestasExploradas.end()){
                               if ( ( (*it2)[0] == v && (*it2)[1] == j) ||  
                                   ( (*it2)[0] == j && (*it2)[1] == v) ) break; 
                               it2++; 
                            }

                            if(it2==arestasExploradas.end()){  
                                int* aresta = new int[2];
                                aresta[0] = v;
                                aresta[1] = j;                          
                                arestasExploradas.push_back(aresta);
                                cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";


                                if(!visitados[j]){
                                    cout << "Aresta de descoberta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                                    this->arestasDescobertas.push_back(aresta);   
                                    achou = true;
                                    break;
                                }
                            }


                        }
                    }

                    if(achou)
                        v = j;
                    else {
                        pilha.pop();

                        if(pilha.empty())
                            break;

                        v = pilha.top();
                    }

                }
                //coloca true no vértice para ele n ser cotado como raiz
            }else {  cout <<"Vertice "<< v+1 <<"inexitente" <<endl; visitados[v]=true;} 
        }
    
        
        restaAlgumVert = 0;
        for(int i = 0; i < n ; i++){
            if(!visitados[i] ){  restaAlgumVert = true;v = i; break;}
        }
        
        
    }while( restaAlgumVert);
    
        delete this->visitados;
        this->visitados = new vector<bool>(visitados,visitados+n);

}
bool Grafo::conectividade(){
        
    int raiz = 0;
    BuscaEmProfundidade(raiz);
    //retorna o valor logico da pesquisa binária sobre o valor falso
    return binary_search(this->visitados->begin(),this->visitados->end(),false);
        
    
}

bool Grafo::temCiclo(){

    buscaCompleta(0);
    if(this->arestasDescobertas.size() == this->mArestas){
        return false;
    }
    
    return true;

}

bool Grafo::isFloresta(){

    return !temCiclo();

}


bool Grafo::isArvore(){
// uma desvantegem de usar os métodos abaixo é que a busca é feita 
//duas vezes
    
//    
//    if(!conectividade() && temCiclo()){
//        return false;
//    }
//    return true;
    buscaEmLargura(0);
    //verifica se eh conexo
    if(binary_search(this->visitados->begin(),this->visitados->end(),false)){
        return false;
        
        //verifica se tem ciclo
    }else if(!this->arestasDescobertas.size() == this->mArestas){
        return false;
    }
    return true;

}


void Grafo::ObterFlorestaGeradora(){
    
    
    
    cout << "--------------Floresta Geradora--------------------------";
    
    bool restaAlgumVert =false;
    
    int v = 1;
    int n = this->nVertices;
    int m = this->mArestas;
    bool visitados[n],vertDaArvore[n];
    list<int*>::iterator it2;    
    for(int i = 0; i < n; i++){
        visitados[i] = false;
        vertDaArvore[i] = false;
    }
    
    vector<bool>::iterator it;
    do {  
               
        stack<int> pilha;         

        bool achou;
        cout<< "------------- Árvore de raiz = "<< v+1 <<  "--------"<< endl;
        if (tipoGrafo == grafoLista ){
            if (this->listaAdj[v].front() != -1){
                while(true){
                    if(!visitados[v]){

                       // cout << "Visitando vertice " << v+1 << "...\n";
                        visitados[v] = true;
                        vertDaArvore[v] = true;
                        //empilha primeiro vértice.
                        pilha.push(v);
                    }

                    achou= false;

                    list<int>::iterator it;


                    for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++)
                    {
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = *it;

                        //verifica se a aresta já foi explorada
                            it2 = arestasExploradas.begin();
                            while(it2!=arestasExploradas.end()){
                               if ( ( (*it2)[0] == v && (*it2)[1] == *it) ||  
                                   ( (*it2)[0] == *it) && (*it2)[1] == v)  break; 
                               it2++; 
                            }

                            if(it2==arestasExploradas.end()){  
                                int* aresta = new int[2];
                                aresta[0] = v;
                                aresta[1] = *it;                          
                                arestasExploradas.push_back(aresta);
                               // cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";

                                if(!visitados[*it]){           
                                    cout << "aresta da árvore  ( " << aresta[0]+1 << "," <<aresta[1]+1 << ")\n";
                                    this->arestasDescobertas.push_back(aresta);                
                                    achou = true;
                                    break;
                                }
                            }    




                    }

                    if(achou)
                        v = * it;
                    else {
                        pilha.pop();

                        if(pilha.empty())
                            break;

                        v = pilha.top();
                    }
                
                 }
                //coloca true no vértice para ele n ser cotado como raiz
                
            }else{  cout <<"Vertice "<< v+1 <<"Excluido" <<endl; visitados[v]=true;}
            
            
        }else if(tipoGrafo == grafoMatriz ){
            //se o vértice raiz existir
            if(this->matriz[v][v] != -1){
                while(true){
                    
                    if(!visitados[v]){
                        vertDaArvore[v] = true;
                        visitados[v] = true;
                        //empilha primeiro vértice.
                        pilha.push(v);
                    }

                    achou = false;

                   int j;
                   for(j =0;j< this->nVertices;j++ )  {

                         if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){


                            //verifica se a aresta já foi explorada
                            it2 = arestasExploradas.begin();
                            while(it2!=arestasExploradas.end()){
                               if ( ( (*it2)[0] == v && (*it2)[1] == j) ||  
                                   ( (*it2)[0] == j && (*it2)[1] == v) ) break; 
                               it2++; 
                            }

                            if(it2==arestasExploradas.end()){  
                                int* aresta = new int[2];
                                aresta[0] = v;
                                aresta[1] = j;                          
                                arestasExploradas.push_back(aresta);
                                

                                if(!visitados[j]){
                                    cout << "aresta da árvore  ( " << aresta[0]+1 << "," <<aresta[1]+1 << ")\n";
                                    this->arestasDescobertas.push_back(aresta);   
                                    achou = true;
                                    break;
                                }
                            }


                        }
                    }

                    if(achou)
                        v = j;
                    else {
                        pilha.pop();

                        if(pilha.empty())
                            break;

                        v = pilha.top();
                    }

                }
                //coloca true no vértice para ele n ser cotado como raiz
            }else {  cout <<"Vertice "<< v+1 <<"inexitente" <<endl; visitados[v]=true;} 
        }
    
        
        restaAlgumVert = 0;
        for(int i = 0; i < n ; i++){
            //testa quais são os vértices da arvore
            
            //verifica se algum vértice n foi visitado, se sim  significa q o grafo n é conexo
            if(!visitados[i] ){  restaAlgumVert = true;v = i; break;}
        }
        
        for(int i = 0; i < n; i++){
            
            if((vertDaArvore[i]) && ((this->matriz != NULL && this->matriz[i][i] != -1) ||
                    (this->listaAdj!= NULL &&  this->listaAdj[i].front() != -1) )){
                
                cout << "vértice da árvore: " << i+1 << endl;
            }
            vertDaArvore[i] = false;
        }
    }while( restaAlgumVert);
    
        

    

}

vector<int>* Grafo::determinarDistancias(int raiz){                
   
    int nivel=1, v = raiz, n= this->nVertices;
     queue<int> fila;
    queue<int> filaNiveis;    
    bool visitados[n];
    vector<int>* distancia = new vector<int>(n);    
    list<int*>::iterator it2;

    for(int i = 0; i < n; i++)
        visitados[i] = false;
            
   filaNiveis.push(nivel);
    
    //marca o vertice v como visitado
    cout << "Visitando vertice " << v+1 << "\n";
    visitados[v] = true;
    if (tipoGrafo == grafoLista){
        while(true){
                       
            list<int>::iterator it;
            //visita todos os filhos de v
            for (it = this->listaAdj[v].begin(); it != listaAdj[v].end(); it++)
            {
               
                //se a aresta ainda n foi explorada, explore-a
                //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == *it) ||  
                           ( (*it2)[0] == *it) && (*it2)[1] == v)  break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = *it;    
                        arestasExploradas.push_back(aresta);
                        //cout << "Explorando aresta ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                          
                       
                    
                        if(!visitados[*it]){
                           // cout << "Visitando vertice " << *it+1 << "\n";
                            cout << "Distancia vertice " << *it+1 <<" =" << nivel << "\n";
                            (*distancia)[*it]=nivel;
                            visitados[*it] = true;
                            //coloca os filhos de v na fila para serem explorados
                            fila.push(*it);
                            arestasDescobertas.push_back(aresta);
                            nivel++;
                            filaNiveis.push(nivel);  
                            
                            
                           // cout << "aresta de descoberta: ( " << aresta[0]+1 << ","<< aresta[1]+1 << ")\n";
                         }
                    }           
                
               
                
            }

            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                v = fila.front();
                fila.pop();
                nivel = filaNiveis.front();
                filaNiveis.pop();
            } else {
                //se a fila tiver vazia é pq o algoritmo já percorreu todos os vértices
                break;
            }
        }
    }else if(tipoGrafo == grafoMatriz){
        while(true){
          
            for(int j =0;j< this->nVertices;j++ ){                
                if(this->matriz[j][j] == -1) continue;
                //verifica se há aresta ( como a tabela matriz está preenchida soh na parte de cima
                // foi preciso verificar onde está marcado a aresta)
                if(( v < j && this->matriz[v][j]==1) || ( j < v && this->matriz[j][v]==1)){
                    
                    //explora a aresta
                    
                    
                    //verifica se a aresta já foi explorada
                    it2 = arestasExploradas.begin();
                    while(it2!=arestasExploradas.end()){
                       if ( ( (*it2)[0] == v && (*it2)[1] == j) ||  
                           ( (*it2)[0] == j) && (*it2)[1] == v)  break; 
                       it2++; 
                    }
                    
                    if(it2==arestasExploradas.end()){
                        int* aresta = new int[2];
                        aresta[0] = v;
                        aresta[1] = j;    
                        arestasExploradas.push_back(aresta);
                        
                                     
                    
                        if(!visitados[j]){
                            // cout << "Visitando vertice " << *it+1 << "\n";                            
                            (*distancia)[j]=nivel;
                            visitados[j] = true;
                            //coloca os filhos de v na fila para serem explorados
                            fila.push(j);
                            arestasDescobertas.push_back(aresta);
                            nivel++;
                            filaNiveis.push(nivel);
                         }
                    }  
                    
                }

            }
            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                  v = fila.front();
                fila.pop();
                nivel = filaNiveis.front();
                filaNiveis.pop();
            } else {
                //se a fila tiver vazia é pq o algoritmo já percorreu todos os vértices
                break;
            }
        }
    }

    
    delete this->visitados;
    this->visitados = new vector<bool>(visitados,visitados+n);
    
    //retorna as distancias do vertice dado.
    return distancia;
} 


string Grafo::imprimeGrafo(){
    string s(" ");
    s= s + "\n vértices: \n";
    
    //imprime os vertices, -1 serve para testar se o
    //vértice foi excluido (ver o método de exclusao de vértices)
    for(int i = 0 ; i < this->nVertices ;i++){
        if ((this->tipoGrafo == grafoMatriz && 
                this->matriz[i][i]==-1) || (this->tipoGrafo == grafoLista)
                && this->listaAdj[i].front()==-1) continue;
         s= s + " "+to_string(i+1)+", "; 
    }

    if (this->tipoGrafo == grafoMatriz){
        s= s + "\nArestas: \n"; 
        for(int i = 0 ; i < this->nVertices ;i++){
            //pula vertices excluidos (ver o método de exclusao de vértices)
               if(matriz[i][i]==-1 )continue;
            for(int j = 0 ; j < this->nVertices ;j++){
                //pula vertices iguais e excluidos (ver o método de exclusao de vértices)
                if(i==j || matriz[j][j]==-1 )continue;
                if(this->matriz[i][j] == 1)
                 s += " (" +to_string(i+1) + " ," + to_string(j+1) + "), "; //consertar
            }
        }
    }else if(this->tipoGrafo == grafoLista){
        s= s + "\nArestas: \n"; 
        for(int i = 0 ; i < this->nVertices; i++){
           for(list<int>::iterator it = this->listaAdj[i].begin(); it != this->listaAdj[i].end(); it++ ){
              //n imprime aresta com vértices excluidos (ver o método de exclusao de vértices).
               if( *it==-1 )continue;
               s += " ("  + to_string(i+1)  + ", " + to_string((*it)+1) + "), ";
           } 
        }
    }
    return s+"\n";
}
//usa o operador << para imprimir o grafo
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

void  Grafo::dijkstra(int orig){
    int n = this->nVertices;
    int dist[n];
    int visitados[n];
    int caminho[n];
    int i = 0;
    cout << "dsa"; 
    for(int i = 0; i < n; i++){
        dist[i] = 10000000;
        visitados[i] = false;
        caminho[i]= -1;
    }
    
    dist[orig-1] = 0;
    
    if(tipoGrafo == grafoLista){
                
        
        while(binary_search(visitados,visitados+n,false)){
            int *min;int peso;
            
            min = std::min_element( dist, dist + n ); 
           
            for (list<int>::iterator it = this->listaAdj[min - dist].begin();
                    it != this->listaAdj[min - dist].end(); ++it){
                
                peso = procuraPeso(min-dist, *it);
                if(dist[*it] > (dist[*it] + peso)){
                    dist[*it] = dist[ min - dist] + peso ;
                    caminho[*it] = *min;
                }

                
            }    
        }
//        
    }else if(tipoGrafo == grafoMatriz){
        while(binary_search(visitados,visitados+n,false)){
            int *min;int peso;
        
            min = std::min_element( dist, dist + n ); 
            int i = ( min - dist);
            visitados[i] = true;
            for (int j = 0 ; j < this-> nVertices ;j++){
                if(( i < j && this->matriz[i][j]==1) || ( j < i && this->matriz[j][i]==1)){
                   
                     peso = procuraPeso(i, j);
                    if(dist[j] > dist[j] + peso){
                        dist[j] = dist[ i] + peso ;
                        caminho[j] = *min;
                        
                    }
                }            
                
            }    
        }
    }
    cout << "Distancias dos vértices ao vértice de origem" <<endl;
    for(int i = 0; i < n; i++){
        cout << dist[i]<< ", ";
    }

}

int Grafo::procuraPeso(int v1, int v2){
    int Achado = 0 ;
    for(int i = 0; i< this->mArestas ;i++){
        if((this->pesoArestas[i].v1 == v1 && this->pesoArestas[i].v2== v2 )||
                (this->pesoArestas[i].v1 == v2 && this->pesoArestas[i].v2== v1)){
            Achado = pesoArestas[i].peso;
        }
    }
    
    return Achado;
}
int  Grafo::getNumAresta(){
    return this->mArestas;
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
       this->matriz[v][v]= -1;  
       
          
            
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
 //void Grafo::Floyd(int **arestas, int n, int mArestas){
    //int** matriz;
    //int** matrizAnt;
    //int *min;

    //for(int i = 0; i < n; i++){
        //for(int j = 0; j < n; j++){
            //if(i == j){
                //matriz[i][j] = 0;
            //} else {                
                //matriz[i][j] = 999999999;
            //}
        //}
    //}

    //for(int i = 0; i < mArestas; i++){
        //if(arestas[i][0] != arestas[i][1]){
            //matriz[arestas[i][0]][arestas[i][1]] = arestas[i][2]; //rand() % 50 + 1;
        //}
    //}

    //for(int k = 0; k < n; k++){
        //matrizAnt = matriz;
        //for(int i = 0; i < n; i++){
            //for(int j = 0; j < n; j++){

                //if(matriz[i][j] > matrizAnt[i][k] + matrizAnt[k][j]){
                     //matrizAnt[i][j] = matrizAnt[i][k] + matrizAnt[k][j];
                //}
            //}
        //}
    //}
