//
// Created by jbruchanov on 29/03/17.
//

#ifndef NATIVEIMAGEAPP_IMAGEMETADATA_H
#define NATIVEIMAGEAPP_IMAGEMETADATA_H

struct ImageMetaData {
public:
    int imageWidth;
    int imageHeight;
    int componentSize;

    int pixelCount() { return imageWidth * imageHeight; }
};

#endif //NATIVEIMAGEAPP_IMAGEMETADATA_H
