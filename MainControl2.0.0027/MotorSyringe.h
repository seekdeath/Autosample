void SyringeReset(void);
void SyringeSetPos(void);
void HandleSyringe(void);
u8 IS_Syringe_R_FLAG(void);
void SET_Syringe_R_FLAG(void);
void RESET_Syringe_R_FLAG(void);
u8 IS_Syringe_MOVE_FLAG(void);
void SET_Syringe_MOVE_FLAG(void);
void RESET_Syringe_MOVE_FLAG(void);
u8 IS_Syringe_Cancel_FLAG(void);
void Set_Syringe_Cancel_FLAG(void);
void Reset_Syringe_Cancel_FLAG(void);
void InitSyringeMotor(void);
void GetSyringePos(u8* SyringeFLAG,s32* Pos, u16 *Per);
void SetSyringePos(u8 SyringeFLAG,s32 Pos,u16 Per);
