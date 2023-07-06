# Defibrillator Relocation Problem
Resolucion del Defibrillator Relocation Problem implementando el algoritmo Greedy y el algoritmo Simulated Annealing.

## Uso
### Compilar programa:
``` bach
make
```
### Correr programa:
``` bach
./bin/DRP "ubicacion del archivo" "maximo numero de iteraciones para el algoritmo Simulated Annealing" "Temperatura inicial del Simulated Annealing"
```
### Ejemplo:
``` bach
./bin/DRP DRP-INSTANCIAS/SJC-DRP/test.txt 1000 100
```
### Output:
``` bach
Número de eventos: 20
Presupuesto: 1.4
Radio de cobertura: 200

Vector de la solucion final: 
0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0 

Número de eventos cubiertos: 17
Porcentaje de eventos cubiertos: 85%
Tiempo de ejecución: 0.0667369 segundos
Presupuesto sobrante: 0
AED en (409199, 435982) (reposicionado)
AED en (409378, 435563) (reposicionado)
AED en (409208, 436263) (nuevo)
```
### Eliminar los archivos de compilación:
``` bach
make clean
```
