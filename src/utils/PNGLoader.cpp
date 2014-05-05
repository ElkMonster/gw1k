#include "utils/PNGLoader.h"

#include "Log.h"


//#define GW1K_ENABLE_GL_ERROR_CHECKS
//#include "GLErrorCheck.h"

#include <png.h>
#include <cstdio>
#include <iostream>
#include <cstring>


namespace gw1k
{


bool loadPngImage(const char* filename, int& width, int& height, bool& alpha, unsigned char*& data)
{
    png_structp pngPtr;
    png_infop pngInfoPtr;

    FILE* pFile = fopen(filename, "rb");

    if (!pFile)
    {
        Log::warning("PNGLoader", Log::os()
            << "Could not open file " << filename);
        return false;
    }

    // Check if we have a valid PNG file by reading the first eight bytes
    unsigned char header[8];
    fread(header, 1, 8, pFile);
    if (png_sig_cmp(header, 0, 8) != 0)
    {
        Log::warning("PNGLoader", Log::os()
            << "Invalid PNG signature in " << filename);
        return false;
    }

    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr)
    {
        Log::warning("PNGLoader", Log::os()
            << "Failed to set up image data of " << filename);
        fclose(pFile);
        return false;
    }

    pngInfoPtr = png_create_info_struct(pngPtr);
    if (!pngInfoPtr)
    {
        Log::warning("PNGLoader", Log::os()
            << "Failed to set up image info of " << filename);
        fclose(pFile);
        png_destroy_read_struct(&pngPtr, NULL, NULL);
        return false;
    }

    if (setjmp(png_jmpbuf(pngPtr))) {
        Log::warning("PNGLoader", Log::os()
            << "Error processing image " << filename);
        fclose(pFile);
        png_destroy_read_struct(&pngPtr, &pngInfoPtr, NULL);
        return false;
    }

    // Do standard initialisation as we will use libpng's built-in image reading
    // capabilities (which use standard FILE* IO methods)
    png_init_io(pngPtr, pFile);

    // Signal that we have already read eight bytes from the stream for the PNG
    // header comparison above
    png_set_sig_bytes(pngPtr, 8);

    // Standard transforms, see URL provided above
    int pngTransforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND;
    png_read_png(pngPtr, pngInfoPtr, pngTransforms, NULL);

    width = png_get_image_width(pngPtr, pngInfoPtr);
    height = png_get_image_height(pngPtr, pngInfoPtr);
    png_byte colorType = png_get_color_type(pngPtr, pngInfoPtr);
    switch (colorType)
    {
        case PNG_COLOR_TYPE_RGBA:
            alpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            alpha = false;
            break;
        default:
            Log::warning("PNGLoader", Log::os()
                << "Unsupported colour type " << colorType
                << " in file " << filename);
            png_destroy_read_struct(&pngPtr, &pngInfoPtr, NULL);
            fclose(pFile);
            return false;
    }

    size_t rowSize = png_get_rowbytes(pngPtr, pngInfoPtr);

    // Allocate enough space to fit in the image's pixel data; this memory must
    // be release manually later by the caller
    data = new unsigned char[rowSize * height];

    png_bytepp pRows = png_get_rows(pngPtr, pngInfoPtr);

    // Copy pixel data to our target memory; invert row order so to fit OpenGL's
    // default coordinate system (bottom-left origin)
    for (int i = 0; i != height; ++i) {
        memcpy(&data[rowSize * (height - 1 - i)], pRows[i], rowSize);
    }

    // Cleanup
    png_destroy_read_struct(&pngPtr, &pngInfoPtr, NULL);
    fclose(pFile);

    return true;
}


bool loadPngTexture(const char* filename, int& width, int& height, bool& alpha, unsigned char*& tex)
{
    if (loadPngImage(filename, width, height, alpha, tex))
    {
        return true;
    }
    else
    {
        width = 0;
        height = 0;
        alpha = false;
        tex = 0;
        return false;
    }
}


} // namespace gw1k
