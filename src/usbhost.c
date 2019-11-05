#include "defines.h"
#include "usbhost.h"

STATIC_UBUF(tmpBuf, 1024);
static USBH_Device_TypeDef device;
static USBH_Ep_TypeDef ep[1];
static USB_EndpointDescriptor_TypeDef *retval;
static USB_EndpointDescriptor_TypeDef checker;

static unsigned char readbuffer[4 * USB_OUTPUT_SIZE] = {0};

bool USBConnect()
{
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	int connectionResult = 1;
	USBH_Init(&is);
#ifndef DEVICE_SADIE
	SegmentLCD_Write("USB IN");
#endif
	while (connectionResult != USB_STATUS_OK) {
		connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 10);
		if ( connectionResult == USB_STATUS_OK ) {
#ifndef DEVICE_SADIE
	    	SegmentLCD_Write("Device");
	    	USBTIMER_DelayMs(500);
	    	SegmentLCD_Write("Added");
	    	USBTIMER_DelayMs(500);
#endif
	    	if (USBH_QueryDeviceB(tmpBuf, sizeof(tmpBuf), USBH_GetPortSpeed())
	          == USB_STATUS_OK) {
	    		USBH_InitDeviceData(&device, tmpBuf, ep, 1, USBH_GetPortSpeed());
	    		int testy = USBH_AssignHostChannel(ep, 2);
	    		retval = USBH_QGetEndpointDescriptor(tmpBuf, 0, 1, 0); 	// This can be removed?
	    		checker = *retval;										// And this?
	    	} else {
	    	}
		}
	}
#ifndef DEVICE_SADIE
	SegmentLCD_Write("meg");
#endif
	return true;
}

bool USBIsConnected(){
	return USBH_DeviceConnected();
}

USB_output USBWaitForData(){
	readbuffer[0] = 0;
	while(readbuffer[0] == 0){ // Nullertull
		USBH_ReadB(device.ep, readbuffer, USB_OUTPUT_SIZE, 0);
	}
	USB_output out;
	for(int i = 0; i < USB_OUTPUT_SIZE; i++){
		out.data[i] = readbuffer[i];
	}
	return out;
}
