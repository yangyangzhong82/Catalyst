#pragma once

#ifdef CATALYST_EXPORTS
#define CATALYST_API __declspec(dllexport)
#else
#define CATALYST_API __declspec(dllimport)
#endif