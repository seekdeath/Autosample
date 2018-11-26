// #include <includes.h>
// #include <MotorTrasmition.h>
// #include <Motor.h>
// #include <MotionErr.h>

void resetAdd1Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Add1SetPos(void);


void InitAdd1Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleAdd1(void);

// u8 Add1_Cancel_FLAG=0;
u8 IS_Add1_Cancel_FLAG(void);
void Set_Add1_Cancel_FLAG(void);
void Reset_Add1_Cancel_FLAG(void);
//复位标志
// u8 Add1_R_FLAG=0;
//复位标志有效
u8 IS_Add1_R_FLAG(void);
u8 GET_Add1_R_FLAG(void);
//设定复位标志有效
void SET_Add1_R_FLAG(void);
//取消复位标志
void RESET_Add1_R_FLAG(void);
//移动标志位
// u8 Add1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Add1_MOVE_FLAG(void);
//置位移动标志位
void SET_Add1_MOVE_FLAG(void);
//复位移动标志位
void RESET_Add1_MOVE_FLAG(void);

//标志
// u8 Add1_FLAG;
//坐标
// s32 Add1_POS;
//获取标志，坐标
void GetAdd1Pos(u8* Add1FLAG,s32* Pos);
//设定标志，坐标
void SetAdd1Pos(u8 Add1FLAG,s32 Pos);
///////////////////AddS2
void resetAdd2Motor();
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Add2SetPos(void);


void InitAdd2Motor(void);

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleAdd2(void);

// u8 Add2_Cancel_FLAG=0;
u8 IS_Add2_Cancel_FLAG(void);
void Set_Add2_Cancel_FLAG(void);
void Reset_Add2_Cancel_FLAG(void);
//复位标志
// u8 Add2_R_FLAG=0;
//复位标志有效
u8 IS_Add2_R_FLAG(void);
u8 GET_Add2_R_FLAG(void);
//设定复位标志有效
void SET_Add2_R_FLAG(void);
//取消复位标志
void RESET_Add2_R_FLAG(void);
//移动标志位
// u8 Add2_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Add2_MOVE_FLAG(void);
//置位移动标志位
void SET_Add2_MOVE_FLAG(void);
//复位移动标志位
void RESET_Add2_MOVE_FLAG(void);

//标志
// u8 Add2_FLAG;
//坐标
// s32 Add2_POS;
//获取标志，坐标
void GetAdd2Pos(u8* Add2FLAG,s32* Pos);
//设定标志，坐标
void SetAdd2Pos(u8 Add2FLAG,s32 Pos);