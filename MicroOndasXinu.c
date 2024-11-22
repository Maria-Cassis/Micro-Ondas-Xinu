#include <xinu.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int nivel_potencia;
    int duracao;
    char tipo_alimento[20];
} CicloCozimento;

typedef struct {
    bool luz_ligada;
} LuzInterna;

void configurar_ciclo(CicloCozimento *ciclo) {
    int potencia, tempo;
    char alimento[20];

    kprintf("Digite o tipo de alimento (ex. Frango): ");
    scanf("%s", alimento);
    kprintf("Digite o nível de potência (0-100): ");
    scanf("%d", &potencia);
    kprintf("Digite a duração em segundos: ");
    scanf("%d", &tempo);

    ciclo->nivel_potencia = potencia;
    ciclo->duracao = tempo;
    snprintf(ciclo->tipo_alimento, sizeof(ciclo->tipo_alimento), "%s", alimento);

    kprintf("Ciclo configurado: %s - Potência %d%% por %d segundos.\n", alimento, potencia, tempo);
}

void iniciar_ciclo(const CicloCozimento *ciclo) {
    if (strlen(ciclo->tipo_alimento) == 0) {
        kprintf("Erro: Nenhum ciclo configurado! Configure primeiro.\n");
        return;
    }

    kprintf("\nIniciando ciclo de cozimento...\n");
    kprintf("Aquecendo %s a %d%% de potência por %d segundos...\n", 
            ciclo->tipo_alimento, ciclo->nivel_potencia, ciclo->duracao);

    for (int i = 0; i < ciclo->duracao; i++) {
        kprintf("Cozinhando... [%d/%d]\n", i + 1, ciclo->duracao);
        sleep(1);
    }

    kprintf("Beep! Cozimento concluído para %s.\n", ciclo->tipo_alimento);
}

void exibir_horario_brasilia() {
    time_t agora = clktime;
    struct tm horario;
    localtime_r(&agora, &horario);

    kprintf("Hora atual em Brasília: %02d:%02d\n", horario.tm_hour, horario.tm_min);
}

void exibir_menu() {
    kprintf("\n--- Menu de Funções ---\n");
    kprintf("1. Configurar Ciclo de Cozimento\n");
    kprintf("2. Iniciar Ciclo de Cozimento\n");
    kprintf("3. Exibir Hora Atual\n");
    kprintf("0. Sair\n");
    kprintf("Escolha uma opção: ");
}

void processar_opcao(int opcao, CicloCozimento *ciclo) {
    switch (opcao) {
        case 1:
            configurar_ciclo(ciclo);
            break;
        case 2:
            iniciar_ciclo(ciclo);
            break;
        case 3:
            exibir_horario_brasilia();
            break;
        case 0:
            kprintf("Encerrando...\n");
            break;
        default:
            kprintf("Opção inválida! Tente novamente.\n");
            break;
    }
}

process main(void) {
    CicloCozimento ciclo = {0, 0, ""};
    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        processar_opcao(opcao, &ciclo);
    } while (opcao != 0);

    return OK;
}
