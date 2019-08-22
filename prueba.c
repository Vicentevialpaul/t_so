#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
int suma(int numero){
	int resultado;
	resultado = numero +5;
	return resultado;
}

int main() 
{ 
    int pid = fork();
    int numero = 6;
    printf("hola mi numero de proceso es %i\n", pid );
    printf("el resultado de mi suma es %i\n", suma(6) );
    numero = numero +1 ;

    return 0; 
} 
