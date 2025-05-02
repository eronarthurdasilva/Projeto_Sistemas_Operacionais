// Trabalho Prático 1 - Algoritmo do Banqueiro
// Implementação em C para Windows

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <stdbool.h>
#include <time.h>

// Definição de constantes para o número de clientes e recursos
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

// Variáveis globais para gerenciar os recursos
int available[NUMBER_OF_RESOURCES]; // Recursos disponíveis
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; // Máximo de recursos que cada cliente pode solicitar
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; // Recursos atualmente alocados para cada cliente
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; // Recursos que cada cliente ainda precisa

HANDLE lock; // Mutex para sincronização entre threads

// Função utilitária para imprimir o estado atual dos recursos
void print_state() {
    printf("\nEstado Atual:\n");
    printf("Available: ");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) printf("%d ", available[i]);
    printf("\nAllocation:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", allocation[i][j]);
        printf("\n");
    }
    printf("Need:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Verifica se o sistema permanece em estado seguro
bool is_safe() {
    int work[NUMBER_OF_RESOURCES]; // Recursos temporários disponíveis
    bool finish[NUMBER_OF_CUSTOMERS] = {false}; // Indica se um cliente terminou

    // Inicializa os recursos temporários com os disponíveis
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];

    while (1) {
        bool found = false; // Indica se foi encontrado um cliente que pode ser atendido
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) { // Se o cliente ainda não foi atendido
                bool possible = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) { // Verifica se os recursos necessários estão disponíveis
                        possible = false;
                        break;
                    }
                }
                if (possible) { // Se possível atender o cliente
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
                        work[j] += allocation[i][j]; // Libera os recursos alocados
                    finish[i] = true; // Marca o cliente como atendido
                    found = true;
                }
            }
        }
        if (!found) break; // Se nenhum cliente pode ser atendido, sai do loop
    }

    // Verifica se todos os clientes foram atendidos
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        if (!finish[i]) return false;
    return true;
}

// Solicitação de recursos por um cliente
int request_resources(int customer_num, int request[]) {
    WaitForSingleObject(lock, INFINITE); // Bloqueia o mutex para evitar condições de corrida

    // Verifica se a solicitação é válida
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            ReleaseMutex(lock); // Libera o mutex
            return -1; // Solicitação inválida
        }
    }

    // Aloca temporariamente os recursos
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // Verifica se o sistema permanece em estado seguro
    if (!is_safe()) {
        // Reverte a alocação se o estado não for seguro
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        ReleaseMutex(lock); // Libera o mutex
        return -1; // Estado inseguro
    }

    ReleaseMutex(lock); // Libera o mutex
    return 0; // Solicitação atendida com sucesso
}

// Liberação de recursos por um cliente
int release_resources(int customer_num, int release[]) {
    WaitForSingleObject(lock, INFINITE); // Bloqueia o mutex
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
        need[customer_num][i] += release[i];
    }
    ReleaseMutex(lock); // Libera o mutex
    return 0; // Recursos liberados com sucesso
}

// Função executada por cada thread de cliente
DWORD WINAPI customer(LPVOID arg) {
    int customer_num = *(int *)arg; // Identificador do cliente
    int request[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    for (int ciclo = 0; ciclo < 5; ciclo++){
        Sleep(1000); // Simula o tempo de espera (1 segundo)

        // Gera uma solicitação aleatória de recursos
        WaitForSingleObject(lock, INFINITE);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            request[i] = rand() % (need[customer_num][i] + 1);
        }
        ReleaseMutex(lock);

        // Solicita os recursos
        if (request_resources(customer_num, request) == 0) {
            printf("Cliente %d obteve recursos.\n", customer_num);
            print_state();

            Sleep(2000); // Simula o uso dos recursos (2 segundos)

            // Gera uma liberação aleatória de recursos
            WaitForSingleObject(lock, INFINITE);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
                release[i] = rand() % (allocation[customer_num][i] + 1);
            ReleaseMutex(lock);

            // Libera os recursos
            release_resources(customer_num, release);
            printf("Cliente %d liberou recursos.\n", customer_num);
            print_state();
        }
    }
    return 0;
}

// Função principal
int main(int argc, char *argv[]) {
    // Verifica se os argumentos foram passados corretamente
    if (argc != NUMBER_OF_RESOURCES + 1) {
        fprintf(stderr, "Uso: %s <recurso1> <recurso2> <recurso3>\n", argv[0]);
        exit(1);
    }

    lock = CreateMutex(NULL, FALSE, NULL); // Inicializa o mutex

    // Inicializa os recursos disponíveis
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        available[i] = atoi(argv[i + 1]);

    srand((unsigned int)time(NULL)); // Inicializa o gerador de números aleatórios

    // Inicializa as matrizes de máximo, alocação e necessidade
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = rand() % (available[j] + 1);
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }

    HANDLE threads[NUMBER_OF_CUSTOMERS]; // Threads para os clientes
    int customer_ids[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        customer_ids[i] = i;
        threads[i] = CreateThread(NULL, 0, customer, &customer_ids[i], 0, NULL); // Cria uma thread para cada cliente
    }

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        WaitForSingleObject(threads[i], INFINITE); // Aguarda todas as threads terminarem

    CloseHandle(lock); // Destroi o mutex
    return 0;
}