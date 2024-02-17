[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver L3GD20H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/l3gd20h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

L3GD20H는 저전력 3축 각속도 센서입니다. 여기에는 디지털 인터페이스(I2C/SPI)를 통해 측정된 각속도를 외부 세계에 제공할 수 있는 감지 요소와 IC 인터페이스가 포함됩니다. 감지 요소는 실리콘에서 관성 센서 및 액추에이터를 생산하기 위해 ST에서 개발한 전용 미세 가공 프로세스를 사용하여 제조됩니다. 웨이퍼. IC 인터페이스는 감지 요소 특성과 더 잘 일치하도록 트리밍된 전용 회로를 설계하기 위해 높은 수준의 통합을 허용하는 CMOS 프로세스를 사용하여 제조됩니다. L3GD20H는 ±245/±500/±2000dps의 전체 스케일을 갖습니다 사용자가 선택 가능한 대역폭으로 속도를 측정할 수 있습니다. 게임 및 가상 현실 입력 장치, MMI(인간-기계 인터페이스)를 사용한 모션 제어, GPS 탐색 시스템, GPS 탐색 시스템 등에 사용할 수 있습니다.

LibDriver L3GD20H는 LibDriver에서 출시한 L3GD20H의 전체 기능 드라이버입니다. 각속도 판독, 각속도 FIFO 모드 획득, 임계값 인터럽트 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver L3GD20H의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver L3GD20H용 플랫폼 독립적인 IIC, SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver L3GD20H드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver L3GD20H프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver L3GD20H오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 L3GD20H데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC, SPI 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC, SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

```c
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

### 문서

온라인 문서: [https://www.libdriver.com/docs/l3gd20h/index.html](https://www.libdriver.com/docs/l3gd20h/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.