#ifndef USBD_COMPOSITE_H
#define USBD_COMPOSITE_H
#include "usbd_def.h"

extern USBD_ClassTypeDef USBD_CMPSIT;

void USBD_CMPSIT_AddClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass, USBD_CompositeClassTypeDef classtype, uint8_t *EpAddr);

#endif