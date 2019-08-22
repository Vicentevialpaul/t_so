/** Módulo estándar de Input y Output */
#include <stdio.h>
/** Módulo estándar de números enteros */
#include <stdint.h>
/** Módulo estándar de valores booleanos */
#include <stdbool.h>
// Archivo de listas ligadas
#include "linked_list.c"

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

/** Esta función es lo que se llama al ejecutar tu programa */
int main()
{

  /* Imprime el mensaje en la consola */
  printf("Bienvenido al Acto 5: Listas\n");

  // En esta sección crearemos una lista ligada a partir de sus constructor
  // y luego la usaremos normalmente. La struct LinkedList esta definida en el
  // archivo linked_list.h junto con todas sus funciones publicas. En le archivo
  // linked_list.c esta el codigo de todas las funciones publicas y pricadas

  // Creo la lista ligada
    LinkedList* ll = ll_init();

    // agrego elemento
    FILE *ptr_file;
    char buf[1000];
    ptr_file =fopen("input.txt","r");
    if (!ptr_file)
        return 1;

    int j = 0;
    while (fgets(buf,1000, ptr_file)!=NULL){
      char** tokens;
      tokens = str_split(buf, ' ');
      for (j = 0; *(tokens + j); j++){
        printf("%i\n", j );
        printf("%s", *(tokens + j));
        ll_append(ll, *(tokens + j));
      }

      if (tokens)
      {
          int i;
          for (i = 0; *(tokens + i); i++)
          {
              free(*(tokens + i));
          }
          free(tokens);
       }
    }

int h = 0;
for (h = 0; h < ll -> count; h++){
  printf("%s\n", ll_get(ll, h));

}



  // Agrego 10 elementos a la lista ligada


  // Imprimo el elemento de la posicion 5
  printf("El elemento en la posicion %d es %s\n", 5, ll_get(ll, 3));

  // Destruyo la lista ligada liberando todos sus recursos
  ll_destroy(ll);

  // Como ejercicio puedes probar el programa usando valgrind para ver que no
  // hay leaks de memoria y luego eliminar la linea que llama a ll_destroy para
  // ver que detecta los leaks de memoria de la lista 

  return 0;
}
