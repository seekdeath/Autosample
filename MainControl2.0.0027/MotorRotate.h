// #include <includes.h>
// #include <MotorTrasmition.h>
// #include <Motor.h>
// #include <MotionErr.h>

//void resetAddMotor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void AddRotatePos(void);


void InitRotateMotor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleRotate(void);

// u8 Add1_Cancel_FLAG=0;
u8 IS_Rotate_Cancel_FLAG(void);
void Set_Rotate_Cancel_FLAG(void);
void Reset_Rotate_Cancel_FLAG(void);
//复位标志
// u8 Add1_R_FLAG=0;
//复位标志有效
u8 IS_Rotate_R_FLAG(void);
u8 GET_Rotate_R_FLAG(void);
//设定复位标志有效
void SET_Rotate_R_FLAG(void);
//取消复位标志
void RESET_Rotate_R_FLAG(void);
//移动标志位
// u8 Add1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Rotate_MOVE_FLAG(void);
//置位移动标志位
void SET_Rotate_MOVE_FLAG(void);
//复位移动标志位
void RESET_Rotate_MOVE_FLAG(void);

//标志
// u8 Add1_FLAG;
//坐标
// s32 Add1_POS;
//获取标志，坐标
void GetRotatePos(u8* RotateFLAG,s32* Pos);
//设定标志，坐标
void SetRotatePos(u8 RotateFLAG,s32 Pos);


