#include <fstream>
#include <iostream>
#include <sstream>
#include "include/leerDatos.h"

std::pair<Informacion, std::vector<Evento>> leerDatos(const std::string &nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);
    if (!archivo)
    {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\n";
        // En caso de error, retornamos un vector vacío
        return {};
    }

    // Lee la primera línea para obtener la Información
    std::string linea;
    std::getline(archivo, linea);
    std::istringstream lineaStream(linea);

    Informacion info;
    std::string campo;

    std::getline(lineaStream, campo, ' ');
    info.numeroEventos = std::stoi(campo);

    std::getline(lineaStream, campo, ' ');
    info.presupuesto = std::stod(campo);

    std::getline(lineaStream, campo, ' ');
    info.radioCobertura = std::stoi(campo);

    // Lee las líneas restantes para obtener los Eventos
    std::vector<Evento> eventos;
    while (std::getline(archivo, linea))
    {
        std::istringstream lineaStream(linea);

        Evento e;

        std::getline(lineaStream, campo, ' ');
        e.x = std::stoi(campo);

        std::getline(lineaStream, campo, ' ');
        e.y = std::stoi(campo);

        std::getline(lineaStream, campo, ' ');
        e.aedInstalado = std::stoi(campo) == 1;

        eventos.push_back(e);
    }

    archivo.close();

    return {info, eventos};
}