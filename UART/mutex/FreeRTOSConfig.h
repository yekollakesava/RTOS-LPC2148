#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* CPU / Tick */
#define configCPU_CLOCK_HZ              ( ( unsigned long ) 60000000UL )
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )
#define configUSE_PREEMPTION            1
#define configUSE_TIME_SLICING          1

/* Memory */
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 12 * 1024 ) )

/* Task settings */
#define configMAX_PRIORITIES            5
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN         12

/* Hooks */
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0

/* Mutex/Semaphores (IMPORTANT for your error) */
#define configUSE_MUTEXES               1
#define configUSE_COUNTING_SEMAPHORES   1

/* Optional features */
#define configUSE_TRACE_FACILITY        0
#define configUSE_16_BIT_TICKS          0

/* API include */
#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskSuspend            1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskPrioritySet        1

#endif