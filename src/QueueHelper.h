#ifndef QUEUEHELPER_H
#define QUEUEHELPER_H

#include "LoraHelper.h"
#include "DisplayHelper.h"

#include "stdint.h"

void QueueHelper_setup(void);
void QueueHelper_loop(void);

void PushToQueue(uint8_t msg[]);
void PushToQueueForLoraSend(uint8_t msg[]);

int qSendGetRemainingCount(void);

String getHttpLog(void);

struct ResultMessage{
  uint8_t msg[52];
};

ResultMessage qSendPop(void);

#endif /* QUEUEHELPER_H */