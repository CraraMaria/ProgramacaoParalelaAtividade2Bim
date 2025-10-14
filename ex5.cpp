/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva
Matrícula : 2312130230
Exercício 5 — Escalonamento
--------------------------------------------------------------------------------------------------------------------------
Use novamente o cálculo de a[i] = x[i]^2 + y[i]^2 + z[i]^2, mas:
a) Execute com schedule(static) e schedule(dynamic, 1000).
b) Compare os tempos em diferentes quantidades de threads (2, 4, 8).
c) Explique em quais situações static e dynamic são mais adequados.
--------------------------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include <vector>
#include <omp.h>
#include <iomanip>
#include <string>

namespace ConsoleColors {
    // Cores
    const std::string RESET = "\033[0m";      // Desfaz todas as formatações
    const std::string AMARELO = "\033[33m";   // Texto Amarelo
    const std::string CIANO = "\033[36m";     // Texto Ciano
    
    // Estilos
    const std::string NEGRITO = "\033[1m";    // Negrito/Brilho
    const std::string FUNDO_AZUL = "\033[44m"; // Fundo Azul
}

int main() {
    const int N = 1000000; // Tamanho dos vetores (1 milhão)
    std::vector<int> x(N), y(N), z(N), v(N);

    // Inicializa os vetores
    for (int i = 0; i < N; i++) {
        x[i] = i;
        y[i] = i + 1;
        z[i] = i + 2;
    }

    //Versao sequencial
    std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ VERSÃO SEQUENCIAL ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n";
    double inicio_seq = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        v[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
    }
    double fim_seq = omp_get_wtime();
    double tempo_seq = fim_seq - inicio_seq;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tempo SEQUENCIAL: " << tempo_seq << " segundos\n\n";

    //CONFIGURAÇÕES PARALELAS
    std::cout << ConsoleColors::NEGRITO << ConsoleColors::CIANO
              << "\n⋆｡‧˚ CONFIGURAÇÕES PARALELAS ⋆｡‧˚" 
              << ConsoleColors::RESET << "\n";
    int num_threads_list[] = {2, 4, 8}; // uma lista com as quantidades dethreads que quero testar
    std::string schedules[] = {"static", "dynamic"};

    for (std::string sched : schedules) {
        std::cout << "-----------------------------\n";
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::AMARELO
              << "      Schedule: " << sched
              << ConsoleColors::RESET << "\n";
        std::cout << "-----------------------------\n";



        for (int t : num_threads_list) { //para cada tempo da lista testatremos o escalonamento
            omp_set_num_threads(t);
            double T0 = omp_get_wtime();

            if (sched == "static") {
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < N; i++) {
                    v[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
                }
            } else if (sched == "dynamic") {
                #pragma omp parallel for schedule(dynamic, 1000)
                for (int i = 0; i < N; i++) {
                    v[i] = x[i] * x[i] + y[i] * y[i] + z[i] * z[i];
                }
            }

            double T1 = omp_get_wtime();
            double tempo_paralelo = T1 - T0;

            std::cout << "Threads: " << t
                      << " | Tempo: " << tempo_paralelo
                      << " s | Speedup: " << tempo_seq / tempo_paralelo << "x\n"; // calculo do speedup tempo = sequencial / tempo paralelo
        }
        std::cout << std::endl;
    }

    return 0;
}
