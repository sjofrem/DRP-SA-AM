#include "include/functions.h"
#include "include/leerDatos.h"
#include <iostream>
#include <filesystem>
#include <chrono>

int main(int argc, char *argv[])
{
  auto inicio = std::chrono::high_resolution_clock::now();
  auto [info, eventos] = leerDatos(argv[1]);
  int tmax = atoi(argv[2]);
  double q_inicial = atoi(argv[3]);

  // Imprime la información:
  std::cout << "Número de eventos: " << info.numeroEventos << "\n";
  std::cout << "Presupuesto: " << info.presupuesto << "\n";
  std::cout << "Radio de cobertura: " << info.radioCobertura << "\n";
  std::cout << "\n";
  // Imprime los datos de los eventos:
  /* for (const auto &e : eventos)
  {
    std::cout << e.x << ", " << e.y << ", " << (e.aedInstalado ? "Sí" : "No") << "\n";
  } */
  std::vector<std::vector<bool>> matriz_cobertura = construir_matriz_cobertura(eventos, info.radioCobertura);

  /*   std::cout << "Matriz de cobertura: \n";
    imprimir_matriz(matriz_cobertura);
    std::cout << "\n"; */

  std::vector<bool> solucion_inicial = algoritmo_greedy(matriz_cobertura, info.presupuesto, eventos);
  /* std::cout << "Vector de la solucion inicial: \n";
  imprimir_vector(solucion_inicial); */

  /* Resultado resultado_inicial = analizar_solucion(solucion_inicial, eventos, matriz_cobertura, info.presupuesto);
  std::cout << "Número de eventos cubiertos: " << resultado_inicial.num_eventos_cubiertos << '\n';
  std::cout << "Porcentaje de eventos cubiertos: " << resultado_inicial.porcentaje_eventos_cubiertos << "%\n";
  std::cout << "Presupuesto sobrante: " << resultado_inicial.presupuesto_sobrante << '\n'; */

  // std::cout << "\n";
  std::vector<bool>
      solucion_final = simulated_annealing(solucion_inicial, matriz_cobertura, info.presupuesto, eventos, tmax, q_inicial);
  std::cout << "Vector de la solucion final: \n";
  imprimir_vector(solucion_final);
  std::cout << "\n";
  Resultado resultado_final = analizar_solucion(solucion_final, eventos, matriz_cobertura, info.presupuesto);

  auto fin = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duracion = fin - inicio;
  double tiempo_ejecucion = duracion.count();

  std::cout << "Número de eventos cubiertos: " << resultado_final.num_eventos_cubiertos << '\n';
  std::cout << "Porcentaje de eventos cubiertos: " << resultado_final.porcentaje_eventos_cubiertos << "%\n";
  std::cout << "Tiempo de ejecución: " << tiempo_ejecucion << " segundos\n";
  std::cout << "Presupuesto sobrante: " << resultado_final.presupuesto_sobrante << '\n';

  ubucacion_aeds(solucion_final, eventos);

  return 0;
}