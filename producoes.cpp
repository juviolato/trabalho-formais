#include "producoes.h"

bool pertenceProducao(Producao p, vector<Producao> P)
{
    for (Producao& x: P)
    {
        if (x.variavel == p.variavel && x.derivacao == p.derivacao)
            return true;
    }

    return false;
}


int Producao::getProducao(string temp)
{
    int i;
    string aux;

    for (i = 0; temp[i] != '>'; i++)
    {
        if (temp[i] >= 'A' && temp[i] <= 'Z')
            variavel += temp[i];
        else if (temp[i] != ' ' &&  temp[i] != '[' && temp[i] != ']' && temp[i] != '>')
            return 0;
    }

    for (i; temp[i] != '#' && temp[i] != '\0'; i++)
    {
        if ((temp[i] >= 'a' && temp[i] <= 'z') || (temp[i] >= 'A' && temp[i] <= 'Z'))
            aux += temp[i];
        else if (temp[i] == ']')
        {
            derivacao.push_back(aux);
            aux.clear();
        }
        else if (temp[i] != ' ' && temp[i] != '[' && temp[i] != '\t' && temp[i] != '>')
            return 0;
    }

    return 1;
}

void Producao::imprime()
{
    cout << variavel << " > ";
    for (int i = 0; i < derivacao.size(); i++)
        cout << derivacao[i] << " ";
}
