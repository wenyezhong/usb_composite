#include "usbd_composite.h"
#include "usbd_def.h"
#include "usbd_cdc.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"



#define USBD_VID     1155
#define USBD_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "STMicroelectronics"
#define USBD_PID_FS     22336
#define USBD_PRODUCT_STRING_FS     "STM32 Virtual ComPort"
#define USBD_CONFIGURATION_STRING_FS     "CDC Config"
#define USBD_INTERFACE_STRING_FS     "CDC Interface"



uint8_t * USBD_CMPSIT_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t * USBD_CMPSIT_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
#if (USBD_LPM_ENABLED == 1)
uint8_t * USBD_CMPSIT_FS_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
#endif /* (USBD_LPM_ENABLED == 1) */


USBD_DescriptorsTypeDef usbCmpsitFS_Desc =
{
  USBD_CMPSIT_FS_DeviceDescriptor
, USBD_CMPSIT_FS_LangIDStrDescriptor
, USBD_CMPSIT_FS_ManufacturerStrDescriptor
, USBD_CMPSIT_FS_ProductStrDescriptor
, USBD_CMPSIT_FS_SerialStrDescriptor
, USBD_CMPSIT_FS_ConfigStrDescriptor
, USBD_CMPSIT_FS_InterfaceStrDescriptor
#if (USBD_LPM_ENABLED == 1)
, USBD_CMPSIT_FS_USR_BOSDescriptor
#endif /* (USBD_LPM_ENABLED == 1) */
};

__ALIGN_BEGIN uint8_t USBD_CMPSIT_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
{
};

#if (USBD_LPM_ENABLED == 1)
__ALIGN_BEGIN uint8_t USBD_CMPSIT_FS_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
  0x5,
  USB_DESC_TYPE_BOS,
  0xC,
  0x0,
  0x1,  /* 1 device capability*/
        /* device capability*/
  0x7,
  USB_DEVICE_CAPABITY_TYPE,
  0x2,
  0x2,  /* LPM capability bit set*/
  0x0,
  0x0,
  0x0
};
#endif /* (USBD_LPM_ENABLED == 1) */

/** USB lang identifier descriptor. */
__ALIGN_BEGIN uint8_t USBD_CMPSIT_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     //USB_LEN_LANGID_STR_DESC,
     //USB_DESC_TYPE_STRING,
     //LOBYTE(USBD_LANGID_STRING),
     //HIBYTE(USBD_LANGID_STRING)
};


/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t USBD_CMPSIT_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;


__ALIGN_BEGIN uint8_t USBD_CMPSIT_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
 /*  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING, */
};
 

uint8_t * USBD_CMPSIT_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_CMPSIT_FS_DeviceDesc);
  return USBD_CMPSIT_FS_DeviceDesc;
}

uint8_t * USBD_CMPSIT_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_CMPSIT_LangIDDesc);
  return USBD_CMPSIT_LangIDDesc;
}
uint8_t * USBD_CMPSIT_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  USBD_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_CMPSIT_StrDesc, length);
  return USBD_CMPSIT_StrDesc;
}

uint8_t * USBD_CMPSIT_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_XSTR(USBD_PRODUCT_STRING_FS), USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_PRODUCT_XSTR(USBD_PRODUCT_STRING_FS), USBD_CMPSIT_StrDesc, length);
  }
  return USBD_CMPSIT_StrDesc;
}

uint8_t * USBD_CMPSIT_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = USB_SIZ_STRING_SERIAL;

  /* Update the serial number string descriptor with the data from the unique
   * ID */
  Get_SerialNum();
  /* USER CODE BEGIN USBD_FS_SerialStrDescriptor */

  /* USER CODE END USBD_FS_SerialStrDescriptor */
  return (uint8_t *) USBD_CMPSIT_StringSerial;
}

uint8_t * USBD_CMPSIT_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  return USBD_CMPSIT_StrDesc;
}


uint8_t * USBD_CMPSIT_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CONFIGURATION_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  return USBD_CMPSIT_StrDesc;
}


#if (USBD_LPM_ENABLED == 1)
/**
  * @brief  Return the BOS descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t * USBD_CMPSIT_FS_USR_BOSDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_CMPSIT_FS_BOSDesc);
  return (uint8_t*)USBD_CMPSIT_FS_BOSDesc;
}
#endif /* (USBD_LPM_ENABLED == 1) */



static uint8_t *USBD_CMPSIT_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_CMPSIT_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_CMPSIT_GetOtherSpeedCfgDesc(uint16_t *length);
uint8_t *USBD_CMPSIT_GetDeviceQualifierDescriptor(uint16_t *length);



USBD_ClassTypeDef USBD_CMPSIT=
{
  NULL,
  NULL,
  NULL,
  NULL,                 /* EP0_TxSent */
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  USBD_CMPSIT_GetFSCfgDesc,
  USBD_CMPSIT_GetHSCfgDesc,
  USBD_CMPSIT_GetOtherSpeedCfgDesc,
  USBD_CMPSIT_GetDeviceQualifierDescriptor
};

#ifdef USE_USBD_COMPOSITE
void USBD_CMPSIT_AddClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass, USBD_CompositeClassTypeDef classtype, uint8_t *EpAddr)
{
}
#endif
static uint8_t *USBD_CMPSIT_GetFSCfgDesc(uint16_t *length)
{
}
static uint8_t *USBD_CMPSIT_GetHSCfgDesc(uint16_t *length)
{
}
static uint8_t *USBD_CMPSIT_GetOtherSpeedCfgDesc(uint16_t *length)
{
}
uint8_t *USBD_CMPSIT_GetDeviceQualifierDescriptor(uint16_t *length)
{
}

