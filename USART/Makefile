TARGET=main.hex
EXECUTABLE=main.elf
BIN_IMAGE=main.bin

CC=arm-none-eabi-gcc
#LD=arm-none-eabi-ld 
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump

BIN=$(CP) -O ihex 

DEFS = -DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DMANGUSTA_DISCOVERY -DUSE_USB_OTG_FS -DHSE_VALUE=8000000

MCU = cortex-m4
MCFLAGS = -mcpu=$(MCU) -mthumb -mlittle-endian -std=c99
STM32_INCLUDES = -I./Utilities/STM32F4-Discovery \
	-I./Libraries/CMSIS/ST/STM32F4xx/Include/ \
	-I./Libraries/CMSIS/Include/ \
	-I./Libraries/STM32F4xx_StdPeriph_Driver/inc/ \
	-I./Libraries/STM32_USB_Device_Library/Class/hid/inc \
	-I./Libraries/STM32_USB_Device_Library/Core/inc/ \
	-I./Libraries/STM32_USB_OTG_Driver/inc/

OPTIMIZE       = -O0

CFLAGS	= $(MCFLAGS)  $(OPTIMIZE)  $(DEFS) -I./ -I./ $(STM32_INCLUDES)  -Wl,-T,stm32_flash.ld
#-mapcs-float use float regs. small increase in code size

STM32_USB_OTG_SRC = ./Libraries/STM32_USB_OTG_Driver/src/usb_dcd_int.c \
	./Libraries/STM32_USB_OTG_Driver/src/usb_core.c \
	./Libraries/STM32_USB_OTG_Driver/src/usb_dcd.c \

STM32_USB_DEVICE_SRC = ./Libraries/STM32_USB_Device_Library/Class/hid/src/usbd_hid_core.c \
	./Libraries/STM32_USB_Device_Library/Core/src/usbd_req.c \
	./Libraries/STM32_USB_Device_Library/Core/src/usbd_core.c \
	./Libraries/STM32_USB_Device_Library/Core/src/usbd_ioreq.c

SRC = main.c \
	system_stm32f4xx.c \
	./Libraries/STM32F4xx_StdPeriph_Driver/src/misc.c \
	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c \
#	./Utilities/STM32F4-Discovery/stm32f4_discovery_lis302dl.c \
#	./Utilities/STM32F4-Discovery/stm32f4_discovery_audio_codec.c \
#	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
#	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
#	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
#	./Libraries/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c \



STARTUP = startup_stm32f4xx.S

OBJDIR = .
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) 
OBJ += Startup.o

all: $(TARGET) $(BIN_IMAGE)
	

$(BIN_IMAGE): $(EXECUTABLE)
	$(CP) -O binary $^ $@

$(TARGET): $(EXECUTABLE)
	$(CP) -O ihex $^ $@

$(EXECUTABLE): $(SRC) $(STARTUP)
	$(CC) $(CFLAGS) $^ -lm -lc -o $@

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

clean:
	rm -rf $(TARGET) $(EXECUTABLE) $(BIN_IMAGE)

