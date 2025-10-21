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

// Pequeno namespace com códigos de cor e estilo para deixar a saída mais legível no terminal
namespace ConsoleColors {
    const std::string RESET = "\033[0m";      
    const std::string VERMELHO = "\033[31m";  
    const std::string VERDE = "\033[32m";     
    const std::string NEGRITO = "\033[1m";    
}

int main() {

    // (a) Criação e inicialização do vetor
    const int N = 100;  // Tamanho do vetor
    std::vector<int> v(N, 1); // Inicializa todos os elementos com o valor 1
    std::vector<std::string> log(N); // Vetor auxiliar para armazenar logs de execução


    // (b) Soma sequencial — referência base
    std::cout << "\n⋆｡‧˚ Soma Sequencial ˚‧｡⋆\n\n";

    int soma_seq = 0;
    for (int i = 0; i < N; i++) {
        soma_seq += v[i];
        std::cout << "i = " << i << " | v[i] = " << v[i] 
                  << " | soma parcial = " << soma_seq << std::endl;
    }
    // Escolha do modo de paralelização:
    // 0 → reduction
    // 1 → atomic
    // 2 → critical
    int modo = 2; // altere esse valor para testar cada abordagem
    std::string nome_modo;

    if (modo == 0) nome_modo = "reduction";
    else if (modo == 1) nome_modo = "atomic";
    else nome_modo = "critical";

    std::cout << "\n⋆｡‧˚ Soma Paralela com " << nome_modo << " ˚‧｡⋆\n\n";

    // Variáveis para a versão paralela
    int soma_par = 0;  // variável compartilhada (cuidado com condições de corrida!)
    int num_threads = omp_get_max_threads(); // obtém o número máximo de threads disponíveis
    std::vector<int> somas_por_thread(num_threads, 0); // registra quanto cada thread somou

    double inicio = omp_get_wtime(); // medição de tempo de execução paralela


    // (c) Implementação paralela — 3 versões

    if (modo == 0) {
        // EDUCTION 
        // Aqui cada thread tem sua própria cópia da variável "soma_par"
        // O OpenMP automaticamente combina todas no final.
        #pragma omp parallel for reduction(+:soma_par)
        for (int i = 0; i < N; i++) {
            int id = omp_get_thread_num(); // ID da thread atual
            soma_par += v[i];              // Soma local (thread-safe por causa do reduction)
            somas_por_thread[id] += v[i];  // Armazena quanto essa thread somou
            log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
        }
    } 
    else if (modo == 1) {
        // ATOMIC 
        // Aqui todas as threads acessam a mesma variável global "soma_par"
        // Mas usamos "atomic" para garantir que cada atualização ocorra de forma indivisível.
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            #pragma omp for
            for (int i = 0; i < N; i++) {
                #pragma omp atomic
                soma_par += v[i]; // atualização atômica — impede leituras simultâneas incorretas
                somas_por_thread[id] += v[i];
                log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
            }
        }
    } 
    else if (modo == 2) {
        // CRITICAL
        // Similar ao atomic, mas usa uma região crítica.
        // Apenas uma thread por vez pode entrar no bloco "critical".
        // É seguro, mas pode ser mais lento que atomic em operações simples.
        #pragma omp parallel
        {
            int id = omp_get_thread_num();
            #pragma omp for
            for (int i = 0; i < N; i++) {
                #pragma omp critical
                {
                    soma_par += v[i]; // acesso controlado — uma thread de cada vez
                }
                somas_por_thread[id] += v[i];
                log[i] = "Thread " + std::to_string(id) + " processando i=" + std::to_string(i);
            }
        }
    }

    double fim = omp_get_wtime(); // tempo final da soma paralela

    // Impressão dos logs de execução
    for (int i = 0; i < N; i++) std::cout << log[i] << std::endl;

    // Mostra quanto cada thread trabalhou (útil para ver o balanceamento)
    std::cout << "\nSoma individual por thread:\n";
    for (int t = 0; t < num_threads; t++) {
        if (somas_por_thread[t] > 0)
            std::cout << "Thread " << t << " somou: " << somas_por_thread[t] << std::endl;
    }

    // Resultado final da soma paralela
    std::cout << "\nSoma paralela dos elementos: " << soma_par << std::endl;
    std::cout << "Tempo total (" << nome_modo << "): " << (fim - inicio) * 1000 << " ms\n";


    // (d) Comparação e conclusão
    std::cout << "\n⋆｡‧˚ Comparação de Resultados ˚‧｡⋆\n\n";
    if (soma_seq == soma_par) {
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERDE
            << "ദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Os resultados são iguais! "
            << nome_modo << " garantiu a correção da soma."
            << ConsoleColors::RESET << "\n";
    } else {
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERMELHO
            << "(˶˃⤙˂˶) Resultados diferentes! Provável condição de corrida no modo "
            << nome_modo << "."
            << ConsoleColors::RESET << "\n";
    }

    // EXPLICAÇÃO CONCEITUAL FINAL
    /*
      • reduction → cada thread cria sua própria variável local e, ao final,
                     o OpenMP combina (reduz) todas as somas em uma só.
                     É a forma mais eficiente e simples para esse tipo de problema.

      • atomic → garante exclusão mútua apenas durante a operação de soma.
                  É mais segura que sem proteção, mas menos eficiente que reduction.

      • critical → cria uma região onde apenas uma thread entra de cada vez.
                   É segura, mas mais lenta (bloqueia o acesso por completo).

      Diferença prática:
        reduction → paralelismo máximo e seguro.
        atomic    → paralelismo médio, pouca contenção.
        critical  → paralelismo limitado (uma thread de cada vez na região crítica).
    */

    return 0;
}
