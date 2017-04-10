//
// Created by scurab on 09/04/17.
//

#include "Effect.h"
#include "Image.hpp"
#include "Errors.h"

EffectResult grayScale(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
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
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

EffectResult crop(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    Json arg = *saveArgs;
    int offsetX = arg["offsetX"].int_value();
    int offsetY = arg["offsetY"].int_value();
    int cropWidth = arg["width"].int_value();
    int cropHeight = arg["height"].int_value();

    if ((offsetX + cropWidth) > width || (offsetY + cropHeight) > height) {
        return EffectResult(INVALID_RESOLUTION);
    }

    int dst = 0;
    for (int y = offsetY; y < (offsetY + cropHeight); y++) {
        for (int x = offsetX; x < (offsetX + cropWidth); x++) {
            int src = ARRAY_INDEX(x, y, width) * componentsPerPixel;
            for (int z = 0; z < componentsPerPixel; z++, dst++) {
                data[dst] = data[src + z];
            }
        }
    }
    unsigned char* newPtr = (unsigned char *) realloc(data, (size_t) (cropWidth * cropHeight * componentsPerPixel));
    return EffectResult(NO_ERR, newPtr, cropWidth, cropHeight, componentsPerPixel);
}

EffectResult brightness(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    Json arg = *saveArgs;
    int brightness = arg["brightness"].int_value();
    if (brightness != 0) {
        for (int i = 0, l = width * height * componentsPerPixel; i < l; i++) {
            if (componentsPerPixel == RGBA && i % RGBA == 0) {
                i++;
            }
            data[i] = (unsigned char) max(0, min(255, ((int) data[i]) + brightness));
        }
    }
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

void init(map<string, EffectFunction> *pMap) {
    pMap->insert(std::pair<string, EffectFunction>(EFF_GRAYSCALE, grayScale));
    pMap->insert(std::pair<string, EffectFunction>(EFF_CROP, crop));
    pMap->insert(std::pair<string, EffectFunction>(EFF_BRIGHTNESS, brightness));
}

EffectFunction Effect::get(std::string name) {
    static std::map<string, EffectFunction> effects;
    if (effects.size() == 0) {
        init(&effects);
    }
    return effects[name];
}