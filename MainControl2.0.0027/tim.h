#include <stm32f10x_tim.h>

void TIM_Configuration1(void);
void TIM_Configuration2(void);
void TIM_Configuration3(void);
void TIM_Configuration8(void);
void TIM_Configuration4(void);
void Set_PWM           (unsigned char channel,TIM_TypeDef* TIMx, u16 Compare);
void Feq_ini           (void);
void Handle_PWM        (int counter);
void PWM_ALL_HIGH      (void);
void Fresh_PWM         (float ch1,float ch2,float ch3,float ch4,float feq);
void Control_Tempriture(float Propor,unsigned char channel);
void PwmIni            (void);
void Fresh_Channel     (float data,float feq,unsigned char channel);
void StopPending       (void);
void StartPending      (float freq);
