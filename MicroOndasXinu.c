#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int nivel_potencia;
    int duracao;
    char tipo_alimento[20];
} CicloCozimento;

typedef struct {
    bool luz_ligada;
} LuzInterna;

void especificar_ciclos(CicloCozimento *ciclo, const char *alimento, int potencia, int tempo) {
    ciclo->nivel_potencia = potencia;
    ciclo->duracao = tempo;
    snprintf(ciclo->tipo_alimento, sizeof(ciclo->tipo_alimento), "%s", alimento);
    kprintf("Ciclo configurado: %s - Potência %d%% por %d segundos.\n", alimento, potencia, tempo);
}

void controlar_klystron(int potencia) {
    kprintf("Klystron operando a %d%% de potência.\n", potencia);
}

void anunciar_bip() {
    kprintf("Beep! Cozimento concluído.\n");
}

void emergencia() {
    kprintf("Parada de emergência ativada! Ciclo cancelado e porta destravada.\n");
}

void programar_acao_futura(CicloCozimento *ciclo, int atraso) {
    kprintf("Ciclo será iniciado em %d segundos...\n", atraso);
    sleep(atraso);
    iniciar_ciclo(ciclo);
}

void refrigeracao() {
    kprintf("Resfriamento em andamento...\n");
    sleep(2);
    kprintf("Resfriamento concluído.\n");
}

void tracao_prato() {
    kprintf("Prato girando para distribuição uniforme de calor.\n");
}

void luz_durante_preparo(LuzInterna *luz, bool estado) {
    luz->luz_ligada = estado;
    kprintf("Luz interna %s.\n", luz->luz_ligada ? "ligada" : "desligada");
}

void exibir_relogio_cortesia() {
    time_t agora = clktime;
    struct tm horario;
    localtime_r(&agora, &horario);
    kprintf("Hora atual em Brasília: %02d:%02d\n", horario.tm_hour, horario.tm_min);
}

void menu_programas_cozimento(CicloCozimento *ciclo) {
    int escolha;
    kprintf("\n--- Programas de Cozimento ---\n");
    kprintf("1. Carnes (80%%, 10 minutos)\n");
    kprintf("2. Peixe (70%%, 8 minutos)\n");
    kprintf("3. Frango (90%%, 12 minutos)\n");
    kprintf("4. Lasanha (60%%, 15 minutos)\n");
    kprintf("5. Pipoca (100%%, 3 minutos)\n");
    kprintf("Escolha uma opção: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            especificar_ciclos(ciclo, "Carnes", 80, 600);
            break;
        case 2:
            especificar_ciclos(ciclo, "Peixe", 70, 480);
            break;
        case 3:
            especificar_ciclos(ciclo, "Frango", 90, 720);
            break;
        case 4:
            especificar_ciclos(ciclo, "Lasanha", 60, 900);
            break;
        case 5:
            especificar_ciclos(ciclo, "Pipoca", 100, 180);
            break;
        default:
            kprintf("Opção inválida!\n");
            break;
    }
}

void exibir_menu() {
    kprintf("\n--- Menu de Funções ---\n");
    kprintf("1. Configurar Ciclo de Cozimento\n");
    kprintf("2. Iniciar Ciclo de Cozimento\n");
    kprintf("3. Programas de Cozimento\n");
    kprintf("4. Programar Ação Futura\n");
    kprintf("5. Parada de Emergência\n");
    kprintf("6. Luz Interna Durante Preparo\n");
    kprintf("7. Resfriamento\n");
    kprintf("8. Tração do Prato\n");
    kprintf("9. Exibir Relógio Cortesia\n");
    kprintf("0. Sair\n");
    kprintf("Escolha uma opção: ");
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

    anunciar_bip();
}

void processar_opcao(int opcao, CicloCozimento *ciclo, LuzInterna *luz) {
    switch (opcao) {
        case 1:
            configurar_ciclo(ciclo);
            break;
        case 2:
            iniciar_ciclo(ciclo);
            break;
        case 3:
            menu_programas_cozimento(ciclo);
            break;
        case 4: {
            int atraso;
            kprintf("Digite o tempo em segundos para iniciar o ciclo: ");
            scanf("%d", &atraso);
            programar_acao_futura(ciclo, atraso);
            break;
        }
        case 5:
            emergencia();
            break;
        case 6: {
            int estado;
            kprintf("Deseja ligar a luz interna? (1 = Sim, 0 = Não): ");
            scanf("%d", &estado);
            luz_durante_preparo(luz, estado == 1);
            break;
        }
        case 7:
            refrigeracao();
            break;
        case 8:
            tracao_prato();
            break;
        case 9:
            exibir_relogio_cortesia();
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
    LuzInterna luz = {false};
    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        processar_opcao(opcao, &ciclo, &luz);
    } while (opcao != 0);

    return OK;
}
