#ifndef SPECIALELEM_H
#define SPECIALELEM_H

int ImgJudgeCircle(int type);
void ImgJudgeStopLine(void);
void ImgJudgeRamp(void);
void ImgJudgeCurveBroken(void);
void ImgJudgeStraightBroken(void);
void ImgJudgeObstacle(void);

void SpecialElemFill(void);
int ImgJudgeSpecialElem(int left_line, int right_line);
int ImgJudgeSpecialLine(int left_line, int left_col, int right_line, int right_col, int type);
int IsBlock(int left_line, int right_line);
int IsRamp(void);
int ImgJudgeOutBroken(void);
int IsStopLine(int line, int left, int right);
int DistStopLine(int line);
int JudgeInBroken(Point pa, Point pb);
int JudgeInBroken_V2(Point pa, Point pb);
int IsConnectPoint(Point pa, Point pb);
int GetSpeedRow(int middle, int left_row, int right_row);
int GetSpeedCount(int row, int speed_max, int speed_min, float a);
#endif // !SPECIALELEM_H
#pragma once
