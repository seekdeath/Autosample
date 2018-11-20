void XYZReset(void);
void XYSetPos(void);
void Z1Z2SetPos(void);
void TestLevel(void);
void HandleXYZ(void);
u8 IS_XYZ_R_FLAG(void);
void Set_XYZ_R_FLAG(void);
void Reset_XYZ_R_FLAG(void);
u8 IS_XYZ_XY_FLAG(void);
void Set_XYZ_XY_FLAG(void);
void Reset_XYZ_XY_FLAG(void);
u8 IS_XYZ_Z1Z2_FLAG(void);
void Set_XYZ_Z1Z2_FLAG(void);
void Reset_XYZ_Z1Z2_FLAG(void);
u8 IS_XYZ_TL_FLAG(void);
void Set_XYZ_TL_FLAG(void);
void Reset_XYZ_TL_FLAG(void);
void Set_XYZ_Pos(s32 XPos,s32 YPos);
void Get_XYZ_Pos(s32* XPos,s32* YPos);
void SetZ1Z2Pos(u8 Z1Z2Flag,s32 Z1Pos,s32 Z2Pos);
void GetZ1Z2Pos(u8* Z1Z2Flag,s32* Z1Pos,s32* Z2Pos);
void ReadXYZPos(s32* XPos,s32* YPos,s32* Z1Pos,s32* Z2Pos);
u8 IS_XYZ_Cancel(void);
void Set_XYZ_Cancel(void);
void Reset_XYZ_Cancel(void);
void InitXYZMotor(void);
