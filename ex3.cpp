/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva
Matrícula : 2312130230
Exercício 3 — Expressão Vetorial
--------------------------------------------------------------------------------------------------------------------------
Implemente a expressão: a[i] = x[i]^2 + y[i]^2 + z[i]^2 para vetores de tamanho 1.000.000.
a) Faça a versão sequencial.
b) Paralelize com #pragma omp parallel for schedule(static).
c) Mostre o tempo total de execução em cada versão.
--------------------------------------------------------------------------------------------------------------------------
*/

#include <iostream>   
#include <vector>     
#include <omp.h> 
#include <string>     
#include <iomanip>    // Para formatar a saída de ponto flutuante (std::setprecision)

namespace ConsoleColors {
    // Cor
    const std::string RESET = "\033[0m";
    const std::string CIANO = "\033[36m";     // Texto Ciano
    // Estilo
    const std::string NEGRITO = "\033[1m";    // Negrito/Brilho
}

int main (){
    const int N = 1000000;   // Tamanho dos vetores (1 milhão), conforme especificado no enunciado.

    // Declaração dos vetores de entrada (x, y, z) e o vetor de resultado (v, que representa 'a')
    std::vector<int> x(N), y(N), z(N), v(N);

    // Inicializa valores simples para os vetores de entrada, testei outra sequencia similar do exemplo
    for (int i = 0; i < N; i++) {
        x[i] = i;      // Valores: [0, 1, 2, 3, 4, ...]
        y[i] = i + 2;  // Valores: [2, 3, 4, 5, 6, ...]
        z[i] = i + 4;  // Valores: [4, 5, 6, 7, 8, ...]
    }

    // Configura a precisão de 6 casas decimais para a exibição do tempo (ponto flutuante).
    std::cout << std::fixed << std::setprecision(6);

    // a) Faça a versão sequencial.
    std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ VERSÃO SEQUENCIAL ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n"; 
    std::cout << "\nCalculando a = x*x + y*y + z*z para vetores de tamanho " << N << "\n";

    // Função OpenMP usada para medir o tempo de execução.
    double inicio_seq = omp_get_wtime();

    // Loop sequencial para calcular a expressão a[i] = x[i]^2 + y[i]^2 + z[i]^2
    for (int i = 0; i < N; i++) {
        // Cálculo da expressão vetorial. A operação é independente em cada índice 'i'.
        v[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }

    double fim_seq = omp_get_wtime();
    double tempo_seq = fim_seq - inicio_seq; // Tempo total da execução sequencial (Item C)

    std::cout << "Tempo SEQUENCIAL: " << tempo_seq << " segundos\n\n";

    // b) Paralelize com #pragma omp parallel for schedule(static).
       std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ VERSÃO PARALELA ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n"; 

    // Tempo inicial para a região paralela
    double T0 = omp_get_wtime();

    #pragma omp parallel for schedule(static) // Diretiva OpenMP para paralelizar o "loop for"
    for (int i = 0; i < N; ++i) {
        v[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i]; // Cálculo da expressão vetorial em paralelo
    }


    double T1 = omp_get_wtime();
    double tempo_paralelo = T1 - T0; // Tempo total da execução paralela (Item C)

    std::cout << "\nTempo PARALELO:     " << tempo_paralelo << " segundos\n";   // tempo gasto usando paralelismo

    // Exibe a diferença de tempo para mostrar o Speedup
    std::cout << "Diferença (Seq - Par): " << tempo_seq - tempo_paralelo << " segundos\n";
    return 0;
}
