/**
  ******************************************************************************
  * @file    audio_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-12-08T02:21:15+0600
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef AUDIO_DATA_PARAMS_H
#define AUDIO_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_AUDIO_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_audio_data_weights_params[1]))
*/

#define AI_AUDIO_DATA_CONFIG               (NULL)


#define AI_AUDIO_DATA_ACTIVATIONS_SIZES \
  { 2500, }
#define AI_AUDIO_DATA_ACTIVATIONS_SIZE     (2500)
#define AI_AUDIO_DATA_ACTIVATIONS_COUNT    (1)
#define AI_AUDIO_DATA_ACTIVATION_1_SIZE    (2500)



#define AI_AUDIO_DATA_WEIGHTS_SIZES \
  { 46120, }
#define AI_AUDIO_DATA_WEIGHTS_SIZE         (46120)
#define AI_AUDIO_DATA_WEIGHTS_COUNT        (1)
#define AI_AUDIO_DATA_WEIGHT_1_SIZE        (46120)



#define AI_AUDIO_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_audio_activations_table[1])

extern ai_handle g_audio_activations_table[1 + 2];



#define AI_AUDIO_DATA_WEIGHTS_TABLE_GET() \
  (&g_audio_weights_table[1])

extern ai_handle g_audio_weights_table[1 + 2];


#endif    /* AUDIO_DATA_PARAMS_H */
