
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
#define MIN_INTERVALO 1000


void vthread_atack(void *pvParameters);
void vthread_defesa(void *pvParameters);
void vthreadmonitor_areas_habitadas( void *pvParameters);

typedef struct {
    int local;
    int tempo_destroier;
    int tempo_de_lancamento;
} missel_struct;

missel_struct missel_pronto[MAX_MISSEIS];
SemaphoreHandle_t semaphoro;
time_t tempo_inicial, tempo_final;
int diferenca_de_tempo;

int main(void)
{
    semaphoro = xSemaphoreCreateMutex();
    tempo_inicial = time (NULL);

    int i;
        for( i= 0; i < MAX_MISSEIS ; i++  ){
            missel_pronto[i].local = 0;
            missel_pronto[i].tempo_destroier = 0;
            missel_pronto[i].tempo_de_lancamento = 0;
        }

    xTaskCreate(&vthread_atack, "ATACK01", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
   //xTaskCreate(&vthread_defesa, "DEFESA", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    //xTaskCreate(&vthreadmonitor_areas_habitadas, "MONITOR", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    vTaskStartScheduler();

    return 0;
}

/*
void vTask1(void *pvParameters)
{
    while(1)
    {
        printf("Task 1-jj\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    while (1)
    {
        printf("Task 2-vai dar certo!\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/

void vthread_atack(void *pvParameters) {
    int misseis,i, cont;
    cont = 0;
    while(1) {
        cont++;
        //vTaskDelay(rand() % (MAX_INTERVALO+50) / portTICK_PERIOD_MS);
        vTaskDelay(pdMS_TO_TICKS(rand() % (MAX_INTERVALO - MIN_INTERVALO + 1) + MIN_INTERVALO));
        
        misseis = ( rand() % MAX_MISSEIS) + 1;

        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){
            // Calcular trajetórias dos misseis e enviar as informações para o processo de defesa
            i=0;
            while( i < misseis) 
            {
                vTaskDelay(pdMS_TO_TICKS(rand() % 1000));
                tempo_final = time(NULL);
                diferenca_de_tempo = difftime(tempo_final,tempo_inicial );
                missel_pronto[i].local = (rand() % MAX_AREA)+ 1 ;
                missel_pronto[i].tempo_destroier = ( rand() % MIN_INTERVALO) + 1;
                missel_pronto[i].tempo_de_lancamento = diferenca_de_tempo;
                printf( "tempo_de_lancamento do missel: %i\n", missel_pronto[i].tempo_de_lancamento );
                printf( "TEMPO do missel: %d\n", missel_pronto[i].tempo_destroier );
                printf( "LOCAL do missel: %d\n\n", missel_pronto[i].local );
               i++;
            }
            printf("Ataque: (%d) foram lancados:(%d) misseis\n",cont, i );
          /*o que fazer agora?
            posso contar o tempo a parte de quando os misseis foram criados e amazenar para cada missel
            acessar p buffer
            verificam a diferença de quando o missel foi criada e o tempo atual
            se positivo armazenar o tempo da missel
            se negativo informar que o missel alcançou o alvo.
            */
            xSemaphoreGive(semaphoro);
        }
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




}


void vthread_lanca_missel( void *pvParameters ){
    int tempo_destroier = 0;
    int posicao, local, i;
/*
    while(1){
        printf("entrou1!");
        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){
            for( i= 0; i < MAX_MISSEIS ; i++  ){
                   printf("entrou 2!");
                if( tempo_destroier > missel_pronto[i].tempo_destroier  & missel_pronto[i].tempo_destroier!= 0){
                    tempo_destroier = missel_pronto[i].tempo_destroier;
                            posicao = missel_pronto[i].nome;
                }
                            /*
                            {
                                    printf("MISSEL ATINGIU O ALVO");
                                    if( (missel_pronto[posicao].local %(2))  == 0){
                                        printf("O MISSEL DESTRUIU UMA ÁREA habitada!");
                                    }
                            }
            }
            xSemaphoreGive(semaphoro);
        }
    
        vTaskDelay(tempo_destroier);

        if (xSemaphoreTake(semaphoro, portMAX_DELAY) == pdTRUE){

            if(missel_pronto[posicao].tempo_destroier == tempo_destroier &
               missel_pronto[posicao].nome            == posicao){
                    printf("MISSEL ATINGIU O ALVO");
                    if( (missel_pronto[posicao].local %(2))  == 0){
                        printf("O MISSEL DESTRUIU UMA ÁREA habitada!");
                    }
                    xSemaphoreGive(semaphoro);
                    vTaskDelete(NULL);
            }
           xSemaphoreGive(semaphoro);
        }
    }
    */
/*
liberar o acesso
verificar o tempo do missel
dar um delay com o tempo do missel
bloquear
    verficar a posição X
    se o missel na posição X tiver o mesmo nome, tempo e local.
    então missel atinjiu a área.
    mostrar na tela missel atingiu a area
liberar o acesso
encerrar a thread
*/

}


void vthreadmonitor_areas_habitadas( void *pvParameters ){
   /* int i,j;
    int aux;
    int array[MAX_MISSEIS];//ARRAY PARA ARMAZENAR OS INDICES DOS MISSEIS QUE CAIRÃO EM AREAS HABITADAS
    //criar um array com 20 posiçoes contantes
    int areas_habitadas[20] = {1,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};

    while (1)
    {
        for( i=0; i < 20 ; i++){
            for( j = 0; j < misseis ; j++){
                if(areas_habitadas[i] ==  missel_pronto[j].local ){
                    printf("O missel (%d) irá cair na área(%d) HABITADA", missel_pronto[j].nome, areas_habitadas[i]);
                }
            }
        }
    }
    */
}
