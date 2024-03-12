#pragma once
#ifdef TAPIOCA_EXPORTS
#define TAPIOCA_API __declspec(dllexport)
#else
#define TAPIOCA_API __declspec(dllimport)
#endif

#ifdef _DEBUG
#include <iostream>
#endif;