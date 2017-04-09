//
// Created by scurab on 09/04/17.
//

#ifndef NATIVEIMAGEAPP_EFFECT_H
#define NATIVEIMAGEAPP_EFFECT_H

#define GRAYSCALE "grayScale"

#include <map>
#include "string"
#include "../json11/json11.hpp"

using namespace std;

typedef void (*EffectFunction)(unsigned char *, int, int, int, json11::Json *saveArgs);

class Effect {
public:
    static EffectFunction get(string name);

};


#endif //NATIVEIMAGEAPP_EFFECT_H
