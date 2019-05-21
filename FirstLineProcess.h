#ifndef FIRSTLINEPROCESS_H
#define FIRSTLINEPROCESS_H

void GetFirstLineEage(void);
char otsuThreshold(unsigned char *image, int col, int row);
void FindFirstEage(void);
int IsFirstEage(int col);
unsigned char GetBinary(unsigned char *FirstLine, int col);


#endif // !FIRSTLINEPROCESS_H
#pragma once
