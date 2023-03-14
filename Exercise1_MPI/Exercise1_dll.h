#ifdef EXERCISE1_DLL_EXPORTS
#define submit_API __declspec(dllexport) 
#else
#define submit_API __declspec(dllimport) 
#endif

int submit_API submit(double(*f)(int, double*, double*));
#pragma once
