#ifndef CANNY_H
#define CANNY_H

void CannyEage(void);
int GetAveGray(int row);
void ShowEage(unsigned char *p);
void qCannyEage(void);


typedef int uint8;

void SpecialColorPuls();
void CannyEdgeTest(unsigned char org[IMG_ROWS][IMG_COLS], uint8 lowThr);
float Fsqrt(float x);
void Quicksort(float array[], uint8 maxlen, uint8 begin, uint8 end);
void Swap(float* a, float* b);
uint8 Atan2(float y, float x);
#endif // !CANNY_H
