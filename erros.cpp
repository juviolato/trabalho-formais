#include "erros.h"

int erroInicializacao()
{
    cout << "Numero incorreto de parametros." << endl;
    cout << "Uso correto: parser.exe <gramatica.txt>" << endl;
    system("pause");
    return 0;
}

int erroAbertura()
{
    cout << "Erro na abertura do arquivo de gramatica." << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}

int erroEstrutura()
{
    cout << "A gramatica nao esta estruturada corretamente." << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}

int erroVariaveis()
{
    cout << "Erro na leitura de variaveis!" << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}

int erroTerminais()
{
    cout << "Erro na leitura de terminais!" << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}

int erroProducoes()
{
    cout << "Erro na leitura de producoes!" << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}

int erroInicial()
{
    cout << "Erro na leitura da variavel inicial!" << endl;
    cout << "Encerrando o programa..." << endl;
    system("pause");
    return 0;
}
