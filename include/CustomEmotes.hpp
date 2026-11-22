#pragma once
#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS
    #ifdef KX_CUSTOMEMOTESAPI_EXPORTING
        #define EMOTESAPI_DLL __declspec(dllexport)
    #else
        #define EMOTESAPI_DLL __declspec(dllimport)
    #endif
#else
    #define EMOTESAPI_DLL __attribute__((visibility("default")))
#endif

namespace CustomEmotes {

}