#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "sha.h"

struct Nodo
{
    std::string partida;
    std::vector<int> cuerpo;
    std::string firma;
    Nodo(std::string p, std::vector<int> c) : partida(p), cuerpo(c)
    {
        std::string concat = partida + " ";
        for (int num : cuerpo)
            concat += std::to_string(num) + " ";
        firma = sha256(concat);
    }
};

struct ResultadoCaso
{
    int n;
    int k;
    double tiempo_ms;
};

std::vector<Nodo> generar_lista(int n, int k, double &tiempo_ms)
{
    auto inicio = std::chrono::high_resolution_clock::now();

    std::vector<Nodo> lista;
    time_t now = time(0);
    std::string partida_actual = sha256(std::ctime(&now));

    for (int i = 0; i < n; i++)
    {
        std::vector<int> cuerpo;
        for (int j = 0; j < k; j++)
            cuerpo.push_back(rand() % 100000 + 1);
        lista.emplace_back(partida_actual, cuerpo);
        partida_actual = lista.back().firma;
    }

    auto fin = std::chrono::high_resolution_clock::now();
    tiempo_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();

    return lista;
}

void mostrar_tabla_comparativa(const std::vector<ResultadoCaso> &resultados)
{
    std::cout << "\n=== TABLA COMPARATIVA DE TIEMPOS ===\n";
    std::cout << "+------+-------+------------+\n";
    std::cout << "|   n  |   k   | Tiempo (ms)|\n";
    std::cout << "+------+-------+------------+\n";

    for (const auto &res : resultados)
    {
        std::cout << "| " << std::setw(4) << res.n << " | "
                  << std::setw(5) << res.k << " | "
                  << std::setw(10) << std::fixed << std::setprecision(2) << res.tiempo_ms << " |\n";
    }

    std::cout << "+------+-------+------------+\n";
}

int main()
{
    std::vector<ResultadoCaso> resultados;
    std::vector<std::pair<int, int>> casos = {
        {3, 4},    // Caso 1
        {10, 200}, // Caso 2
        {200, 10}  // Caso 3
    };

    srand(time(0));

    for (const auto &caso : casos)
    {
        double tiempo = 0.0;
        auto lista = generar_lista(caso.first, caso.second, tiempo);
        resultados.push_back({caso.first, caso.second, tiempo});

        // Mostrar detalles del primer nodo como ejemplo
        if (!lista.empty())
        {
            std::cout << "\nCaso n=" << caso.first << ", k=" << caso.second << " - Primer nodo:\n";
            std::cout << "Partida: " << lista[0].partida.substr(0, 10) << "...\n";
            std::cout << "Cuerpo (" << lista[0].cuerpo.size() << " elementos): ";
            for (size_t j = 0; j < std::min(5, (int)lista[0].cuerpo.size()); j++)
                std::cout << lista[0].cuerpo[j] << " ";
            if (lista[0].cuerpo.size() > 5)
                std::cout << "...";
            std::cout << "\nFirma: " << lista[0].firma.substr(0, 10) << "...\n";
        }
    }

    mostrar_tabla_comparativa(resultados);

    return 0;
}