/* 开发板硬件相关头文件 */
 #include "board.h" 

/* RT-Thread 相关头文件 */
#include <rthw.h>
#include <rtthread.h>
#if 0
/*========================================================*/

/* 内核外设 NVIC 相关的寄存器定义 */
#define _SCB_BASE (0xE000E010UL)
#define _SYSTICK_CTRL (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI (*(rt_uint8_t *)(0xE000ED23UL))

 /* 外部时钟和函数声明 */
extern void SystemCoreClockUpdate(void);
extern uint32_t SystemCoreClock;

 /* 系统定时器 SysTick 初始化 */
static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
if ((ticks - 1) > 0xFFFFFF)
{
return 1;
}

_SYSTICK_LOAD = ticks - 1;
_SYSTICK_PRI = 0xFF;
_SYSTICK_VAL = 0;
_SYSTICK_CTRL = 0x07;

return 0;
}
/*=====================================================*/
#endif

 #if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
 #define RT_HEAP_SIZE 1024
 /* 从内部 SRAM 里面分配一部分静态内存来作为 rtt 的堆空间，这里配置为 4KB */
static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void *rt_heap_begin_get(void)
{
return rt_heap;
}
RT_WEAK void *rt_heap_end_get(void)
{
return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
* @brief 开发板硬件初始化函数
* @param 无
* @retval 无
*
* @attention
* RTT 把开发板相关的初始化函数统一放到 board.c 文件中实现，
* 当然，你想把这些函数统一放到 main.c 文件也是可以的。
*/
void rt_hw_board_init()
{
#if 0 
/* 更新系统时钟 */
SystemCoreClockUpdate();
/* SysTick 初始化 */
SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
#endif

 /* 硬件 BSP 初始化统统放在这里，比如 LED，串口， LCD 等 */

 /* 调用组件初始化函数 (use INIT_BOARD_EXPORT()) */
 #ifdef RT_USING_COMPONENTS_INIT
 rt_components_board_init();
 #endif

 #if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
 rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
 #endif

 #if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
 rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
 #endif
 }

 /**
 * @brief SysTick 中断服务函数
 * @param 无
 * @retval 无
 *
 * @attention
 * SysTick 中断服务函数在固件库文件 stm32f10x_it.c 中也定义了，而现在
 * 在 board.c 中又定义一次，那么编译的时候会出现重复定义的错误，解决
 * 方法是可以把 stm32f10x_it.c 中的注释或者删除即可。
 */
 void SysTick_Handler(void)
 {
 /* 进入中断 */
 rt_interrupt_enter();

 /* 更新时基 */
 rt_tick_increase();
 Project_Timer();
 /* 离开中断 */
 rt_interrupt_leave();
 }
