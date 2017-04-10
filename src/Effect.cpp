//
// Created by scurab on 09/04/17.
//

#include <cmath>
#include "Effect.h"
#include "Image.hpp"
#include "Errors.h"

#define COLOR_TRUNCATE(value) max(0, min(255, value))

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
            data[i] = (unsigned char) COLOR_TRUNCATE(((int) data[i]) + brightness);
        }
    }
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

EffectResult contrast(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    Json arg = *saveArgs;
    int contrast = arg["contrast"].int_value();
    if (contrast != 0) {
        for (int i = 0, l = width * height * componentsPerPixel; i < l; i++) {
            if (componentsPerPixel == RGBA && i % RGBA == 0) {
                i++;
            }
            double factor = ((259 * (contrast + 255.0)) / (255 * (259 - contrast)));
            data[i] = (unsigned char) COLOR_TRUNCATE((int)(factor * (data[i] - 128) + 128));
        }
    }
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

EffectResult gamma(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    Json arg = *saveArgs;
    double gamma = arg["gamma"].number_value();
    if (gamma != 1.0) {
        for (int i = 0, l = width * height * componentsPerPixel; i < l; i++) {
            if (componentsPerPixel == RGBA && i % RGBA == 0) {
                i++;
            }
            double gammaCorrection = 1.0 / gamma;
            double value = 255.0 * pow((((int) data[i]) / 255.0), gammaCorrection);
            data[i] = (unsigned char) COLOR_TRUNCATE((int) round(value));
        }
    }
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

EffectResult inverse(unsigned char *data, int width, int height, int componentsPerPixel, json11::Json *saveArgs) {
    for (int i = 0, l = width * height * componentsPerPixel; i < l; i++) {
        if (componentsPerPixel == RGBA && i % RGBA == 0) {
            i++;
        }
        data[i] = (unsigned char) (255 - (int)data[i]);
    }
    return EffectResult(NO_ERR, data, width, height, componentsPerPixel);
}

void init(map<string, EffectFunction> *pMap) {
    pMap->insert(std::pair<string, EffectFunction>(EFF_GRAYSCALE, grayScale));
    pMap->insert(std::pair<string, EffectFunction>(EFF_CROP, crop));
    pMap->insert(std::pair<string, EffectFunction>(EFF_BRIGHTNESS, brightness));
    pMap->insert(std::pair<string, EffectFunction>(EFF_CONTRAST, contrast));
    pMap->insert(std::pair<string, EffectFunction>(EFF_GAMMA, gamma));
    pMap->insert(std::pair<string, EffectFunction>(EFF_INVERSE, inverse));
}

EffectFunction Effect::get(std::string name) {
    static std::map<string, EffectFunction> effects;
    if (effects.size() == 0) {
        init(&effects);
    }
    return effects[name];
}