/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva 
Matrícula : 2312130230
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
/* #include <vector>  -> Ela fornece as diretivas para definir regiões e loops paralelos, 
e funções para gerenciar e obter informações sobre as threads de execução. */

namespace ConsoleColors {
    // Cor
    const std::string RESET = "\033[0m";      // Desfaz todas as formatações
    const std::string VERMELHO = "\033[31m";  // Texto Vermelho
    const std::string VERDE = "\033[32m";     // Texto Verde
    // Estilo
    const std::string NEGRITO = "\033[1m";    // Negrito/Brilho
}

int main() {

    // a) Crie um vetor v de tamanho 100 e inicialize todos os elementos com o valor 1.
    const int N = 100;
    std::vector<int> v(N, 1);  //  vetor de tamanho "N" e inicializa todos os elementos com 1
    std::vector<std::string> log(N); // para armazenar mensagens de cada índice

    // b) Escreva um loop sequencial que soma todos os elementos.
    std::cout << "\n";
    std::cout << "⋆｡‧˚ Soma Sequencial ˚‧｡⋆" <<"\n";
    std::cout << "\n";
    std::cout << "Calculando a soma sequencial dos elementos do vetor v de tamanho " << N << "\n";

    // soma sequencial de todos os vetores de N
    int soma_seq = 0;       // armazena o resultado da soma sequencial
    for (int i = 0; i < N; i++) {  //loop para somar todos os elementos
        soma_seq += v[i]; //adiciona o vetor[i] em soma_seq até ele i ser menor que N
        // Imprime o progresso do cálculo sequencial
        std::cout << "i = " << i << " | v[i] = " << v[i] << " | soma parcial = " << soma_seq << std::endl;
    }

    //c) Refaça o loop com #pragma omp parallel for reduction(+:soma).
    std::cout << "\n";
    std::cout << "⋆｡‧˚ Soma Paralela ˚‧｡⋆" << "\n";
    std::cout << "\n";
    std::cout << "\nCalculando a soma paralela dos elementos do vetor v de tamanho " << N << "\n";

    int soma_par = 0; // armazena o resultado paralelo, variável de redução
    int num_threads = omp_get_max_threads();  // Obtém o número máximo de threads disponíveis
    std::vector<int> somas_por_thread(num_threads, 0); // Rastreia as somas individuais de cada thread

    //reduction é necessaria para evitar condição de "corrida" para cada thread acumular sua parte sem interferir na outra
    #pragma omp parallel for reduction(+:soma_par) // Cria threads e distribui as iterações do loop entre elas.
    for (int i = 0; i < N; i++) {
        int thread_id = omp_get_thread_num(); // pega o ID da thread atual
        soma_par += v[i]; // thread-safe
        somas_por_thread[thread_id] += v[i]; // acumula na posição da thread
    
       // Armazena a mensagem de log para impressão ordenada 
        log[i] = "Thread " + std::to_string(thread_id) + " processando i = " + std::to_string(i) + " | v[i] = " + std::to_string(v[i]);
    }

        // Impressão ordenada após o loop
    for (int i = 0; i < N; i++) {
        std::cout << log[i] << std::endl;
    }

    // Impressão da soma por thread, eu queria ver a soma que cada thread fez individualmente.
    std::cout << "\n";
    std::cout << "\nSoma individual por thread:\n";
    // threads que foram de fato utilizadas.
    for (int t = 0; t < num_threads; t++) {
        std::cout << "Thread " << t << " somou: " << somas_por_thread[t] << std::endl;
    }

    std::cout << "\nSoma paralela dos elementos: " << soma_par << std::endl;


    // d) Compare os resultados e explique por que a diretiva reduction é necessária.
    std::cout << "\n";
    std::cout << "⋆｡‧˚ Comparação de Resultados ˚‧｡⋆" << "\n";
    std::cout << "\n";
    if (soma_seq == soma_par) {
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERDE
              << "\nദ്ദി(˵ •̀ ᴗ - ˵ ) ✧ Maravilha! os resultados são iguais. A diretiva reduction garantiu que cada thread somasse corretamente."
              << ConsoleColors::RESET << "\n";
    } else {
              std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERMELHO
              << "\n(˶˃⤙˂˶) Poxa! Os resultados são diferentes. Isso pode indicar ausência ou erro na diretiva reduction."
              << ConsoleColors::RESET << "\n" << "\n";

    }
    // obs: 
    // A diretiva 'reduction' é NECESSÁRIA para variáveis de acumulação (como a soma) porque, em um ambiente paralelo, várias threads tentariam ler e escrever na
    // mesma variável simultaneamente. A REDUCTION resolve isso criando uma cópia PRIVADA da variável de redução para cada thread e no fim sincroniza as treads 
    // e realiza, de forma segura, o armazenamento do resultado IFNAL na variável Global.
    return 0;
}
