/*-------------------------------------------------------------------------------------------------------------------------
Autora    : Maria Clara Fontenele Silva
Matrícula : 2312130230
Exercício 1 - Imprimir "Hello World!" na tela e exemplificar atomic/critical.
--------------------------------------------------------------------------------------------------------------------------
a) Crie uma região paralela com #pragma omp parallel.
b) Cada thread deve imprimir uma mensagem indicando seu número (omp_get_thread_num()) e o total de threads (omp_get_num_threads()).
c) Configure o programa para rodar com 4 threads.
d) Adiciona uma operação 'atomic' para incrementar um contador.
--------------------------------------------------------------------------------------------------------------------------
*/

#include <iostream> // biblioteca padrão de entrada e saída (std::cout, std::endl)
#include <omp.h> // biblioteca OpenMP para programação paralela

// Variável compartilhada para o exemplo de 'atomic'
int contador_compartilhado = 0;

int main() {
    // Define a diretiva OpenMP para criar uma região paralela.
    #pragma omp parallel num_threads(4)
    {
        // Obtém o número da thread atual
        int thread_id = omp_get_thread_num();
        // Obtém o número total de threads que estão executando ativamente a região paralela atual
        int total_threads = omp_get_num_threads();

        // Exemplo de 'atomic': Operação atômica em uma única variável compartilhada
        #pragma omp atomic
        contador_compartilhado++; // Garante que o incremento seja feito de forma segura.

        // Exemplo de 'critical': Usado para proteger blocos maiores ou chamadas complexas (como E/S)
        #pragma omp critical // garante que apenas uma thread por vez possa executar o bloco a seguir
        // Imprime a mensagem "Hello world from thread " com o número da thread e o total de threads
        std::cout << "Hello world from thread: " << thread_id << " of " << total_threads << std::endl;

    } 

    // Imprime o resultado final do contador após a região paralela
    std::cout << "\nValor final do contador compartilhado (via atomic): " << contador_compartilhado << std::endl;
    return 0;
}
