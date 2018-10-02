#ifndef _GRAMMAR_H
#define _GRAMMAR_H
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "erros.h"
#include "producoes.h"
#define MAX 50
using namespace std;

struct Nodo
{
     string str;
     Nodo *esq, *dir;
     int lin, col;

     Nodo(string s, int y, int x)
     {
          this->str = s;
          this->esq = nullptr;
          this->dir = nullptr;
          lin = y;
          col = x;
     }
};

class Gramatica
{
    vector<string> variaveis;
    vector<string> terminais;
    string varInicial;
    vector<Producao> producoes;

    void simplificaVazias();
    void substVars();
    void simbInut();

public:
    int getVariavel(string& temp);
    int getTerminal(string& temp);
    int getProducao(string& temp);
    int getInicial(string& temp);
    void imprime();

    void simplifica();
    void fncEtapa2();
    void fncEtapa3();

	void copiaArvores(Nodo*& a, Nodo* b);
	bool comparaArvores(Nodo* a, Nodo* b);
    void montaArvore(vector<Nodo*>& a, vector<string> tabela[MAX][MAX], int n);
    void montaAux(vector<Nodo*>& a, vector<string> tabela[MAX][MAX], Nodo *t, Nodo *n);
    void filtraAux(Nodo* a, vector<string>& fraseVars);
    void filtraArvores(vector<Nodo*>& a, vector<string>& w);
	void imprimeArvore(Nodo *n, vector<string>& listaW);
    void reconheceCYK(vector<string>& w);
	bool ehVariavel(string v);
};

#endif
