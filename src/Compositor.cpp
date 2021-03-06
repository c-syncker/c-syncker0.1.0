#include "Compositor.h"

Compositor::Compositor()
{
    dicionarioAlturasCifradasNumerosMIDI["c"] = 0;
    dicionarioAlturasCifradasNumerosMIDI["c#"] = 1;
    dicionarioAlturasCifradasNumerosMIDI["d"] = 2;
    dicionarioAlturasCifradasNumerosMIDI["d#"] = 3;
    dicionarioAlturasCifradasNumerosMIDI["e"] = 4;
    dicionarioAlturasCifradasNumerosMIDI["f"] = 5;
    dicionarioAlturasCifradasNumerosMIDI["f#"] = 6;
    dicionarioAlturasCifradasNumerosMIDI["g"] = 7;
    dicionarioAlturasCifradasNumerosMIDI["g#"] = 8;
    dicionarioAlturasCifradasNumerosMIDI["a"] = 9;
    dicionarioAlturasCifradasNumerosMIDI["a#"] = 10;
    dicionarioAlturasCifradasNumerosMIDI["b"] = 11;
}

Compositor::~Compositor()
{
    //dtor
}

list<Nota> Compositor::associeEmNotas(list<string> &alturasRecebidas, list<int> &duracoesRecebidas)
{
   
    // limpo a lista de notas para não ficar acumulando resultados passados.
    listaDeNotas.erase(listaDeNotas.begin(), listaDeNotas.end());
    if(alturasRecebidas.size() == duracoesRecebidas.size() && (!alturasRecebidas.empty() && !duracoesRecebidas.empty())){

            int duracao = 0;
            // auto altura = alturas.begin();
            // auto duracao = duracoes.begin();
            string uniaoDasListas;

            vector<int> vetorDeDuracoes = convertaListEmVector(duracoesRecebidas);
            vector<string> vetorDeAlturasCifras = convertaListEmVector(alturasRecebidas);

            for(int i=0; i < vetorDeAlturasCifras.size(); i++)
            {
                duracao = vetorDeDuracoes[i];
                uniaoDasListas = vetorDeAlturasCifras[i] + to_string(duracao);
                calculeEnarmonia(uniaoDasListas, uniaoDasListas.size());
            }


    }else{
        cout << "Tamanho das listas diferente. Por favor, sincronize-as primeiro." << endl;
    }

    return listaDeNotas;

}

list<Nota> Compositor::associeEmNotas(list<string> &alturasRecebidas, list<string> &duracoesRecebidas)
{
    list<int> listaAux;
    if(alturasCifras.empty() || duracoes.empty())
    {
        cout << "Listas de tamanhos diferentes!" << endl;

        limpeListaDeNotas();

        return listaDeNotas;
    }

    for(auto elemento : duracoesRecebidas)
    {
        listaAux.push_back(stoi(elemento));
    }

    associeEmNotas(alturasRecebidas, listaAux);

    return listaDeNotas;
}

void Compositor::desassociar(list<Nota> &listaDeNotas){
   
    alturasCifras.erase(alturasCifras.begin(), alturasCifras.end());
    duracoes.erase(duracoes.begin(), duracoes.end());
    listaDeIDs.erase(listaDeIDs.begin(), listaDeIDs.end());

    if(listaDeNotas.empty())
    {
        cout << "Lista de notas vazia!" << endl;
    }
    else
    {
        for(auto nota : listaDeNotas)
        {
            alturasCifras.push_back(nota.pegueAltura()+nota.pegueAcidente1()+nota.pegueAcidente2()+to_string(nota.pegueOitava()));
            alturasCifrasSemOitavas.push_back(nota.pegueAltura()+nota.pegueAcidente1()+nota.pegueAcidente2());
            duracoes.push_back(nota.pegueDuracao());
            listaDeIDs.push_back(nota.pegueID());
            //intensidades.push_back(nota->pegueIntensidade());
        }
    }
}

void Compositor::calculeEnarmonia(string entrada, int tam)
{

    Nota nota;
    //int tam = entrada.size();
    int contadorDeAcidentes = 0;

    if(tam == 5){

        nota.atribuaAltura    ( string(1,entrada[tam-5]) );
        nota.atribuaAcidente1 ( string(1,entrada[tam-4]) );
        nota.atribuaAcidente2 ( string(1,entrada[tam-3]) );
        nota.atribuaOitava    ( entrada[tam-2] - '0' );
        nota.atribuaDuracao   ( entrada[tam-1] - '0' );

        // testando o acidente
        if(entrada[tam-4] == 'b'){
            contadorDeAcidentes -= 1;
        }else if(entrada[tam-4] == '#'){
            contadorDeAcidentes += 1;
        }

        // testando o acidente
        if(entrada[tam-3] == 'b'){
            contadorDeAcidentes -= 1;
        }else if(entrada[tam-3] == '#'){
            contadorDeAcidentes += 1;
        }
        
    }else{

        /*
            Esse bloco processa a string deixando-a apenas com números
            e os coloca em durações e oitavas
        */
        string copiaEntrada = entrada;
        removaChars(copiaEntrada);

        if(copiaEntrada.size() == 0){
            nota.atribuaOitava (5);
            nota.atribuaDuracao (0);
        }else{
            if(copiaEntrada.size() == 1){
                nota.atribuaOitava  ( 5 );
                nota.atribuaDuracao ( stoi(copiaEntrada) );
            }else{
                nota.atribuaOitava  ( copiaEntrada.at(0) - '0');
                nota.atribuaDuracao ( copiaEntrada.at(1) - '0');
            }
        } 
    
        /*
            Esse bloco processa a string, deixando apenas 
            as letras
        */
        string entradaSemNumeros, format="";
        regex number("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
        entradaSemNumeros = regex_replace(entrada, number, format, regex_constants::format_default);

        if (entradaSemNumeros.size() == 0){
            nota.atribuaAltura    ("");
            nota.atribuaAcidente1 ("");
            nota.atribuaAcidente2 ("");

        } else if(entradaSemNumeros.size() == 1){
            nota.atribuaAltura    ( entradaSemNumeros );
            nota.atribuaAcidente1 ("");
            nota.atribuaAcidente2 ("");
        } else {
            if(entradaSemNumeros.size() == 2){
                nota.atribuaAltura    ( string(1,entradaSemNumeros[0]) );
                nota.atribuaAcidente1 ( string(1,entradaSemNumeros[1]) );
                
                // testando o acidente
                if(entradaSemNumeros[1] == 'b'){
                    contadorDeAcidentes -= 1;
                }else if(entradaSemNumeros[1] == '#'){
                    contadorDeAcidentes += 1;
                }

                nota.atribuaAcidente2 ("");
            }else{
                nota.atribuaAltura    ( string(1, entradaSemNumeros.at(0) ) );
                nota.atribuaAcidente1 ( string(1, entradaSemNumeros.at(1) ) );
                nota.atribuaAcidente2 ( string(1, entradaSemNumeros.at(2) ) );
                
                // testando o acidente
                if(entradaSemNumeros[1] == 'b'){
                    contadorDeAcidentes -= 1;
                }else if(entradaSemNumeros[1] == '#'){
                    contadorDeAcidentes += 1;
                }

                // testando o acidente
                if(entradaSemNumeros[2] == 'b'){
                    contadorDeAcidentes -= 1;
                }else if(entradaSemNumeros[2] == '#'){
                    contadorDeAcidentes += 1;
                }
            }
        }
    }

    int numeroMIDI = 0;
    numeroMIDI = nota.pegueOitava() * 12;
    numeroMIDI += contadorDeAcidentes;
    
    string chave = nota.pegueAltura();
    transform(chave.begin(), chave.end(), chave.begin(), ::tolower);

    numeroMIDI += dicionarioAlturasCifradasNumerosMIDI[chave];
    
    nota.atribuaAlturaNumerica(numeroMIDI);

    if(nota.pegueDuracao() != 0){
        duracoesString.push_back( to_string(nota.pegueDuracao()) );
    }

    alturasCifras.push_back( nota.pegueAltura()+nota.pegueAcidente1()+nota.pegueAcidente2() );
    alturasConvertidasCifrasNumerosMIDI.push_back(nota.pegueAlturaNumerica());
    listaDeNotas.push_back(nota);

}

void Compositor::removaChars(string &entrada){

    unordered_map<char, int> digits = {
        {'0',1},
        {'1',1},
        {'2',1},
        {'3',1},
        {'4',1},
        {'5',1},
        {'6',1},
        {'7',1},
        {'8',1},
        {'9',1}
    };

    regex whitespace("[[:s:]]+"); 

    int i=0;
    for(; i < entrada.size(); i++){
        if( digits.find(entrada[i]) == digits.end()){
            entrada[i] = ' ';
        }
    }
    const string format="";
    entrada = regex_replace(entrada, whitespace, format, regex_constants::format_default);
}

template <typename T>
vector<T> Compositor::convertaListEmVector(list<T> listaParaConverter)
{
    vector<T> resultado;
    resultado  = { listaParaConverter.begin(), listaParaConverter.end() };
    return resultado;
}


void Compositor::limpeListaDeNotas() { listaDeNotas.erase(listaDeNotas.begin(), listaDeNotas.end()); }
void Compositor::limpeListaDeDuracoes() { duracoes.erase(duracoes.begin(), duracoes.end()); }

void Compositor::limpeListaDeAlturas() 
{ 
    alturasCifras.erase(alturasCifras.begin(), alturasCifras.end()); 
    alturasCifrasSemOitavas.erase( alturasCifrasSemOitavas.begin(), alturasCifrasSemOitavas.end() );
}

void Compositor::limpeListas()
{
    limpeListaDeNotas();
    limpeListaDeAlturas();
    limpeListaDeDuracoes();
}

list<Nota>   *Compositor::pegueListaDeNotas() { return &listaDeNotas;}
list<int>    *Compositor::pegueListaDuracoes() { return &duracoes;}
list<int>    *Compositor::pegueListaIntensidades() { return &intensidades;}
list<string> *Compositor::pegueListaAlturas() { return &alturasCifras;}
list<string> *Compositor::pegueListaDeIDs() { return &listaDeIDs;}

list<string> *Compositor::pegueListaDeAlturasSemOitavas() { 
    return &alturasCifrasSemOitavas;
}

list<int> Compositor::pegueAlturasNumerosMIDI() { return alturasConvertidasCifrasNumerosMIDI; }
