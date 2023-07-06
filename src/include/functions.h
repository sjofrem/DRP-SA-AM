
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>
#include "estructuras.h"

double distancia(const Evento &e1, const Evento &e2);

std::vector<std::vector<bool>> construir_matriz_cobertura(const std::vector<Evento> &eventos, double radio_cobertura);

void imprimir_matriz(const std::vector<std::vector<bool>> &matriz);

std::vector<bool> algoritmo_greedy(const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos);

void imprimir_vector(const std::vector<bool> &vector);

std::vector<bool> simulated_annealing(std::vector<bool> solucion_inicial, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos, int tmax, double q_inicial);

double calcular_costo(const std::vector<bool> &solucion, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos);

void ubucacion_aeds(const std::vector<bool> &solucion, const std::vector<Evento> &eventos);

Resultado analizar_solucion(const std::vector<bool> &solucion, const std::vector<Evento> &eventos, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto);

#endif