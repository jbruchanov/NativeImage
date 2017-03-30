//
// Created by jbruchanov on 29/03/17.
//

#include "stdio.h"
#include <setjmp.h>
#include "JPEGImage.hpp"
#include "Errors.h"
#include "LogHelper.h"
#include "cstring"

#define NUM_COLORS 3

struct _Error {
    struct jpeg_error_mgr pub;    /* "public" fields */
    jmp_buf setjmp_buffer;    /* for return to caller */
};
typedef struct _Error *_ErrorPtr;

ImageMetaData JPEGImage::getMetaData() {
    return mMetaData;
}

JPEGImage::JPEGImage() {
    mMetaData = {};
    mRawData = 0;
}

JPEGImage::~JPEGImage() {
    releaseRawData();
}

void JPEGImage::releaseRawData() {
    if (mRawData != 0) {
        delete[] mRawData;
    }
    mRawData = nullptr;
}

string JPEGImage::getAndClearLastError() {
    string err = string(mLastError);
    int len = sizeof(mLastError);
    memset(mLastError, '\0', len);
    return err;
}

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

    mMetaData.imageWidth = cinfo.image_width;
    mMetaData.imageHeight = cinfo.image_height;
    mMetaData.componentSize = cinfo.num_components;
    mRawData = new int[mMetaData.imageWidth * mMetaData.imageHeight];

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
     * jpeg_read_header(), so we do nothing here.
     */

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
        storeRawData(buffer[0], row_stride, cinfo.num_components, &index);
    }

    /* Step 7: Finish decompression */

    (void) jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
     * with the stdio data source.
     */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

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
    return NO_ERR;
}

void JPEGImage::storeRawData(JSAMPROW row, int stride, int rawDataIndex, int *pInt) {
    for (int i = 0; i < stride; i += NUM_COLORS) {
        unsigned char a, b, c;
        a = row[i + 0];
        b = row[i + 1];
        c = row[i + 2];
        mRawData[*pInt] = 0xFF000000 | a << 0 | b << 8 | c << 16;
        (*pInt)++;
    }
}

RawData JPEGImage::getRawData() {
    RawData r;
    r.rawData = mRawData;
    r.size = mMetaData.imageWidth * mMetaData.imageHeight;
    return r;
}

void JPEGImage::setPixels(int *target) {
    if (mRawData != NULL && target != NULL) {

    }
}
