#ifndef USBD_COMPOSITE_H
#define USBD_COMPOSITE_H
#include "usbd_def.h"


#define USB_CMPSIT_CONFIG_DESC_SIZ                     67U+39U

extern USBD_ClassTypeDef USBD_CMPSIT;
extern USBD_DescriptorsTypeDef usbCmpsitFS_Desc;
#ifdef USE_USBD_COMPOSITE
void USBD_CMPSIT_AddClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass, USBD_CompositeClassTypeDef classtype, uint8_t *EpAddr);
#endif
#endif