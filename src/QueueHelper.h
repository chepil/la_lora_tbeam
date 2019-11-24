#ifndef QUEUEHELPER_H
#define QUEUEHELPER_H

#include "LoraHelper.h"
#include "DisplayHelper.h"

#include "stdint.h"

void QueueHelper_setup(void);
void QueueHelper_loop(void);

void PushToQueue(uint8_t msg[]);

#endif /* QUEUEHELPER_H */