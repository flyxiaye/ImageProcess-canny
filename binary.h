#ifndef BINARY_H
#define BINARY_H

void gray2Binary(unsigned char* srcImage, unsigned char* dstImage);
//unsigned char otsuThreshold(unsigned char* image, int col, int row);
void lowLight(unsigned char* srcImage, int start_row, int start_col, int height, int width, int diff_light);
#endif // !BINARY_H
