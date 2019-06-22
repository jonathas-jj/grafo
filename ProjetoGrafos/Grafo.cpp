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


void Grafo::buscaEmLargura(int raiz){
    //cria fila para enfileiras os vértices filhos.
    //Cria um array para listar os vértices visitados    
    queue<int> fila;
     int v = raiz;
    int n= this->nVertices;
    bool visitados[n];
    
    list<int*>::iterator it2;

        for(int i = 0; i < n; i++)
            visitados[i] = false;
        //marca o vertice v como visitado
        cout << "Visitando vertice " << v << "\n";
        visitados[v] = true;
    
    if (tipoGrafo == grafoLista){
        while(true){
            
            
            list<int>::iterator it;
            //visita todos os filhos de v
            for (it = this->listaAdj[v].begin(); it != listaAdj[v].end(); it++)
            {
                int* aresta = new int[2];
                aresta[0] = v;
                aresta[1] = *it; 
                //se o vertice n foi visitado, visite ele e descubra a aresta
                 if(!visitados[*it]){
                    cout << "Visitando vertice " << *it << "\n";
                    visitados[*it] = true;
                    //coloca os filhos de v na fila para serem explorados
                    fila.push(*it);
                    this->arestasDescobertas.push_back(aresta);
                }
                //se a aresta ainda n foi explorada, explore-a
                it2 = find (this->arestasExploradas.begin(), this->arestasExploradas.end(), aresta); 
                if(it2!=this->arestasExploradas.end()){  
                    this->arestasExploradas.push_back(aresta);
                    cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
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
            for(int j =0;j< this->nVertices;j++ ){
                
                
                //se houver aresta
                if(this->matriz[v][j]==1){
                    
                    //explora a aresta
                    int* aresta = new int[2];
                    aresta[0] = v;
                    aresta[1] = j;    
                    arestasExploradas.push_back(aresta);
                    cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] <<")\n";
                       
                    
                    if(!visitados[j]){
                        cout << "Visitando vertice " << j << "\n";
                        visitados[j] = true;
                        //coloca os filhos de v na fila para serem explorados
                        fila.push(j);
                        arestasDescobertas.push_back(aresta);
                     }
                    
                    it2 = find (arestasExploradas.begin(), arestasExploradas.end(), aresta); 
                    if(it2!=arestasExploradas.end()){  
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
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

void Grafo::BuscaEmProfundidade(int raiz){
    stack<int> pilha;
    int v = raiz;
    int n = this->nVertices;
    int m = this->mArestas;
    bool visitados[n];
        
    for(int i = 0; i < n; i++)
        visitados[i] = false;
    
    
    if (tipoGrafo == grafoLista){
        while(true){
            if(!visitados[v]){
                cout << "Visitando vertice " << v << "...\n";
                visitados[v] = true;
                //empilha primeiro vértice.
                pilha.push(v);
            }

            bool achou = false;
            list<int>::iterator it;

            for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++)
            {
                int* aresta = new int[2];
                aresta[0] = v;
                aresta[1] = *it;
                
                cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
                
                this->arestasExploradas.push_back(aresta);
                
                if(!visitados[*it]){                    
                    this->arestasDescobertas.push_back(aresta);                
                    achou = true;
                    break;
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
            if(!visitados[v]){
                cout << "Visitando vertice " << v << "...\n";
                visitados[v] = true;
                //empilha primeiro vértice.
                pilha.push(v);
            }

            bool achou = false;
            list<int>::iterator it;

           for(int j =0;j< this->nVertices;j++ )  {
                if(this->matriz[v][j]==1){
                    
                    int* aresta = new int[2];
                    aresta[0] = v;
                    aresta[1] = j;
                    
                    cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
                    this->arestasExploradas.push_back(aresta);
                    
                    if(!visitados[*it]){
                        
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
    }
    //retorna um vetor com os vértices visitados
    delete this->visitados;
    this->visitados = new vector<bool>(visitados,visitados+n);
}

//flag serve pra não entrar no primeiro if 
// que  limpa o vetor visitados.
void Grafo::BuscaEmProfundidadeRec(int raiz,bool flag){
    int v = raiz;
    if (flag==true){
        delete this->visitados;
        this->visitados = new vector<bool>(this->nVertices);
    }
    list<int>::iterator it;
    for (it = this->listaAdj[v].begin(); it != this->listaAdj[v].end(); it++){
        int* aresta = new int[2];
        aresta[0] = v;
        aresta[1] = *it;
        if(!binary_search(this->visitados->begin(),this->visitados->end(),*it)){ 
            
            cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
            this->arestasExploradas.push_back(aresta); 
            (*this->visitados)[*it] = true;
        }else{        
            aresta[0] = v;
            aresta[1] = *it;
            this->arestasDescobertas.push_back(aresta);
            BuscaEmProfundidadeRec(*it,false);
        }
    }
    
}




/*
//AS duas buscas do slide5 num só método
bool * Grafo::busca(int raiz)
{
    int countAtual;
    int v = this->nVertices;
    int m = this->mArestas;
    
     bool visitados[v];
    
    int arestasExploradas[m][3];
    list<int *> arestasDescobertas;
    
    for(int i = 0; i < v; i++)
        visitados[i] = false;

    
    visitados[raiz] = true;
    
    if(tipoGrafo == grafoLista){
        
        int m=0;
        //coloca todas as arestas existentes num array
        for(int i = 0 ; i < this->nVertices ;i++){
            for(list<int>::iterator it = this->listaAdj[i].begin(); it != this->listaAdj[i].end(); it++ ){
                if( (*it) > i){
                    
                }else{
                    
                }
            } 
        }
        while(existemArestasInexploradas e que possuem vertices visitados){
            
        }        
        
    }else if(tipoGrafo == grafoMatriz){
        
        //gera um array de arestas n/2
        for(int i = 0; i< this->nVertices ;i++){  
            int count=0;
            for(int j = 0; j< this->nVertices ;j++){
                //percorre apenas a matrix superior
                if (j <= i)continue;
                //cria um array de arestas inexploradas.                
                if (matriz[i][j] == 1){
                    if(i == raiz|| j == raiz) countAtual = count;
                    arestasExploradas[count][0] = i;
                    arestasExploradas[count][1] = j;
                    arestasExploradas[count][2] = 0; //false. n explorada
                    count++;
                }
            }            
        }
        
        
        
        
        //percorre todas as arestas feitas anteriormente
        for(int i=0; i < m ;i++){
            //checa se a aresta não foi explorada e 
            //o primeiro vertice da aresta foi visitado .
            if(arestasExploradas[i][2]== 0 and
               (visitados[arestasExploradas[i][0]] == true)||
               (visitados[arestasExploradas[i][1]] == true)){
                
            }
            for(int j = 0; j< this->nVertices ;j++){
                //percorre apenas a matrix superior
                if (j <= verticeAtual)continue;
                                
                if (matriz[verticeAtual][j] == 1){
                    arestasExploradas[verticeAtual][j] = true;
                }
                if (!visitados[j]){
                    visitados[j]=true;
                    
                    int* aresta = new int[2];
                    aresta[0] = v;
                    aresta[1] = *it;
                    arestasDescobertas = ;
                }
            }            
        }
               
        
    }
    
    return visitados;
         
}
*/

void  Grafo::buscaCompleta(int r){
    int raiz = r;
   
    
    vector<bool>::iterator it;
    
    do {
        BuscaEmProfundidade(raiz);
        it  = find(this->visitados->begin(), this->visitados->end(),false);
        
    }while(it !=this->visitados->end());
    
    
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


Grafo* Grafo::ObterFlorestaGeradora(){
    buscaCompleta(0);
    // cria um grafo florestaGeradora
    Grafo* florestaGeradora = new Grafo(this->tipoGrafo);
    
      // insere os vértices do grafo atual no grafo florestaGeradora
    for(int i =0; i< this->nVertices;i++){
        florestaGeradora->insereVertice();
    }
    //insere as arestas descobertas  no novo grafo florestaGeradora
    for(list<int*>::iterator it = this->arestasDescobertas.begin(); it!= this->arestasDescobertas.end();++it){
        florestaGeradora->insereAresta((*it)[0],(*it)[1]);
    }
    return florestaGeradora;
}

vector<int> Grafo::determinarDistancias(int raiz){                
    queue<int*> fila;
    int nivel=1, v = raiz, n= this->nVertices;
    bool visitados[n];
    vector<int> distancia;    
    list<int*>::iterator it2;

    for(int i = 0; i < n; i++)
        visitados[i] = false;
            
    fila.push(new int[2]{v,nivel});
    
    //marca o vertice v como visitado
    cout << "Visitando vertice " << v << "\n";
    visitados[v] = true;
    
    if (tipoGrafo == grafoLista){
        while(true){
            
            
            list<int>::iterator it;
            //visita todos os filhos de v
            for (it = this->listaAdj[v].begin(); it != listaAdj[v].end(); it++)
            {
                int* aresta = new int[2];
                aresta[0] = v;
                aresta[1] = *it; 
                //se o vertice n foi visitado, visite ele e descubra a aresta
                 if(!visitados[*it]){
                    cout << "Visitando vertice " << *it << "\n";
                    visitados[*it] = true;
                    //coloca os filhos de v na fila com os seus níveis
                    fila.push(new int[2]{*it,nivel++});
                    this->arestasDescobertas.push_back(aresta);
                    distancia[v]=nivel;
                }
                //se a aresta ainda n foi explorada, explore-a
                it2 = find (this->arestasExploradas.begin(), this->arestasExploradas.end(), aresta); 
                if(it2!=this->arestasExploradas.end()){  
                    this->arestasExploradas.push_back(aresta);
                    cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
                }          
                
               
                
            }

            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                v = fila.front()[0];
                nivel = fila.front()[1];
                fila.pop();
            } else {
                //se a fila tiver vazia é pq o algoritmo já percorreu todos os vértices
                break;
            }
        }
        
    }else if(tipoGrafo == grafoMatriz){
        while(true){
            for(int j =0;j< this->nVertices;j++ ){
                
                
                //se houver aresta
                if(this->matriz[v][j]==1){
                    
                    //explora a aresta
                    int* aresta = new int[2];
                    aresta[0] = v;
                    aresta[1] = j;    
                    arestasExploradas.push_back(aresta);
                    cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] <<")\n";
                       
                    
                    if(!visitados[j]){
                        cout << "Visitando vertice " << j << "\n";
                        visitados[j] = true;
                        
                         //coloca os filhos de v na fila com os seus níveis
                        //COLOCA O NIVEL DE DETERMINADO VERTICE NO VETOR
                        
                        fila.push(new int[2]{j,nivel++});
                        this->arestasDescobertas.push_back(aresta);
                        distancia[v]=nivel;
                     }
                    
                    it2 = find (arestasExploradas.begin(), arestasExploradas.end(), aresta); 
                    if(it2!=arestasExploradas.end()){  
                        arestasExploradas.push_back(aresta);
                        cout << "Explorando aresta ( " << aresta[0] << ","+aresta[1] << ")\n";
                    }  
                    
                }

            }
            if(!fila.empty()){
                //passa a explorar os filhos de determinado vertice q foi enfileirado antes
                v     = fila.front()[0];
                nivel = fila.front()[1];
                //limpa a regiao alocada
                delete &fila.front();
                fila.pop();
                
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