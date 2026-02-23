/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        systick simulator.c
 * Description:  
 *
 * $Date:        15 February 2023
 * $Revision:    V0.0.1
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <stdint.h>

extern void SysTickSetup (void);
extern void Graph_SysTick_Handler (void);

/* ============================================  TIME  ============================================== */

#define TIME_BASE_1MS
#define PROCESSOR_CLOCK 350000000L
static volatile uint64_t graph_interpreter_time64;              /* one global variable */

/*  graph_interpreter_time64" using a global variable
 *  FEDCBA987654321 FEDCBA987654321 FEDCBA987654321 FEDCBA9876543210
 *  ____ssssssssssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqq q32.28 [s]  140 Y + Q28 [s]
 *
 *  increments for  1ms systick =  0x00041893 =  1ms x 2^28
 *  increments for 10ms systick =  0x0028F5C2 = 10ms x 2^28
 */

#ifdef TIME_BASE_1MS
#define SYSTICK_LOAD_1MS (PROCESSOR_CLOCK/1000)
#define GRAPHTIME64INC 0x00041893L
#define TIME_BASE 0.001
#endif
#ifdef TIME_BASE_10MS
#define SYSTICK_LOAD_1MS (PROCESSOR_CLOCK/100)
#define GRAPHTIME64INC 0x0028F5C2L
#define TIME_BASE 0.01
#endif

#define INV_RVR (0x100000000/GRAPHTIME64INC)

/* ================================================================================================== */
clock_t SysTick_previous_time;

/* ============================================  TIME  ============================================== */
/*  SysTick IRQ handler
 */
extern void graph_systick_scheduler(uint64_t time64);
extern void Graph_SysTick_Handler (void);
       void Graph_SysTick_Handler (void)
{
    extern void graph_test_scheduler(uint64_t time64);
    clock_t current_time;
    float delta_t;

    current_time = clock();
    delta_t = (float)(current_time - SysTick_previous_time)/CLOCKS_PER_SEC;
    
    if (delta_t > TIME_BASE)
    {   graph_interpreter_time64 = graph_interpreter_time64 + GRAPHTIME64INC;
        graph_test_scheduler(graph_interpreter_time64);
        SysTick_previous_time = current_time;
    }
}


extern void SysTickSetup (void);
       void SysTickSetup (void)
{   
    SysTick_previous_time = clock();
    graph_interpreter_time64  = 0U;     // graph interpreter global counter
}
/* ================================================================================================= */
