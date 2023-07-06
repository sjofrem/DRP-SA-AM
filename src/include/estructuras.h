#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <vector>

struct Informacion
{
    int numeroEventos;
    double presupuesto;
    int radioCobertura;
};

struct Evento
{
    int x;
    int y;
    bool aedInstalado;
};

struct Resultado
{
    int num_eventos_cubiertos;
    double porcentaje_eventos_cubiertos;
    std::vector<Evento> aeds;
    double presupuesto_sobrante;
};

#endif