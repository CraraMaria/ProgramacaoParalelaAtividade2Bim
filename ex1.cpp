/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva
Matrícula : 2312130230
Exercício 1 - Imprimir "Hello World!" na tela.
--------------------------------------------------------------------------------------------------------------------------
a) Crie uma região paralela com #pragma omp parallel.
b) Cada thread deve imprimir uma mensagem indicando seu número (omp_get_thread_num()) e o total de threads (omp_get_num_threads()).
c) Configure o programa para rodar com 4 threads.
--------------------------------------------------------------------------------------------------------------------------
*/

#include <iostream> // biblioteca padrão de entrada e saída (std::cout, std::endl)
#include <omp.h> // biblioteca OpenMP para programação paralela

int main() {
    // Define a diretiva OpenMP para criar uma região paralela.
    #pragma omp parallel num_threads(4)
    {
        // Obtém o número da thread atual
        int thread_id = omp_get_thread_num();
        // Obtém o número total de threads que estão executando ativamente a região paralela atual
        int total_threads = omp_get_num_threads();

        #pragma omp critical // garante que apenas uma thread por vez possa executar o bloco a seguir
        // Imprime a mensagem "Hello world from thread " com o número da thread e o total de threads
        std::cout << "Hello world from thread: " << thread_id << " of " << total_threads << std::endl;

    } 
    return 0;
}
