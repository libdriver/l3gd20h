[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver L3GD20H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/l3gd20h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

L3GD20H是一款低功耗的三軸陀螺儀。它通過數字接口IIC和SPI進行通信。傳感元件是ST專用微加工工藝生產的慣性傳感器和執行器矽片。 IC接口採用CMOS工藝製造。高級別集成的過程設計了一個專用電路，該電路被裁剪成更好地匹配傳感元件特性。 L3GD20H的滿量程為±245/±500/±2000 dps並且能夠配置可選擇的測量速率帶寬。該設備被用於遊戲和虛擬現實輸入設備、運動控制、GPS導航定位和機器人等。

LibDriver L3GD20H是LibDriver推出的L3GD20H的全功能驅動，該驅動提供角速度讀取、角速度FIFO模式採集、閾值中斷等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver L3GD20H的源文件。

/interface目錄包含了LibDriver L3GD20H與平台無關的IIC、SPI總線模板。

/test目錄包含了LibDriver L3GD20H驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver L3GD20H編程範例。

/doc目錄包含了LibDriver L3GD20H離線文檔。

/datasheet目錄包含了L3GD20H數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC、SPI總線模板，完成指定平台的IIC、SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example basic

```C
#include "driver_l3gd20h_basic.h"

uint8_t res;
float dps[3];

res = l3gd20h_basic_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0);
if (res != 0)
{
    return 1;
}

...

for (i = 0; i < 3; i++)
{
    res = l3gd20h_basic_read((float *)dps);
    if (res != 0)
    {
        (void)l3gd20h_basic_deinit();

        return 1;
    }
    l3gd20h_interface_debug_print("l3gd20h: x %0.2f dps.\n", dps[0]);
    l3gd20h_interface_debug_print("l3gd20h: y %0.2f dps.\n", dps[1]);
    l3gd20h_interface_debug_print("l3gd20h: z %0.2f dps.\n", dps[2]);
    l3gd20h_interface_delay_ms(1000);
    
    ...
    
}

...

(void)l3gd20h_basic_deinit();

return 0;
```

#### example fifo

```C
#include "driver_l3gd20h_fifo.h"

uint8_t res;

static void a_l3gd20h_fifo_receive_callback(float (*dps)[3], uint16_t len)
{
    ...
        
    return 0;
}

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}

res = l3gd20h_fifo_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0, a_l3gd20h_fifo_receive_callback);
if (res != 0)
{
    (void)l3gd20h_fifo_deinit();
    (void)gpio_interrupt_deinit();

    return 1;
}

...

while (times != 0)
{

...
    
}

...

(void)l3gd20h_fifo_deinit();
(void)gpio_interrupt_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_l3gd20h_interrupt.h"

uint8_t res;

static void a_l3gd20h_interrupt_receive_callback(uint8_t type)
{
    switch (type)
    {
        case L3GD20H_INTERRUPT1_Z_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq z high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_Y_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq y high threshold.\n");
            
            break;
        }
        case L3GD20H_INTERRUPT1_X_HIGH :
        {
            l3gd20h_interface_debug_print("l3gd20h: irq x high threshold.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}

res = l3gd20h_interrupt_init(L3GD20H_INTERFACE_IIC, L3GD20H_ADDRESS_SDO_0, 20.f, a_l3gd20h_interrupt_receive_callback);
if (res != 0)
{
    (void)l3gd20h_interrupt_deinit();
    (void)gpio_interrupt_deinit();
    
    return 1;
}

...

while (1)
{
    
...
    
}

...

(void)l3gd20h_interrupt_deinit();
(void)gpio_interrupt_deinit();

return 0;
```
### 文檔

在線文檔: [https://www.libdriver.com/docs/l3gd20h/index.html](https://www.libdriver.com/docs/l3gd20h/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。