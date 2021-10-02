#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

int escolhe_tratamento() {
    string tratamento;

    cout << "1. Escurecer\n" << "2. Clarear\n" << "3. Negativo\n" << "4. Espelhar\n" << endl;
    cin >> tratamento;

    vector<vector<string>> respostas_possiveis = {{"1", "escurecer"},
                                                  {"2", "clarear"},
                                                  {"3", "negativo"},
                                                  {"4", "espelhar"}};

    /* usa o método std::any_of para comparar a resposta (transformada para minusculo)
    * dada pelo usuário com o vetor de possíveis respostas para cada tratamento,
    * caso não esteja no vetor de respostas, repete até uma resposta válida seja dada
    */
    int index = 0;
    while(true) {
        transform(tratamento.begin(), tratamento.end(), tratamento.begin(), ::tolower);
        for(auto opcao : respostas_possiveis) {
            index++;
            if(any_of(opcao.begin(), opcao.end(), [tratamento](string o){return o == tratamento;})) {
                return index;
            } else if(index == respostas_possiveis.size()) {
                index = 0;
                cout << "Não existe a opcão " << tratamento << ", escolha novamente: " << endl;
                cin >> tratamento;
            }
        }
    }
}




