#include "record.h"
#include <display.h>
#include <stdio.h>

#define BUF_SIZE AUDIO_IN_PCM_BUFFER_SIZE    //half word = 4*2304 =9216

uint16_t buffer[BUF_SIZE];             //buffer[9216]
uint16_t buffer_size = BUF_SIZE;

uint8_t pHeaderBuff[44];                //standard PCM WAV header length is 44 bytes

static AUDIO_IN_BufferTypeDef  BufferCtl;
static __IO uint32_t uwVolume = 100;
WAVE_FormatTypeDef WaveFormat;

/* SDRAM scratch area for temporary recording when SD is missing.
  SDRAM on STM32746G-Discovery is mapped at 0xC0000000 by FMC in CubeMX
  We store raw PCM 16-bit samples here until SD is available to flush.
  Configure size for 10 seconds: DEFAULT_AUDIO_IN_FREQ * channels * bytes_per_sample * 10
*/
#define SDRAM_BASE_ADDR     ((uint32_t)0xC0000000)
#define TEMP_REC_SECONDS    10 /* user requirement */
#define TEMP_REC_BYTES      (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * (AUDIODATA_SIZE) * TEMP_REC_SECONDS)
#define TEMP_REC_HALFWORDS  (TEMP_REC_BYTES/2)

static uint32_t sdram_rec_pos = 0; /* position in half-words (uint16_t) */
static uint8_t recording_to_sdram = 0;

/* Accessor functions for other modules (playback) */
uint16_t* RECORD_GetSDRAMPtr(void)
{
  return (uint16_t*)SDRAM_BASE_ADDR;
}

uint32_t RECORD_GetSDRAMSizeHalfWords(void)
{
  return sdram_rec_pos;
}

void RECORD_ClearSDRAM(void)
{
  sdram_rec_pos = 0;
}

              /*
              Initialize the wave header file
              pHeader: Header Buffer to be filled
              pWaveFormatStruct: Pointer to the wave structure to be filled.
              0 if passed, !0 if failed.
              */

static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';

  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0x00;
  pHeader[5] = 0x4C;
  pHeader[6] = 0x1D;
  pHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';

  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  pHeader[22]  = pWaveFormatStruct->NbrChannels;
  pHeader[23]  = 0x00;

  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pWaveFormatStruct->SampleRate & 0xFF));
  pHeader[25]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 8) & 0xFF);
  pHeader[26]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 16) & 0xFF);
  pHeader[27]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 24) & 0xFF);

  /* Write the Byte Rate -----------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pWaveFormatStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 24) & 0xFF);

  /* Write the block alignment -----------------------------------------------*/
  pHeader[32]  = pWaveFormatStruct->BlockAlign;
  pHeader[33]  = 0x00;

  /* Write the number of bits per sample -------------------------------------*/
  pHeader[34]  = pWaveFormatStruct->BitPerSample;
  pHeader[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';

  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0x00;
  pHeader[41]  = 0x4C;
  pHeader[42]  = 0x1D;
  pHeader[43]  = 0x00;

  /* Return 0 if all operations are OK */
  return 0;
}

/*
            Encoder initialization.
            Freq: Sampling frequency.
            pHeader: Pointer to the WAV file header to be written.
            0 if success, !0 else.
  */

static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t *pHeader)
{
  /* Initialize the encoder structure */
  WaveFormat.SampleRate = Freq;        /* Audio sampling frequency */
  WaveFormat.NbrChannels = 1;          /* Number of channels: 1:Mono or 2:Stereo */
  WaveFormat.BitPerSample = 16;        /* Number of bits per sample (16, 24 or 32) */
  WaveFormat.FileSize = 0x001D4C00;    /* Total length of useful audio data (payload)  0x001D4C00= 1920000 bytes */
  WaveFormat.SubChunk1Size = 44;       /* The file header chunk size */
  WaveFormat.ByteRate = (WaveFormat.SampleRate * \
                        (WaveFormat.BitPerSample/8) * \
                         WaveFormat.NbrChannels);     /* Number of bytes per second  (sample rate * block align)  */
  WaveFormat.BlockAlign = WaveFormat.NbrChannels * \
                         (WaveFormat.BitPerSample/8); /* channels * bits/sample / 8 */

  /* Parse the wav file header and extract required information */
  if(WavProcess_HeaderInit(pHeader, &WaveFormat))
  {
    return 1;
  }
  return 0;
}



/*
            Initialize the wave header file
            pHeader: Header Buffer to be filled
            pWaveFormatStruct: Pointer to the wave structure to be filled.
            0 if passed, !0 if failed.
*/

static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = (uint8_t)(BufferCtl.fptr);
  pHeader[5] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[6] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[7] = (uint8_t)(BufferCtl.fptr >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  BufferCtl.fptr -=44;
  pHeader[40] = (uint8_t)(BufferCtl.fptr);
  pHeader[41] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[42] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[43] = (uint8_t)(BufferCtl.fptr >> 24);

  /* Return 0 if all operations are OK */
  return 0;
}





AUDIO_ErrorTypeDef recordStart()
{
	FRESULT res; /* FatFs function common result code */
	uint32_t byteswritten = 0;
	uwVolume = 100;
  /* If SD card is present try to open file and record directly to SD
     otherwise record to SDRAM temporary buffer and flush later */
  if (BSP_SD_IsDetected())
  {
    res = f_open(&SDFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE);
    if( res != FR_OK)
    {
      serialPrintln(&vcp, "cannot open file, code error : %d", res);
      text(0, 120, 'c', "SD open ERR", 'r', 'k');
      /* fallback to SDRAM */
      recording_to_sdram = 1;
      sdram_rec_pos = 0;
    }
    else
    {
      serialPrintln(&vcp,"open file");
      WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, pHeaderBuff);                  // Initialize header file

      if(f_write(&SDFile, pHeaderBuff, 44, (void*)&byteswritten) == FR_OK)            //Write header file
      {
        if(byteswritten != 0)
        {
          serialPrintln(&vcp,"start record to SD");
          recording_to_sdram = 0;
          BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 1);
          BSP_AUDIO_IN_Record((uint16_t*)&BufferCtl.pcm_buff[0], AUDIO_IN_PCM_BUFFER_SIZE);
          BufferCtl.fptr = byteswritten;
          BufferCtl.pcm_ptr = 0;
          BufferCtl.offset = 0;
          BufferCtl.wr_state = BUFFER_EMPTY;
          HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, SET);
        }
      }
    }
  }
  else
  {
    /* SD not present: record into SDRAM temporary area */
    serialPrintln(&vcp, "SD not present: recording to SDRAM (%d s)", TEMP_REC_SECONDS);
    recording_to_sdram = 1;
    sdram_rec_pos = 0;
    WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, pHeaderBuff); /* prepare header metadata */
    BSP_AUDIO_IN_Init(DEFAULT_AUDIO_IN_FREQ, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 1);
    BSP_AUDIO_IN_Record((uint16_t*)&BufferCtl.pcm_buff[0], AUDIO_IN_PCM_BUFFER_SIZE);
    BufferCtl.fptr = 44; /* we'll simulate header size already written */
    BufferCtl.pcm_ptr = 0;
    BufferCtl.offset = 0;
    BufferCtl.wr_state = BUFFER_EMPTY;
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, SET);

    /* show LCD indicator and initial remaining seconds */
    char info[32];
    snprintf(info, sizeof(info), "Recording (SDRAM) %ds", TEMP_REC_SECONDS);
    text(0, 120, 'c', info, 'g', 'k');
  }

	return (AUDIO_ERROR_NONE);
}




AUDIO_ErrorTypeDef recordProcess()
{
	uint32_t elapsed_time;
	static uint32_t prev_elapsed_time = 0xFFFFFFFF;

	FRESULT res;                                       // FatFs function common result code
	uint32_t byteswritten = 0;

	if(BufferCtl.fptr >= REC_SAMPLE_LENGTH)            // MAX Recording time reached, so stop audio interface and close file
	{
	  return (AUDIO_ERROR_EOF);                        //signaling end of recording
	}

	if (BufferCtl.wr_state == BUFFER_FULL)
	{
		for(int i = 0; i < AUDIO_IN_PCM_BUFFER_SIZE/2; i ++)
		{
			buffer[i] =  BufferCtl.pcm_buff[BufferCtl.offset + i*4];
		}

    if (recording_to_sdram)
    {
      /* copy converted samples into SDRAM temporary buffer */
      uint32_t copy_count = AUDIO_IN_PCM_BUFFER_SIZE/2; /* number of half-words */
      /* check overflow */
      if ((sdram_rec_pos + copy_count) > TEMP_REC_HALFWORDS)
      {
        /* cannot store all; clamp and signal EOF after writing what fits */
        copy_count = (TEMP_REC_HALFWORDS > sdram_rec_pos) ? (TEMP_REC_HALFWORDS - sdram_rec_pos) : 0;
      }
      if (copy_count > 0)
      {
        uint16_t *dst = (uint16_t*)(SDRAM_BASE_ADDR + sdram_rec_pos * 2);
        for (uint32_t k = 0; k < copy_count; k++) dst[k] = buffer[k];
        sdram_rec_pos += copy_count;
        byteswritten = copy_count; /* keep same unit as original (half-words) */
        BufferCtl.fptr += byteswritten * 2; /* bytes */
      }
      if (sdram_rec_pos >= TEMP_REC_HALFWORDS)
      {
        /* reached SDRAM capacity */
        BufferCtl.wr_state = BUFFER_EMPTY;
        return AUDIO_ERROR_EOF;
      }
      BufferCtl.wr_state = BUFFER_EMPTY;
    }
    else
    {
      res = f_write(&SDFile, (uint16_t*)(buffer),AUDIO_IN_PCM_BUFFER_SIZE/2,(void*)&byteswritten);        // write buffer in file
      if(res != FR_OK)
      {
        serialPrintln(&vcp, "cannot store data, code error : %d",res);
      }
      BufferCtl.fptr += byteswritten;
      BufferCtl.wr_state = BUFFER_EMPTY;
    }
	}

    elapsed_time = BufferCtl.fptr / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2);                // Display elapsed time
    if(prev_elapsed_time != elapsed_time)
    {
      /* update elapsed_time and optionally LCD when recording to SDRAM */
      prev_elapsed_time = elapsed_time;
      if (recording_to_sdram)
      {
        /* compute remaining seconds */
        uint32_t bytes_written = BufferCtl.fptr - 44; /* subtract fake header */
        uint32_t bytes_per_sec = DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * (AUDIODATA_SIZE);
        uint32_t elapsed_sec = (bytes_per_sec > 0) ? (bytes_written / bytes_per_sec) : 0;
        int32_t sec_left = (int32_t)TEMP_REC_SECONDS - (int32_t)elapsed_sec;
        if (sec_left < 0) sec_left = 0;
        char info[32];
        snprintf(info, sizeof(info), "Recording (SDRAM) %lds", (long)sec_left);
        text(0, 120, 'c', info, 'g', 'k');
      }
    }

	return (AUDIO_ERROR_NONE);
}



AUDIO_ErrorTypeDef recordStop()
{
	FRESULT res;                                             // FatFs function common result code
	uint32_t byteswritten = 0;


    BSP_AUDIO_IN_Stop(CODEC_PDWN_SW);                        // Stop recorder
    HAL_Delay(150);

  if (recording_to_sdram)
  {
    /* Try to flush SDRAM contents to SD if available */
    uint8_t flushed_ok = 0;
    if (BSP_SD_IsDetected())
    {
      /* create file and write header placeholder */
      res = f_open(&SDFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE);
      if (res != FR_OK)
      {
        serialPrintln(&vcp, "cannot create file to flush SDRAM: %d", res);
      }
      else
      {
        /* write placeholder header */
        WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, pHeaderBuff);
        if (f_write(&SDFile, pHeaderBuff, 44, (void*)&byteswritten) != FR_OK)
        {
          serialPrintln(&vcp, "cannot write header when flushing: %d", res);
        }
        else
        {
          /* write SDRAM buffer in chunks */
          uint16_t *src = (uint16_t*)SDRAM_BASE_ADDR;
          uint32_t remaining = sdram_rec_pos; /* half-words */
          uint32_t pos = 0;
          while (remaining > 0)
          {
            uint32_t write_count = (remaining > (AUDIO_IN_PCM_BUFFER_SIZE/2)) ? (AUDIO_IN_PCM_BUFFER_SIZE/2) : remaining;
            UINT bw = 0;
            FRESULT r = f_write(&SDFile, &src[pos], write_count, &bw);
            if (r != FR_OK)
            {
              serialPrintln(&vcp, "SD write error during flush: %d", r);
              break;
            }
            pos += write_count;
            remaining -= write_count;
          }
          /* set fptr to header + data bytes for header update */
          BufferCtl.fptr = 44 + (sdram_rec_pos * 2);
          /* update header */
          res = f_lseek(&SDFile, 0);
          if (res == FR_OK)
          {
            WavProcess_HeaderUpdate(pHeaderBuff, &WaveFormat);
            res = f_write(&SDFile, pHeaderBuff, sizeof(WAVE_FormatTypeDef), (void*)&byteswritten);
            if (res != FR_OK)
            {
              serialPrintln(&vcp, "cannot finalize header after flush: %d", res);
            }
          }
          f_close(&SDFile);
          flushed_ok = 1;
        }
      }
    }
    else
    {
      /* SD still not present: keep data in SDRAM */
      serialPrintln(&vcp, "SD not present: recording saved in SDRAM (%lu half-words)", sdram_rec_pos);
    }
    /* show LCD message about where data was saved */
    if (flushed_ok)
    {
      text(0, 120, 'c', "Recording saved to SD", 'g', 'k');
    }
    else
    {
      char info2[40];
      snprintf(info2, sizeof(info2), "Saved in SDRAM (%lus)", (unsigned long)( (sdram_rec_pos*2) / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * (AUDIODATA_SIZE)) ));
      text(0, 120, 'c', info2, 'r', 'k');
    }
    recording_to_sdram = 0;
  }
  else
  {
    /* original SD-based flow: finalize header and close file */
    res = f_lseek(&SDFile, 0);                                // Move file pointer of the file object
    if(res != FR_OK)
    {
      serialPrintln(&vcp, "f_lseek error, code error : %d",res);
    }
    else
    {
      WavProcess_HeaderUpdate(pHeaderBuff, &WaveFormat);                      //Update the wav file header save it into wav file

      res = f_write(&SDFile, pHeaderBuff, sizeof(WAVE_FormatTypeDef), (void*)&byteswritten);
      if(res != FR_OK)
      {
        serialPrintln(&vcp, "cannot end file, code error : %d",res);
      }
      else
      {
        serialPrintln(&vcp, "end of file",res);
      }
    }
    f_close(&SDFile);                                                       // Close file
  }

  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, RESET);

  serialPrintln(&vcp, "recording success");

  return (AUDIO_ERROR_NONE);
}


void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
	BufferCtl.wr_state = BUFFER_FULL;
	BufferCtl.offset = 0;

}


void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
	BufferCtl.wr_state = BUFFER_FULL;
	BufferCtl.offset = AUDIO_IN_PCM_BUFFER_SIZE;
}
