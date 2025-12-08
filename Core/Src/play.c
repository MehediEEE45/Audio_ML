#include "play.h"
#include "record.h"

static AUDIO_OUT_BufferTypeDef  BufferCtl;
static int16_t FilePos = 0;
static __IO uint32_t uwVolume = 100;

int tes1,tes2;

AUDIO_PLAYBACK_StateTypeDef AudioState;

WAVE_FormatTypeDef WaveReadFormat;
FILELIST_FileTypeDef FileList;

/* playback-from-SDRAM state */
static uint8_t playback_from_sdram = 0;
static uint32_t sdram_play_pos = 0; /* half-words already consumed */
static uint32_t sdram_play_total = 0; /* total half-words available */

/* Private function prototypes -----------------------------------------------*/
static AUDIO_ErrorTypeDef GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info);
static uint8_t PlayerInit(uint32_t AudioFreq);

FILELIST_FileTypeDef FileList;
uint16_t NumObs = 0;

/*
              Copies disk content in the explorer list.
              None
              Operation result
  */
FRESULT AUDIO_StorageParse(void)
{
  FRESULT res = FR_OK;
  FILINFO fno;
  DIR dir;
  char *fn;

  res = f_opendir(&dir, SDPath);
  FileList.ptr = 0;

  if(res == FR_OK)
  {
    while(BSP_SD_IsDetected())
    {
      res = f_readdir(&dir, &fno);
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if(fno.fname[0] == '.')
      {
        continue;
      }
      fn = fno.fname;

      if(FileList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, "wav")) || (strstr(fn, "WAV")))
          {
            strncpy((char *)FileList.file[FileList.ptr].name, (char *)fn, FILEMGR_FILE_NAME_SIZE);
            FileList.file[FileList.ptr].type = FILETYPE_FILE;
            FileList.ptr++;
          }
        }
      }
    }
  }
  else
  {
	  serialPrintln(&vcp,  "cannot open dir : %d",res);

  }
  NumObs = FileList.ptr;
  serialPrintln(&vcp,"NumbObs : %d",NumObs);
  f_closedir(&dir);
  return res;
}


/*
              Shows audio file (*.wav) on the root
  */


uint8_t AUDIO_ShowWavFiles(void)
{
  if(AUDIO_StorageParse() ==  FR_OK)
  {
    if(FileList.ptr > 0)
    {
      return 0;
    }
    return 1;
  }
  return 2;
}

/*
                     Gets Wav Object Number.
  */
uint16_t AUDIO_GetWavObjectNumber(void)
{
  return NumObs;
}

AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(void)
{
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, uwVolume, AUDIO_FREQUENCY_48K) == 0)
  {
    return AUDIO_ERROR_NONE;
  }
  else
  {
    return AUDIO_ERROR_IO;
  }
}

/*
                    Initializes the Wave player.
                    AudioFreq: Audio sampling frequency
  */
static uint8_t PlayerInit(uint32_t AudioFreq)
{
  /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */
  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, uwVolume, AudioFreq) != 0)
  {
    return 1;
  }
  else
  {
    BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
    return 0;
  }
}

/*
                Gets the file info.
                file_idx: File index
                info: Pointer to WAV file info
                Audio error
  */
static AUDIO_ErrorTypeDef GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info)
{
  uint32_t bytesread;
  uint32_t duration;
  uint8_t str[FILEMGR_FILE_NAME_SIZE + 20];

  if(f_open(&SDFile, (char *)FileList.file[file_idx].name, FA_OPEN_EXISTING | FA_READ) == FR_OK)
  {
    /* Fill the buffer to Send */
    if(f_read(&SDFile, info, sizeof(WaveReadFormat), (void *)&bytesread) == FR_OK)
    {

      return AUDIO_ERROR_NONE;
    }
    f_close(&SDFile);
  }
  return AUDIO_ERROR_IO;
}


AUDIO_ErrorTypeDef AUDIO_PLAYER_Start(uint8_t idx)
{
  uint32_t bytesread;

  f_close(&SDFile);

  /* Prefer SD playback when SD is present */
  if (BSP_SD_IsDetected())
  {
    if(AUDIO_GetWavObjectNumber() > idx)
    {
      GetFileInfo(idx, &WaveReadFormat);
      /* Adjust the Audio frequency */
      PlayerInit(WaveReadFormat.SampleRate);
      BufferCtl.state = BUFFER_OFFSET_NONE;
      /* Read from SD */
      f_lseek(&SDFile, 0);
      if(f_read(&SDFile, &BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE, (void *)&bytesread) == FR_OK)
      {
        if(bytesread != 0)
        {
          BSP_AUDIO_OUT_Play((uint16_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE/2);
          BufferCtl.fptr = bytesread;
          playback_from_sdram = 0;
          return AUDIO_ERROR_NONE;
        }
      }
    }
  }

  /* If SD unavailable or read failed, try playback from SDRAM temporary buffer */
  sdram_play_total = RECORD_GetSDRAMSizeHalfWords();
  if ((sdram_play_total > 0) && (RECORD_GetSDRAMPtr() != NULL))
  {
    /* Use the same sample rate used during recording */
    PlayerInit(DEFAULT_AUDIO_IN_FREQ);
    BufferCtl.state = BUFFER_OFFSET_NONE;
    /* copy initial buffer from SDRAM */
    uint16_t *src = RECORD_GetSDRAMPtr();
    uint32_t tocopy = (sdram_play_total >= AUDIO_OUT_BUFFER_SIZE) ? AUDIO_OUT_BUFFER_SIZE : sdram_play_total;
    for (uint32_t i = 0; i < tocopy; i++) BufferCtl.buff[i] = src[i];
    if (tocopy < AUDIO_OUT_BUFFER_SIZE)
    {
      for (uint32_t i = tocopy; i < AUDIO_OUT_BUFFER_SIZE; i++) BufferCtl.buff[i] = 0;
    }
    BSP_AUDIO_OUT_Play((uint16_t*)&BufferCtl.buff[0], AUDIO_OUT_BUFFER_SIZE/2);
    BufferCtl.fptr = tocopy * 2; /* bytes */
    sdram_play_pos = tocopy; /* half-words consumed into buffer */
    playback_from_sdram = 1;
    return AUDIO_ERROR_NONE;
  }

  return AUDIO_ERROR_IO;
}



AUDIO_ErrorTypeDef playStart(int index)
{
	int res;
	serialPrintln(&vcp,"init play");
	res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
	if(res != FR_OK)
	{
	  serialPrintln(&vcp,"error mount, code error : %d",res);
	}

	AUDIO_ShowWavFiles();

	res = AUDIO_PLAYER_Start(index);
	if(res != AUDIO_ERROR_NONE)
	{
	  serialPrintln(&vcp,"error start audio : %d",res);
	}
	else
	{
		serialPrintln(&vcp,"audio start gaes");
	}

	return (AUDIO_ERROR_NONE);
}

AUDIO_ErrorTypeDef playProcess(void)
{
  uint32_t bytesread, elapsed_time;
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;
  static uint32_t prev_elapsed_time = 0xFFFFFFFF;


    if(BufferCtl.fptr >= WaveReadFormat.FileSize)
    {
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
      audio_error = AUDIO_ERROR_EOF;
    }

    if(BufferCtl.state == BUFFER_OFFSET_HALF)
    {
      if (playback_from_sdram)
      {
        uint16_t *src = RECORD_GetSDRAMPtr();
        uint32_t remaining = (sdram_play_total > sdram_play_pos) ? (sdram_play_total - sdram_play_pos) : 0;
        uint32_t copy = (remaining >= (AUDIO_OUT_BUFFER_SIZE/2)) ? (AUDIO_OUT_BUFFER_SIZE/2) : remaining;
        for (uint32_t i = 0; i < copy; i++) BufferCtl.buff[i] = src[sdram_play_pos + i];
        if (copy < (AUDIO_OUT_BUFFER_SIZE/2))
        {
          for (uint32_t i = copy; i < (AUDIO_OUT_BUFFER_SIZE/2); i++) BufferCtl.buff[i] = 0;
        }
        sdram_play_pos += copy;
        BufferCtl.state = BUFFER_OFFSET_NONE;
        BufferCtl.fptr += copy * 2;
      }
      else
      {
        if(f_read(&SDFile,
                  &BufferCtl.buff[0],
                  AUDIO_OUT_BUFFER_SIZE/2,
                  (void *)&bytesread) != FR_OK)
        {
          BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
          return AUDIO_ERROR_IO;
        }
        BufferCtl.state = BUFFER_OFFSET_NONE;
        BufferCtl.fptr += bytesread;
      }
    }

    if(BufferCtl.state == BUFFER_OFFSET_FULL)
    {
      if (playback_from_sdram)
      {
        uint16_t *src = RECORD_GetSDRAMPtr();
        uint32_t remaining = (sdram_play_total > sdram_play_pos) ? (sdram_play_total - sdram_play_pos) : 0;
        uint32_t copy = (remaining >= (AUDIO_OUT_BUFFER_SIZE/2)) ? (AUDIO_OUT_BUFFER_SIZE/2) : remaining;
        for (uint32_t i = 0; i < copy; i++) BufferCtl.buff[(AUDIO_OUT_BUFFER_SIZE/2) + i] = src[sdram_play_pos + i];
        if (copy < (AUDIO_OUT_BUFFER_SIZE/2))
        {
          for (uint32_t i = copy; i < (AUDIO_OUT_BUFFER_SIZE/2); i++) BufferCtl.buff[(AUDIO_OUT_BUFFER_SIZE/2) + i] = 0;
        }
        sdram_play_pos += copy;
        BufferCtl.state = BUFFER_OFFSET_NONE;
        BufferCtl.fptr += copy * 2;
      }
      else
      {
        if(f_read(&SDFile,
                  &BufferCtl.buff[AUDIO_OUT_BUFFER_SIZE/2],
                  AUDIO_OUT_BUFFER_SIZE/2,
                  (void *)&bytesread) != FR_OK)
        {
          BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
          return AUDIO_ERROR_IO;
        }

        BufferCtl.state = BUFFER_OFFSET_NONE;
        BufferCtl.fptr += bytesread;
      }
    }

    /* Display elapsed time */
    elapsed_time = BufferCtl.fptr / WaveReadFormat.ByteRate;
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
    }
  return audio_error;
}


void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    BufferCtl.state = BUFFER_OFFSET_FULL;
}

void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    BufferCtl.state = BUFFER_OFFSET_HALF;
}

AUDIO_ErrorTypeDef playStop(void)
{
  AudioState = AUDIO_STATE_STOP;
  FilePos = 0;

  BSP_AUDIO_OUT_Stop(CODEC_PDWN_SW);
  if (!playback_from_sdram) f_close(&SDFile);
  playback_from_sdram = 0;
  sdram_play_pos = 0;
  sdram_play_total = 0;

  serialPrintln(&vcp,"selesai play");
  return AUDIO_ERROR_NONE;
}
