/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        platform_computer_io_services.c
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
extern uint8_t one_file_is_closed;

extern void NanoGraph_io_ack (uint8_t HW_io_idx, void *data, uintptr_t size);

/*
 * NULL TASK
 */
void NanoGraph_null_task (int32_t c, nanograph_handle_t i, void *d, uint32_t *s)  {}



/* Local IO functions */
extern void data_sink      (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 0   SINK   */
extern void data_in_1      (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 1   io_ra8e1_data_in_1.txt   */
extern void analog_in_0    (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 2   io_ra8e1_analog_in_0.txt */
extern void motion_in_0    (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 3   io_ra8e1_motion_in_0.txt */
extern void audio_in_0     (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 4   io_ra8e1_audio_in_0.txt  */
extern void sensor_2d_in_0 (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 5   io_ra8e1_2d_in_0.txt  */
extern void line_out_0     (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 6   io_ra8e1_line_out_0.txt  */
extern void gpio_out_0     (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 7   io_ra8e1_gpio_out_0.txt  */
extern void gpio_out_1     (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 8   io_ra8e1_gpio_out_1.txt  */
extern void data_out_0     (uint32_t command, nanograph_xdmbuffer_t* data);        /*  IO_AL_idx = 9   io_ra8e1_data_out_0.txt  */


/*
 * NULL TASK
 */
void arm_stream_null_task (int32_t c, nanograph_handle_t i, void *d, uint32_t *s)  {}


/* --------------------------------------------------------------------------------------- 
    FW IO FUNCTIONS

    const p_io_function_ctrl platform_io [LAST_IO_FUNCTION_PLATFORM] =
    {
        (void *)&data_sink,        // 0
        (void *)&data_in_1,        // 1
        (void *)&sensor_0,         // 2
        (void *)&motion_in_0,      // 3
        (void *)&audio_in_0,       // 4
        (void *)&d2_in_0,          // 5
        (void *)&line_out_0,       // 6
        (void *)&gpio_out_0,       // 7
        (void *)&gpio_out_1,       // 8
        (void *)&data_out_0,       // 9
    };
*/




#ifdef _MSC_VER 
FILE* ptf_data_sink;
FILE* ptf_data_in_1;
FILE* ptf_sensor_0;
FILE* ptf_analog_0;
FILE* ptf_audio_in_0;
FILE* ptf_debug_detector;
FILE* ptf_line_out_0;
FILE* ptf_gpio_out_0;
FILE* ptf_gpio_out_1;
FILE* ptf_data_out_0;
#endif


#define size_data_sink 16
static int16_t buffer_data_sink[size_data_sink / sizeof(int16_t)];

#define size_data_out_0 32
static int16_t buffer_data_out_0[size_data_out_0 / sizeof(int16_t)];

/*  void audio_in_0(uint32_t command, nanograph_xdmbuffer_t* data) */
#define size_audio_in_0 32                                                  // BYTES               IO_AL_idx = 4
static int16_t buffer_audio_in_0[size_audio_in_0 / sizeof(int16_t)];        // NANOGRAPH_SET_BUFFER

#define size_gpio_out_0 96                                                  // IO_AL_idx = 7
static uint32_t buffer_gpio_out_0[size_gpio_out_0/ sizeof(int32_t)];        // 96 Bytes = 24 samples_32b (=3 samples x 8 channels)


/* void gpio_out_1 (uint32_t command, nanograph_xdmbuffer_t *data)  */
#define size_gpio_out_1 2
//static int16_t buffer_gpio_out_1[size_gpio_out_1 / sizeof(int16_t)];      // IO_AL_idx = 8



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
*  Graph declaration : set0copy1 0          IO_AL_idx = 1
*/
void data_in_1 (uint32_t command, nanograph_xdmbuffer_t *data) 
{   switch (command)
    {
    case NANOGRAPH_RESET:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;
    case NANOGRAPH_SET_PARAMETER:  /* presets reloaded */
        break;
    case NANOGRAPH_SET_BUFFER:     /* if memory allocation is made in the graph */
        break;
    case NANOGRAPH_RUN:            /* data moves */
        break;
    case NANOGRAPH_STOP:           /* stop data moves */
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}

/*
*  Graph declaration : set0copy1 1       IO_AL_idx = 2  
*/
void sensor_in_0 (uint32_t command, nanograph_xdmbuffer_t *data) 
{   int32_t tmp, nanograph_format_io_setting;
#define FORMAT_PRODUCER_FRAME_SIZE 8

    switch (command)
    {
    case NANOGRAPH_RESET:
        //if (NULL == (ptf_in_nanograph_in_0_data = fopen("..\\nanograph_test\\sine_noise_offset.wav", "rb"))) 
        if (NULL == (ptf_sensor_0 = fopen("..\\test\\ID2_sensor_in_0.raw", "rb")))
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
        /* "io_platform_nanograph_in_0," frame_size option in samples + FORMAT-0 in the example graph */
        tmp = (int32_t) fread(data, 1, FORMAT_PRODUCER_FRAME_SIZE, ptf_sensor_0);

        if (FORMAT_PRODUCER_FRAME_SIZE != tmp)
        {   NanoGraph_io_ack (IO_PLATFORM_SENSOR_0, data, 0);
            fclose (ptf_sensor_0);
            one_file_is_closed = 1;

        }
        else
        {   NanoGraph_io_ack (IO_PLATFORM_SENSOR_0, (uint8_t *)data, FORMAT_PRODUCER_FRAME_SIZE);
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

/*          IO_AL_idx = 3           */
void motion_in_0 (uint32_t command, nanograph_xdmbuffer_t *data) 
{
    switch (command)
    {
    case NANOGRAPH_RESET:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;
    case NANOGRAPH_SET_PARAMETER:  /* presets reloaded */
        break;
    case NANOGRAPH_SET_BUFFER:     /* if memory allocation is made in the graph */
        break;
    case NANOGRAPH_RUN:            /* data moves */
        break;
    case NANOGRAPH_STOP:           /* stop data moves */
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}

/*          IO_AL_idx = 4          */
void audio_in_0 (uint32_t command, nanograph_xdmbuffer_t *data) 
{   int32_t tmp, nanograph_format_io_setting;
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:
        if (NULL == (ptf_debug_detector = fopen("..\\Integration\\test_patterns\\detector_6ch.raw", "wb")))
        {   exit(1);
        }

        if (NULL == (ptf_audio_in_0 = fopen("..\\Integration\\test_patterns\\ID4_audio_in_0.raw", "rb")))    //Integration\test_patterns
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

/*          IO_AL_idx = 5          */

void d2_in_0 (uint32_t command, nanograph_xdmbuffer_t *data) 
{
    switch (command)
    {
    case NANOGRAPH_RESET:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;        
    case NANOGRAPH_SET_PARAMETER:  /* presets reloaded */
        break;
    case NANOGRAPH_SET_BUFFER:     /* if memory allocation is made in the graph */
        break;
    case NANOGRAPH_RUN:            /* data moves */
        break;
    case NANOGRAPH_STOP:           /* stop data moves */
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;        
    }
}



/*          IO_AL_idx = 6          */
void line_out_0 (uint32_t command, nanograph_xdmbuffer_t *data)
{
    switch (command)
    {
    case NANOGRAPH_RESET:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;        
    case NANOGRAPH_SET_PARAMETER:  /* presets reloaded */
        break;
    case NANOGRAPH_SET_BUFFER:     /* if memory allocation is made in the graph */
        break;
    case NANOGRAPH_RUN:            /* data moves */
        break;
    case NANOGRAPH_STOP:           /* stop data moves */
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;      
    }
}

/*
*  Graph declaration : set0copy1 0          IO_AL_idx = 7
*/
void gpio_out_0(uint32_t command, nanograph_xdmbuffer_t* data)
{
    switch (command)
    {
    case NANOGRAPH_RESET:
        //nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;        
    case NANOGRAPH_SET_PARAMETER:  /* presets reloaded */
        break;
    case NANOGRAPH_SET_BUFFER:     /* if memory allocation is made in the graph */
        break;
    case NANOGRAPH_RUN:            /* data moves */
        break;
    case NANOGRAPH_STOP:           /* stop data moves */
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}


/*          IO_AL_idx = 8       */
void gpio_out_1(uint32_t command, nanograph_xdmbuffer_t* data)
{
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:
        #define FILE_GPIO_OUT_1 "..\\Integration\\test_patterns\\ID8_gpio_out_1.raw"
        if (NULL == (ptf_gpio_out_1 = fopen(FILE_GPIO_OUT_1, "wb")))
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
        NanoGraph_io_ack (IO_PLATFORM_GPIO_OUT_1, (uint8_t *)pt_pt->address, pt_pt->size);
        fwrite((uint8_t *)pt_pt->address, 1, pt_pt->size, ptf_gpio_out_1);
        fflush(ptf_gpio_out_1);
        break;
    case NANOGRAPH_STOP:
            fclose (ptf_gpio_out_1);
            one_file_is_closed = 1;
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}


/*          IO_AL_idx = 9       */
void data_out_0(uint32_t command, nanograph_xdmbuffer_t* data)
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
 * -----------------------------------------------------------------------
 */
