#ifndef LEERDATOS_H
#define LEERDATOS_H

#include <vector>
#include <string>
#include "estructuras.h"

std::pair<Informacion, std::vector<Evento>> leerDatos(const std::string &nombreArchivo);

#endif