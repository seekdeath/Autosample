void CloseMagnetic(u8 Channel);
void HookReset(void);
void HookMove(void);
void HandleHook(void);
u8 IS_Hook_R_FLAG(void);
void Set_Hook_R_FLAG(void);
void Reset_Hook_R_FLAG(void);
u8 IS_Hook_Cancel(void);
void Set_XYZ_Cancel(void);
void Reset_XYZ_Cancel(void);
u8 IS_Hook_FLAG(void);
void Set_Hook_FLAG(void);
void Reset_Hook_FLAG(void);
void SetHookParam(u8 flag,s16 Pos_x,s16 Pos_y,u8 Pos_z);
void GetHookParam(u8* flag,s16* Pos_x,s16* Pos_y,u8* Pos_z);
u8  IS_Hook_Cancel_FLAG(void);
void Set_Hook_Cancel_FLAG(void);
void Reset_Hook_Cancel_FLAG(void);
void Init_HookMagnetic(void);
u8 IS_BEEP_CON(void);
void SET_BEEP_CON(void);
void RESET_BEEP_CON(void);
void GetBeepParam(u16 *OnTime,u16 *OffTime);
void SetBeepParam(u16 OnTime,u16 OffTime);
u8 GetMagneticSelect(void);
void SetMagneticSelect(u8 flag);
u8 GetMagneticState(void);
void SetMagneticState(u8 flag);
u8 IS_MAGNETIC_ACT(void);
void SET_MAGNETIC_ACT(void);
void RESET_MAGNETIC_ACT(void);
void Init_BeepGPIO(void);
void BeepControl(void);
void MagneticControl(void);
void InitHookMotor(void);

