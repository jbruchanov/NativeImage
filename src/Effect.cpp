//
// Created by scurab on 09/04/17.
//

#include "Effect.h"
#include "Image.hpp"

void grayScale(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    unsigned char v;
    for (int i = 0, l = width * height * componentsPerPixel; i < l; i += componentsPerPixel) {
        if (componentsPerPixel == RGBA) {
            i++;
        }
        v = (unsigned char) ((data[i] + data[i + 1] + data[i + 2]) / 3);
        data[i] = data[i + 1] = data[i + 2] = v;
        if (componentsPerPixel == RGBA) {
            i--;
        }
    }
}

void init(map<string, EffectFunction> *pMap) {
    pMap->insert(std::pair<string, EffectFunction>(GRAYSCALE, grayScale));
}

EffectFunction Effect::get(std::string name) {
    static std::map<string, EffectFunction> effects;
    if (effects.size() == 0) {
        init(&effects);
    }
    return effects[name];
}