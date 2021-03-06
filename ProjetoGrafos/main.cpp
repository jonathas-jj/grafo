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
#include "GrafoCreator.h"
#include <fstream>  
#include <sstream>

using namespace std;

/*
 * 
 */

int buscaGrafos(Grafo *g);

void abreArq(string s, Grafo *g){
     int n=0,m=0;
    fstream arq(s);
    Json::Value JSONRaiz;

    arq >> JSONRaiz;
    cout << JSONRaiz["nome"];
    n = JSONRaiz["vertices"].size(); 
    m = JSONRaiz["arestas"].size();
    
    g->criaGrafoJSON(n,m,JSONRaiz);
   
    arq.close();
}
   

int main() {
    Grafo *g;// grafo do tipo matriz
    // grafo do tipo lista
    int nDoGrafo,tipoGrafo,opcao = 1;
    
    //cria arquivos JSON caso n existam.
    criarGrafos();
    cout << "\nDigite o grafo que deseja ler:"
            "n=5, n=6, n=7, n=8, n=9, n=10,  n=20,"
            " n=50, n=100, n=200, n=500, n=1000 \n"
            " " << endl;
    cin>>nDoGrafo;
    cout << "\nDigite o tipo de grafo(matriz = 1, lista = 2):" ;
    cin>>tipoGrafo;
    
    g = new Grafo(tipoGrafo);
    abreArq("dados/grafoN"+to_string(nDoGrafo)+".json",  g);
    
     //imprime o grafo
    cout << *g ;
    while(opcao!=0){
        
        int vertViz,vertice,verticeA,verticeB;
        string s;
        
        char add;
        
                
       
        
        cout <<"\n\nDigite 0 para sair"<<endl;                   
        cout <<"Digite 1 para mostras vértices vizinhos"<<endl;                   
        cout <<"Digite 2 para remover vértice"<<endl;                   
        cout <<"Digite 3 para adicionar vértice"<<endl;                   
        cout <<"Digite 4 para remover aresta"<<endl;
        cout <<"Digite 5 para adicionar aresta"<<endl;
        cout <<"Digite 6 para imprimir grafo"<<endl;
        cout <<"Digite 7 para ler outro arquivo json"<<endl;
        cout <<"Digite 8 para executar alguma busca no grafo."<<endl;
        cin >> opcao;
       
        
        
        if(opcao==1){
            cout << "\nDigite o vértice que deja saber os vizinhos.";
            cin>>vertViz;
            cout<< "vizinhos: ";
            for(int i=0;; i++){        
                vector<int> vizinhos(g->vertVizinhos(vertViz));

                if( vizinhos.size() == 0 || vizinhos[i]==-1 )break; //flag colocada para parada do loop
                cout<<vizinhos[i]<<" ";

            }
            
        }else if(opcao==2){
            cout<< "\nDigite um vertice para excluir:(-1 pra não excluir) ";
            cin >> vertice;
            if(vertice!=-1){
                g->RemoveVertice(vertice);
                cout<< "\n Vertice "<< to_string(vertice)<< " para excluido!! ";
            }
            
        }else if(opcao==3){
            cout<< "\nDeseja add um vertice?(s para sim ou qualquer tec para ñ) ";
            cin >> add;
            if(add == 's'){
                
                cout<< "\n Vertice" + to_string(g->insereVertice()) +" incluido!! ";
            } 
            
        }else if(opcao==4){
            
            cout<< "\nDeseja excluir uma aresta?(digite os 2 vertices(v1 v2) ou -1 para pular) "<<endl;
            
            cin >> verticeA;
            
            if(verticeA != -1){
                cin >> verticeB;
                g->RemoveAresta(verticeA,verticeB);
                cout<< "\n aresta: (";cout << verticeA;cout << ", "; cout<<verticeB; cout<< ") removida!! ";
            } 
            
        }else if(opcao==5){
            cout<< "\nDeseja add uma aresta?(digite os 2 vertices(v1 v2) ou -1 para pular) ";
            cin >> verticeA;

            if(verticeA != -1){
                cin >> verticeB;
                g->insereAresta(verticeA,verticeB);
                cout<< "\n aresta: (";cout << verticeA;cout << ", "; cout<<verticeB; cout<< ") incluida!! ";
            } 
            
        }else if(opcao==6){ 
           cout << *g ;
           
        }else if(opcao==7){
                    
            cout << "\nDigite o grafo que deseja ler:"
            "n=5, n=6, n=7, n=8, n=9, n=10,  n=20,"
            " n=50, n=100, n=200, n=500, n=1000 \n"
             << endl;
        
            cin>>nDoGrafo;
            cout << "\nDigite o tipo de grafo(matriz = 1, lista = 2):" ;
            
            cin>>tipoGrafo;
           
            delete g;
            
            g = new Grafo(tipoGrafo);
            //abre arquivo JSON e joga no objeto grafo com o tipo desejado
            abreArq("dados/grafoN"+to_string(nDoGrafo)+".json",  g);
            //imprime o grafo
            cout << *g ;
        }else if(opcao==8){
              buscaGrafos(g);      
            
        
        }else if(opcao==9){
                   
            
        }
    }
     
    
    return 0;
}

int buscaGrafos(Grafo *g){
    int opcao;              
    cout <<"Digite 1 para realizar a busca em largura"<<endl;                   
    cout <<"Digite 2 para realizar a busca em profundidade"<<endl;                   
    cout <<"Digite 3 para realizar a busca Completa"<<endl;                   
    cout <<"Digite 4 para testar a conextividade"<<endl;
    cout <<"Digite 5 para testar se tem ciclo"<<endl;
    cout <<"Digite 6 para testar se é floresta"<<endl;
    cout <<"Digite 7  para testar se é árvore "<<endl;    
    cout <<"Digite 8 para encontrar uma floresta geradora."<<endl;
    cout <<"Digite 9  para realizar a Busca em Profundidade Recursiva"<<endl; 
    cout <<"Digite 10 para determinar as Distancias a partir de um vértice ."<<endl;
    cout << "Digite 11  para realizar o algoritmo de dijkstra"<<endl; 
    cout << "Digite 12 para determinar o algoritmo de floyd "<<endl;
    cin  >> opcao;
        
    if( opcao==1 ){
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        g->buscaEmLargura(raiz-1);
    }
    else if( opcao==2 ){ 
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        g->BuscaEmProfundidade(raiz-1);
    
    }   
    else if( opcao==3 ){
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        g->buscaCompleta(raiz-1);
    }
    else if( opcao==4 ){
        if (g->conectividade()){
            cout << "Grafo é conexo"<<endl;
        }else cout << "Grafo NÂO é conexo!"<<endl;
    }
    else if( opcao==5 ){
        if (g->temCiclo()){
            cout << "Grafo  tem ciclo!"<<endl;
        }else cout << "Grafo NÂO tem ciclo!"<<endl;
    }
    else if( opcao==6 ){
         if (g->isFloresta()){
             cout << "Grafo é floresta!"<<endl;
         }else cout << "Grafo NÂO é floresta!"<<endl;
    }
    else if( opcao==7 ){
        if (g->isArvore()){
            cout << "Grafo é árvore!"<<endl;
         }else cout << "Grafo NÂO é árvore!"<<endl;
    }
    else if( opcao==8 ){

        g->ObterFlorestaGeradora() ;
       
    }    
    else if( opcao==9 ){  
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        g->BuscaEmProfundidadeRec(raiz-1);
    }    
    else if(opcao==10 ){
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        vector<int>* discancias = g->determinarDistancias(raiz-1);
        for(vector<int>::iterator it = discancias->begin() ; it < discancias->end() ; it++){
            cout << "Distancia  do vertice " << ( it - discancias->begin() )+1 <<" =" << (*it) << "\n";
        }
        delete discancias;
    }else if(opcao==11 ){
        int raiz;
        cout <<"Digite a raiz "<<endl;
        cin >> raiz;
        cout<<"12321";
        g->dijkstra(raiz);
        
      
    }else if(opcao==12 ){
        //int raiz;
        //cout <<"Digite a raiz "<<endl;
        
        //g->Floyd();     
    }
}

