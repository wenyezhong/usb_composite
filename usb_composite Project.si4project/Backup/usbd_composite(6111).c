#include "usbd_composite.h"
#include "usbd_def.h"
#include "usbd_cdc.h"
#include "usbd_msc.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"
#include <stdio.h>

#define USBD_PRODUCT_XSTR(s) USBD_PRODUCT_STR(s)
#define USBD_PRODUCT_STR(s) #s

#define USBD_CMPSIT_VID     1155
#define USBD_CMPSIT_LANGID_STRING     1033
#define USBD_CMPSIT_MANUFACTURER_STRING     "wenyz@wolfGroup"
#define USBD_CMPSIT_PID_FS     22336
#define USBD_CMPSIT_PRODUCT_STRING_FS     "STM32 composite product"
#define USBD_CMPSIT_CONFIGURATION_STRING_FS     "Composite Config"
#define USBD_CMPSIT_INTERFACE_STRING_FS     "Composite Interface"

#define USBD_CDC_INTERFACE_STRING_FS     "CDC Interface"
#define USBD_MSC_INTERFACE_STRING_FS     "MSC Interface"


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
	0x12,						/*bLength */
	USB_DESC_TYPE_DEVICE, 	  /*bDescriptorType*/
#if (USBD_LPM_ENABLED == 1)
	0x01, 					  /*bcdUSB */ /* changed to USB version 2.01
											 in order to support LPM L1 suspend
											 resume test of USBCV3.0*/
#else
	0x00, 					  /*bcdUSB */
#endif /* (USBD_LPM_ENABLED == 1) */
	0x02,
	// Notify OS that this is a composite device
	0xEF, 					  /*bDeviceClass*/
	0x02, 					  /*bDeviceSubClass*/
	0x01, 					  /*bDeviceProtocol*/
	USB_MAX_EP0_SIZE, 		  /*bMaxPacketSize*/
	LOBYTE(USBD_CMPSIT_VID), 		  /*idVendor*/
	HIBYTE(USBD_CMPSIT_VID), 		  /*idVendor*/
	LOBYTE(USBD_CMPSIT_PID_FS),		  /*idProduct*/
	HIBYTE(USBD_CMPSIT_PID_FS),		  /*idProduct*/
	0x00, 					  /*bcdDevice rel. 2.00*/
	0x03, 					  /* bNumInterfaces */
	USBD_IDX_MFC_STR, 		  /*Index of manufacturer  string*/
	USBD_IDX_PRODUCT_STR, 	  /*Index of product string*/
	USBD_IDX_SERIAL_STR,		  /*Index of serial number string*/
	USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/

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
     USB_LEN_LANGID_STR_DESC,
     USB_DESC_TYPE_STRING,
     LOBYTE(USBD_CMPSIT_LANGID_STRING),
     HIBYTE(USBD_CMPSIT_LANGID_STRING)
};


/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t USBD_CMPSIT_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;


__ALIGN_BEGIN uint8_t USBD_CMPSIT_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END = {
  USB_SIZ_STRING_SERIAL,
  USB_DESC_TYPE_STRING,
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
  USBD_GetString((uint8_t *)USBD_CMPSIT_MANUFACTURER_STRING, USBD_CMPSIT_StrDesc, length);
  return USBD_CMPSIT_StrDesc;
}

uint8_t * USBD_CMPSIT_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_PRODUCT_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_PRODUCT_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  return USBD_CMPSIT_StrDesc;
}

static void IntToUnicode(uint32_t value, uint8_t * pbuf, uint8_t len)
{
  uint8_t idx = 0;

  for (idx = 0; idx < len; idx++)
  {
    if (((value >> 28)) < 0xA)
    {
      pbuf[2 * idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2 * idx] = (value >> 28) + 'A' - 10;
    }

    value = value << 4;

    pbuf[2 * idx + 1] = 0;
  }
}
static void Get_CMPSIT_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;

  deviceserial0 = *(uint32_t *) DEVICE_ID1;
  deviceserial1 = *(uint32_t *) DEVICE_ID2;
  deviceserial2 = *(uint32_t *) DEVICE_ID3;

  deviceserial0 += deviceserial2;

  if (deviceserial0 != 0)
  {
    IntToUnicode(deviceserial0, &USBD_CMPSIT_StringSerial[2], 8);
    IntToUnicode(deviceserial1, &USBD_CMPSIT_StringSerial[18], 4);
  }
}
uint8_t * USBD_CMPSIT_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  UNUSED(speed);
  *length = USB_SIZ_STRING_SERIAL;

  /* Update the serial number string descriptor with the data from the unique
   * ID */
  Get_CMPSIT_SerialNum();
  /* USER CODE BEGIN USBD_FS_SerialStrDescriptor */

  /* USER CODE END USBD_FS_SerialStrDescriptor */
  return (uint8_t *) USBD_CMPSIT_StringSerial;
}

uint8_t * USBD_CMPSIT_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == 0)
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  return USBD_CMPSIT_StrDesc;
}


uint8_t * USBD_CMPSIT_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length)
{
  if(speed == USBD_SPEED_HIGH)
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_CONFIGURATION_STRING_FS, USBD_CMPSIT_StrDesc, length);
  }
  else
  {
    USBD_GetString((uint8_t *)USBD_CMPSIT_CONFIGURATION_STRING_FS, USBD_CMPSIT_StrDesc, length);
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


__ALIGN_BEGIN static uint8_t USBD_CMPSIT_CfgDesc[USB_CMPSIT_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /* Configuration Descriptor */
  //0
  0x09,                                       /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,                /* bDescriptorType: Configuration */
  USB_CMPSIT_CONFIG_DESC_SIZ,                    /* wTotalLength */
  0x00,
  0x03,                                       /* bNumInterfaces: 2 interfaces */
  0x01,                                       /* bConfigurationValue: Configuration value */
  0x00,                                       /* iConfiguration: Index of string descriptor
                                                 describing the configuration */
#if (USBD_SELF_POWERED == 1U)
  0xC0,                                       /* bmAttributes: Bus Powered according to user configuration */
#else
  0x80,                                       /* bmAttributes: Bus Powered according to user configuration */
#endif /* USBD_SELF_POWERED */
  USBD_MAX_POWER,                             /* MaxPower (mA) */

  /*---------------------------------------------------------------------------*/
  //9
  /* Interface Association Descriptor: CDC device (virtual com port) */
  0x08,   /* bLength: IAD size */
  0x0B,   /* bDescriptorType: Interface Association Descriptor */
  0x00,   /* bFirstInterface */
  0x02,   /* bInterfaceCount */
  0x02,   /* bFunctionClass: Communication Interface Class */
  0x02,   /* bFunctionSubClass: Abstract Control Model */
  0x01,   /* bFunctionProtocol: Common AT commands */
  0x06,   /* iFunction */
  //17
  /* Interface Descriptor */
  0x09,                                       /* bLength: Interface Descriptor size */
  USB_DESC_TYPE_INTERFACE,                    /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,                                       /* bInterfaceNumber: Number of Interface */
  0x00,                                       /* bAlternateSetting: Alternate setting */
  0x01,                                       /* bNumEndpoints: One endpoint used */
  0x02,                                       /* bInterfaceClass: Communication Interface Class */
  0x02,                                       /* bInterfaceSubClass: Abstract Control Model */
  0x01,                                       /* bInterfaceProtocol: Common AT commands */
  0x06,                                       /* iInterface */
  //26
  /* Header Functional Descriptor */
  0x05,                                       /* bLength: Endpoint Descriptor size */
  0x24,                                       /* bDescriptorType: CS_INTERFACE */
  0x00,                                       /* bDescriptorSubtype: Header Func Desc */
  0x10,                                       /* bcdCDC: spec release number */
  0x01,
  //31
  /* Call Management Functional Descriptor */
  0x05,                                       /* bFunctionLength */
  0x24,                                       /* bDescriptorType: CS_INTERFACE */
  0x01,                                       /* bDescriptorSubtype: Call Management Func Desc */
  0x00,                                       /* bmCapabilities: D0+D1 */
  0x01,                                       /* bDataInterface */
  //36
  /* ACM Functional Descriptor */
  0x04,                                       /* bFunctionLength */
  0x24,                                       /* bDescriptorType: CS_INTERFACE */
  0x02,                                       /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,                                       /* bmCapabilities */
  //40
  /* Union Functional Descriptor */
  0x05,                                       /* bFunctionLength */
  0x24,                                       /* bDescriptorType: CS_INTERFACE */
  0x06,                                       /* bDescriptorSubtype: Union func desc */
  0x00,                                       /* bMasterInterface: Communication class interface */
  0x01,                                       /* bSlaveInterface0: Data Class Interface */
  //45
  /* Endpoint 2 Descriptor */
  0x07,                                       /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
  CDC_CMD_EP,                                 /* bEndpointAddress */
  0x03,                                       /* bmAttributes: Interrupt */
  LOBYTE(CDC_CMD_PACKET_SIZE),                /* wMaxPacketSize */
  HIBYTE(CDC_CMD_PACKET_SIZE),
  CDC_FS_BINTERVAL,                           /* bInterval */
  /*---------------------------------------------------------------------------*/
  //52
  /* Data class interface descriptor */
  0x09,                                       /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_INTERFACE,                    /* bDescriptorType: */
  0x01,                                       /* bInterfaceNumber: Number of Interface */
  0x00,                                       /* bAlternateSetting: Alternate setting */
  0x02,                                       /* bNumEndpoints: Two endpoints used */
  0x0A,                                       /* bInterfaceClass: CDC */
  0x00,                                       /* bInterfaceSubClass */
  0x00,                                       /* bInterfaceProtocol */
  0x06,                                       /* iInterface */
  //61
  /* Endpoint OUT Descriptor */
  0x07,                                       /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
  CDC_OUT_EP,                                 /* bEndpointAddress */
  0x02,                                       /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),        /* wMaxPacketSize */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                                       /* bInterval */
  //68
  /* Endpoint IN Descriptor */
  0x07,                                       /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                     /* bDescriptorType: Endpoint */
  CDC_IN_EP,                                  /* bEndpointAddress */
  0x02,                                       /* bmAttributes: Bulk */
  LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),        /* wMaxPacketSize */
  HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
  0x00,                                        /* bInterval */
  //75
  /* Interface Association Descriptor: Mass Storage device */
  0x08,   /* bLength: IAD size */
  0x0B,   /* bDescriptorType: Interface Association Descriptor */
  0x02,   /* bFirstInterface */
  0x01,   /* bInterfaceCount */
  0x08,   /* bFunctionClass: */
  0x06,   /* bFunctionSubClass: */
  0x50,   /* bFunctionProtocol: */
  0x07,   /* iFunction */
  //83
  /********************  Mass Storage interface ********************/
  0x09,                                            /* bLength: Interface Descriptor size */
  0x04,                                            /* bDescriptorType: */
  0x02,                                            /* bInterfaceNumber: Number of Interface */
  0x00,                                            /* bAlternateSetting: Alternate setting */
  0x02,                                            /* bNumEndpoints */
  0x08,                                            /* bInterfaceClass: MSC Class */
  0x06,                                            /* bInterfaceSubClass : SCSI transparent*/
  0x50,                                            /* nInterfaceProtocol */
  0x07,                                            /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  //92
  0x07,                                            /* Endpoint descriptor length = 7 */
  0x05,                                            /* Endpoint descriptor type */
  MSC_EPIN_ADDR,                                   /* Endpoint address (IN, address 1) */
  0x02,                                            /* Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00,                                            /* Polling interval in milliseconds */
  //99
  0x07,                                            /* Endpoint descriptor length = 7 */
  0x05,                                            /* Endpoint descriptor type */
  MSC_EPOUT_ADDR,                                  /* Endpoint address (OUT, address 1) */
  0x02,                                            /* Bulk endpoint type */
  LOBYTE(MSC_MAX_FS_PACKET),
  HIBYTE(MSC_MAX_FS_PACKET),
  0x00                                             /* Polling interval in milliseconds */
  //106
};
__ALIGN_BEGIN static uint8_t USBD_CMPSIT_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

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
  USBD_CMPSIT_GetHSCfgDesc,
  USBD_CMPSIT_GetFSCfgDesc,  
  USBD_CMPSIT_GetOtherSpeedCfgDesc,
  USBD_CMPSIT_GetDeviceQualifierDescriptor  
};

#ifdef USE_USBD_COMPOSITE
void USBD_CMPSIT_AddClass(USBD_HandleTypeDef *pdev, USBD_ClassTypeDef *pclass, USBD_CompositeClassTypeDef classtype, uint8_t *EpAddr)
{
}
#endif

uint8_t * USBD_UsrStrDescriptor(struct _USBD_HandleTypeDef *pdev, uint8_t index,  uint16_t *length)
{
  *length = 0;
  printf("index=%d\r\n",index);
 /*  if (USBD_IDX_MICROSOFT_DESC_STR == index) {
    *length = sizeof (USBD_MS_OS_StringDescriptor);
    return USBD_MS_OS_StringDescriptor;
  } else if (USBD_IDX_ODRIVE_INTF_STR == index) {
    USBD_GetString((uint8_t *)USBD_PRODUCT_XSTR(NATIVE_STRING), USBD_StrDesc, length);
    return USBD_StrDesc;
  }  */
  if (USBD_IDX_CDC_INTF_STR == index) {
    USBD_GetString((uint8_t *)USBD_CDC_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
    return USBD_CMPSIT_StrDesc;
  }  
  else if (USBD_IDX_MSC_INTF_STR == index) {    
    USBD_GetString((uint8_t *)USBD_MSC_INTERFACE_STRING_FS, USBD_CMPSIT_StrDesc, length);
    return USBD_CMPSIT_StrDesc;
  }  
  return NULL;
}

static uint8_t *USBD_CMPSIT_GetFSCfgDesc(uint16_t *length)
{
	// printf("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
	*length = (uint16_t)sizeof(USBD_CMPSIT_CfgDesc);
	return USBD_CMPSIT_CfgDesc;
}
static uint8_t *USBD_CMPSIT_GetHSCfgDesc(uint16_t *length)
{
}
static uint8_t *USBD_CMPSIT_GetOtherSpeedCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_CMPSIT_CfgDesc);
  return USBD_CMPSIT_CfgDesc;
}
uint8_t *USBD_CMPSIT_GetDeviceQualifierDescriptor(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_CMPSIT_DeviceQualifierDesc);
  return USBD_CMPSIT_DeviceQualifierDesc;
}

