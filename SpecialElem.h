#ifndef SPECIALELEM_H
#define SPECIALELEM_H

int ImgJudgeCircle(int type);
void ImgJudgeStopLine(void);
void ImgJudgeRamp(void);
void ImgJudgeCurveBroken(void);
void ImgJudgeStraightBroken(void);
void ImgJudgeObstacle(void);

void SpecialElemFill(void);
int ImgJudgeSpecialLine(int left_line, int left_col, int right_line, int right_col, int type, int start_line);
int ImgJudgeOutBroken(void);
int ImgIsStopLine(int line, int left, int right);
int DistStopLine(int line);
int GetSpeedRow(int middle, int left_row, int right_row);
int GetSpeedCount(int row, int speed_max, int speed_min, float a);
#endif // !SPECIALELEM_H
#pragma once
