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
#include "tratamentos.h"

const int MAXALTURA  = 800;				//tamanho maximo aceito (pode ser alterado)
const int MAXLARGURA = 1200;
const int RGB = 3;

const int SOBEL_VER[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
const int SOBEL_HOR[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
const int GAUSS[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
const int LAPLACE[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

using namespace std;

int main() {
    unsigned char imagem[MAXALTURA][MAXLARGURA][RGB];	//a imagem propriamente dita
    int largura, altura;						    //dimensoes da imagem
    char tipo[4];								    //tipo da imagem
    ifstream arqentrada;						    //arquivo que contem a imagem original
    ofstream arqsaida;							    //arquivo que contera a imagem gerada
    char comentario[200], c;				        //auxiliares
    int i, j, k, valor, valor_aux;					//auxiliares

//*** LEITURA DA IMAGEM ***//
//inicialmente nao sera necessario entender nem mudar nada nesta parte

    //*** Abertura do arquivo ***//
    arqentrada.open("../img/ufv.pnm",ios::in); //Abre arquivo para leitura
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
            for(k=0;k<RGB;k++) {
                arqentrada >> valor;
                imagem[i][j][k] = (unsigned char)valor;
            }
        }
    //************************************//

    arqentrada.close();  //Fecha arquivo apos a leitura

//*** FIM DA LEITURA DA IMAGEM ***//



//*** TRATAMENTO DA IMAGEM ***//
//inicialmente e' nesta parte do codigo que voce vai trabalhar

    int fator, tipo_tratamento;

    tipo_tratamento = escolhe_tratamento();

    if(tipo_tratamento <= 2) {
        cout << "Qual o fator de escurecimento/clareamento (1-100) ? " << endl;
        cin >> fator;
    }

    // Faz a cópia da matriz original
    unsigned char imagem_c[MAXALTURA][MAXLARGURA][RGB];
    for(i = 0; i < altura; i++) {
        for(j = 0; j < largura; j++) {
            for(k = 0; k < RGB; k++) {
                imagem_c[i][j][k] = imagem[i][j][k];
            }
        }
    }

    switch(tipo_tratamento) {
        case 1: // escurece a imagem
            cout << "Escurecendo imagem..." << endl;
            for(i = 0; i < altura; i++)
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        valor = (int) imagem[i][j][k];            // pega o valor do pixel
                        valor -= fator;                           // escurece o pixel
                        valida_pixel(valor);                   // se der negativo deixa preto
                        imagem[i][j][k] = (unsigned char) valor;  // modifica o pixel
                    }
                }
            break;
        case 2: // clareia a imagem
            cout << "Clareando imagem..." << endl;
            for(i = 0; i < altura; i++)
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        valor = (int) imagem[i][j][k];            // pega o valor do pixel
                        valor += fator;                           // clareia o pixel
                        valida_pixel(valor);                   // se der maior que 255 deixa branco
                        imagem[i][j][k] = (unsigned char) valor;  //modifica o pixel
                    }
                }
            break;
        case 3: // imagem negativa
            cout << "Modificando a imagem para cores negativas..." << endl;
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        valor = (int) imagem[i][j][k];
                        valor = 255 - valor;                    // inverte a cor
                        imagem[i][j][k] = (unsigned char) valor;
                    }
                }
            }
            break;
        case 4: // imagem espelhada
            cout << "Espelhando imagem..." << endl;
            for(j = 0; j < largura/2; j++) {  // itera sobre cada coluna até a metade, se não elas retrocam de lugar
                for(i = 0; i < altura; i++) {
                    for(k = 0; k < RGB; k++) {
                        valor = (int) imagem[i][j][k];
                        valor_aux = (int) imagem[i][(largura - j)][k];       // valor_aux para guardar o valor das últimas colunas antes de trocar
                        imagem[i][(largura - j)][k] = (unsigned char) valor; // faz as trocas
                        imagem[i][j][k] = (unsigned char) valor_aux;
                    }
                }
            }
            break;
        case 5: // filtro de sobel
            cout << "Aplicando filtro de Sobel..." << endl;
            /*
             * Inicializa as matrizes do filtro de sobel, depois para cada pixel da imagem e
             * faz a soma da multiplição dos pixels ao redor com a matriz de sobel para horizontal e vertical,
             * valida se a soma não é negativa nem maior do que 255 e depois associa o valor máximos entre as duas somas
             * e modifica o pixel da imagem original.
             */
            int soma_ver, soma_hor;
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        soma_ver = (int) ((imagem_c[i-1][j-1][k] * SOBEL_VER[0][0]) + (imagem_c[i-1][j][k] * SOBEL_VER[0][1]) + (imagem_c[i-1][j+1][k] * SOBEL_VER[0][2]) +
                                          (imagem_c[i+1][j-1][k] * SOBEL_VER[2][0]) + (imagem_c[i+1][j][k] * SOBEL_VER[2][1]) + (imagem_c[i+1][j+1][k] * SOBEL_VER[2][2]));

                        soma_hor = (int) ((imagem_c[i-1][j-1][k] * SOBEL_HOR[0][0]) + (imagem_c[i-1][j+1][k] * SOBEL_HOR[0][2]) +
                                          (imagem_c[i][j-1][k] * SOBEL_HOR[1][0]) + (imagem_c[i][j+1][k] * SOBEL_HOR[1][2]) +
                                          (imagem_c[i+1][j-1][k] * SOBEL_HOR[2][0]) + (imagem_c[i+1][j+1][k] * SOBEL_HOR[2][2]));

                        valida_pixel(soma_ver);
                        valida_pixel(soma_hor);

                        soma_ver >= soma_hor ? valor = soma_ver : valor = soma_hor;

                        imagem[i][j][k] = (unsigned char) valor;
                    }
                }
            }
            break;
        case 6:
            cout << "Aplicando filtro de Laplace..." << endl;
            /*
             * Aplica o filtro de Laplace em cima da desfocagem gaussiana
             */
            int soma_gauss, soma_laplace;
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        soma_gauss = (int) ((imagem[i-1][j-1][k] * GAUSS[0][0]) + (imagem[i-1][j][k] * GAUSS[0][1]) + (imagem[i-1][j+1][k] * GAUSS[0][2]) +
                                            (imagem[i][j-1][k] * GAUSS[1][0]) + (imagem[i][j][k] * GAUSS[1][1]) + (imagem[i][j+1][k] * GAUSS[1][2]) +
                                            (imagem[i+1][j-1][k] * GAUSS[2][0]) + (imagem[i+1][j][k] * GAUSS[2][1]) + (imagem[i+1][j+1][k] * GAUSS[2][2]));

                        soma_gauss /= 8;

                        valida_pixel(soma_gauss);

                        imagem_c[i][j][k] = (unsigned char) soma_gauss;
                    }
                }
            }
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    for(k = 0; k < RGB; k++) {
                        soma_laplace = (int) ((imagem_c[i-1][j-1][k] * LAPLACE[0][0]) + (imagem_c[i-1][j][k] * LAPLACE[0][1]) + (imagem_c[i-1][j+1][k] * LAPLACE[0][2]) +
                                              (imagem_c[i][j-1][k] * LAPLACE[1][0]) + (imagem_c[i][j][k] * LAPLACE[1][1]) + (imagem_c[i][j+1][k] * LAPLACE[1][2]) +
                                              (imagem_c[i+1][j-1][k] * LAPLACE[2][0]) + (imagem_c[i+1][j][k] * LAPLACE[2][1]) + (imagem_c[i+1][j+1][k] * LAPLACE[2][2]));

                        valida_pixel(soma_laplace);

                        imagem[i][j][k] = (unsigned char) soma_laplace;
                    }
                }
            }
            break;
        case 7: // conversão para cinza
            cout << "Convertendo imagem para cinza..." << endl;
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {

                    // modifica os valores RGB com a média aritmética dos 3
                    valor = (int) (imagem[i][j][0] + imagem[i][j][1] + imagem[i][j][2]) / 3;

                    imagem[i][j][0] = (unsigned char) valor;
                    imagem[i][j][1] = (unsigned char) valor;
                    imagem[i][j][2] = (unsigned char) valor;
                }
            }
            break;
        case 8:
            cout << "Convertendo para preto e branco..." << endl;
            for(i = 0; i < altura; i++) {
                for(j = 0; j < largura; j++) {
                    valor = (int) (imagem[i][j][0] + imagem[i][j][1] + imagem[i][j][2]) / 3;

                    if(valor > 128) {
                        imagem[i][j][0] = (unsigned char) 255;
                        imagem[i][j][1] = (unsigned char) 255;
                        imagem[i][j][2] = (unsigned char) 255;
                    } else {
                        imagem[i][j][0] = (unsigned char) 0;
                        imagem[i][j][1] = (unsigned char) 0;
                        imagem[i][j][2] = (unsigned char) 0;
                    }
                }
            }
    }

//*** FIM DO TRATAMENTO DA IMAGEM ***//



//*** GRAVACAO DA IMAGEM ***//
//inicialmente nao sera necessario entender nem mudar nada nesta parte

    //*** Grava a nova imagem ***//
    arqsaida.open("../img/novaimagem.pnm",ios::out);	//Abre arquivo para escrita
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
            for(k=0;k<RGB;k++)
            arqsaida << (int)imagem[i][j][k] << endl;	//pixels

    arqsaida.close();		//fecha o arquivo
    //***************************//

    cout << "A imagem foi modificada com sucesso!" << endl;

//*** FIM DA GRAVACAO DA IMAGEM ***//

    return 0;
}
