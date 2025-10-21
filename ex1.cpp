/*-------------------------------------------------------------------------------------------------------------------------
Autores    : Maria Clara Fontenele Silva E João Marcos Santos E Carvalho
Matrícula : 2312130230                      231112130063
Exercício 1 - Imprimir "Hello World!" na tela e exemplificar atomic/critical.
--------------------------------------------------------------------------------------------------------------------------
a) Crie uma região paralela com #pragma omp parallel.
b) Cada thread deve imprimir uma mensagem indicando seu número (omp_get_thread_num()) e o total de threads (omp_get_num_threads()).
c) Configure o programa para rodar com 4 threads.
d) Adiciona uma operação 'atomic' para incrementar um contador.
--------------------------------------------------------------------------------------------------------------------------
As diretivas atomic e critical no OpenMP são ferramentas usadas para gerenciar o acesso a dados compartilhados 
por múltiplas threads em uma região paralela.
*/

#include <iostream> // Entrada e saída (std::cout, std::endl)
#include <omp.h> // biblioteca OpenMP

// Variável compartilhada para o exemplo de 'atomic'
int contador_compartilhado = 0;

//Cores e Estilo
namespace ConsoleColors {
    const std::string RESET = "\033[0m";      // Desfaz todas as formatações
    const std::string VERDE = "\033[32m";     // Texto Verde
    const std::string NEGRITO = "\033[1m";    // Negrito/Brilho
}

int main() {
    //Define a diretiva OpenMP para criar uma região paralela com 4 threads
    #pragma omp parallel num_threads(4)
    {
        //Vaiável para armazenar o ID da thread atual
        int thread_id = omp_get_thread_num();
        //Vaiável para armazenar o número total de threads
        int total_threads = omp_get_num_threads();

        //EXEMPLO DE ATOMIC -> operação atômica em uma única variável compartilhada
        #pragma omp atomic
        contador_compartilhado++; // Incrementa o contador compartilhado de forma atômica

        //EXEMPLO DE CRITICAL -> seção crítica para proteger o bloco de código
        #pragma omp critical  // garante que apenas uma thread por vez possa executar o bloco a seguir
        std::cout << "Hello world from thread: " << thread_id << " of " << total_threads << std::endl;
    }
        std::cout << ConsoleColors::NEGRITO << ConsoleColors::VERDE
              << "\nValor final do contador compartilhado (via atomic): " << contador_compartilhado << std::endl << "\n"
              << ConsoleColors::RESET << "\n";
        return 0;
}

// Explicação:
// A diretiva 'atomic' é usada para garantir que a operação de incremento do contador_comp  artilhado seja realizada de forma atômica,
// ou seja, sem interferência de outras threads. Isso é crucial para evitar condições de corrida
