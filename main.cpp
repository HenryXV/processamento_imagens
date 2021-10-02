// INF110 - Trabalho pratico 3
//
// programa para ler, modificar e gravar uma imagem no formato PNM
//
// Autores: Andre Gustavo dos Santos			(criado em 16/06/14)
//          Andre Gustavo dos Santos			(modificado em 22/05/18)
//			Andre Gustavo dos Santos			(modificado em 13/09/21)
//          Henrique Resende Silva

#include <iostream>
#include <fstream>
#include <cstring>
#include "opcoes_tratamento.h"

const int MAXALTURA  = 500;				//tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 500;

using namespace std;

int main() {
    unsigned char imagem[MAXALTURA][MAXLARGURA];	//a imagem propriamente dita
    int largura, altura;						    //dimensoes da imagem
    char tipo[4];								    //tipo da imagem
    ifstream arqentrada;						    //arquivo que contem a imagem original
    ofstream arqsaida;							    //arquivo que contera a imagem gerada
    char comentario[200], c;				        //auxiliares
    int i, j, valor;								//auxiliares


//*** LEITURA DA IMAGEM ***//
//inicialmente nao sera necessario entender nem mudar nada nesta parte

    //*** Abertura do arquivo ***//
    arqentrada.open("/home/henry/CLionProjects/processamento_imagens/img_exemplos/escultura.pnm",ios::in); //Abre arquivo para leitura
    if (!arqentrada) {
        cout << "Nao consegui abrir arquivo\n";
        return 0;
    }
    //***************************//


    //*** Leitura do cabecalho ***//
    arqentrada >> tipo;	//Le o tipo de arquivo
    arqentrada.get();		//Le e descarta o \n do final da 1a. linha

    if (strcmp(tipo,"P2")==0) {
        cout << "Imagem em tons de cinza\n";
    }
    else if (strcmp(tipo,"P3")==0) {
        cout << "Imagem colorida\n";
    }
    else if (strcmp(tipo,"P1")==0) {
        cout << "Imagem preto e branco\n";
        cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
        arqentrada.close();
        return 0;
    }
    else if (strcmp(tipo,"P4")==0 || strcmp(tipo,"P5")==0 || strcmp(tipo,"P6")==0) {
        cout << "Imagem no formato RAW\n";
        cout << "Desculpe, nao trabalho com esse tipo de imagem.\n";
        arqentrada.close();
        return 0;
    }

    while((c = arqentrada.get()) == '#')	//Enquanto for comentario
        arqentrada.getline(comentario,200);	//Le e descarta a linha "inteira"

    arqentrada.putback(c);	//Devolve o caractere lido para a entrada, pois como
    //nao era comentario, era o primeiro digito da largura

    arqentrada >> largura >> altura;	//Le o numero de pixels da horizontal e vertical
    cout << "Tamanho: " << largura << " x " << altura << endl;
    if (largura>MAXLARGURA) {
        cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXLARGURA << " pixels de largura.\n";
        arqentrada.close();
        return 0;
    }
    if (altura>MAXALTURA) {
        cout << "Desculpe, ainda nao trabalho com imagens com mais de " << MAXALTURA << " pixels de altura.\n";
        arqentrada.close();
        return 0;
    }

    arqentrada >> valor;	//Valor maximo do pixel (temos que ler, mas nao sera usado)
    //****************************//


    //*** Leitura dos pixels da imagem ***//
    for(i=0;i<altura;i++)
        for(j=0;j<largura;j++) {
            arqentrada >> valor;
            imagem[i][j] = (unsigned char)valor;
        }
    //************************************//

    arqentrada.close();  //Fecha arquivo apos a leitura

//*** FIM DA LEITURA DA IMAGEM ***//



//*** TRATAMENTO DA IMAGEM ***//
//inicialmente e' nesta parte do codigo que voce vai trabalhar

    int fator, tipo_tratamento;

    tipo_tratamento = escolhe_tratamento();

    if(tipo_tratamento <= 2) {
        cout << "Qual o fator de escurecimento/clareamento (1-255) ? " << endl;
        cin >> fator;
    }

    switch(tipo_tratamento) {
        case 1: // escurece a imagem
            for(i = 0; i < altura; i++)
                for(j = 0; j < largura; j++) {
                    valor = (int) imagem[i][j];            // pega o valor do pixel
                    valor -= fator;                        // escurece o pixel
                    if (valor < 0)                         // se der negativo
                        valor = 0;                         // deixa preto
                    imagem[i][j] = (unsigned char) valor;  // modifica o pixel
                }
            break;
        case 2: // clareia a imagem
            for(i = 0; i < altura; i++)
                for(j = 0; j < largura; j++) {
                    valor = (int) imagem[i][j];            // pega o valor do pixel
                    valor += fator;                        // clareia o pixel
                    if (valor > 255)                       // se der maior que 255
                        valor = 255;                       // deixa branco
                    imagem[i][j] = (unsigned char) valor;  //modifica o pixel
                }
            break;
        case 3: // imagem negativa
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    valor = (int) imagem[i][j];
                    valor = 255 - valor;                    // inverte a cor
                    imagem[i][j] = (unsigned char) valor;
                }
            }
            break;
        case 4: // imagem espelhada
            int valor_aux;
            for(j = 0; j < largura/2; j++) {  // itera sobre cada coluna até a metade, se não elas retrocam de lugar
                for(i = 0; i < altura; i++) {
                    valor = (int) imagem[i][j];
                    valor_aux = (int) imagem[i][(largura - j)];       // valor_aux para guardar o valor das últimas colunas antes de trocar
                    imagem[i][(largura - j)] = (unsigned char) valor; // faz as trocas
                    imagem[i][j] = (unsigned char) valor_aux;
                }
            }
            break;
    }

//*** FIM DO TRATAMENTO DA IMAGEM ***//



//*** GRAVACAO DA IMAGEM ***//
//inicialmente nao sera necessario entender nem mudar nada nesta parte

    //*** Grava a nova imagem ***//
    arqsaida.open("/home/henry/CLionProjects/processamento_imagens/img_exemplos/novaimagem.pnm",ios::out);	//Abre arquivo para escrita
    if (!arqsaida) {
        cout << "Nao consegui criar novaimagem.pnm\n";
        return 0;
    }

    arqsaida << tipo << endl;							//tipo
    arqsaida << "# INF110\n";							//comentario
    arqsaida << largura << " " << altura;	//dimensoes
    arqsaida << " " << 255 << endl;				//maior valor
    for(i=0;i<altura;i++)
        for(j=0;j<largura;j++)
            arqsaida << (int)imagem[i][j] << endl;	//pixels

    arqsaida.close();		//fecha o arquivo
    //***************************//

//*** FIM DA GRAVACAO DA IMAGEM ***//

    return 0;
}
