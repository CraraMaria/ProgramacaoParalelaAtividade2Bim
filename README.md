
<h2 align="center">Atividade do Segundo Bimestre — Programação Paralela</h2>

<p align="center">
  <strong>Lista de Exercícios — Programação Paralela (OpenMP)</strong><br>
  <strong>Prof.:</strong> RodriGo Gonçalves<br>
  <strong>Alunos:</strong> João Marcos Santos E Carvalho - 23122130063<br>
  Maria Clara Fontenele Silva - 2312130230
</p>

<hr>

<h3>Exercício 1 — “Hello World” Paraleloe exemplificar atomic/critical (<code>ex1.cpp</code>)</h3>
<p>
  a) Crie uma região paralela com <code>#pragma omp parallel</code>.<br>
  b) Cada thread deve imprimir uma mensagem indicando seu número (<code>omp_get_thread_num()</code>) e o total de threads (<code>omp_get_num_threads()</code>).<br>
  c) Configure o programa para rodar com <strong>4 threads</strong>.
</p>

<hr>

<h3>Exercício 2 — Paralelizando um for simples exemplificar atomic/critical (<code>ex2.cpp</code>) </h3>
<p>
  a) Crie um vetor <code>v</code> de tamanho 100 e inicialize todos os elementos com o valor <strong>1</strong>.<br>
  b) Escreva um loop sequencial que soma todos os elementos.<br>
  c) Refaça o loop com <code>#pragma omp parallel for reduction(+:soma)</code>.<br>
  d) Compare os resultados e explique por que a diretiva <strong>reduction</strong> é necessária.
</p>

<hr>

<h3>Exercício 3 — Expressão Vetorial e acrescentar barrier e ordered (<code>ex3.cpp</code>) </h3>
<p>
  Implemente a expressão: <code>a[i] = x[i]^2 + y[i]^2 + z[i]^2</code> para vetores de tamanho <strong>1.000.000</strong>.<br>
  a) Faça a versão sequencial.<br>
  b) Paralelize com <code>#pragma omp parallel for schedule(static)</code>.<br>
  c) Mostre o tempo total de execução em cada versão.
</p>

<hr>

<h3>Exercício 4 — Medindo tempo por thread e acrescentar barrier e ordered (<code>ex4.cpp</code>) </h3>
<p>
  Adapte o código do exercício anterior para:<br>
  a) Medir e exibir o tempo total de execução.<br>
  b) Medir e exibir o tempo gasto por cada thread.<br>
  c) Mostrar quantas threads foram utilizadas no cálculo.
</p>

<hr>

<h3>Exercício 5 — Escalonamento (<code>ex5.cpp</code>) </h3>
<p>
  Use novamente o cálculo de <code>a[i] = x[i]^2 + y[i]^2 + z[i]^2</code>, mas:<br>
  a) Execute com <code>schedule(static)</code> e <code>schedule(dynamic, 1000)</code>.<br>
  b) Compare os tempos em diferentes quantidades de threads (2, 4, 8).<br>
  c) Explique em quais situações <strong>static</strong> e <strong>dynamic</strong> são mais adequados.
</p>

