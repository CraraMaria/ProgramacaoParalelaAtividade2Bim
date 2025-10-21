/*-------------------------------------------------------------------------------------------------------------------------
Autores    : Maria Clara Fontenele Silva E João Marcos Santos E Carvalho
Matrícula : 2312130230                      231112130063
Exercício 2 — Paralelizando um "for" simples.
--------------------------------------------------------------------------------------------------------------------------
a) Crie um vetor v de tamanho 100 e inicialize todos os elementos com o valor 1.
b) Escreva um loop sequencial que soma todos os elementos.
c) Refaça o loop com #pragma omp parallel for reduction(+:soma).
d) Compare os resultados e explique por que a diretiva reduction é necessária.
--------------------------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include <omp.h> 
#include <vector> 

namespace ConsoleColors {
    const std::string RESET = "\033[0m";      
    const std::string VERMELHO = "\033[31m";  
    const std::string VERDE = "\033[32m";     
    const std::string NEGRITO = "\033[1m";    
}

int main() {
    const int N = 100;
    std::vector<int> v(N, 1);
    std::vector<std::string> log(N);

    // Soma sequencial
    std::cout << "\n⋆｡‧˚ Soma Sequencial ˚‧｡⋆\n\n";
    int soma_seq = 0;
    for (int i = 0; i < N; i++) {
        soma_seq += v[i];
        std::cout << "i = " << i << " | v[i] = " << v[i] << " | soma parcial = " << soma_seq << std::endl;
    }

    // Escolha do modo: 0=reduction | 1=atomic | 2=critical
    int modo = 0; // Altere aqui o modo que deseja testar
    std::string nome_modo;

    if (modo == 0) nome_modo = "reduction";
    else if (modo == 1) nome_modo = "atomic";
    else nome_modo = "critical";

    std::cout << "\n⋆｡‧˚ Soma Paralela com " << nome_modo << " ˚‧｡⋆\n\n";

    int soma_par = 0;
    int num_threads = omp_get_max_threads();
    std::vector<int> somas_por_thread(num_threads, 0);

    double inicio = omp_get_wtime();

    if (modo == 0) {
        // reduction
        #pragma omp parallel for reduction(+:soma_par)
        for (int i = 0; i < N; i++) {
            int id = omp_get_thread_num();
            soma_par += v[i];
            somas_por_thread[id] += v[i];
            log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
        }
    } 
    else if (modo == 1) {
        // atomic
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            #pragma omp for
            for (int i = 0; i < N; i++) {
                #pragma omp atomic
                soma_par += v[i]; // atualização atômica
                somas_por_thread[id] += v[i];
                log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
            }
        }
    } 
    else if (modo == 2) {
        // critical
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            #pragma omp for
            for (int i = 0; i < N; i++) {
                #pragma omp critical
                {
                    soma_par += v[i]; // região crítica
                }
                somas_por_thread[id] += v[i];
                log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
            }
        }
    }

    double fim = omp_get_wtime();

    // Logs
    for (int i = 0; i < N; i++) std::cout << log[i] << std::endl;

    std::cout << "\nSoma individual por thread:\n";
    for (int t = 0; t < num_threads; t++) {
        if (somas_por_thread[t] > 0)
            std::cout << "Thread " << t << " somou: " << somas_por_thread[t] << std::endl;
    }

    std::cout << "\nSoma paralela dos elementos: " << soma_par << std::endl;
    std::cout << "Tempo total (" << nome_modo << "): " << (fim - inicio) * 1000 << " ms\n";

    // Comparação
    std::cout << "\n⋆｡‧˚ Comparação de Resultados ˚‧｡⋆\n\n";
    if (soma_seq == soma_par) {
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERDE
            << "ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Os resultados são iguais! " << nome_modo << " funcionou corretamente."
            << ConsoleColors::RESET << "\n";
    } else {
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERMELHO
            << "(˶˃⤙˂˶) Resultados diferentes! Problema no uso de " << nome_modo
            << ConsoleColors::RESET << "\n";
    }

    return 0;
}
