/*
	主程序
*/

#include "stm32f4xx.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "button.h"
#include "Screenlogic.h"



void Mian_loop(void)
{
    Fan_Main();
	Screen_Main();
	Speak_Main();
	USART_main();	
	Button_Main();	
	Usb_Main();
	Slave_Main();
	Print_Main();
    AT_main();
	APP_main();
	Battery_Main();	
	Flashisp_main();
	ProcessGetMinuteData();
	Flux_Main();
	LTC2944_Main();
	DS18B20_Main();
	IWDG_ReloadCounter();//喂狗

}

int main(void)
{
	
	SCB->VTOR = FLASH_BASE | 0x40000; //SCB->VTOR = FLASH_BASE | 0x40000;              //带升级语句
//	SystemInit();	
	SystemCoreClockUpdate();
	Project_Init();		//项目各项初始化
	//Screen_GetData()->RunMode=0x8100;
	for( ;; )
	{
		Mian_loop();     
	}
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

