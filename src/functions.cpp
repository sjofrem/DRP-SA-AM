#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <random>
#include "include/functions.h"

double distancia(const Evento &e1, const Evento &e2)
{
    double dx = e1.x - e2.x;
    double dy = e1.y - e2.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::vector<bool>> construir_matriz_cobertura(const std::vector<Evento> &eventos, double radio_cobertura)
{
    int num_eventos = eventos.size();
    std::vector<std::vector<bool>> matriz_cobertura(num_eventos, std::vector<bool>(num_eventos, false));

    for (int i = 0; i < num_eventos; ++i)
    {
        for (int j = 0; j < num_eventos; ++j)
        {
            if (distancia(eventos[i], eventos[j]) <= radio_cobertura)
            {
                matriz_cobertura[i][j] = true;
            }
        }
    }

    return matriz_cobertura;
}

void imprimir_matriz(const std::vector<std::vector<bool>> &matriz)
{
    for (const auto &fila : matriz)
    {
        for (bool valor : fila)
        {
            std::cout << valor << ' ';
        }
        std::cout << '\n';
    }
}

std::vector<bool> algoritmo_greedy(const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos)
{
    int num_eventos = matriz_cobertura.size();
    std::vector<bool> solucion(num_eventos, false);
    std::vector<bool> eventos_cubiertos(num_eventos, false);

    for (int i = 0; i < num_eventos; ++i)
    {
        if (eventos[i].aedInstalado)
        {
            solucion[i] = true;
            for (int j = 0; j < num_eventos; ++j)
            {
                if (matriz_cobertura[i][j])
                {
                    eventos_cubiertos[j] = true;
                }
            }
        }
    }

    for (int p = 0; presupuesto - p >= 1; ++p)
    {
        int mejor_ubicacion = -1;
        int mejor_cobertura = -1;

        for (int i = 0; i < num_eventos; ++i)
        {
            if (solucion[i])
                continue; // Si ya hay un AED en esta ubicación, pasa a la siguiente

            int cobertura = 0;
            for (int j = 0; j < num_eventos; ++j)
            {
                if (matriz_cobertura[i][j] && !eventos_cubiertos[j])
                {
                    ++cobertura;
                }
            }

            if (cobertura > mejor_cobertura)
            {
                mejor_ubicacion = i;
                mejor_cobertura = cobertura;
            }
        }

        if (mejor_ubicacion != -1)
        {
            solucion[mejor_ubicacion] = true;
            for (int j = 0; j < num_eventos; ++j)
            {
                if (matriz_cobertura[mejor_ubicacion][j])
                {
                    eventos_cubiertos[j] = true;
                }
            }
        }
    }

    return solucion;
}

void imprimir_vector(const std::vector<bool> &vector)
{
    for (auto valor : vector)
    {
        std::cout << valor << ' ';
    }
    std::cout << '\n';
}

std::vector<bool> generar_vecino(const std::vector<bool> &solucion)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, solucion.size() - 1);

    int pos = distrib(gen); // Genera una posición aleatoria

    std::vector<bool> vecino = solucion;
    vecino[pos] = !vecino[pos]; // Realiza el bit flip

    return vecino;
}

double calcular_costo(const std::vector<bool> &solucion, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos)
{
    int num_eventos = eventos.size();
    int num_eventos_cubiertos = 0;
    std::vector<bool> eventos_cubiertos(num_eventos, false);
    double costo = 0;

    for (int i = 0; i < num_eventos; ++i)
    {
        if (solucion[i])
        {
            if (!eventos[i].aedInstalado)
            {
                costo += 1; // AED nuevo
            }
            for (int j = 0; j < num_eventos; ++j)
            {
                if (matriz_cobertura[i][j] && !eventos_cubiertos[j])
                {
                    eventos_cubiertos[j] = true;
                    ++num_eventos_cubiertos;
                }
            }
        }
        else if (eventos[i].aedInstalado && !solucion[i])
        {
            costo -= 0.8; // AED inicial no reubicado, se ahorra parte del presupuesto
        }
    }

    if (costo > presupuesto)
    {
        // Si la solución se pasa del presupuesto, se le asigna un costo muy alto
        return std::numeric_limits<double>::max();
    }
    else
    {
        // De lo contrario, el costo sera el número de eventos no cubiertos
        return num_eventos - num_eventos_cubiertos;
    }
}

std::vector<bool> simulated_annealing(std::vector<bool> solucion_inicial, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto, const std::vector<Evento> &eventos, int tmax, double q_inicial)
{
    std::vector<bool> X = solucion_inicial;
    std::vector<bool> mejor_solucion = X;
    double q = q_inicial;
    int t = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0);

    while (t < tmax)
    {
        // Paso 2: Posible Movimiento
        std::vector<bool> X_t_plus_1 = generar_vecino(X);
        double delta_objetivo = calcular_costo(X_t_plus_1, matriz_cobertura, presupuesto, eventos) - calcular_costo(X, matriz_cobertura, presupuesto, eventos);

        // Paso 3: Aceptación
        if (delta_objetivo < 0 || std::exp(-delta_objetivo / q) >= distrib(gen))
        {
            X = X_t_plus_1;
        }

        // Paso 4: Reemplazar el mejor
        if (calcular_costo(X, matriz_cobertura, presupuesto, eventos) < calcular_costo(mejor_solucion, matriz_cobertura, presupuesto, eventos))
        {
            /* std::cout << "\n";
            imprimir_vector(X);
            std::cout << calcular_costo(X, matriz_cobertura, presupuesto, eventos) << "\n";
            imprimir_vector(mejor_solucion);
            std::cout << calcular_costo(mejor_solucion, matriz_cobertura, presupuesto, eventos) << "\n";
            std::cout << "\n"; */
            mejor_solucion = X;
        }

        // Paso 5: reducción de la Temperatura
        if (t % 100 == 0)
        {             // reduce la temperatura cada 100 iteraciones
            q *= 0.9; // reduce la temperatura en un 10%
        }
        // Paso 6: Incrementar
        ++t;
    }

    return mejor_solucion;
}

void ubucacion_aeds(const std::vector<bool> &solucion, const std::vector<Evento> &eventos)
{
    int num_eventos = eventos.size();
    int contadore_aeds_reubicados = 0;

    for (int i = 0; i < num_eventos; ++i)
    {
        if (eventos[i].aedInstalado && !solucion[i])
        {
            contadore_aeds_reubicados++;
        }
    }
    for (int i = 0; i < num_eventos; ++i)
    {
        if (solucion[i])
        {
            std::cout << "AED en (" << eventos[i].x << ", " << eventos[i].y << ")";
            if (contadore_aeds_reubicados > 0)
            {
                std::cout << " (reposicionado)\n";
                contadore_aeds_reubicados--;
            }
            else
            {
                std::cout << " (nuevo)\n";
            }
        }
    }
}

Resultado analizar_solucion(const std::vector<bool> &solucion, const std::vector<Evento> &eventos, const std::vector<std::vector<bool>> &matriz_cobertura, double presupuesto)
{

    int num_eventos = eventos.size();
    int num_eventos_cubiertos = 0;
    std::vector<bool> eventos_cubiertos(num_eventos, false);

    std::vector<Evento> aeds;
    double costo = 0;
    for (int i = 0; i < num_eventos; ++i)
    {
        if (solucion[i])
        {
            aeds.push_back(eventos[i]);
            if (!eventos[i].aedInstalado)
            {
                costo += 1; // Nuevo AED
            }
            for (int j = 0; j < num_eventos; ++j)
            {
                if (matriz_cobertura[i][j] && !eventos_cubiertos[j])
                {
                    eventos_cubiertos[j] = true;
                    ++num_eventos_cubiertos;
                }
            }
        }
        else if (eventos[i].aedInstalado && !solucion[i])
        {
            costo -= 0.8; // AED inicial no reubicado, se ahorra parte del presupuesto
        }
    }

    double porcentaje_eventos_cubiertos = 100.0 * num_eventos_cubiertos / num_eventos;

    double presupuesto_sobrante = round((presupuesto - costo) * 100.0) / 100.0;

    return {num_eventos_cubiertos, porcentaje_eventos_cubiertos, aeds, presupuesto_sobrante};
}
