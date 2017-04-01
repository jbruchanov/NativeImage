//
// Created by scurab on 01/04/17.
//

#include "PNGImage.hpp"
#include "LogHelper.h"
#include "Errors.h"
#include <png.h>

int PNGImage::loadImage(const char *path) {
    png_byte header[8];    // 8 is the maximum size that can be checked
    int x, y;

    png_uint_32 width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;

    FILE *infile;
    if ((infile = fopen(path, "rb")) == NULL) {
        LOGE("can't open %s\n", path);
        return CANT_OPEN_FILE;
    }

    fread(header, 1, 8, infile);
    if (png_sig_cmp(header, 0, 8)) {
        LOGE("[read_png_file] File %s is not recognized as a PNG file", path);
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


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))) {
        LOGE("[read_png_file] Error during read_image");
        return ERR_UNKNOWN;
    }

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    fclose(infile);
}

int PNGImage::saveImage(const char *path, int quality) {
    return 0;
}
