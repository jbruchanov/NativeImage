//
// Created by scurab on 09/04/17.
//

#ifndef NATIVEIMAGEAPP_EFFECT_H
#define NATIVEIMAGEAPP_EFFECT_H

#define EFF_GRAYSCALE "grayScale"
#define EFF_CROP "crop"
#define EFF_BRIGHTNESS "brightness"

#include <map>
#include "string"
#include "../json11/json11.hpp"
#include "ImageProcessor.hpp"

using namespace std;

struct EffectResult : public ImageData {
public:
    int result;
    int componentsPerPixel;

    EffectResult(int result) {
        this->result = result;
    }

    EffectResult(int result, unsigned char* data, int w, int h, int comps) {
        this->result = result;
        this->data = data;
        this->metaData.imageWidth = w;
        this->metaData.imageHeight = h;
        this->componentsPerPixel = comps;
    }
};

typedef EffectResult (*EffectFunction)(unsigned char *, int, int, int, json11::Json *saveArgs);

class Effect {
public:
    static EffectFunction get(string name);

};


#endif //NATIVEIMAGEAPP_EFFECT_H
