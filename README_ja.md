[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver L3GD20H

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/l3gd20h/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

L3GD20Hは、低電力の3軸角速度センサーです。センシング素子と、デジタルインターフェース（I2C / SPI）を介して測定された角速度を外界に提供できるICインターフェースを備えています。センシング素子は、STが開発した専用のマイクロマシニングプロセスを使用して製造され、シリコン上に慣性センサーとアクチュエーターを製造します。 ICインターフェースはCMOSプロセスを使用して製造されており、高レベルの集積により、検出素子の特性によりよく一致するようにトリミングされた専用回路を設計できます。L3GD20Hのフルスケールは±245/±500/±2000dpsです。また、ユーザーが選択可能な帯域幅でレートを測定できます。ゲームや仮想現実の入力デバイス、MMI（マンマシンインターフェース）によるモーションコントロール、GPSナビゲーションシステム、GPSナビゲーションシステムなどで使用できます。

LibDriver L3GD20Hは、LibDriverによって起動されたL3GD20Hの全機能ドライバーであり、角速度の読み取り、角速度FIFOモードの取得、しきい値割り込みなどの機能を提供します。 LibDriverはMISRAに準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example fifo](#example-fifo)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver L3GD20Hのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver L3GD20H用のプラットフォームに依存しないIIC, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver L3GD20Hドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver L3GD20Hプログラミング例が含まれています。

/ docディレクトリには、LibDriver L3GD20Hオフラインドキュメントが含まれています。

/ datasheetディレクトリには、L3GD20Hデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIIC, SPIバステンプレートを参照して、指定したプラットフォームのIIC, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/l3gd20h/index.html](https://www.libdriver.com/docs/l3gd20h/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。