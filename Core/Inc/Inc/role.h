#ifndef INC_ROLE_H_
#define INC_ROLE_H_

#include "serial.h"
#include "record.h"
#include "play.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  RECORD_START = 0,
  RECORD_PROCESS,
  RECORD_STOP,
  PLAY_START,
  PLAY_PROCESS,
  PLAY_STOP,
  AUDIO_GRAPH
} ROLE_TypeDef;

/* Current application role (defined in Core/Src/role.c) */
extern ROLE_TypeDef role;

/* Initialize role subsystem (called from default task) */
void roleInit(void);

/* Role state machine tick; call repeatedly from main loop/task */
void roleNode(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_ROLE_H_ */
/*
 * role.h
 *
 *  Created on: Sep 17, 2022
 *      Author: HP
 */

#ifndef INC_ROLE_H_
#define INC_ROLE_H_

#include "serial.h"
#include "record.h"
#include "play.h"

typedef enum {
  RECORD_START = 0,
  RECORD_PROCESS,
  RECORD_STOP,
  PLAY_START,
  PLAY_PROCESS,
  PLAY_STOP,
  AUDIO_GRAPH
}ROLE_TypeDef;

void roleInit(void);
void roleNode(void);
void ai_on(void);
#endif /* INC_ROLE_H_ */
