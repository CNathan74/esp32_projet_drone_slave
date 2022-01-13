#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

typedef union{
  unsigned int ui_StateBtnLaby     :4;
  boolean b_StateBtnLaby[4];
}StateBtnLaby;


#define DRT_BTN_GAUCHE            19
#define DRT_BTN_DROITE            21
#define DRT_BTN_AV                18
#define DRT_BTN_AR                5

#define DRT_BTN_1                 27
#define DRT_BTN_2                 14
#define DRT_BTN_3                 12

#define DRT_RETRO_BTN_1           4
#define DRT_RETRO_BTN_2           2
#define DRT_RETRO_BTN_3           15



#define FREQUENCE_TIMER_PRINCIPAL 10
#define NB_PERIODE_SEQUENCEUR     10 

#endif
