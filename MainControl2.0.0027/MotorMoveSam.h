// #include <includes.h>
// #include <MotorTrasmition.h>
// #include <Motor.h>
// #include <MotionErr.h>
// #include <MotorMoveSam.h>
void resetMove1Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move1SetPos(void);


void InitMove1Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove1(void);

// u8 Move1_Cancel_FLAG=0;
u8 IS_Move1_Cancel_FLAG(void);
void Set_Move1_Cancel_FLAG(void);
void Reset_Move1_Cancel_FLAG(void);
//复位标志
// u8 Move1_R_FLAG=0;
//复位标志有效

u8 IS_Move1_R_FLAG(void);
u8 IS_Move2_R_FLAG(void);
u8 IS_Move3_R_FLAG(void);
u8 IS_Move4_R_FLAG(void);
u8 GET_Move_R_FLAG(void);
//设定复位标志有效
void SET_Move1_R_FLAG(void);
//取消复位标志
void RESET_Move1_R_FLAG(void);
//移动标志�?// u8 Move1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move1_MOVE_FLAG(void);
//置位移动标志�?
void SET_Move1_MOVE_FLAG(void);
//复位移动标志�?
void RESET_Move1_MOVE_FLAG(void);
//标志
// u8 Move1_FLAG;
//坐标
// s32 Move1_POS;
//获取标志，坐�?
void GetMove1Pos(u8* Move1FLAG,s32* Pos);
//设定标志，坐�?
void SetMove1Pos(u8 Move1FLAG,s32 Pos);
////////////////////Move2
void resetMove2Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move2SetPos(void);


void InitMove2Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove2(void);

// u8 Move2_Cancel_FLAG=0;
u8 IS_Move2_Cancel_FLAG(void);
void Set_Move2_Cancel_FLAG(void);
void Reset_Move2_Cancel_FLAG(void);
//复位标志
// u8 Move2_R_FLAG=0;
//复位标志有效
u8 GET_Move2_R_FLAG(void);
//设定复位标志有效
void SET_Move2_R_FLAG(void);
//取消复位标志
void RESET_Move2_R_FLAG(void);
//移动标志�?// u8 Move2_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move2_MOVE_FLAG(void);
//置位移动标志�?
void SET_Move2_MOVE_FLAG(void);
//复位移动标志�?
void RESET_Move2_MOVE_FLAG(void);

//标志
// u8 Move2_FLAG;
//坐标
// s32 Move2_POS;
//获取标志，坐�?
void GetMove2Pos(u8* Move2FLAG,s32* Pos);
//设定标志，坐�?
void SetMove2Pos(u8 Move2FLAG,s32 Pos);
////////////////////Move3
void resetMove3Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move3SetPos(void);


void InitMove3Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove3(void);

// u8 Move3_Cancel_FLAG=0;
u8 IS_Move3_Cancel_FLAG(void);
void Set_Move3_Cancel_FLAG(void);
void Reset_Move3_Cancel_FLAG(void);
//复位标志
// u8 Move3_R_FLAG=0;
//复位标志有效
u8 GET_Move3_R_FLAG(void);
//设定复位标志有效
void SET_Move3_R_FLAG(void);
//取消复位标志
void RESET_Move3_R_FLAG(void);
//移动标志�?// u8 Move3_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move3_MOVE_FLAG(void);
//置位移动标志�?
void SET_Move3_MOVE_FLAG(void);
//复位移动标志�?
void RESET_Move3_MOVE_FLAG(void);

//标志
// u8 Move3_FLAG;
//坐标
// s32 Move3_POS;
//获取标志，坐�?
void GetMove3Pos(u8* Move3FLAG,s32* Pos);
//设定标志，坐�?
void SetMove3Pos(u8 Move3FLAG,s32 Pos);
////////////////////Move4
void resetMove4Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move4SetPos(void);


void InitMove4Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove4(void);

// u8 Move4_Cancel_FLAG=0;
u8 IS_Move4_Cancel_FLAG(void);
void Set_Move4_Cancel_FLAG(void);
void Reset_Move4_Cancel_FLAG(void);
//复位标志
// u8 Move4_R_FLAG=0;
//复位标志有效
u8 GET_Move4_R_FLAG(void);
//设定复位标志有效
void SET_Move4_R_FLAG(void);
//取消复位标志
void RESET_Move4_R_FLAG(void);
//移动标志�?// u8 Move4_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move4_MOVE_FLAG(void);
//置位移动标志�?
void SET_Move4_MOVE_FLAG(void);
//复位移动标志�?
void RESET_Move4_MOVE_FLAG(void);

//标志
// u8 Move4_FLAG;
// //坐标
// s32 Move4_POS;
//获取标志，坐�?
void GetMove4Pos(u8* Move4FLAG,s32* Pos);
//设定标志，坐�?
void SetMove4Pos(u8 Move4FLAG,s32 Pos);

void move1Work(void);
void move2Work(void);
void move3Work(void);
void move4Work(void);