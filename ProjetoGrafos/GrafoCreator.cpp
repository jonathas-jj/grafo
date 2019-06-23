/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GrafoCreator.cpp
 * Author: jofra
 * 
 * Created on 4 de Maio de 2019, 14:07
 */
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "GrafoCreator.h"
 using namespace std;


//n=5, n=6, n=7, n=8, n=9, n=10,  n=20, n=50, n=100, n=200, n=500, n=1000.
void criarGrafos(){
    fstream arquivo;
    
    int nDoGrafo[12];
   //define o numero de vértices que cada arquivo vai ter . 
    nDoGrafo[0]=5;   nDoGrafo[1]=6;
    nDoGrafo[2]=7;   nDoGrafo[3]=8;
    nDoGrafo[4]=9;   nDoGrafo[5]=10;
    nDoGrafo[6]=20;  nDoGrafo[7]=50;
    nDoGrafo[8]=100; nDoGrafo[9]=200;
    nDoGrafo[10]=500;
    nDoGrafo[11]=1000;
    
    for(int i = 0 ; i < 12 ;i++){
       //gera o nome de cada arquivo grafo cm um numero X de vertices : 'grafoNX.json' 
        string nome="";int posiF;
        
        nome.append("dados/grafoN");
        nome.append(to_string(nDoGrafo[i]));
        nome.append(".json");
        
        arquivo.open(nome);
         
        if(!arquivo){
          
          Json::Value JSONArq;
          Json::StyledWriter styled;
          posiF = nome.find("j");
          
          JSONArq["nome"] = nome.substr(6,posiF-7);
          int verticeA,verticeB,mArestas,arestas[2];
                  
          
          //gera os vértices em json
          for(int j = 1 ; j <=  nDoGrafo[i] ;j++){
              verticeA = j;
               JSONArq["vertices"][j-1] = verticeA ;
               
               //sorteia o numero de aresta que o vertice terá
               mArestas= rand() % (nDoGrafo[i]-1) + 1;
               for(int j = 0 ; j <  mArestas ;j++){
                   vector<int> vertJaUsad(nDoGrafo[i]);
                   
                   //sorteia o vértice que será pareado.
                   verticeB = rand() % nDoGrafo[i] + 1;
                   if(verticeB == verticeA || binary_search(vertJaUsad.begin(),vertJaUsad.end(),verticeB)) continue;
                  
                   //vê se o vérticeB jah foi usado pra forma uma aresta com o verticeA 
                   vertJaUsad.push_back(verticeB);
                   JSONArq["arestas"][j][0] = verticeA;
                   JSONArq["arestas"][j][1] = verticeB;
                   
                   //define os pesos das arestas entre 1 e 100
                   JSONArq["arestas"][j][2] = rand() % (100) + 1;
               }
          }
          
      
         
          string s=styled.write(JSONArq);
          
          arquivo.open(nome, fstream::app);
          arquivo << s;
          
        }
        
        arquivo.close();
    }
}    