/*
projeto 03 - GAME DE ARTILHARIA
IMPLEMENTADO NO FReeRTOS
SISTEMAS EM TEMPO REAL
ALUNOS: JOSÉ ÍVINES MATOS SILVA
        LUCAS HENRIQUE ANDRADE VILA NOVA
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define MAX_INTERVALO 10000
#define MAX_MISSEIS 10
#define MAX_AREA 100
#define VILAS 20
#define MIN_INTERVALO 1000

void vthread_atack(void *pvParameters);
void vthread_defesa(void *pvParameters);
void vthreadmonitor_areas_habitadas( void *pvParameters);

typedef struct {
    int nome;
    int local;
    int tempo_destroier;
    int tempo_de_lancamento;
} missel_struct;

typedef struct {
    int nome;
    int nome_alvo_missel;
} area_habitada;

area_habitada vilarejo[VILAS];
missel_struct missel_pronto[MAX_MISSEIS];
SemaphoreHandle_t semaphoro;
SemaphoreHandle_t semaphoro2;
time_t tempo_inicial, tempo_final;
int diferenca_de_tempo;

int main(void)
{
    semaphoro = xSemaphoreCreateMutex();
    semaphoro2 = xSemaphoreCreateMutex();
    tempo_inicial = time (NULL);

    int i,j;
        for( i= 0; i < MAX_MISSEIS ; i++  ){
            missel_pronto[i].nome = 0;
            missel_pronto[i].local = 0;
            missel_pronto[i].tempo_destroier = 0;
            missel_pronto[i].tempo_de_lancamento = 0;
        }
        for( i= 0; i < VILAS ; i++  ){
                j= 5*(i+1);
            vilarejo[i].nome = j;
            vilarejo[i].nome_alvo_missel= 0;
        }

    xTaskCreate(&vthread_atack, "ATACK01", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(&vthreadmonitor_areas_habitadas, "MONITOR", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(&vthread_defesa, "DEFESA", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    vTaskStartScheduler();
    return 0;
}
void vthread_atack(void *pvParameters) {
    int misseis,i, cont;
    cont = 0;
    while(1) {
        cont++;        
        misseis = ( rand() % MAX_MISSEIS) + 1;
        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){
            // Calcular trajetórias dos misseis e enviar as informações para o processo de defesa
            i=0;
            while( i < misseis) 
            {
                vTaskDelay(pdMS_TO_TICKS(rand() % 1000));
                tempo_final = time(NULL);
                diferenca_de_tempo = difftime(tempo_final,tempo_inicial );
                missel_pronto[i].nome = i+1;
                missel_pronto[i].local = (rand() % MAX_AREA)+ 1 ;
                missel_pronto[i].tempo_destroier = ( rand() % MIN_INTERVALO) + 1;
                missel_pronto[i].tempo_de_lancamento = diferenca_de_tempo;
                printf( "tempo_de_lancamento do missel: %i\n", missel_pronto[i].tempo_de_lancamento );
                printf( "TEMPO do missel: %d\n", missel_pronto[i].tempo_destroier );
                printf( "LOCAL do missel: %d\n\n", missel_pronto[i].local );
               i++;
            }
            printf("Ataque: (%d) foram lancados:(%d) misseis\n",cont, i );
            xSemaphoreGive(semaphoro);
        }
        vTaskDelay(pdMS_TO_TICKS(rand() % (MAX_INTERVALO - MIN_INTERVALO + 1) + MIN_INTERVALO));
    }
}

void vthread_defesa(void *pvParameters) {
    //pegar o acesso ao buffer
    //receber os endereços dos misseis que estão para areas habitadas
    //verificar o menor tempo maior que ZERO;
    //interceptar enviando misseis para destruir
    //mostrar na tela quantos misseis foram lançados par aintercetar
    //ZERAR OS DADOS DO MISSEL
    //INFORMAR MISSEL DESTRUIDO
    int i;
    int j;
    int aux, aux2;
    
    while(1){
        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){
            if (xSemaphoreTake(semaphoro2, portMAX_DELAY) == pdTRUE){
                    for( i = 0 ; i < VILAS ; i++ )
                    {
                            aux = vilarejo[i].nome_alvo_missel;
                            if( aux != 0){
                                 tempo_final = time(NULL);
                                diferenca_de_tempo = difftime(tempo_final, tempo_inicial );
                                aux2 = diferenca_de_tempo - missel_pronto[aux].tempo_de_lancamento;
                                if ( aux2 <  missel_pronto[aux].tempo_destroier) 
                                    {
                                        printf( "Missel(%d) foi interceptado\n", missel_pronto[aux].nome );
                                        missel_pronto[aux].nome = 0;
                                        missel_pronto[aux].local = 0;
                                        missel_pronto[aux].tempo_destroier = 0;
                                        missel_pronto[aux].tempo_de_lancamento = 0;
                                    }
                                else 
                                {
                                        printf(  "Missel(%d) atingiu uma area Habitada\n", missel_pronto[aux].nome);
                                        missel_pronto[aux].nome = 0;
                                        missel_pronto[aux].local = 0;
                                        missel_pronto[aux].tempo_destroier = 0;
                                        missel_pronto[aux].tempo_de_lancamento = 0;
                                }
                            }
                    }
             }
                xSemaphoreGive(semaphoro2);
            }
           xSemaphoreGive(semaphoro); 
    }
}

void vthreadmonitor_areas_habitadas( void *pvParameters ){
        int i, j ;
   /*
   acessar o buffer
   verficar quais misseis estão para as areas habitadas
   preencher o array vilarejos com nomes dos misseis que estão para atingir a area
   */
    while (1){
        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){
            if (xSemaphoreTake(semaphoro2, portMAX_DELAY) == pdTRUE){
                for(i= 0 ;  i < VILAS ; i++){
                        for(j= 0 ;  j < MAX_MISSEIS & missel_pronto[j].nome != 0 ; j++){
                            if(vilarejo[i].nome == missel_pronto[j].local)                            
                                    vilarejo[i].nome_alvo_missel= missel_pronto[j].nome;
                            else
                            {
                                if( missel_pronto[j].nome != 0)
                                vilarejo[i].nome_alvo_missel=0;
                            }
                        }
                }
                xSemaphoreGive(semaphoro2);
            }
               xSemaphoreGive(semaphoro);
        }
    }
}
