#include "grammar.h"
#define MAX 50

void Gramatica::copiaArvores(Nodo*& a, Nodo* b)
{
	if (b == nullptr)
		return;
	else
	{
		a = new Nodo(b->str, b->lin, b->col);
		copiaArvores(a->esq, b->esq);
		copiaArvores(a->dir, b->dir);
	}
}

bool Gramatica::comparaArvores(Nodo* a, Nodo* b)
{
	if (a == nullptr && b == nullptr)
		return true;
	else if (a != nullptr && b != nullptr)
	{
		if (a-> str == b->str && a->lin == b->lin && a->col == b->col)
			return comparaArvores(a->esq, b->esq) && comparaArvores(a->dir, b->dir);
		else
			return false;
	}
	else
		return true;
}

void Gramatica::montaArvore(vector<Nodo*>& a, vector<string> tabela[MAX][MAX], int n)
{
     for (int k = 1; k < n; k++)
     {
          for (string t: tabela[k-1][0])
          {
               for (string u: tabela[n-1-k][k])
               {
                    Producao p;
                    p.variavel = varInicial;
                    p.derivacao.push_back(t);
                    p.derivacao.push_back(u);

                    if (pertenceProducao(p, producoes))
                    {
                         Nodo *temp;
                         temp = new Nodo(varInicial, n-1, 0);
                         temp->esq = new Nodo(t, k-1, 0);
                         temp->dir = new Nodo(u, n-1-k, k);
                         a.push_back(temp);
                    }
               }
          }
     }

	 for (int i = 0; i < a.size(); i++)
	 {
		 montaAux(a, tabela, a[i], a[i]->esq);
		 montaAux(a, tabela, a[i], a[i]->dir);
	 }
}

void Gramatica::montaAux(vector<Nodo*>& a, vector<string> tabela[MAX][MAX], Nodo *t, Nodo *n)
{
	int i{n->lin}, j{n->col};

    for (int k = 1; k <= i; k++)
    {
    	for (string r: tabela[k-1][j])
        {
        	for (string s: tabela[i-k][j+k])
            {
            	Producao p;
                p.variavel = n->str;
                p.derivacao.push_back(r);
            	p.derivacao.push_back(s);

                if (pertenceProducao(p, producoes))
                {
                	if (n->esq == nullptr && n->dir == nullptr)
                    {
                    	n->esq = new Nodo(r, k-1, j);
                        n->dir = new Nodo(s, i-k, j+k);
                    }
                    else
                    {
                    	if ((n->esq->str != r || (n->esq->lin != k-1 || n->esq->col != j)) || (n->dir->str != s || (n->dir->lin != i-k || n->dir->col != j+k)))
                        {
							Nodo *auxEsq{n->esq}, *auxDir{n->dir};
							bool existe{false};

							n->esq = new Nodo(r, k-1, j);
							n->dir = new Nodo(s, i-k, j+k);
							Nodo *aux = nullptr;
							copiaArvores(aux, t);
							n->esq = auxEsq;
                            n->dir = auxDir;

							for (int m = 0; m < a.size(); m++)
							{
								if (comparaArvores(aux, a[m]))
									existe = true;
							}

							if (!existe && aux != nullptr)
								a.push_back(aux);
                        }
                    }
                }
			}
		}
	}
    if (n->lin > 0)
    {
    	montaAux(a, tabela, t, n->esq);
        montaAux(a, tabela, t, n->dir);
    }
}

void Gramatica::filtraAux(Nodo* a, vector<string>& fraseVars)
{
    if (a->esq == nullptr && a->dir == nullptr)
    {
        fraseVars.push_back(a->str);    // se a for folha da arvore, insere na fraseVars
    }
    else
    {
        filtraAux(a->esq, fraseVars);
        filtraAux(a->dir, fraseVars);
    }
}

void Gramatica::filtraArvores(vector<Nodo*>& a, vector<string>& w)
{
    for (int i = 0; i < a.size(); i++)
    {
        Nodo *arvore = a[i];
        vector<string> fraseVars;
        bool arvoreinvalida = false;

        filtraAux(a[i], fraseVars);     // fraseVars recebe a sequencia das variaveis-folha da arvore

        if (w.size() == fraseVars.size())
        {
            for (int j = 0; j < w.size(); j++)  // testa se todas as variaveis de fraseVars geram
            {                                       // exatamente os terminais de w
                Producao temp;
                temp.variavel = fraseVars[j];
                temp.derivacao.push_back(w[j]);

                if (!pertenceProducao(temp, producoes))     // se nao existe prod unitaria fraseVars[j] > w[j]
                    arvoreinvalida = true;
            }
        }
        else
            arvoreinvalida = true;      // se tiverem tamanhos diferentes, a arvore eh invalida

        if (arvoreinvalida)
            a.erase(a.begin()+i);      // exclui a[i]
    }
}

void Gramatica::imprimeArvore(Nodo *n, vector<string>& listaW)
{
	int written = 0;
   if (n != nullptr)
    {
        cout << "( " << n->str << " ";
        imprimeArvore(n->esq, listaW);
        imprimeArvore(n->dir, listaW);

		if (n->esq == nullptr && n->dir == nullptr)
		{
			for(Producao& p: producoes)
			{
				if((n->str) == p.variavel)
				{
					for (string& s: p.derivacao)
					{
						if(!ehVariavel(s) && written !=1)
						{
							cout <<" " << listaW.front() << " ";
							listaW.erase(listaW.begin());
							written = 1;
						}

					}
				}
			}
		}

	   cout << ") ";

	}


}

int Gramatica::getVariavel(string& temp)
{
    string var;
    for (int i = 0; temp[i] != '#' && temp[i] != '\0'; i++)
    {
        if (temp[i] >= 'A' && temp[i] <= 'Z')
        {
            var += temp[i];
        }
        else if (temp[i] != ' ' && temp[i] != '[' && temp[i] != ']' && temp[i] != '\t')
            return 0;
    }
    variaveis.push_back(var);
    return 1;
}

int Gramatica::getTerminal(string& temp)
{
    string term;
    for (int i = 0; temp[i] != '#' && temp[i] != '\0'; i++)
    {
        if (temp[i] >= 'a' && temp[i] <= 'z')
            term += temp[i];
        else if (temp[i] != ' ' && temp[i] != '[' && temp[i] != ']' && temp[i] != '\t')
            return 0;
    }

    terminais.push_back(term);
    return 1;
}

int Gramatica::getProducao(string& temp)
{
    Producao prod;
    if(!prod.getProducao(temp))
        return 0;
    producoes.push_back(prod);
    return 1;
}

int Gramatica::getInicial(string& temp)
{
    string var;
    for (int i = 0; temp[i] != '#' && temp[i] != '\0'; i++)
    {
        if (temp[i] >= 'A' && temp[i] <= 'Z')
            var += temp[i];
        else if (temp[i] != ' ' && temp[i] != '[' && temp[i] != ']' && temp[i] != '\t')
            return 0;
    }
    varInicial = var;
    return 1;
}

void Gramatica::simplificaVazias()
{
    vector<Producao> vazias, novas;
    int tamAnterior;

    for (Producao& p: producoes)    // insere no conjunto vazias todas as producoes que geram diretamente a palavra vazia
    {
        if (count(p.derivacao.begin(), p.derivacao.end(), "V"))
            vazias.push_back(p);
    }

    do          // insere as producoes que geram a palavra vazia indiretamente
    {
        tamAnterior = vazias.size();

        for (Producao& p: producoes)
        {
			bool insere{true};
            for (string& s: p.derivacao)
            {
                bool temVazia{false};

                for (Producao& v: vazias)       // se a derivacao de uma producao contiver variaveis que ja foram inseridas
                {                                    // no conjunto de producoes vazias, entao essa producao tambem e inserida nesse conjunto
                    if (v.variavel == s)
                        temVazia = true;
                }
				if (!temVazia)
					insere = false;
            }
			if (insere && !pertenceProducao(p, vazias))
                    vazias.push_back(p);
        }
    } while (tamAnterior != vazias.size());      // ate nao encontrar mais nenhuma

    for (Producao& p: producoes)        // mantem todas as producoes nao vazias
    {
        if (!count(p.derivacao.begin(), p.derivacao.end(), "V"))
            novas.push_back(p);
    }

    do
    {
        tamAnterior = novas.size();

        for (Producao& n: novas)        // pra cada producao nao vazia mantida
        {
            int i{0};
            for (string& s: n.derivacao)
            {
                bool temVazia{false};

                for (Producao& v: vazias)
                {
                    if (v.variavel == s)
                        temVazia = true;        // se a derivacao inclui uma variavel que leva a uma producao vazia
                }

                if (n.derivacao.size() > 1 && temVazia)
                {
                    Producao temp{n};                // cria uma nova producao sem essa variavel e a inclui
                    temp.derivacao.erase(temp.derivacao.begin()+i);            // no conjunto de producoes novas
                    if (!pertenceProducao(temp, novas))
                        novas.push_back(temp);
                }
                i++;
            }
        }
    } while (tamAnterior != novas.size());
    producoes = novas;
}

void Gramatica::substVars()
{
    map< string, vector<string> > fecho;

    // etapa 1: fecho transitivo de cada variavel
    for (string& A: variaveis)
    {
    	int tamAnterior;

    	do
    	{
    		tamAnterior = fecho[A].size();

    		for (Producao& p: producoes)  // para cada producao da gramatica
    		{
                // se a variavel for A ou pertencer ao fecho transitivo de A
    			if (p.variavel == A || count(fecho[A].begin(), fecho[A].end(), p.variavel))
    			{
    				string B = p.derivacao[0];

                    // se a producao for unitaria gerando uma variavel
    				if (A != B && p.derivacao.size() == 1 && count(variaveis.begin(), variaveis.end(), B))
    				{
                        // e essa variavel ainda nao tiver sido inclusa no fecho
    					if (!count(fecho[A].begin(), fecho[A].end(), B))
    						fecho[A].push_back(B);		// insere
    				}
    			}
    		}
    	} while (tamAnterior != fecho[A].size());  // ate que o tamanho do fecho pare de aumentar
    }



    // etapa 2: exclusao das producoes que substituem variaveis
    vector<Producao> P1;

    for (Producao& p: producoes)
    {
    	// se eh uma producao da forma A > alfa, com alfa nao sendo uma variavel
    	if (p.derivacao.size() > 1 || !count(variaveis.begin(), variaveis.end(), p.derivacao[0]))
    		P1.push_back(p);	// insere a producao no novo conjunto
    }

	for (string& A: variaveis)     // para toda A pertencente a V
	{
		for (string& B: fecho[A])  // e para todo B pertencente a FECHO-A
		{
			for (Producao& p: producoes)
			{
				if (B == p.variavel)    // se B > alfa pertence a P
				{
					vector<string> alfa = p.derivacao; // e alfa nao eh variavel
					if (p.derivacao.size() > 1 || !count(variaveis.begin(), variaveis.end(), alfa[0]))
					{
						Producao temp;
						temp.variavel = A;
						temp.derivacao = alfa;

                        if (!pertenceProducao(temp, P1))    // se ja nao houver producao igual em P1
						    P1.push_back(temp);             // entao P1 = P1 U {A > alfa}
					}
				}
			}
		}
	}

    producoes = P1;
}

void Gramatica::simbInut()
{
    // ETAPA 1

    // constroi V1 com todas as variaveis que geram terminais diretamente (A > a)
    vector<string> V1;
    for (Producao& p: producoes)
    {
        if (p.derivacao.size() == 1 && count(terminais.begin(), terminais.end(), p.derivacao[0]) && !count(V1.begin(), V1.end(), p.variavel))
            V1.push_back(p.variavel);
    }

    // adiciona a V1 todas as variaveis que geram terminais indiretamente (B > Ab)
    int tamAnteriorV1;
    do {
        tamAnteriorV1 = V1.size();

        for (Producao& p: producoes)
        {
            string A = p.variavel;
            bool insere = true;
            for (string& d: p.derivacao)     // se existe producao A > alfa
            {
                // tal que alfa seja palavra somente de terminais e/ou variaveis de V1
                if (!count(terminais.begin(), terminais.end(), d) && !count(V1.begin(), V1.end(), d))
                    insere = false;
            }

            if (insere && !count(V1.begin(), V1.end(), A))
                V1.push_back(A);        // insere A em V1
        }

    } while (tamAnteriorV1 != V1.size());   // ate que o cardinal de V1 nao aumente

    // constroi P1 com todas as producoes...
    vector<Producao> P1;
    for (Producao& p: producoes)
    {
        // ...cujas variaveis pertencam a V1 e que...
        if (count(V1.begin(), V1.end(), p.variavel))
        {
            bool insere = true;
            for (string& d: p.derivacao)
            {
                // ... geram apenas elementos de V1 ou terminais
                if (!count(V1.begin(), V1.end(), d) && !count(terminais.begin(), terminais.end(), d))
                    insere = false;
            }

            if (insere && !pertenceProducao(p, P1))
                P1.push_back(p);
        }
    }


    // ETAPA 2

    // constroi V2 e T2 com todas as variaveis e todos os terminais atingiveis a partir da variavel inicial, respectivamente
    vector<string> V2, T2;
    V2.push_back("S");  // V2 eh inicializado com a variavel inicial
    int tamAnteriorV2, tamAnteriorT2;

    do {
        tamAnteriorV2 = V2.size();
        tamAnteriorT2 = T2.size();

        for (Producao& p: P1)
        {
            string X = p.variavel;
            if (count(V2.begin(), V2.end(), X)) // para toda producao de P1 com variavel X pertencente a V2
            {
                for (string& d: p.derivacao)
                {
                    if (count(variaveis.begin(), variaveis.end(), d) && !count(V2.begin(), V2.end(), d))
                        V2.push_back(d);    // insere em V2 as variaveis da derivacao
                    else if(count(terminais.begin(), terminais.end(), d) && !count(T2.begin(), T2.end(), d))
                        T2.push_back(d);    // insere em T2 os terminais da derivacao
                }
            }
        }
    } while (tamAnteriorV2 != V2.size() || tamAnteriorT2 != T2.size()); // ate que os cardinais de V2 e T2 nao aumentem

    // constroi P2 com todas as producoes cujos simbolos pertencem a V2 ou T2
    vector<Producao> P2;
    for (Producao& p: producoes)
    {
        if (count(V2.begin(), V2.end(), p.variavel))    // se a variavel de p pertence a V2
        {
            bool insere = true;
            for (string& d: p.derivacao)
            {
                // se todas as derivacoes de p geram apenas elementos de V2 ou T2
                if (!count(V2.begin(), V2.end(), d) && !count(T2.begin(), T2.end(), d))
                    insere = false;
            }

            if (insere && !pertenceProducao(p,P2))
                P2.push_back(p);    // insere em P2
        }
    }

    variaveis = V2;
    terminais = T2;
    producoes = P2;

}

void Gramatica::simplifica()
{
     simplificaVazias();
     substVars();
     simbInut();
}

void Gramatica::fncEtapa2()
{
    int indice{0};
    vector<Producao> P1;
    int i = 1;

    for (Producao& p: producoes)
    {
        if (p.derivacao.size() >= 2)
        {
            for (string& term: p.derivacao)
            {
                bool substituido = false;
                if (count(terminais.begin(), terminais.end(), term))     //  se a derivacao contiver um terminal
                {
                    Producao temp;
                    temp.variavel = "C" + to_string(indice);        // cria uma variavel intermediaria cuja unica producao e unitaria e gera esse terminal
                    variaveis.push_back(temp.variavel);
                    temp.derivacao.push_back(term);     // adiciona a variavel na gramatica
                    P1.push_back(temp);
                    term = temp.variavel;           // substituindo o terminal da derivacao pela variavel dessa nova producao
                    indice++;
                }
            }
        }

        if (!pertenceProducao(p, P1))
            P1.push_back(p);
    }

    producoes = P1;
}

void Gramatica::fncEtapa3()
{
    int indice{0};
    vector<Producao> P1;
    int i = 1;

    for (Producao& p: producoes)
    {
		while (p.derivacao.size() >= 3)
		{
			Producao temp;
            temp.variavel = "D" + to_string(indice);        // cria uma variavel intermediaria cuja unica producao e unitaria e gera esse terminal
            variaveis.push_back(temp.variavel);
			temp.derivacao.push_back(p.derivacao[0]);     // adiciona a variavel na gramatica
			temp.derivacao.push_back(p.derivacao[1]);
			p.derivacao.erase(p.derivacao.begin(), p.derivacao.begin()+2);
			p.derivacao.insert(p.derivacao.begin(),temp.variavel);
			indice++;
			P1.push_back(temp);
		}

		if (!pertenceProducao(p, P1))
			P1.push_back(p);
	}

    producoes = P1;
}

void Gramatica::imprime()
{
    cout << endl;
    cout << "Variaveis = {";
    for (int i = 0; i < variaveis.size(); i++)
    {
        cout << variaveis[i];
        if (i < variaveis.size() - 1)
            cout << ", ";
        else
            cout << "}" << endl;
    }
    cout << "Terminais = {";
    for (int i = 0; i < terminais.size(); i++)
    {
        cout << terminais[i];
        if (i < terminais.size() - 1)
            cout << ", ";
        else
            cout << "}" << endl;
    }
    cout << "Inicial = " << varInicial << endl;
    cout << "Regras = {";
    for (int i = 0; i < producoes.size(); i++)
    {
        producoes[i].imprime();
        if (i < producoes.size() - 1)
            cout << ", ";
        else
            cout << "}" << endl;
    }
    cout << endl;
}

void Gramatica::reconheceCYK(vector<string>& w)
{
	int i{0}, n = w.size();
    vector<string> tabela[MAX][MAX];

    for (string t: w)       // monta primeira linha da tabela
    {                               // com producoes unitarias de terminais
        for (Producao p: producoes)
        {
            if (p.derivacao[0] == t)
                tabela[0][i].push_back(p.variavel);
        }
        i++;
    }

	for (int i = 0; i < n; i++)        // monta linhas seguintes
	{
		for (int j = 0; j <= n-i-1; j++)
		{
			for (int k = 1; k <= i; k++)
			{
				for (Producao& p: producoes)
				{
					if (p.derivacao.size() > 1)    // para cada derivacao, insere variavel na posicao atual
					{                      // se a sua derivacao estiver nas posicoes [k-1][j] e [i-k][j+k]
						if (count(tabela[k-1][j].begin(), tabela[k-1][j].end(), p.derivacao[0]) && count(tabela[i-k][j+k].begin(), tabela[i-k][j+k].end(), p.derivacao[1]) && !count(tabela[i][j].begin(),tabela[i][j].end(), p.variavel))
							tabela[i][j].push_back(p.variavel);
					}
				}
			}
		}
	}

    int max = 0;
    for (int i = n-1; i >=0; i--)
	{
        for (int j = 0; j < n-i; j++)
		{
            if (tabela[i][j].size() > max)
            max = tabela[i][j].size();
		}
	}

    cout << endl << endl << "TABELA CYK:" << endl;
	for (int i = n-1; i >=0; i--)     // exibe tabela montada
	{
        for (int k = 0; k < (n-i)*(3*max+2)+1; k++)
            cout << "-";

        cout << endl;
		for (int j = 0; j < n-i; j++)
		{
            int falta = 3*max;
			cout << "| ";
			for (string s: tabela[i][j])
            {
                cout << setw(2) << s << " ";
                falta -= 3;
            }
            for (int k = 0; k < falta; k++)
                cout << " ";
		}
		cout << "|" << endl;
	}

    for (int k = 0; k < n*(3*max+2)+1; k++)
        cout << "-";
    cout << endl;
    for (string s: w)
    {
        int falta = 3*max;
        cout << "| " << s;
        falta -= s.size();
        for (int i = 0; i < falta; i++)
            cout << " ";
    }
    cout << "|" << endl;
    for (int k = 0; k < n*(3*max+2)+1; k++)
        cout << "-";


	if (count(tabela[n-1][0].begin(), tabela[n-1][0].end(), varInicial))
	{
        cout << endl << "Essa sentenca pertence a linguagem gerada por essa gramatica!" << endl << endl;

        vector<Nodo*> a;
        montaArvore(a, tabela, n);

        filtraArvores(a, w);

        cout << endl << "ARVORES DE DERIVACAO:" << endl;
        vector<string> listaW;
		for(string& z: w)
		{
			listaW.push_back(z);
		}
		for (int i = 0; i < a.size(); i++)
        {
            imprimeArvore(a[i], listaW);
            cout << endl << endl;
			for(string& z: w)
			{
				listaW.push_back(z);
			}
        }
        cout << endl << endl;
    }
	else
		cout << endl << "Essa sentenca nao pertence a linguagem gerada por essa gramatica :(" << endl << endl;

}

bool Gramatica::ehVariavel(string v)
{
	for(string& i: variaveis)
	{
		if(v==i)
			return true;
	}
	return false;
}
