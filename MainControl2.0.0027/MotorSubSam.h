// #include <includes.h>
// #include <MotorTrasmition.h>
// #include <Motor.h>
// #include <MotionErr.h>
// #include <MotorSubSam.h>
void resetSubMotor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void SubSetPos(void);


void InitSubMotor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleSub(void);

// u8 Sub_Cancel_FLAG=0;
u8 IS_Sub_Cancel_FLAG(void);
void Set_Sub_Cancel_FLAG(void);
void Reset_Sub_Cancel_FLAG(void);
//复位标志
// u8 Sub_R_FLAG=0;
//复位标志有效
u8 IS_Sub_R_FLAG(void);
u8 GET_Sub_R_FLAG(void);
//设定复位标志有效
void SET_Sub_R_FLAG(void);
//取消复位标志
void RESET_Sub_R_FLAG(void);
//移动标志�?// u8 Sub_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Sub_MOVE_FLAG(void);
//置位移动标志�?
void SET_Sub_MOVE_FLAG(void);
//复位移动标志�?
void RESET_Sub_MOVE_FLAG(void);

//标志
// u8 Sub_FLAG;
//坐标
// s32 Sub_POS;
//获取标志，坐�?
void GetSubPos(u8* SubFLAG,s32* Pos);
//设定标志，坐�?
void SetSubPos(u8 SubFLAG,s32 Pos);

void subWork(void);