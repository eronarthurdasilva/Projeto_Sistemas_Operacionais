# Trabalho Prático 1 - Algoritmo do Banqueiro
### Descrição 
O Algoritmo do Banqueiro é usado para evitar deadlocks em sistemas que gerencia múltiplos recursos compartilhados entre vários processos (ou threads). Garantindo que o sistema só conceda recursos se após a concessão, assim permanecendo em um estado de segurança.

## Estruturas e Variáveis que utilizei 
- NUMBER_OF_CUSTOMERS: número de clientes (threads).
- NUMBER_OF_RESOURCES: número de tipos de recursos.
- available[]: vetor com a quantidade disponível de cada recurso.
- maximum[][]: matriz com o máximo de cada recurso que cada cliente pode pedir.
- allocation[][]: matriz com a quantidade de cada recurso atualmente alocada para cada cliente.
- need[][]: matriz com a quantidade de cada recurso que cada cliente ainda pode pedir (need = maximum - allocation).
- lock: mutex para garantir que apenas uma thread manipule os recursos por vez.

## Funcinamento 
Cada cliente (simulado por uma thread) realiza pedidos aleatórios de recursos, respeitando os limites definidos pelas suas necessidades máximas. O sistema verifica se conceder o pedido mantém o estado seguro. Se sim, o pedido é aceito; caso contrário, é negado.

##  Requisitos

- GCC (compilador C) para Windows (ex: MinGW)
- Windows (usa API de threads do Windows)

##  Compilação

No terminal, execute:

```
gcc -o AlgoritmoBanqueiro AlgoritmoBanqueiro.c -lws2_32
```

##  Execução

O programa espera que você passe os valores iniciais dos recursos como argumento:

```
AlgoritmoBanqueiro.exe [numeros a sua escolha] [numeros a sua escolha] [numeros a sua escolha]
```

Ou, no terminal do VS Code:

```
.\AlgoritmoBanqueiro.exe [numeros a sua escolha] [numeros a sua escolha] [numeros a sua escolha]
```

##  O Que o Programa Faz

- Inicializa os recursos disponíveis com base nos argumentos
- Gera aleatoriamente a matriz de necessidades máximas (`maximum`)
- Executa 5 threads de clientes que:
  - Solicitam recursos com base nas suas necessidades (`need`)
  - Liberam recursos após um tempo
  - A cada ação, o estado do sistema é impresso
- Cada cliente executa 5 ciclos de requisição/liberação e o programa termina automaticamente

## Resumo visual 
main()
 └─ Cria threads clientes
      └─ Cada cliente:
           ├─ Gera pedido aleatório
           ├─ request_resources()
           │    └─ is_safe()
           ├─ Usa recursos
           ├─ release_resources()
           └─ Repete ciclo 5 vezes

##  Arquivos Importantes

- `AlgoritmoBanqueiro.c`: Código principal com o algoritmo do banqueiro e lógica das threads.

##  Referência Teórica

- Dijkstra, E. W. (1965). "Banker's Algorithm" - prevenção de deadlock por verificação de estado seguro.
- Silberschatz, A., Galvin, P. B., Gagne, G. (2021). *Operating System Concepts*. Wiley.
