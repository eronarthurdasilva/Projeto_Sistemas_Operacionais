# Trabalho Prático 1 - Algoritmo do Banqueiro

Este projeto implementa o Algoritmo do Banqueiro utilizando múltiplas threads e controle de concorrência com mutexes.

## 👨‍💻 Como Funciona

Cada cliente (simulado por uma thread) realiza pedidos aleatórios de recursos, respeitando os limites definidos pelas suas necessidades máximas. O sistema verifica se conceder o pedido mantém o estado seguro. Se sim, o pedido é aceito; caso contrário, é negado.

## 🔧 Requisitos

- GCC (compilador C)
- Bibliotecas POSIX (pthreads)

## 🚀 Compilação

```
gcc -o banqueiro main.c -lpthread
```

## 🧪 Execução

O programa espera que você passe os valores iniciais dos recursos como argumento:

```
./banqueiro 10 5 7
```

Onde:
- `10` = instâncias do recurso 0 disponíveis
- `5` = instâncias do recurso 1 disponíveis
- `7` = instâncias do recurso 2 disponíveis

## 📝 O Que o Programa Faz

- Inicializa os recursos disponíveis com base nos argumentos
- Gera aleatoriamente a matriz de necessidades máximas (`maximum`)
- Executa 5 threads de clientes que:
  - Solicitam recursos com base nas suas necessidades (`need`)
  - Liberam recursos após um tempo
  - A cada ação, o estado do sistema é impresso

## 📘 Arquivos Importantes

- `main.c`: Código principal com o algoritmo do banqueiro e lógica das threads.

## 📚 Referência Teórica

- Dijkstra, E. W. (1965). "Banker's Algorithm" - prevenção de deadlock por verificação de estado seguro.
- Silberschatz, A., Galvin, P. B., Gagne, G. (2021). *Operating System Concepts*. Wiley.

## 📷 Exemplo de Saída
```
Cliente 2 obteve recursos.
Available: 6 4 3
...
Cliente 2 liberou recursos.
Available: 8 5 4
...
```

## 👨‍🏫 Autor
- Nome do Aluno
- Engenharia da Computação
- Universidade
