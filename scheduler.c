#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// funcion para hacer split a input de file
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    result = malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}


struct persona {
  char name[255];
  char estado[255];
  int*  tiempos;   // un arreglo
  int  indicador;
  int prioridad;
  int tiempo_inicio;
  int N;
  int tiempo_para_ready;
  int cantidad_usos_cpu;
  int n_interrupciones;
  int tiempo_finished;
  int response_time;
  int indice_response;
  int tiempo_ejecute;
};

// cada proceso se definira como una persona 
typedef struct persona Person;


typedef struct colapersonas {
  Person** people;
  int length;
} QueuePeople;

int main(int argc, char* argv[])
{
    // cuento las lineas
    FILE *fp; 
    int count = 0;  
    char buf_1[1000];  
    fp = fopen(argv[1], "r"); 
    if (fp == NULL) 
    { 
        return 0; 
    } 
    while (fgets(buf_1,1000, fp)!=NULL)
        {count = count + 1; }
    fclose(fp);
    //
    // creo una cola de personas, que seran los procesos
   QueuePeople* cola = malloc(sizeof(QueuePeople));
    cola -> length = count;
    cola -> people = malloc(sizeof(Person*)*(cola -> length));
    // abro el archivo y  agrego procesos a la cola
    FILE *ptr_file;
    char buf[1000];
    ptr_file =fopen(argv[1],"r");
    if (!ptr_file)
        return 1;
    int i;
    i = 0;
    while (fgets(buf,1000, ptr_file)!=NULL){
        // se string en month
        // Crear proceso
        Person* persona_nueva = malloc(sizeof(Person));
        // se hace split a string de archivo
        char** tokens;
        tokens = str_split(buf, ' ');
        // se agregan elementos a lista 
        int j;
        for (j = 0; *(tokens + j); j++){
                {if (j == 0){strcpy(persona_nueva -> name, *(tokens + j));}
                if (j == 1){persona_nueva -> prioridad = atoi(*(tokens + j));}
                if (j == 2){persona_nueva -> tiempo_inicio = atoi(*(tokens + j));}
                if (j == 3){persona_nueva -> N = atoi(*(tokens + j));
                    // se agrega lista con tiempos
                    persona_nueva -> tiempos = malloc(sizeof(int)*(persona_nueva -> N )*2 - 1);}
                if (j > 3){persona_nueva -> tiempos[j-3] = atoi(*(tokens + j));}}
            }

            // se libera la memoria, primero del split del archivo

            if (tokens)
            {
                int i;
                for (i = 0; *(tokens + i); i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);
            }
        persona_nueva -> indicador = 0 ; // se necesita para evidenciar estado 0
        persona_nueva ->tiempo_para_ready = persona_nueva -> tiempo_inicio; // numero para identificar que se esta partiendo
        strcpy(persona_nueva -> estado, "Waiting") ;
        // se agrega persona a la cola
        cola -> people[i] = persona_nueva;
        i++;}

    // luego de agregarse los procesos a la cola , se procede con la simulación
    int t = 0;
    Person* proceso_a_ejecutar; // mallo*c?
    int tiempo_global;
    tiempo_global = 0;
    // version no preemptive
    if (strcmp(argv[3],"np") == 0){
        while (1) {
        t++;
        // se ve proceso a ejecutar
        int preferencia;
        // se guarda la preferencia del proceso a ejecutar en esta variable
        preferencia = 0;
        int segunda_preferencia = 0;
        int hay_readys = 0;
        // waiting time
        int waiting_time;
        waiting_time = 10000;
        // se tiene una referencia del waiting time(esto sera en caso de que no hay ninguno en ready)
        // para un instante determinado se hace lo siguiente por cada proceso en la cola.
        for (i = 0; i < cola -> length; i++){
            // se revisa si algún proceso tiene que pasar de waiting a ready
            if(strcmp(cola -> people[i] -> estado ,"Waiting") == 0){
                if (tiempo_global >= cola -> people[i] -> tiempo_para_ready){
                    strcpy(cola -> people[i] -> estado,"Ready");
                    cola -> people[i] -> tiempo_para_ready = 0;
                }
            }
             // se revisa si por ahora es el mejor candidato para ejecutarse 
            if(strcmp (cola -> people[i] -> estado,"Ready") == 0){
                if (cola -> people[i] -> prioridad > preferencia){
                    preferencia = cola -> people[i] -> prioridad;
                    segunda_preferencia = cola -> people[i] -> tiempo_inicio;
                    proceso_a_ejecutar = cola -> people[i];
                    hay_readys = 1;}
                if (cola -> people[i] -> prioridad == preferencia && 
                    segunda_preferencia <= cola -> people[i] -> tiempo_inicio){
                    preferencia = cola -> people[i] -> prioridad;
                    segunda_preferencia = cola -> people[i] -> tiempo_inicio;
                    proceso_a_ejecutar = cola -> people[i];
                    hay_readys = 1;}
            }
        }

        // si luego de revisar todos los procesos no hay readys, se busca el de menor waiting time

        if (hay_readys == 0){
            int j;
            int indicador_aux; 
            for (j = 0; j < cola -> length; j++){
                if(cola -> people[j] -> indicador == 0){
                    if (cola -> people[j] -> tiempo_inicio < waiting_time)
                        {waiting_time = cola -> people[j] -> tiempo_inicio;
                        proceso_a_ejecutar = cola -> people[j];
                        hay_readys = 1;}
                }
                // los procesos finished tendran waiting_time = 10000
                indicador_aux = cola->people[j]->indicador;
                if(indicador_aux > 0 ){
                    if (cola -> people[j] -> tiempo_para_ready < waiting_time)
                        {waiting_time = cola -> people[j] -> tiempo_para_ready;
                        proceso_a_ejecutar = cola -> people[j];
                        hay_readys = 1;}
                }
            }
            tiempo_global = waiting_time ;
        }

        // si luego de revisar los waiting time, ninguno esta listo, significa que todos los procesos ya terminaron

        if (hay_readys == 0){
            break;}

        // ejecuto el proceso el proceso seleccionado anteriormente 
        int tiempo_ejecucion;
        int indicador;
        indicador = proceso_a_ejecutar->indicador;
        if (indicador == 0){
            proceso_a_ejecutar -> response_time = tiempo_global - proceso_a_ejecutar ->tiempo_inicio;
        }
        strcpy(proceso_a_ejecutar->estado,"Waiting");
        tiempo_ejecucion = proceso_a_ejecutar -> tiempos [indicador+1];
        proceso_a_ejecutar -> tiempo_ejecute = proceso_a_ejecutar -> tiempo_ejecute + tiempo_ejecucion;
        proceso_a_ejecutar->indicador = indicador + 2;
        indicador = indicador + 2;
        tiempo_global = tiempo_global + tiempo_ejecucion;
        proceso_a_ejecutar -> tiempo_para_ready = proceso_a_ejecutar->tiempos[indicador] +tiempo_global;
        int indicador_2;
        indicador_2 = indicador;
        proceso_a_ejecutar -> cantidad_usos_cpu = proceso_a_ejecutar->cantidad_usos_cpu +1;
        if (proceso_a_ejecutar->N*2 - 1 < indicador_2){
            strcpy(proceso_a_ejecutar->estado,"Finished");
            proceso_a_ejecutar -> tiempo_finished = tiempo_global;
            proceso_a_ejecutar -> tiempo_para_ready = 10000;
           }
        }
    }

    // se ejecuta para preemptive
    if (strcmp(argv[3],"p") == 0) {
        int quantum = 3; // cambio
        if(argc == 5){   // cambio
            quantum = atoi(argv[4]);}  //cambio
        while (1) {
        t++;
        // se ve proceso a ejecutar
        int preferencia;
        // se guarda la preferencia del proceso a ejecutar
        preferencia = 0;
        int hay_readys = 0;
        // waiting time
        int waiting_time;
        waiting_time = 10000;
        int segunda_preferencia = 0;
        // se tiene una referencia del waiting time(esto sera en caso de que no hay ninguno en ready)
        // se revisa por cada elemento en la cola, y se busca el proximo proceso a ejecutar
        for (i = 0; i < cola -> length; i++){
            if(strcmp(cola -> people[i] -> estado ,"Waiting") == 0){
                if (tiempo_global >= cola -> people[i] -> tiempo_para_ready){
                    strcpy(cola -> people[i] -> estado,"Ready");
                    cola -> people[i] -> tiempo_para_ready = 0;
                }
            }

            if(strcmp (cola -> people[i] -> estado,"Ready") == 0){
                if (cola -> people[i] -> prioridad > preferencia){
                    preferencia = cola -> people[i] -> prioridad;
                    segunda_preferencia = cola -> people[i] -> tiempo_inicio;
                    proceso_a_ejecutar = cola -> people[i];
                    hay_readys = 1;}
                if (cola -> people[i] -> prioridad == preferencia && 
                    segunda_preferencia <= cola -> people[i] -> tiempo_inicio){
                    preferencia = cola -> people[i] -> prioridad;
                    segunda_preferencia = cola -> people[i] -> tiempo_inicio;
                    proceso_a_ejecutar = cola -> people[i];
                    hay_readys = 1;}
            }}
        if (hay_readys == 0){
            int j;
            int indicador_aux; 
            for (j = 0; j < cola -> length; j++){
                if(cola -> people[j] -> indicador == 0){
                    if (cola -> people[j] -> tiempo_inicio < waiting_time)
                        {waiting_time = cola -> people[j] -> tiempo_inicio;
                        proceso_a_ejecutar = cola -> people[j];
                        hay_readys = 1;}
                }
                // los procesos finished tendran waiting_time = 10000
                indicador_aux = cola->people[j]->indicador;
                if(indicador_aux > 0 ){
                    if (cola -> people[j] -> tiempo_para_ready < waiting_time)
                        {waiting_time = cola -> people[j] -> tiempo_para_ready;
                        proceso_a_ejecutar = cola -> people[j];
                        hay_readys = 1;}
                }
            }
            tiempo_global = waiting_time ;
        }
        if (hay_readys == 0){
            break;}

        // ejecuto el proceso
        int tiempo_ejecucion;
        int indicador;

        indicador = proceso_a_ejecutar->indicador;
        tiempo_ejecucion = proceso_a_ejecutar -> tiempos [indicador+1];
        if ((indicador == 0) && (proceso_a_ejecutar -> indice_response != 1) ){
            proceso_a_ejecutar -> response_time = tiempo_global - proceso_a_ejecutar ->tiempo_inicio;
            proceso_a_ejecutar ->indice_response = 1;
        }
        if (quantum >= tiempo_ejecucion){
            proceso_a_ejecutar -> tiempo_ejecute = proceso_a_ejecutar -> tiempo_ejecute + tiempo_ejecucion;
            strcpy(proceso_a_ejecutar->estado,"Waiting");
            proceso_a_ejecutar->indicador = indicador + 2;
            indicador = indicador + 2;
            tiempo_global = tiempo_global + tiempo_ejecucion;
            proceso_a_ejecutar -> tiempo_para_ready = proceso_a_ejecutar->tiempos[indicador] +tiempo_global;
            int indicador_2;
            indicador_2 = indicador;
            if (proceso_a_ejecutar->N*2 - 1 < indicador_2){
                strcpy(proceso_a_ejecutar->estado,"Finished");
                proceso_a_ejecutar -> tiempo_para_ready = 10000;
                proceso_a_ejecutar -> tiempo_finished = tiempo_global;
               }
            if (quantum == tiempo_ejecucion){
                proceso_a_ejecutar -> n_interrupciones = proceso_a_ejecutar->n_interrupciones + 1;
            }
           }

        // si cuantum es mas chico, estado queda en ready, se mantiene tiemo para ejectuar con ready
        else{ 
            proceso_a_ejecutar -> tiempo_ejecute = proceso_a_ejecutar -> tiempo_ejecute + quantum;
            strcpy(proceso_a_ejecutar->estado,"Ready");
            tiempo_global = tiempo_global + quantum;
            proceso_a_ejecutar -> tiempos[indicador+1] = proceso_a_ejecutar -> tiempos[indicador+1] - quantum;
            proceso_a_ejecutar -> n_interrupciones = proceso_a_ejecutar -> n_interrupciones + 1;
            }
        proceso_a_ejecutar -> cantidad_usos_cpu = proceso_a_ejecutar->cantidad_usos_cpu +1;
        }
    }

    FILE *ptr_file_2;
    ptr_file_2 =fopen(argv[2], "w");
    if (!ptr_file_2)
        return 1;
    for (i = 0; i < cola -> length; i++){
        fprintf(ptr_file_2,"%s,",cola -> people[i]-> name );
        fprintf(ptr_file_2,"%i,", cola -> people[i]-> cantidad_usos_cpu);
        fprintf(ptr_file_2,"%i,", cola -> people[i]-> n_interrupciones);
        fprintf(ptr_file_2,"%i,", cola -> people[i]-> tiempo_finished - cola->people[i]->tiempo_inicio);
        fprintf(ptr_file_2,"%i,", cola->people[i]->response_time);
        fprintf(ptr_file_2,"%i\n", cola -> people[i]-> tiempo_finished-cola->people[i]->tiempo_inicio-cola->people[i]->tiempo_ejecute );
    } 
    fclose(ptr_file_2);


    // Finalmente, liberamos el espacio de memoria solicitado.
    // Regla fundamental: Un free por cada malloc.
    for (i = 0; i < cola -> length; i++){
          free(cola->people[i]->tiempos)  ;
          free(cola -> people[i]);
        }
    free(cola -> people);
    free(cola);

    return 0;
}