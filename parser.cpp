/* > Leitor de Gramaticas Livres de Contexto a partir de um arquivo de texto. 11/05/2018
   > Simplifica GLCs e as transforma na Forma Normal de Chomsky. 18/05/2018
   > Verifica pertinencia de sentencas a linguagem gerada por uma gramatica, usando o algoritmo CYK,
        e imprime as arvores de derivacao possiveis. 15/06/2018

   Feito em C++11 por Artur Waquil Campana, Julia Peixoto Violato e Nicolau Pereira Alff.
   Compilar como segue: g++ -std=c++11 erros.cpp grammar.cpp producoes.cpp arquivo.cpp parser.cpp -o parser */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "grammar.h"
#include "producoes.h"
#include "erros.h"
#include "arquivo.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)  // se nao receber exatamente um parametro, encerra com mensagem de erro
        return erroInicializacao();

    char* nomeArq = argv[1];    // nomeArq recebe o nome passado na entrada

	vector<string> sentenca;
	string temp;
    Gramatica grammar;

	cout << string(100, '\n');

    if (!leGramatica(grammar, nomeArq)) // le a gramatica a partir do arquivo de texto
        return 0;                       // se der erro na leitura, encerra o programa
	cout << endl << "Gramatica lida:" << endl;
    grammar.imprime();

    grammar.simplifica();
	cout << endl << "Gramatica simplificada:" << endl;
	grammar.imprime();

    grammar.fncEtapa2();
	grammar.fncEtapa3();
	cout << endl << "Gramatica na Forma Normal de Chomsky:" << endl;
	grammar.imprime();

	cout << endl << "Sentenca (simbolos separados por espacos, por favor - terminar input com Ctrl+D) > ";
	while (cin >> temp)
		sentenca.push_back(temp);

	grammar.reconheceCYK(sentenca);

    return 0;
}
