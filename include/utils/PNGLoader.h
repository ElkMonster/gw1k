#ifndef GW1K_PNGLOADER_H_
#define GW1K_PNGLOADER_H_


namespace gw1k
{

bool loadPngTexture(const char* filename, int& width, int& height, bool& alpha, unsigned char*& tex);

bool loadPngImage(const char* filename, int& width, int& height, bool& alpha, unsigned char*& data);


} // namespace gw1k

#endif // GW1K_PNGLOADER_H_
