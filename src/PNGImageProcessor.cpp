//
// Created by scurab on 01/04/17.
//

#include "PNGImageProcessor.hpp"
#include "LogHelper.h"
#include "Errors.h"
#include <png.h>
#include <cstring>

IOResult PNGImageProcessor::loadImage(const char *path, int componentsPerPixel, char *err) {
    png_byte header[8];    // 8 is the maximum size that can be checked
    int x, y;

    png_uint_32 width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;

    FILE *infile;
    if ((infile = fopen(path, "rb")) == NULL) {
        LOGE("can't open %s\n", path);
        return CANT_OPEN_FILE;
    }

    fread(header, 1, 8, infile);
    if (png_sig_cmp(header, 0, 8)) {
        LOGE("[read_png_file] File %s is not recognized as a PNG file", path);
        fclose(infile);
        return INVALID_PNG;
    }

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        LOGE("[read_png_file] png_create_read_struct failed");
        return ERR_UNKNOWN;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        LOGE("[read_png_file] png_create_info_struct failed");
        return ERR_UNKNOWN;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[read_png_file] Error during init_io");
        return ERR_UNKNOWN;
    }


    png_init_io(png_ptr, infile);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    if (bit_depth != BIT_DEPTH) {
        fclose(infile);
        LOGE("[read_png_file] Invalid bit_depth:%d expected:8", bit_depth);
        return NOT_SUPPORTED_PNG_CONFIGURATION;
    }

    if (!(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGBA)) {
        fclose(infile);
        LOGE("[read_png_file] Invalid color_type:%d expected:PNG_COLOR_TYPE_RGB or PNG_COLOR_TYPE_RGBA", color_type);
        return NOT_SUPPORTED_PNG_CONFIGURATION;
    }

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[read_png_file] Error during read_image");
        fclose(infile);
        return ERR_UNKNOWN;
    }

    bool removeAlpha = componentsPerPixel == RGB && color_type == PNG_COLOR_TYPE_RGBA;
    bool createAlpha = componentsPerPixel == RGBA && color_type == PNG_COLOR_TYPE_RGB;
    if (removeAlpha) {
        //seems i have to convert it by myself => load it as RGBA and then remove alpha channel
        componentsPerPixel = RGBA;
    }

    ImageMetaData metaData;
    bytep_t *data;
    metaData.imageWidth = (int) width;
    metaData.imageHeight = (int) height;
    data = (bytep_t *) malloc((size_t) (metaData.imageWidth * metaData.imageHeight * componentsPerPixel));
    if (data == 0) {
        fclose(infile);
        return IOResult(OUT_OF_MEMORY);
    }
    memset(data, 0, (size_t) (metaData.imageWidth * metaData.imageHeight * componentsPerPixel));
    png_bytep row_pointers[height];
    int rowPointersComponents = createAlpha ? RGB : componentsPerPixel;
    for (y = 0; y < height; y++) {
        row_pointers[y] = (data + (y * metaData.imageWidth * rowPointersComponents));
    }

    //switch rgb to bgr for android native
    /* flip the RGB pixels to BGR (or RGBA to BGRA) */
    if (color_type & PNG_COLOR_MASK_COLOR) {
        png_set_bgr(png_ptr);
    }
    /* swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
    png_set_swap_alpha(png_ptr);

    //read image
    png_read_image(png_ptr, (png_bytepp) &row_pointers);

    if (removeAlpha) {
        data = ImageProcessor::removeAlpha(data, (int) width, (int) height, RGBA);
    } else if (createAlpha) {
        addAlpha(data, (int) width, (int) height);
    }

    IOResult ior;
    ior.data = data;
    ior.metaData = metaData;
    ior.result = NO_ERR;
    fclose(infile);
    return ior;
}



int PNGImageProcessor::saveImage(const char *path, InputData &inputData) {
    png_structp png_ptr;
    png_infop info_ptr;
    png_byte color_type = (png_byte) (inputData.componentsPerPixel == RGBA ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB);

    int width = inputData.metaData.imageWidth;
    int height = inputData.metaData.imageHeight;

    /* create file */
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        LOGE("[write_png_file] File %s could not be opened for writing", path);
        return CANT_OPEN_FILE;
    }

    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    /* flip the RGB pixels to BGR (or RGBA to BGRA) */
    if (color_type & PNG_COLOR_MASK_COLOR) {
        png_set_bgr(png_ptr);
    }
    /* swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
    png_set_swap_alpha(png_ptr);

    if (!png_ptr) {
        LOGE("[write_png_file] png_create_write_struct failed");
        fclose(fp);
        return ERR_UNKNOWN;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        LOGE("[write_png_file] png_create_info_struct failed");
        fclose(fp);
        return ERR_UNKNOWN;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[write_png_file] Error during init_io");
        fclose(fp);
        return ERR_UNKNOWN;
    }

    png_init_io(png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[write_png_file] Error during writing header");
        fclose(fp);
        return ERR_UNKNOWN;
    }

    png_set_IHDR(png_ptr, info_ptr, (png_uint_32) width, (png_uint_32) height,
                 BIT_DEPTH, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[write_png_file] Error during writing bytes");
        fclose(fp);
        return ERR_UNKNOWN;
    }
    png_bytepp row_pointers[height];
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (png_bytepp) (inputData.data + i * inputData.componentsPerPixel * width);
    }
    png_write_image(png_ptr, (png_bytepp) &row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[write_png_file] Error during end of write");
        fclose(fp);
        return ERR_UNKNOWN;
    }


    png_write_end(png_ptr, NULL);
    fclose(fp);
    return NO_ERR;
}

void PNGImageProcessor::addAlpha(bytep_t *data, int width, int height) {
    for (int i4 = (width * height * RGBA) - 1, i3 = (width * height * RGB) - 1; i4 >= 0 && i3 >= 0; i4--, i3--) {
        data[i4] = data[i3];
        data[--i4] = data[--i3];
        data[--i4] = data[--i3];
        data[--i4] = (bytep_t)0xFF;
    }
}