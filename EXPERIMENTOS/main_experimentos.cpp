#include "include/functions.h"
#include "include/leerDatos.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
  std::string ruta_carpeta = "DRP-INSTANCIAS/SJC-DRP-AEDs";
  std::vector<int> temperatura = {100, 500, 1000};
  std::vector<int> iteraciones = {1000, 5000, 10000};
  if (!fs::is_directory(ruta_carpeta))
  {
    std::cout << "La ruta no es una carpeta válida." << std::endl;
    return 1;
  }

  // Recorrer los archivos dentro de la carpeta
  for (const auto &archivo : fs::directory_iterator(ruta_carpeta))
  {
    // Comprobar si el elemento es un archivo regular
    if (fs::is_regular_file(archivo.path()))
    {
      if (archivo.path().filename().string() == ".DS_Store")
      {
        continue;
      }
      std::string nombreArchivo = archivo.path().filename().string();
      auto [info, eventos] = leerDatos(archivo.path());

      std::ofstream archivo(nombreArchivo);
      if (archivo.is_open())
      {
        // Imprime la información:
        archivo << "Número de eventos: " << info.numeroEventos << "\n";
        archivo << "Presupuesto: " << info.presupuesto << "\n";
        archivo << "Radio de cobertura: " << info.radioCobertura << "\n";
        archivo << "\n";

        std::vector<std::vector<bool>> matriz_cobertura = construir_matriz_cobertura(eventos, info.radioCobertura);

        std::vector<bool> solucion_inicial = algoritmo_greedy(matriz_cobertura, info.presupuesto, eventos);
        for (int temp : temperatura)
        {
          for (int i : iteraciones)
          {
            auto inicio = std::chrono::high_resolution_clock::now();
            int tmax = i;
            double q_inicial = temp;

            std::vector<bool>
                solucion_final = simulated_annealing(solucion_inicial, matriz_cobertura, info.presupuesto, eventos, tmax, q_inicial);

            Resultado resultado_final = analizar_solucion(solucion_final, eventos, matriz_cobertura, info.presupuesto);

            auto fin = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duracion = fin - inicio;
            double tiempo_ejecucion = duracion.count();

            archivo << "q_inicial: " << temp << ", máximos de iteraciones: " << i << '\n';
            archivo << "Número de eventos cubiertos: " << resultado_final.num_eventos_cubiertos << '\n';
            archivo << "Porcentaje de eventos cubiertos: " << resultado_final.porcentaje_eventos_cubiertos << "%\n";
            archivo << "Tiempo de ejecución: " << tiempo_ejecucion << " segundos\n";
            archivo << "Presupuesto sobrante: " << resultado_final.presupuesto_sobrante << '\n';
            archivo << '\n';

            // ubucacion_aeds(solucion_final, eventos);
          }
        }
        archivo.close();
        std::cout << "Se ha escrito en " << nombreArchivo << std::endl;
      }
      else
      {
        std::cout << "No se pudo crear el archivo: " << nombreArchivo << std::endl;
        return 1;
      }
    }
  }

  return 0;
}