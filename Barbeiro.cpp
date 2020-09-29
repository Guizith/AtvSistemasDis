#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"

#define CHAIRS 5                
#define TRUE 1

int horas = 0;
int cliente = 0;
int dorme = 0;

sem_t customers;                
sem_t barbers;                  
sem_t mutex;                    
int waiting = 0;               

/* protótipos */
void* barber(void *arg);
void* customer(void *arg);
void cut_hair();
void customer_arrived();
void get_haircut();
void giveup_haircut();

int main() {
sem_init(&customers, TRUE, 0);
sem_init(&barbers, TRUE, 0);
sem_init(&mutex, TRUE, 1);

pthread_t b, c;

 /* criando único barbeiro */
 pthread_create(&b, NULL, (void *) barber, NULL);


 /* criação indefinida de clientes */
 while(TRUE) {
pthread_create(&c, NULL, (void *) customer, NULL);
sleep(1);

}

printf("Dorme: %d, Clientes: %d, Horas:%d ",dorme, cliente, horas);
return 0;
}

void* barber(void *arg) {
while(horas<25) {
horas++;
sem_wait(&customers);   /* vai dormir se o número de clientes for 0 */
dorme+=1;
sem_wait(&mutex);       /* obtém acesso a 'waiting' */
waiting = waiting - 1;  /*descresce de um o contador de clientes à espera */
sem_post(&barbers);     /* um barbeiro está agora pronto para cortar cabelo */
sem_post(&mutex);       /* libera 'waiting' */
cut_hair();             /* corta o cabelo (fora da região crítica) */
}

pthread_exit(NULL);
}

void* customer(void *arg) {

sem_wait(&mutex);           /* entra na região crítica */

if(waiting < CHAIRS) {      /* se não houver cadeiras vazias, saia */
customer_arrived();
waiting = waiting + 1;  /* incrementa o contador de clientes à espera */
sem_post(&customers);   /* acorda o barbeiro se necessário */
sem_post(&mutex);       /* libera o acesso a 'waiting' */
sem_wait(&barbers);     /* vai dormir se o número de barbeiros livres for 0 */
get_haircut();          /* sentado e sendo servido */
} else {
sem_post(&mutex);       /* a barbearia está cheia; não espera */
giveup_haircut();

}

pthread_exit(NULL);
}

void cut_hair() {
printf("Barbeiro estah cortando o cabelo de alguem!\n");
sleep(0.41);
}

void customer_arrived() {
printf("Cliente chegou para cortar cabelo!\n");
printf("Dorme: %d, Clientes: %d, Horas:%d ",dorme, cliente, horas);
if(horas>24){
    printf("Dorme: %d, Clientes: %d, Horas:%d ",dorme, cliente, horas);
  }
}
void get_haircut() {
printf("Cliente estah tendo o cabelo cortado!\n");
cliente++;
}

void giveup_haircut() {
printf("Cliente desistiu! (O salao estah muito cheio!)\n");
}
