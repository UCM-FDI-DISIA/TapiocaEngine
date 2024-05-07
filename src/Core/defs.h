#pragma once
#ifdef TAPIOCA_EXPORTS
#define TAPIOCA_API __declspec(dllexport)
#else
#define TAPIOCA_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#include <iostream>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
namespace Tapioca {
class GameObject;

/**
* @brief Estructura de un evento
*/
struct Event {
    GameObject* emisor;     // Emisor del evento
    std::string const id;   // Identificador del evento
    void* info;             // Informacion adicional
    bool global;            // Si se propaga a todos los objetos
};
inline void logInfo(const char* message) {
#ifdef _DEBUG
#ifdef TAPIOCA_EXPORTS
    std::cout << "[INFO|MOTOR] " << message << '\n';
#else
    std::cout << "[INFO|JUEGO] " << message << '\n';
#endif   // TAPIOCA_EXPORTS
#endif   // _DEBUG
}
inline void logWarn(const char* message) {
#ifdef _DEBUG
#ifdef TAPIOCA_EXPORTS
    std::cerr << "\033[38;2;252;245;35m[WARN|MOTOR] " << message << "\033[0m\n";
#else
    std::cerr << "\033[38;2;252;245;35m[WARN|JUEGO] " << message << "\033[0m\n";
#endif   // TAPIOCA_EXPORTS
#endif   // _DEBUG
}
inline void logError(const char* message) {
#ifdef _DEBUG
#ifdef TAPIOCA_EXPORTS
    std::cerr << "\033[38;2;252;30;30m[ERROR|MOTOR] " << message << "\033[0m\n";
#else
    std::cerr << "\033[38;2;252;30;30m[ERROR|JUEGO] " << message << "\033[0m\n";
#endif   // TAPIOCA_EXPORTS
#endif   // _DEBUG
}
};
