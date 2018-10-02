#ifndef _PRODUCOES_H
#define _PRODUCOES_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "erros.h"
using namespace std;

class Producao
{
public:
    string variavel;
    vector<string> derivacao;

    int getProducao(string temp);
    void imprime();
};

bool pertenceProducao(Producao p, vector<Producao> P);



#endif
