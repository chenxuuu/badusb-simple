/****************************************************************************
软件程序设计
	
 	main.c
		该函数中主要初始化系统以及USB接口。
	usb_desc.c
		该文件中没有任何函数，只是包含一些定义USB设备的描述符常数，读者若要开发非标准的USB
		设备，则还需要开发PC上运行的非标准设备的驱动程序。
 	stm32f10x_it.c
		该文件中包含USB中断服务程序，由于USB中断有很多情况，这里的中断服务程序只是调
		用usb_Istr.c文件中的USB_Istr函数，由USB_Istr函数再做轮询处理。参考程序如下：
 	usb_Istr.c
		该文件中只有一个函数，即USB中断的USB_Istr函数，该函数对各类引起USB中断的事件作轮询
		处理。参考程序如下: 
 	usb_prop.c
		该文件用于实现相关设备的USB协议，例如初始化、SETUP包、IN包、OUT包等等。
 	usb_pwr.c
		该文件中包含处理上电、调电、挂起和恢复事件的函数，
 	hw_config.c
		该文件中包含系统配置的函数。其中，Set_System函数用于配置时钟、通用端口；
		Set_USBClock函数用于配置USB端口时钟；USB_Interrupts_Config函数用于配置USB中断；
		USB_Cable_Config函数配置USB电缆状态。

