#ifndef FillSpecialLine_H
#define FillSpecialLine_H


int FillLineUp(int type, int line);
int FillLineDown(int type, int line);
int FillLineDownK(int type, int line, int end_line);
int FillLinePnt(int type, int line, int end_line);
int FillLevelCross_4(void);
int FillLevelCross(void);
int FillBevelCross(void);
void FillMiddleLine(void);
void StopFill(void);
#endif // !FillSpecialLine_H
#pragma once
