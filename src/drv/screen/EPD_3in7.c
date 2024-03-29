/*****************************************************************************
* | File        :   EPD_3IN7.C
* | Author      :   Waveshare team
* | Function    :   3.7inch e-paper
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2020-08-10
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_3in7.h"
#include "debug.h"

static const UBYTE lut_4Gray_GC[] = {
    0x2A, 0x06, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 1
    0x28, 0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 2
    0x20, 0x06, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 3
    0x14, 0x06, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 4
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 5
    0x00, 0x02, 0x02, 0x0A, 0x00, 0x00, 0x00, 0x08, 0x08, 0x02,// 6
    0x00, 0x02, 0x02, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 10
    0x22, 0x22, 0x22, 0x22, 0x22};

static const UBYTE lut_1Gray_GC[] = {
    0x2A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 1
    0x05, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 2
    0x2A, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 3
    0x05, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 4
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 5
    0x00, 0x02, 0x03, 0x0A, 0x00, 0x02, 0x06, 0x0A, 0x05, 0x00,// 6
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 10
    0x22, 0x22, 0x22, 0x22, 0x22};

static const UBYTE lut_1Gray_DU[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 1
    0x01, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0A, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 3
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 5
    0x00, 0x00, 0x05, 0x05, 0x00, 0x05, 0x03, 0x05, 0x05, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x22, 0x22, 0x22, 0x22};

static const UBYTE lut_1Gray_A2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 1
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 2
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 3
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 4
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 5
    0x00, 0x00, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 6
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 7
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 9
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,// 10
    0x22, 0x22, 0x22, 0x22, 0x22};

/******************************************************************************
function :  Software reset
parameter:
******************************************************************************/
static void EPD_3IN7_Reset(void) {
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(50);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(3);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(50);
}

/******************************************************************************
function :  send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_3IN7_SendCommand(UBYTE Reg) {
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :  send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_3IN7_SendData(UBYTE Data) {
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_3IN7_ReadBusy_HIGH(void) {
    // debug("e-Paper busy\r\n");
    UBYTE busy;
    do {
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
    } while (busy);
    // debug("e-Paper busy release\r\n");
}

static void EPD_3IN7_common_init(void) {
    EPD_3IN7_Reset();

    EPD_3IN7_SendCommand(EPD_CMD_SW_RESET);
    DEV_Delay_ms(10);

    EPD_3IN7_SendCommand(0x46);
    EPD_3IN7_SendData(0xF7);
    EPD_3IN7_ReadBusy_HIGH();
    EPD_3IN7_SendCommand(0x47);
    EPD_3IN7_SendData(0xF7);
    EPD_3IN7_ReadBusy_HIGH();

    EPD_3IN7_SendCommand(EPD_CMD_DRV_OUTPUT_CTRL);// setting gate number
    EPD_3IN7_SendData(0xDF);
    EPD_3IN7_SendData(0x01);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_GATE_DRV_CTRL);// set gate voltage
    EPD_3IN7_SendData(vgh_20);

    EPD_3IN7_SendCommand(EPD_CMD_SRC_DRV_VOLT_CTRL);// set source voltage
    EPD_3IN7_SendData(vshx_15);                     // VSH1
    EPD_3IN7_SendData(vshx_5);                      // VSH2
    EPD_3IN7_SendData(vsl_minus_15);

    EPD_3IN7_SendCommand(EPD_CMD_DATA_ENTRY);// set data entry sequence
    EPD_3IN7_SendData(y_increment_x_increment);

    EPD_3IN7_SendCommand(EPD_CMD_BORDER_WAVEFORM);// set border
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(
        EPD_CMD_BOOSTER_SOFT_START_CTRL);// set booster strength, level 2
    EPD_3IN7_SendData(0xAE);
    EPD_3IN7_SendData(0xC7);
    EPD_3IN7_SendData(0xC3);
    EPD_3IN7_SendData(0xC0);
    EPD_3IN7_SendData(0x80);

    EPD_3IN7_SendCommand(EPD_CMD_TEMP_SENS);// set internal sensor on
    EPD_3IN7_SendData(0x80);

    EPD_3IN7_SendCommand(EPD_CMD_VCOM_CONF_3);// set vcom value
    EPD_3IN7_SendData(vcom_minus_1_7);

    EPD_3IN7_SendCommand(
        EPD_CMD_RAM_X_POS); // setting X direction start/end position of RAM
    EPD_3IN7_SendData(0x00);// 0
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x17);// 279
    EPD_3IN7_SendData(0x01);

    EPD_3IN7_SendCommand(
        EPD_CMD_RAM_Y_POS); // setting Y direction start/end position of RAM
    EPD_3IN7_SendData(0x00);// 0
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0xDF);// 479
    EPD_3IN7_SendData(0x01);
}

/******************************************************************************
function :  set the look-up tables
parameter:
******************************************************************************/
void EPD_3IN7_Load_LUT(UBYTE lut) {
    UWORD i;
    EPD_3IN7_SendCommand(EPD_CMD_WRITE_LUT);
    for (i = 0; i < 105; i++) {
        if (lut == 0) {
            EPD_3IN7_SendData(lut_4Gray_GC[i]);
        } else if (lut == 1) {
            EPD_3IN7_SendData(lut_1Gray_GC[i]);
        } else if (lut == 2) {
            EPD_3IN7_SendData(lut_1Gray_DU[i]);
        } else if (lut == 3) {
            EPD_3IN7_SendData(lut_1Gray_A2[i]);
        } else {
            debug("There is no such lut \r\n");
        }
    }
}

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN7_4Gray_Init(void) {
    EPD_3IN7_common_init();

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_DISP_OPT);// set display option, these
                                                 // setting turn on previous
                                                 // function
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_DISP_UPT_CTRL_2);// Display Update Control 2
    EPD_3IN7_SendData(0xC7);
}

/******************************************************************************
function :  Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN7_1Gray_Init(void) {
    EPD_3IN7_common_init();

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_DISP_OPT);// set display option, these
                                                 // setting turn on previous
                                                 // function
    EPD_3IN7_SendData(0x00);                     // can switch 1 gray or 4 gray
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0x4F);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);
    EPD_3IN7_SendData(0xFF);

    EPD_3IN7_SendCommand(EPD_CMD_DISP_UPT_CTRL_2);// Display Update Control 2
    EPD_3IN7_SendData(0xCF);
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void EPD_3IN7_4Gray_Clear(void) {
    UWORD Width, Height;
    Width = (EPD_3IN7_WIDTH % 8 == 0) ? (EPD_3IN7_WIDTH / 8)
                                      : (EPD_3IN7_WIDTH / 8 + 1);
    Height = EPD_3IN7_HEIGHT;

    EPD_3IN7_SendCommand(0x49);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_BW);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7_SendData(0xff);
        }
    }

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_GREY);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_3IN7_SendData(0xff);
        }
    }

    EPD_3IN7_Load_LUT(0);
    EPD_3IN7_SendCommand(EPD_CMD_DISP_UPT_CTRL_2);
    EPD_3IN7_SendData(0xC7);

    EPD_3IN7_SendCommand(EPD_CMD_UPDATE_SCREEN);
    EPD_3IN7_ReadBusy_HIGH();
}

/******************************************************************************
function :  Clear screen
parameter:
******************************************************************************/
void EPD_3IN7_1Gray_Clear(void) {
    UWORD i;
    UWORD IMAGE_COUNTER = EPD_3IN7_WIDTH * EPD_3IN7_HEIGHT / 8;

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_BW);
    for (i = 0; i < IMAGE_COUNTER; i++) {
        EPD_3IN7_SendData(0xff);
    }

    EPD_3IN7_Load_LUT(1);

    EPD_3IN7_SendCommand(EPD_CMD_UPDATE_SCREEN);
    EPD_3IN7_ReadBusy_HIGH();
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN7_4Gray_Display(const UBYTE *Image) {
    UDOUBLE i, j, k;
    UBYTE temp1, temp2, temp3;

    EPD_3IN7_SendCommand(0x49);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_BW);
    for (i = 0; i < 16800; i++) {
        temp3 = 0;
        for (j = 0; j < 2; j++) {
            temp1 = Image[i * 2 + j];
            for (k = 0; k < 2; k++) {
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)
                    temp3 |= 0x01;// white
                else if (temp2 == 0x00)
                    temp3 |= 0x00;// black
                else if (temp2 == 0x80)
                    temp3 |= 0x00;// gray1
                else              // 0x40
                    temp3 |= 0x01;// gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)// white
                    temp3 |= 0x01;
                else if (temp2 == 0x00)// black
                    temp3 |= 0x00;
                else if (temp2 == 0x80)
                    temp3 |= 0x00;// gray1
                else              // 0x40
                    temp3 |= 0x01;// gray2
                if (j != 1 || k != 1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_3IN7_SendData(temp3);
    }
    // new  data
    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_GREY);
    for (i = 0; i < 16800; i++) {
        temp3 = 0;
        for (j = 0; j < 2; j++) {
            temp1 = Image[i * 2 + j];
            for (k = 0; k < 2; k++) {
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)
                    temp3 |= 0x01;// white
                else if (temp2 == 0x00)
                    temp3 |= 0x00;// black
                else if (temp2 == 0x80)
                    temp3 |= 0x01;// gray1
                else              // 0x40
                    temp3 |= 0x00;// gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1 & 0xC0;
                if (temp2 == 0xC0)// white
                    temp3 |= 0x01;
                else if (temp2 == 0x00)// black
                    temp3 |= 0x00;
                else if (temp2 == 0x80)
                    temp3 |= 0x01;// gray1
                else              // 0x40
                    temp3 |= 0x00;// gray2
                if (j != 1 || k != 1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_3IN7_SendData(temp3);
    }

    EPD_3IN7_Load_LUT(0);

    EPD_3IN7_SendCommand(EPD_CMD_DISP_UPT_CTRL_2);
    EPD_3IN7_SendData(0xC7);

    EPD_3IN7_SendCommand(EPD_CMD_UPDATE_SCREEN);

    EPD_3IN7_ReadBusy_HIGH();
}

/******************************************************************************
function :  Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_3IN7_1Gray_Display(const UBYTE *Image) {
    UWORD i;
    UWORD IMAGE_COUNTER = EPD_3IN7_WIDTH * EPD_3IN7_HEIGHT / 8;

    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendCommand(EPD_CMD_SET_RAM_X_POS);
    EPD_3IN7_SendData(0x00);
    EPD_3IN7_SendData(0x00);

    EPD_3IN7_SendCommand(EPD_CMD_WRITE_RAM_BW);
    for (i = 0; i < IMAGE_COUNTER; i++) {
        EPD_3IN7_SendData(Image[i]);
    }

    EPD_3IN7_SendCommand(EPD_CMD_DISP_UPT_CTRL_2);
    EPD_3IN7_SendData(0xCF);

    EPD_3IN7_Load_LUT(1);
    EPD_3IN7_SendCommand(EPD_CMD_UPDATE_SCREEN);
    EPD_3IN7_ReadBusy_HIGH();
}

/******************************************************************************
function :  Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN7_Sleep(void) {
    EPD_3IN7_SendCommand(0X50);
    EPD_3IN7_SendData(0xf7);
    EPD_3IN7_SendCommand(0X02);// power off
    EPD_3IN7_SendCommand(0X07);// deep sleep
    EPD_3IN7_SendData(0xA5);
}
