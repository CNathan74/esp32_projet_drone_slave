#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

typedef union{
  unsigned int ui_StateBtnLaby     :4;
  boolean b_StateBtnLaby[4];
}StateBtnLaby;

#define PMP_ANEMO                 0

#define DRT_BTN_GAUCHE            27//35
#define DRT_BTN_DROITE            12//34
#define DRT_BTN_AV                32//
#define DRT_BTN_AR                33//

#define DRT_BTN_1                 2
#define DRT_BTN_2                 4
#define DRT_BTN_3                 15


#define FREQUENCE_TIMER_PRINCIPAL 10
#define NB_PERIODE_SEQUENCEUR     10 

#endif
