/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        platform_io_services.c
 * Description:  abstraction layer to BSP and streams from the application
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

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define DATA_FROM_FILES 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../nanograph_common.h"
#include "../nanograph_interpreter.h"
#include "../top_manifest_included.h"
#ifdef __cplusplus
 extern "C" {
#endif


/*-----------------------------------------------------------------------*/

extern uint8_t one_file_is_closed;         /* flag used to exit */

extern void NanoGraph_io_ack (uint8_t HW_io_idx, void *data, uintptr_t size);

/*
 * NULL TASK
 */
extern void NanoGraph_null_task(int32_t c, nanograph_handle_t i, void* d, uint32_t* s);
void NanoGraph_null_task(int32_t c, nanograph_handle_t i, void* d, uint32_t* s) {}


/* 
    IO interfaces functions to the possible streams of the platform
*/
  extern void data_sink           (uint32_t, nanograph_xdmbuffer_t *);  //  0
  extern void io_data_in_0        (uint32_t, nanograph_xdmbuffer_t *);  //  1
  extern void io_data_in_1        (uint32_t, nanograph_xdmbuffer_t *);  //  2
  extern void io_data_out_0       (uint32_t, nanograph_xdmbuffer_t *);  //  3
  extern void io_data_out_1       (uint32_t, nanograph_xdmbuffer_t *);  //  4
  extern void io_sensor_in_0      (uint32_t, nanograph_xdmbuffer_t *);  //  5
//extern void io_sensor_in_1      (uint32_t, nanograph_xdmbuffer_t *);  //  6
//extern void io_sensor_in_2      (uint32_t, nanograph_xdmbuffer_t *);  //  7
//extern void io_sensor_in_3      (uint32_t, nanograph_xdmbuffer_t *);  //  8
  extern void io_timer_0          (uint32_t, nanograph_xdmbuffer_t *);  //  9
//extern void io_timer_1          (uint32_t, nanograph_xdmbuffer_t *);  // 10
  extern void io_ui_in_0          (uint32_t, nanograph_xdmbuffer_t *);  // 11
//extern void io_ui_in_1          (uint32_t, nanograph_xdmbuffer_t *);  // 12
//extern void io_ui_in_2          (uint32_t, nanograph_xdmbuffer_t *);  // 13
//extern void io_ui_in_3          (uint32_t, nanograph_xdmbuffer_t *);  // 14
  extern void io_ui_out_0         (uint32_t, nanograph_xdmbuffer_t *);  // 15
//extern void io_ui_out_1         (uint32_t, nanograph_xdmbuffer_t *);  // 16
//extern void io_ui_out_2         (uint32_t, nanograph_xdmbuffer_t *);  // 17
//extern void io_ui_out_3         (uint32_t, nanograph_xdmbuffer_t *);  // 18
  extern void io_serial_in_0      (uint32_t, nanograph_xdmbuffer_t *);  // 19
//extern void io_serial_out_0     (uint32_t, nanograph_xdmbuffer_t *);  // 20
  extern void io_analog_in_0      (uint32_t, nanograph_xdmbuffer_t *);  // 21
//extern void io_analog_out_0     (uint32_t, nanograph_xdmbuffer_t *);  // 22
  extern void io_audio_in_0       (uint32_t, nanograph_xdmbuffer_t *);  // 23
//extern void io_audio_in_1       (uint32_t, nanograph_xdmbuffer_t *);  // 24
//extern void io_audio_in_2       (uint32_t, nanograph_xdmbuffer_t *);  // 25
  extern void io_audio_out_0      (uint32_t, nanograph_xdmbuffer_t *);  // 26
//extern void io_audio_out_1      (uint32_t, nanograph_xdmbuffer_t *);  // 27
//extern void io_audio_out_2      (uint32_t, nanograph_xdmbuffer_t *);  // 28
  extern void io_2d_in_0          (uint32_t, nanograph_xdmbuffer_t *);  // 30
//extern void io_2d_in_1          (uint32_t, nanograph_xdmbuffer_t *);  // 31
//extern void io_2d_out_0         (uint32_t, nanograph_xdmbuffer_t *);  // 32
//extern void io_2d_out_1         (uint32_t, nanograph_xdmbuffer_t *);  // 33


/* --------------------------------------------------------------------------------------- 
    FW IO FUNCTIONS
*/

FILE* ptf_data_sink;            // 0
FILE* ptf_data_in_0;            // 1
FILE* ptf_data_out_0;           // 3
FILE* ptf_sensor_0;             // 5
FILE* ptf_ui_out_0;             // 15
FILE* ptf_analog_0;             // 21
FILE* ptf_audio_in_0;           // 23
FILE* ptf_debug_detector;       


#define size_data_sink 16
static int16_t buffer_data_sink[size_data_sink / sizeof(int16_t)];

#define size_data_out_0 32
static int16_t buffer_data_out_0[size_data_out_0 / sizeof(int16_t)];        // 3    general purpose and links to the application ("general" domain)

#define size_sensor_in_0 32
static int16_t buffer_sensor_in_0[size_sensor_in_0 / sizeof(int16_t)];      // 5    motion, temperature, proximity, CO2, PIR ("motion" or "analog_in" or "gpio" domains)

#define size_timer_0 4
static int16_t buffer_timer_0[size_timer_0 / sizeof(int32_t)];              // 9    timer at 100ms, 1s ("timer" domain)

#define size_ui_in_0 4
static int16_t buffer_ui_in_0[size_ui_in_0 / sizeof(int32_t)];              // 11   buttons, slider ("user_interface" domain)

#define size_io_ui_out_0 96                                                 // 15   LED, LCD, Digits, Metadata ("user_interface" domain)
static uint32_t buffer_io_ui_out_0[size_io_ui_out_0 / sizeof(int32_t)];     

#define size_analog_in_0 96                                                 // 21   analog A/D input ("motion" or "analog_in" or "gpio" domain)
static uint32_t buffer_analog_in_0[size_analog_in_0 / sizeof(int32_t)];       

#define size_audio_in_0 96                                                  // 23   microphone, PDM, line-in, modem, USB audio ("audio_in" domain)
static uint32_t buffer_audio_in_0[size_audio_in_0 / sizeof(int32_t)];       

#define size_2d_in_0 96                                                     // 30   camera ("2d_in" domain)
static uint32_t buffer_2d_in_0[size_2d_in_0 / sizeof(int32_t)];     



/* 
    multichannel tests          IO_AL_idx = 0
*/
void data_sink(uint32_t command, nanograph_xdmbuffer_t* data)
{   
    int32_t tmp, nanograph_format_io_setting, count;

    switch (command)
    {
    case NANOGRAPH_RESET:
        if (NULL == (ptf_data_sink = fopen("..\\test\\ID0_data_sink", "rb"))) 
        {
            exit(1);
        }
        else
        {
            int i, c;
            for (i = 0; i < 64; i++)
            {
                fread(&c, 1, 1, ptf_data_sink); // skip WAV header
            }
}
        nanograph_format_io_setting = *(uint32_t*)(data->address);
        break;
        case NANOGRAPH_SET_PARAMETER:
        break;
        case NANOGRAPH_SET_BUFFER:
    {
        nanograph_xdmbuffer_t* pt_pt;
        pt_pt = (nanograph_xdmbuffer_t*)data;
        pt_pt->address = (intptr_t)buffer_data_sink;
        pt_pt->size = size_data_sink;
    }
    break;
        case NANOGRAPH_RUN:
        count = size_data_sink / sizeof(int16_t);
        tmp = (int32_t)fread(buffer_data_sink, sizeof(int16_t), count, ptf_data_sink);
        if (tmp != count)
        {
            NanoGraph_io_ack(IO_PLATFORM_DATA_SINK, buffer_data_sink, 0);
            fclose(ptf_data_sink); 
            one_file_is_closed = 1;
        }
        else
        {
            NanoGraph_io_ack(IO_PLATFORM_DATA_SINK, buffer_data_sink, size_data_sink);
        }
        break;
        case NANOGRAPH_STOP:
        fclose(ptf_data_sink);
        one_file_is_closed = 1;
        break;
    default:
        break;
    }
}


/*
 * ---------------------IO_AL_idx = 1-----------------------------------
 */

void io_data_in_0(uint32_t command, nanograph_xdmbuffer_t* data)
{
}

/*
 * ---------------------IO_AL_idx = 2-----------------------------------
 */

void io_data_in_1(uint32_t command, nanograph_xdmbuffer_t* data)
{
}



/*
 * ---------------------IO_AL_idx = 3-----------------------------------
 */

void io_data_out_0(uint32_t command, nanograph_xdmbuffer_t* data)
{
    switch (command)
    {
    case NANOGRAPH_RESET:
        #define FILE_DATA_OUT_0 "..\\Integration\\test_patterns\\ID9_data_out_0.raw"
            if (NULL == (ptf_data_out_0 = fopen(FILE_DATA_OUT_0, "wb")))
            {   exit ( 1);
            }
        break;
    case NANOGRAPH_SET_PARAMETER:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);          
        break;
    case NANOGRAPH_SET_BUFFER:
        {   nanograph_xdmbuffer_t *pt_pt;
        pt_pt = (nanograph_xdmbuffer_t*)data;
        pt_pt->address = (intptr_t)buffer_data_out_0;
        pt_pt->size = size_data_out_0;
    }
    break;
    case NANOGRAPH_RUN:
            NanoGraph_io_ack(IO_PLATFORM_DATA_OUT_0, buffer_data_out_0, size_data_out_0);
            fwrite(buffer_data_out_0, 1, size_data_out_0, ptf_data_out_0);
            fflush(ptf_data_out_0);
        break;
    case NANOGRAPH_STOP:
            fclose (ptf_data_out_0);
            one_file_is_closed = 1;
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}



/*
 * ---------------------IO_AL_idx = 4-----------------------------------
 */

void io_data_out_1(uint32_t command, nanograph_xdmbuffer_t* data)
{
}

/*
 * ---------------------IO_AL_idx = 5-----------------------------------
 */

void io_sensor_in_0 (uint32_t command, nanograph_xdmbuffer_t *data)             //  motion, temperature, proximity, CO2, PIR ("motion" or "analog_in" or "gpio" domains)
{   int32_t tmp, nanograph_format_io_setting;

#define IO_SENSOR_IN_0_SIZE 8
    int16_t local_buffer[IO_SENSOR_IN_0_SIZE];

    switch (command)
    {
    case NANOGRAPH_RESET:
        //if (NULL == (ptf_in_nanograph_in_0_data = fopen("..\\nanograph_test\\sine_noise_offset.wav", "rb"))) 
        //if (NULL == (ptf_sensor_0 = fopen("..\\test\\ID5_sensor_in_0.raw", "rb")))
        if (NULL == (ptf_debug_detector = fopen("..\\Integration\\test_patterns\\detector_6ch.raw", "wb")))
        {   exit(1);
        }

        if (NULL == (ptf_sensor_0 = fopen("..\\Integration\\test_patterns\\ID5_sensor_in_0.wav", "rb")))    //Integration\test_patterns
        {   exit ( 1); 
        }
        else 
        {   int i, c; 
            for(i=0;i<48;i++) 
            { fread(&c,1,1,ptf_sensor_0); // skip WAV header
            }
        }
        nanograph_format_io_setting = *(uint32_t *)(data->address);          
        break;
        case NANOGRAPH_SET_PARAMETER:
        break;
        case NANOGRAPH_SET_BUFFER:
        break;
        case NANOGRAPH_RUN:

        tmp = (int32_t)fread(local_buffer, sizeof(int16_t), IO_SENSOR_IN_0_SIZE, ptf_sensor_0);

        if (IO_SENSOR_IN_0_SIZE != tmp)
        {   NanoGraph_io_ack (IO_PLATFORM_SENSOR_IN_0, (uint8_t *)(data->address), 0);
            fclose (ptf_sensor_0);
            one_file_is_closed = 1;
        }
        else
        {   NanoGraph_io_ack (IO_PLATFORM_SENSOR_IN_0, local_buffer, IO_SENSOR_IN_0_SIZE * sizeof(int16_t));
        }
        break;
        case NANOGRAPH_STOP:
            fclose (ptf_sensor_0);
            one_file_is_closed = 1;
        break;
    default:
        break;        
    }
}



/*
 * ---------------------IO_AL_idx = 9-----------------------------------
 */

void io_timer_0 (uint32_t command, nanograph_xdmbuffer_t* data)              // timer at 100ms, 1s ("timer" domain)
{
}


/*
 * ---------------------IO_AL_idx = 11-----------------------------------
 */

void io_ui_in_0 (uint32_t command, nanograph_xdmbuffer_t* data)              // buttons, slider ("user_interface" domain)
{
}


/*
 * ---------------------IO_AL_idx = 15-----------------------------------
 */

void io_ui_out_0(uint32_t command, nanograph_xdmbuffer_t* data)             // LED, LCD, Digits, Metadata ("user_interface" domain)
{
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:
        #define FILE_ui_out_0 "..\\Integration\\test_patterns\\ID15_ui_out_0.raw"
        if (NULL == (ptf_ui_out_0 = fopen(FILE_ui_out_0, "wb")))
        {   exit(1);
        }     
        break;
    case NANOGRAPH_SET_PARAMETER:
        break;
    case NANOGRAPH_SET_BUFFER:
    break;
    case NANOGRAPH_RUN:
        /* "io_platform_nanograph_in_1," frame_size option in samples + FORMAT-0 in the example graph */
        pt_pt = (nanograph_xdmbuffer_t*)data;
        NanoGraph_io_ack (IO_PLATFORM_UI_OUT_0, (uint8_t *)pt_pt->address, pt_pt->size);
        fwrite((uint8_t *)pt_pt->address, 1, pt_pt->size, ptf_ui_out_0);
        fflush(ptf_ui_out_0);
        break;
    case NANOGRAPH_STOP:
            fclose (ptf_ui_out_0);
            one_file_is_closed = 1;
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}


/*
 * ---------------------IO_AL_idx = 19-----------------------------------
 */

void io_serial_in_0(uint32_t command, nanograph_xdmbuffer_t* data)      // communication RX for debug and remote control ("general" domain)
{
}


/*
 * ---------------------IO_AL_idx = 21-----------------------------------
 */

void io_analog_in_0(uint32_t command, nanograph_xdmbuffer_t* data)      // analog A/D input ("motion" or "analog_in" or "gpio" domain)
{
}


/*
 * ---------------------IO_AL_idx = 23-----------------------------------
 */

void io_audio_in_0 (uint32_t command, nanograph_xdmbuffer_t *data)      // microphone, PDM, line-in, modem, USB audio ("audio_in" domain)
{   int32_t tmp, nanograph_format_io_setting;
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:

        if (NULL == (ptf_audio_in_0 = fopen("..\\Integration\\test_patterns\\ID23_audio_in_0.raw", "rb")))    //Integration\test_patterns
        {   exit (1); 
        }
        else 
        {   int i, c; 
            for(i=0;i<64;i++) 
            {   fread(&c,1,1,ptf_audio_in_0); // skip WAV header
            }
        }
        nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;
    case NANOGRAPH_SET_PARAMETER:
        break;
    case NANOGRAPH_SET_BUFFER:
        {   pt_pt = (nanograph_xdmbuffer_t *)data;
            pt_pt->address = (intptr_t)buffer_audio_in_0;
            pt_pt->size = size_audio_in_0;
        }
        break;
    case NANOGRAPH_RUN:
        pt_pt = (nanograph_xdmbuffer_t*)data;
        //tmp = (int32_t)fread((uint8_t*)(pt_pt->address), 1, pt_pt->size, ptf_audio_in_0);
        tmp = (int32_t)fread((uint8_t*)(buffer_audio_in_0), 1, size_audio_in_0, ptf_audio_in_0);

        if (tmp != size_audio_in_0)
        {   fclose (ptf_audio_in_0);
            one_file_is_closed = 1;
        }
        else
        {   NanoGraph_io_ack (IO_PLATFORM_AUDIO_IN_0, buffer_audio_in_0, pt_pt->size);
        }
        break;
    case NANOGRAPH_STOP:
            fclose (ptf_audio_in_0);
            one_file_is_closed = 1;
        break;
    default:
        break;
    }
}


/*
 * ---------------------IO_AL_idx = 26-----------------------------------
 */

void io_audio_out_0(uint32_t command, nanograph_xdmbuffer_t* data)           // buzzer, class-D, earphone, line-out, ultrasound
{
}

/*

/*
 * ---------------------IO_AL_idx = 30-----------------------------------
 */

void io_2d_in_0(uint32_t command, nanograph_xdmbuffer_t* data)              // camera ("2d_in" domain)
{
}

/*
 * -----------------------------------------------------------------------
 */
