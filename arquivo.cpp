#include "arquivo.h"

int leGramatica(Gramatica& grammar, string nomeArq)
{
    ifstream arquivo;
    arquivo.open(nomeArq);
    if (!arquivo.is_open())
        return erroAbertura();      // encerra o programa se nao conseguir abrir o arquivo

    string temp;

    getline(arquivo, temp);
    if(temp.find("#Terminais") == temp.npos)    // testa se a primeira linha é o marcador de terminais
        return erroEstrutura();    // encerra o programa se nao encontrar o marcador correto

    getline(arquivo, temp);
    while (temp.find("#Variaveis") == temp.npos)    // le terminais ate encontrar marcador de variaveis
    {
        if(!grammar.getTerminal(temp))
            return erroTerminais();     // encerra se gramatica nao estiver corretamente formatada
        getline(arquivo, temp);
    }

    getline(arquivo, temp);
    while (temp.find("#Inicial") == temp.npos)      // le variaveis ate encontrar marcador de variaveis
    {
        if(!grammar.getVariavel(temp))
            return erroVariaveis();     // encerra se gramatica nao estiver corretamente formatada
        getline(arquivo, temp);
    }

    getline(arquivo, temp);
    if(!grammar.getInicial(temp))   // le a variavel inicial
        return erroInicial();       // encerra se gramatica nao estiver corretamente formatada

    getline(arquivo, temp);
    if (temp.find("#Regras") == temp.npos)
        return erroEstrutura();     // encerra o programa se nao encontrar o marcador correto

    getline(arquivo, temp);
    while(arquivo.good())       // le producoes ate encontrar o final do arquivo ou um erro
    {
        if(!grammar.getProducao(temp))
            return erroProducoes();     // encerra se gramatica nao estiver corretamente formatada
        getline(arquivo, temp);
    }
    arquivo.close();

    return 1;
}
