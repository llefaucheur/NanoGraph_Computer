/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        main.c
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

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern void main_init(uint32_t *graph);
extern int main_run(void);
extern void main_stop(void);
extern uint8_t one_file_is_closed;


int main(void)
{   
    /* here the flag GRAPH_FROM_PLATFORM is "defined" in top_manifest.h
       otherwise the application share the graph with main_init(graph);
    */
    main_init(0);

    /* interpret the graph until no more data is available */
    while (0 == one_file_is_closed)
    {   extern void Graph_SysTick_Handler (void);
        Graph_SysTick_Handler();
        main_run();
    }

    /* release memory */
    main_stop();
}

