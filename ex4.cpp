/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva
Matrícula : 2312130230
Exercício 4 — Medindo tempo por thread
--------------------------------------------------------------------------------------------------------------------------
Adapte o código do exercício anterior para:
a) Medir e exibir o tempo total de execução.
b) Medir e exibir o tempo gasto por cada thread.
c) Mostrar quantas threads foram utilizadas no cálculo.
--------------------------------------------------------------------------------------------------------------------------
*/


#include <iostream>
#include <vector>
#include <omp.h>
#include <iomanip>

namespace ConsoleColors {
    // Cores
    const std::string RESET = "\033[0m";        
    const std::string CIANO = "\033[36m";    
    
    // Estilos
    const std::string NEGRITO = "\033[1m";    // Negrito/Brilho
    const std::string FUNDO_AZUL = "\033[44m"; // Fundo Azul
}


int main (){
    const int N = 1000000;   // tamanho dos vetores (um milhão), conforme o enunciado.
    std::vector<int> x(N), y(N), z(N), v(N);

    // Inicializa vetores x, y ,z 
    for (int i = 0; i < N; i++) {
        x[i] = i;
        y[i] = i + 1;
        z[i] = i + 2;
    }

    std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ VERSÃO SEQUENCIAL ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n"; 
    double inicio_seq = omp_get_wtime(); // tempo inicial da versão sequencial
    std::cout << "Calculando a = x*x + y*y + z*z para vetores de tamanho " << N << "\n";

    // Calcula a[i] = x[i]^2 + y[i]^2 + z[i]^2
    for (int i = 0; i < N; i++) {
        // Cálculo da expressão vetorial e armazenamento no vetor de resultado 'v' (ou 'a')
        v[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];  
    }
    
    double fim_seq = omp_get_wtime(); // tempo final
    double tempo_seq = fim_seq - inicio_seq;

    // a) Medir e exibir o tempo total de execução.
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tempo SEQUENCIAL: " << tempo_seq << " segundos\n\n";
    
    std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ VERSÃO PARALELA ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n"; 

    // b) Medir e exibir o tempo gasto por cada thread.
    // Vetor para armazenar o tempo gasto por thread
    std::vector<double> thread_time(omp_get_max_threads(), 0.0);

    int threads_usadas = 0;

    // Tempo total da região paralela (somente o cálculo)
    double T0 = omp_get_wtime();


    // c) Mostrar quantas threads foram utilizadas no cálculo.
        #pragma omp parallel
    {
        int tid = omp_get_thread_num();

        // numero de threads usadas
        #pragma omp single
        {
            threads_usadas = omp_get_num_threads();
        }
        
        // o tempo gasto por uma thread individual nesse trabalho
        double t0 = omp_get_wtime();

        // Divisão do trabalho por índices (cada thread pega um subconjunto de i)
        #pragma omp for schedule(static)
        for (int i = 0; i < N; ++i) {
            v[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }

        double t1 = omp_get_wtime();
        thread_time[tid] += (t1 - t0);

    }

    double T1 = omp_get_wtime();
    double tempo_total = T1 - T0;
 
    std::cout << std::setprecision(6);
    std::cout << "Threads utilizadas: " << threads_usadas << "\n";
    std::cout << "Tempo TOTAL (s):    " << tempo_total << "\n";

      std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ RELATÓRIO DAS THREADS E TEMPO GASTO USANDO PALALELISMO ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n";
              
    std::cout << "\nTempo por thread (s):\n";
    for (int t = 0; t < threads_usadas; ++t) {
        std::cout << "  Thread " << t << ": " << thread_time[t] << "\n";
    }
    std::cout << "A diferença de tempo entre a versão sequencial e a versão paralela é: " << tempo_seq - tempo_total << " segundos\n";
    return 0;
}
