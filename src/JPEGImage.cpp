//
// Created by jbruchanov on 29/03/17.
//

#include "stdio.h"
#include <setjmp.h>
#include "JPEGImage.hpp"
#include "Errors.h"
#include "LogHelper.h"

struct _Error {
    struct jpeg_error_mgr pub;    /* "public" fields */
    jmp_buf setjmp_buffer;    /* for return to caller */
};
typedef struct _Error *_ErrorPtr;

int JPEGImage::loadImage(const char *path) {
    mMetaData = {};//clear metaData
    releaseRawData();

    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct _Error jerr;
    FILE *infile;        /* source file */
    JSAMPARRAY buffer;   /* Output row buffer */
    int row_stride;      /* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
     * so that the setjmp() error recovery below can assume the file is open.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to read binary files.
     */

    if ((infile = fopen(path, "rb")) == NULL) {
        LOGE("can't open %s\n", path);
        return CANT_OPEN_FILE;
    }

    /* Step 1: allocate and initialize JPEG decompression object */
    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr.pub);
    //this needs to be setup, otherwise crash if error
    jerr.pub.error_exit = [](j_common_ptr cinfo) {
        _ErrorPtr myerr = (_ErrorPtr) cinfo->err;
        longjmp(myerr->setjmp_buffer, 1);
    };

    /* Establish the setjmp return context for my_error_exit to use. */
    if (setjmp(jerr.setjmp_buffer)) {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        cinfo.err->format_message((j_common_ptr) &cinfo, mLastError);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return cinfo.err->msg_code;
    }
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);
    /* Step 2: specify data source (eg, a file) */
    jpeg_stdio_src(&cinfo, infile);
    /* Step 3: read file parameters with jpeg_read_header() */
    (void) jpeg_read_header(&cinfo, TRUE);

    int result = NO_ERR;

    mMetaData.imageWidth = cinfo.image_width;
    mMetaData.imageHeight = cinfo.image_height;
    mMetaData.componentSize = cinfo.num_components;
    mRawData = new (nothrow) int[mMetaData.imageWidth * mMetaData.imageHeight];
    if (mRawData == 0) {
        fclose(infile);
        return OUT_OF_MEMORY;
    }


    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

    if (NO_ERR == result) {
        /* Step 5: Start decompressor */
        jpeg_start_decompress(&cinfo);
        /* JSAMPLEs per row in output buffer */
        row_stride = cinfo.output_width * cinfo.output_components;
        /* Make a one-row-high sample array that will go away when done with image */
        buffer = (*cinfo.mem->alloc_sarray)
                ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

        /* Step 6: while (scan lines remain to be read) */
        /*           jpeg_read_scanlines(...); */

        /* Here we use the library's state variable cinfo.output_scanline as the
         * loop counter, so that we don't have to keep track ourselves.
         */
        int index = 0;
        while (cinfo.output_scanline < cinfo.output_height) {
            /* jpeg_read_scanlines expects an array of pointers to scanlines.
             * Here the array is only one element long, but you could ask for
             * more than one scanline at a time if that's more convenient.
             */
            (void) jpeg_read_scanlines(&cinfo, buffer, 1);
            /* Assume put_scanline_someplace wants a pointer and sample count. */
            index = storeRawData(buffer[0], row_stride, index);
        }

        /* Step 7: Finish decompression */

        (void) jpeg_finish_decompress(&cinfo);
        /* We can ignore the return value since suspension is not possible
         * with the stdio data source.
         */

        /* Step 8: Release JPEG decompression object */

        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_decompress(&cinfo);
    }

    /* After finish_decompress, we can close the input file.
     * Here we postpone it until after no more JPEG errors are possible,
     * so as to simplify the setjmp error logic above.  (Actually, I don't
     * think that jpeg_destroy can do an error exit, but why assume anything...)
     */
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
     * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
     */

    /* And we're done! */
    return result;
}

int JPEGImage::saveImage(const char *path, int quality) {
    if (mRawData == nullptr) {
        return NO_DATA;
    }

    //moreless copy&paste of example.c from libjpeg sourcecode
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE *outfile;
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;        /* physical row width in image buffer */

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(path, "wb")) == NULL) {
        LOGE("can't open %s", path);
        return CANT_OPEN_FILE;
    }

    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    ImageMetaData metaData = getMetaData();
    cinfo.image_width = (JDIMENSION) metaData.imageWidth;/* image width and height, in pixels */
    cinfo.image_height = (JDIMENSION) metaData.imageHeight;
    cinfo.input_components = 3;        /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;    /* colorspace of input image */


    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = metaData.imageWidth * 3;    /* JSAMPLEs per row in image_buffer */

    unsigned char tmp[row_stride];
    int iPixel = 0;
    while (cinfo.next_scanline < cinfo.image_height) {
        //convert back our internal bitmap format to jpeg format
        for (int i = 0; i < row_stride; i++) {
            int px = mRawData[iPixel++];
            tmp[i] = (unsigned char) (px);
            tmp[++i] = (unsigned char) (px >> 8);
            tmp[++i] = (unsigned char) (px >> 16);
        }
        row_pointer[0] = &tmp[0];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
    return NO_ERR;
}

int JPEGImage::storeRawData(JSAMPROW row, int stride, int pixelIndex) {
    unsigned char a, b, c;
    int i = 0;
    while (i < stride) {
        a = row[i++];
        b = row[i++];
        c = row[i++];
        mRawData[pixelIndex++] = 0xFF000000 | a << 0 | b << 8 | c << 16;
    }
    return pixelIndex;
}


