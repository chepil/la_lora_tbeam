#ifndef LORAHELPER_H
#define LORAHELPER_H

struct PACKET {
  char serialNumber[6];
  unsigned long counter;
  char lat[12];
  char lng[12];

  char date[9];
  char time[7];
};

void LoraHelper_setup(void);
void LoraHelper_send_loop(void);
void LoraHelper_receive_loop(void);



#endif /* LORAHELPER_H */
