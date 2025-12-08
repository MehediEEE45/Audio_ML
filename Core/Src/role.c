#include <display.h>
#include "role.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "mfcc.h"
#include "record.h"
#include <stdio.h>
#include <math.h>
/* X-CUBE-AI model header */
#include "audio.h"
#include "audio_data.h"
/* Ensure AI platform types are visible */
#include "ai_platform.h"

/* Forward-declare AI helpers in case the generated header prototypes are not visible */
extern ai_error ai_audio_create_and_init(ai_handle* network, const ai_handle activations[], const ai_handle weights[]);
extern ai_buffer* ai_audio_inputs_get(ai_handle network, ai_u16 *n_buffer);
extern ai_buffer* ai_audio_outputs_get(ai_handle network, ai_u16 *n_buffer);
extern ai_i32 ai_audio_run(ai_handle network, const ai_buffer* input, ai_buffer* output);

/* Fallback definitions in case the generated headers are not picked up */
#ifndef AI_AUDIO_OUT_1_SIZE
#define AI_AUDIO_OUT_1_SIZE  (5)
#endif
#ifndef AI_AUDIO_IN_NUM
#define AI_AUDIO_IN_NUM (1)
#endif
#ifndef AI_AUDIO_OUT_NUM
#define AI_AUDIO_OUT_NUM (1)
#endif

ROLE_TypeDef role = RECORD_START;

#define SONG_NUMBER 0



void roleInit()
{
	FRESULT res; /* FatFs function common result code */


	/*init library serial*/
	serialInit(&vcp,USART1,usart1_buffer, sizeof(usart1_buffer));

	serialPrintln(&vcp,"mount logical drive and create a FAT volume");
	/*Mount a logical drive*/
	res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	if(res != FR_OK)
	{
	  serialPrintln(&vcp,"error mount, code error : %d",res);
	}
//	else
//	{
//		/*create a FAT volume*/
//		res = f_mkfs((TCHAR const*)SDPath, FM_ANY, 0, rtext, sizeof(rtext));
//		if(res != FR_OK)
//		{
//			serialPrintln(&vcp,"error mkfs, code error : %d",res);
//		}
//	}

	LCD_Config();
//	BSP_TS_Init(480,272);
	HAL_Delay(500);

}


void roleNode()
{
//	char buf[25];
	switch(role)
	{
	case RECORD_START:
		recordStart();
		text(0,120, 'c', "Recording...", 'r', 'k');
		role = RECORD_PROCESS;
		break;
	case RECORD_PROCESS:
		if(recordProcess() == AUDIO_ERROR_EOF)
			role = RECORD_STOP;
		break;
	case RECORD_STOP:
		recordStop();
		text(100, 120, 'c', "Recording stopped", 'r', 'b');
		HAL_Delay(1000);
		/* After recording stopped, compute MFCC on the first frame and show */
		{

			/* AI model handle (static to keep across calls) */
			static ai_handle audio_net = AI_HANDLE_NULL;
			/* input quantization params from generated model metadata */
			const float ai_input_scale = 2.7867205142974854f;
			const int ai_input_zp = 99;
			/* output quant params (for dequant) */
			const float ai_output_scale = 0.00390625f; /* from audio.c */
			const int ai_output_zp = -128;

			/* try SDRAM first */
			uint32_t sdram_size = RECORD_GetSDRAMSizeHalfWords();
			int16_t pcm_frame[512];
			/* buffer for 40-dim log-mel features */
			float logmel[40];
			int8_t qin[40];
			int8_t qout[AI_AUDIO_OUT_1_SIZE];
			/* outputs buffer (not stored here) */
			if (sdram_size >= 512)
			{
				uint16_t *src = RECORD_GetSDRAMPtr();
				for (int i=0;i<512;i++) pcm_frame[i] = (int16_t)src[i];
				/* compute 40-dim log-mel features */
					if (mfcc_compute_logmel(pcm_frame, 512, DEFAULT_AUDIO_IN_FREQ, 40, logmel) == 0)
				{
					/* quantize to int8 according to model input scale & zp */
					for (int i=0;i<40;i++)
					{
						int q = (int)roundf(logmel[i] / ai_input_scale) + ai_input_zp;
						if (q > 127) {
							q = 127;
						}
						if (q < -128) {
							q = -128;
						}
						qin[i] = (int8_t)q;
					}

					/* lazy init the AI network */
					if (audio_net == AI_HANDLE_NULL)
					{
						ai_error err = ai_audio_create_and_init(&audio_net,
									AI_AUDIO_DATA_ACTIVATIONS_TABLE_GET(),
									AI_AUDIO_DATA_WEIGHTS_TABLE_GET());
						if (err.type != AI_ERROR_NONE)
						{
							serialPrintln(&vcp, "AI init error: type=%d code=%d", err.type, err.code);
							/* proceed without running model */
							goto skip_ai_sdram;
						}
					}

					/* prepare input/output buffers */
					ai_buffer inb[AI_AUDIO_IN_NUM];
					ai_buffer outb[AI_AUDIO_OUT_NUM];
					ai_u16 n_in = 0; ai_u16 n_out = 0;
					ai_buffer* pin = ai_audio_inputs_get(audio_net, &n_in);
					ai_buffer* pout = ai_audio_outputs_get(audio_net, &n_out);
					if (!pin || !pout) { serialPrintln(&vcp, "AI buffers unavailable"); goto skip_ai_sdram; }
					/* copy descriptor and set data pointer */
					inb[0] = pin[0]; outb[0] = pout[0];
					inb[0].data = AI_HANDLE_PTR(qin);
					outb[0].data = AI_HANDLE_PTR(qout);

					/* run network */
						if (ai_audio_run(audio_net, inb, outb) > 0)
					{
						/* dequantize outputs and pick argmax */
							int best = 0; float bestv = -1e30f;
							for (int k=0;k<AI_AUDIO_OUT_1_SIZE;k++)
							{
								int q = (int)qout[k];
								float f = ((float)(q - ai_output_zp)) * ai_output_scale;
								if (f > bestv) { bestv = f; best = k; }
							}
							/* map predicted index to label text */
							{
								const char *labels[] = {"fahim", "talukdar", "nayeem","Fa_him","imran"};
								const char *pred_lbl = "unknown";
								if (best >= 0 && best < (int)(sizeof(labels)/sizeof(labels[0]))) pred_lbl = labels[best];
										serialPrintln(&vcp, "AI predicted=%d (%s) score=%d", best, pred_lbl, (int)(bestv*1000.0f));
										/* also show index on LCD (short) */
										{
											char idxbuf[32];
											snprintf(idxbuf, sizeof(idxbuf), "Idx:%d", best);
											text(100, 40, 'c', idxbuf, 'g', 'k');
										}
								/* display predicted label on LCD */
								char lbl[64];
								snprintf(lbl, sizeof(lbl), "Idx:%d %s Score:%d", best, pred_lbl, (int)(bestv*1000.0f));
								text(0, 40, 'c', lbl, 'g', 'k');
								/* keep the prediction visible for 10 seconds */
								HAL_Delay(10000);
							}
					}

				skip_ai_sdram: ;
				/* if AI wasn't run or to indicate completion show compact info */
				multitext(0, 60, 'c', "Computed log-mel (40)", 'g');
				/* also print log-mel values over serial for debugging */
								for (int i=0;i<40;i++) serialPrintln(&vcp, "logmel[%d]=%d", i, (int)(logmel[i]*1000.0f));
				/* fall-through: we already printed features */
				}
			}
			else
			{
				/* try reading from SD file */
				FRESULT res;
				res = f_open(&SDFile, REC_WAVE_NAME, FA_OPEN_EXISTING | FA_READ);
				if (res == FR_OK)
				{
					UINT br;
					/* skip header */
					f_lseek(&SDFile, 44);
					if (f_read(&SDFile, pcm_frame, sizeof(int16_t)*512, &br) == FR_OK)
					{
						/* compute 40-dim log-mel features (same flow as SDRAM path) */
						if (mfcc_compute_logmel(pcm_frame, 512, DEFAULT_AUDIO_IN_FREQ, 40, logmel) == 0)
						{
							for (int i=0;i<40;i++)
							{
								int q = (int)roundf(logmel[i] / ai_input_scale) + ai_input_zp;
								if (q > 127) { q = 127; }
								if (q < -128) { q = -128; }
								qin[i] = (int8_t)q;
							}

							/* lazy init the AI network */
							if (audio_net == AI_HANDLE_NULL)
							{
								ai_error err = ai_audio_create_and_init(&audio_net,
										AI_AUDIO_DATA_ACTIVATIONS_TABLE_GET(),
										AI_AUDIO_DATA_WEIGHTS_TABLE_GET());
								if (err.type != AI_ERROR_NONE)
								{
									serialPrintln(&vcp, "AI init error: type=%d code=%d", err.type, err.code);
									goto skip_ai_sd;
								}
							}

							ai_u16 n_in = 0; ai_u16 n_out = 0;
							ai_buffer* pin = ai_audio_inputs_get(audio_net, &n_in);
							ai_buffer* pout = ai_audio_outputs_get(audio_net, &n_out);
							if (!pin || !pout) { serialPrintln(&vcp, "AI buffers unavailable"); goto skip_ai_sd; }
							ai_buffer inb0 = pin[0]; ai_buffer outb0 = pout[0];
							inb0.data = AI_HANDLE_PTR(qin);
							outb0.data = AI_HANDLE_PTR(qout);
							if (ai_audio_run(audio_net, &inb0, &outb0) > 0)
								{
									int best = 0; float bestv = -1e30f;
									for (int k=0;k<AI_AUDIO_OUT_1_SIZE;k++)
									{
										int q = (int)qout[k];
										float f = ((float)(q - ai_output_zp)) * ai_output_scale;
										if (f > bestv) { bestv = f; best = k; }
									}
									/* map predicted index to label text */
									{
										const char *labels[] = {"fahim", "talukdar", "nayeem","s","imran"};
										const char *pred_lbl = "unknown";
										if (best >= 0 && best < (int)(sizeof(labels)/sizeof(labels[0]))) pred_lbl = labels[best];
										serialPrintln(&vcp, "AI predicted=%d (%s) score=%d", best, pred_lbl, (int)(bestv*1000.0f));
										/* display predicted label on LCD */
										char lbl[64];
										snprintf(lbl, sizeof(lbl), "%s (%d)", pred_lbl, (int)(bestv*1000.0f));
//										text(100, 40, 'c', lbl, 'g', 'k');
										/* keep the prediction visible for 10 seconds */
										HAL_Delay(10000);
									}
								}

						skip_ai_sd: ;
							/* print log-mel */
							for (int i=0;i<40;i++) serialPrintln(&vcp, "logmel[%d]=%d", i, (int)(logmel[i]*1000.0f));
						}
					}
					f_close(&SDFile);
				}
			}
		}
		// role = PLAY_START;
		break;
	// case PLAY_START:
	// 	text(0,120, 'c', "play...", 'g', 'k');
	// 	playStart(SONG_NUMBER);
	// 	role = PLAY_PROCESS;
	// 	break;
	// case PLAY_PROCESS:
	// 	if(playProcess() == AUDIO_ERROR_EOF)
	// 		role = PLAY_STOP;
	// 	break;
	// case PLAY_STOP:
	// 	playStop();
	// 	role = AUDIO_GRAPH;
	// 	break;
	// case AUDIO_GRAPH:
	// 	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	// 	BSP_LCD_DrawHLine(0, 136, 480);
	// 	break;
	}
}
