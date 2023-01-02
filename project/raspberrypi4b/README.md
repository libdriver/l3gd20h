### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(l3gd20h REQUIRED)
```

### 3. L3GD20H

#### 3.1 Command Instruction

1. Show l3gd20h chip and driver information.

   ```shell
   l3gd20h (-i | --information)
   ```

2. Show l3gd20h help.

   ```shell
   l3gd20h (-h | --help)
   ```

3. Show l3gd20h pin connections of the current board.

   ```shell
   l3gd20h (-p | --port)
   ```

4. run l3gd20h register test.

   ```shell
   l3gd20h (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

5. Run l3gd20h read test, num means the test times.

   ```shell
   l3gd20h (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

6. Run l3gd20h fifo test.

   ```shell
   l3gd20h (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

7. Run l3gd20h interrupt test.

   ```shell
   l3gd20h (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]
   ```

8. Run l3gd20h basic function, num is the read times.

   ```shell
   l3gd20h (-e read | --example=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
   ```

9. Run l3gd20h fifo function, num is the read times, ms is the timeout in ms.

   ```shell
   l3gd20h (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>] [--timeout=<ms>]
   ```

10. Run l3gd20h interrupt function, th is the interrupt threshold, ms is the timeout in ms.

    ```shell
    l3gd20h (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--threshold=<th>] [--timeout=<ms>]
    ```

#### 3.2 Command Example

```shell
./l3gd20h -i

l3gd20h: chip is STMicroelectronic L3GD20H.
l3gd20h: manufacturer is STMicroelectronic.
l3gd20h: interface is IIC SPI.
l3gd20h: driver version is 2.0.
l3gd20h: min supply voltage is 2.2V.
l3gd20h: max supply voltage is 3.6V.
l3gd20h: max current is 5.00mA.
l3gd20h: max temperature is 85.0C.
l3gd20h: min temperature is -40.0C.
```

```shell
./l3gd20h -p

l3gd20h: SPI interface SCK connected to GPIO11(BCM).
l3gd20h: SPI interface MISO connected to GPIO9(BCM).
l3gd20h: SPI interface MOSI connected to GPIO10(BCM).
l3gd20h: SPI interface CS connected to GPIO8(BCM).
l3gd20h: IIC interface SCL connected to GPIO3(BCM).
l3gd20h: IIC interface SDA connected to GPIO2(BCM).
l3gd20h: INT connected to GPIOB17(BCM).
```

```shell
./l3gd20h -t reg --interface=spi

l3gd20h: chip is STMicroelectronic L3GD20H.
l3gd20h: manufacturer is STMicroelectronic.
l3gd20h: interface is IIC SPI.
l3gd20h: driver version is 2.0.
l3gd20h: min supply voltage is 2.2V.
l3gd20h: max supply voltage is 3.6V.
l3gd20h: max current is 5.00mA.
l3gd20h: max temperature is 85.0C.
l3gd20h: min temperature is -40.0C.
l3gd20h: start register test.
l3gd20h: l3gd20h_set_interface/l3gd20h_get_interface test.
l3gd20h: set interface iic.
l3gd20h: check interface ok.
l3gd20h: set interface spi.
l3gd20h: check interface ok.
l3gd20h: l3gd20h_set_addr_pin/l3gd20h_get_addr_pin test.
l3gd20h: set addr pin sdo0.
l3gd20h: check addr pin ok.
l3gd20h: set addr pin sdo1.
l3gd20h: check addr pin ok.
l3gd20h: l3gd20h_set_mode/l3gd20h_get_mode test.
l3gd20h: set power down mode.
l3gd20h: check mode ok.
l3gd20h: set sleep mode.
l3gd20h: check mode ok.
l3gd20h: set normal mode.
l3gd20h: check mode ok.
l3gd20h: l3gd20h_set_axis/l3gd20h_get_axis test.
l3gd20h: set axis x false.
l3gd20h: check axis ok.
l3gd20h: set axis x true.
l3gd20h: check axis ok.
l3gd20h: set axis y false.
l3gd20h: check axis ok.
l3gd20h: set axis y true.
l3gd20h: check axis ok.
l3gd20h: set axis z false.
l3gd20h: check axis ok.
l3gd20h: set axis z true.
l3gd20h: check axis ok.
l3gd20h: l3gd20h_set_rate_bandwidth/l3gd20h_get_rate_bandwidth test.
l3gd20h: set rate 100Hz bandwidth 12.5Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 100Hz bandwidth 25Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 100Hz bandwidth 25Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 100Hz bandwidth 25Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 200Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 200Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 200Hz bandwidth 70Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 400Hz bandwidth 20Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 400Hz bandwidth 25Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 400Hz bandwidth 50Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 400Hz bandwidth 110Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 800Hz bandwidth 30Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 800Hz bandwidth 35Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 800Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set rate 800Hz bandwidth 100Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 12.5Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 12.5Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 12.5Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 12.5Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 25Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 25Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 25Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 25Hz bandwidth NA.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: set low power rate 50Hz bandwidth 16.6Hz.
l3gd20h: check rate bandwidth ok.
l3gd20h: l3gd20h_set_edge_trigger/l3gd20h_get_edge_trigger test.
l3gd20h: enable edge trigger.
l3gd20h: check edge trigger ok.
l3gd20h: disable edge trigger.
l3gd20h: check edge trigger ok.
l3gd20h: l3gd20h_set_level_trigger/l3gd20h_get_level_trigger test.
l3gd20h: enable level trigger.
l3gd20h: check level trigger ok.
l3gd20h: disable level trigger.
l3gd20h: check level trigger ok.
l3gd20h: l3gd20h_set_high_pass_filter_mode/l3gd20h_get_high_pass_filter_mode test.
l3gd20h: set high pass filter normal reset mode.
l3gd20h: check high pass filter mode ok.
l3gd20h: set high pass filter refernce signal mode.
l3gd20h: check high pass filter mode ok.
l3gd20h: set high pass filter normal mode.
l3gd20h: check high pass filter mode ok.
l3gd20h: set high pass filter autoreset on int mode.
l3gd20h: check high pass filter mode ok.
l3gd20h: l3gd20h_set_high_pass_filter_cut_off_frequency/l3gd20h_get_high_pass_filter_cut_off_frequency test.
l3gd20h: set high pass filter cut off frequency 0.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 1.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 2.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 3.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 4.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 5.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 6.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 7.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 8.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: set high pass filter cut off frequency 9.
l3gd20h: check high pass filter cut off frequency ok.
l3gd20h: l3gd20h_set_block_data_update/l3gd20h_get_block_data_update test.
l3gd20h: enable block data update.
l3gd20h: check block data update ok.
l3gd20h: disable block data update.
l3gd20h: check block data update ok.
l3gd20h: l3gd20h_set_data_format/l3gd20h_get_data_format test.
l3gd20h: set data format big endian.
l3gd20h: check data format ok.
l3gd20h: set data format little endian.
l3gd20h: check data format ok.
l3gd20h: l3gd20h_set_full_scale/l3gd20h_get_full_scale test.
l3gd20h: set full scale ± 245 dps.
l3gd20h: check full scale ok.
l3gd20h: set full scale ± 500 dps.
l3gd20h: check full scale ok.
l3gd20h: set full scale ± 2000 dps.
l3gd20h: check full scale ok.
l3gd20h: l3gd20h_set_level_sensitive_latched/l3gd20h_get_level_sensitive_latched test.
l3gd20h: disable level sensitive latched.
l3gd20h: check level sensitive latched ok.
l3gd20h: enable level sensitive latched.
l3gd20h: check level sensitive latched ok.
l3gd20h: l3gd20h_set_self_test/l3gd20h_get_self_test test.
l3gd20h: set self test 0.
l3gd20h: check self test ok.
l3gd20h: set self test 1.
l3gd20h: check self test ok.
l3gd20h: set self test normal.
l3gd20h: check self test ok.
l3gd20h: l3gd20h_set_high_pass_filter/l3gd20h_get_high_pass_filter test.
l3gd20h: disable high pass filter.
l3gd20h: check high pass filter ok.
l3gd20h: enable high pass filter.
l3gd20h: check high pass filter ok.
l3gd20h: l3gd20h_set_out_selection/l3gd20h_get_out_selection test.
l3gd20h: set out selection LPF1.
l3gd20h: check out selection ok.
l3gd20h: set out selection LPF1-HPF.
l3gd20h: check out selection ok.
l3gd20h: set out selection LPF1-HPF-LPF2.
l3gd20h: check out selection ok.
l3gd20h: l3gd20h_set_high_pass_filter_reference/l3gd20h_get_high_pass_filter_reference test.
l3gd20h: set high pass filter reference 0x67.
l3gd20h: check high pass filter reference ok.
l3gd20h: l3gd20h_set_iic/l3gd20h_get_iic test.
l3gd20h: disable iic.
l3gd20h: check iic ok.
l3gd20h: enable iic.
l3gd20h: check iic ok.
l3gd20h: l3gd20h_set_interrupt1/l3gd20h_get_interrupt1 test.
l3gd20h: enable interrupt1.
l3gd20h: check interrupt1 ok.
l3gd20h: disable interrupt1.
l3gd20h: check interrupt1 ok.
l3gd20h: l3gd20h_set_boot_on_interrupt1/l3gd20h_get_boot_on_interrupt1 test.
l3gd20h: enable boot on interrupt1.
l3gd20h: check boot on interrupt1 ok.
l3gd20h: disable boot on interrupt1.
l3gd20h: check boot on interrupt1 ok.
l3gd20h: l3gd20h_set_interrupt_active_level/l3gd20h_get_interrupt_active_level test.
l3gd20h: set interrupt active level high.
l3gd20h: check interrupt active level ok.
l3gd20h: set interrupt active level low.
l3gd20h: check interrupt active level ok.
l3gd20h: l3gd20h_set_interrupt_pin_type/l3gd20h_get_interrupt_pin_type test.
l3gd20h: set push pull pin type.
l3gd20h: check interrupt pin type ok.
l3gd20h: set open drain pin type.
l3gd20h: check interrupt pin type ok.
l3gd20h: l3gd20h_set_data_ready_on_interrupt2/l3gd20h_get_data_ready_on_interrupt2 test.
l3gd20h: disable data ready on interrupt2.
l3gd20h: check data ready on interrupt2 ok.
l3gd20h: enable data ready on interrupt2.
l3gd20h: check data ready on interrupt2 ok.
l3gd20h: l3gd20h_set_fifo_threshold_on_interrupt2/l3gd20h_get_fifo_threshold_on_interrupt2 test.
l3gd20h: disable fifo threshold on interrupt2.
l3gd20h: check fifo threshold on interrupt2 ok.
l3gd20h: enable fifo threshold on interrupt2.
l3gd20h: check fifo threshold on interrupt2 ok.
l3gd20h: l3gd20h_set_fifo_overrun_on_interrupt2/l3gd20h_get_fifo_overrun_on_interrupt2 test.
l3gd20h: enable fifo overrun on interrupt2.
l3gd20h: check fifo overrun on interrupt2 ok.
l3gd20h: disable fifo overrun on interrupt2.
l3gd20h: check fifo overrun on interrupt2 ok.
l3gd20h: l3gd20h_set_fifo_empty_on_interrupt2/l3gd20h_get_fifo_empty_on_interrupt2 test.
l3gd20h: enable fifo empty on interrupt2.
l3gd20h: check fifo empty on interrupt2 ok.
l3gd20h: disable fifo empty on interrupt2.
l3gd20h: check fifo empty on interrupt2 ok.
l3gd20h: l3gd20h_set_interrupt_selection/l3gd20h_get_interrupt_selection test.
l3gd20h: set interrupt selection LPF1.
l3gd20h: check interrupt selection ok.
l3gd20h: set interrupt selection LPF1-HPF.
l3gd20h: check interrupt selection ok.
l3gd20h: set interrupt selection LPF1-HPF-LPF2.
l3gd20h: check interrupt selection ok.
l3gd20h: l3gd20h_set_interrupt_event/l3gd20h_get_interrupt_event test.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_AND_OR_COMBINATION.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_LATCH.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_LATCH.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Z_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Z_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Y_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_Y_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_X_HIGH_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: enable interrupt event L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: disable interrupt event L3GD20H_INTERRUPT_EVENT_X_LOW_EVENT.
l3gd20h: check interrupt event ok.
l3gd20h: l3gd20h_set_x_interrupt_threshold/l3gd20h_get_x_interrupt_threshold test.
l3gd20h: set x interrupt threshold 9158.
l3gd20h: check x interrupt threshold ok.
l3gd20h: l3gd20h_set_y_interrupt_threshold/l3gd20h_get_y_interrupt_threshold test.
l3gd20h: set y interrupt threshold 6249.
l3gd20h: check y interrupt threshold ok.
l3gd20h: l3gd20h_set_z_interrupt_threshold/l3gd20h_get_z_interrupt_threshold test.
l3gd20h: set z interrupt threshold 18547.
l3gd20h: check z interrupt threshold ok.
l3gd20h: l3gd20h_interrupt_threshold_convert_to_register/l3gd20h_interrupt_threshold_convert_to_data test.
l3gd20h: ±245 dps full scale.
l3gd20h: 179.3000 to 209.1775 with reg 0x23906.
l3gd20h: ±500 dps full scale.
l3gd20h: 33.5000 to 38.3075 with reg 0x2189.
l3gd20h: ±2000 dps full scale.
l3gd20h: 138.6000 to 159.0400 with reg 0x2272.
l3gd20h: l3gd20h_set_counter_mode/l3gd20h_get_counter_mode test.
l3gd20h: set decrement counter mode.
l3gd20h: check counter mode ok.
l3gd20h: set reset counter mode.
l3gd20h: check counter mode ok.
l3gd20h: l3gd20h_set_wait/l3gd20h_get_wait test.
l3gd20h: enable wait.
l3gd20h: check wait ok.
l3gd20h: disable wait.
l3gd20h: check wait ok.
l3gd20h: l3gd20h_set_duration/l3gd20h_get_duration test.
l3gd20h: set duration 0x6C.
l3gd20h: check duration ok.
l3gd20h: l3gd20h_set_data_ready_active_level/l3gd20h_get_data_ready_active_level test.
l3gd20h: set data ready active level high.
l3gd20h: check data ready active level ok.
l3gd20h: set data ready active level low.
l3gd20h: check data ready active level ok.
l3gd20h: l3gd20h_set_fifo/l3gd20h_get_fifo test.
l3gd20h: enable fifo.
l3gd20h: check fifo ok.
l3gd20h: disable fifo.
l3gd20h: check fifo ok.
l3gd20h: l3gd20h_set_stop_on_fifo_threshold/l3gd20h_get_stop_on_fifo_threshold test.
l3gd20h: enable stop on fifo threshold.
l3gd20h: check stop on fifo threshold ok.
l3gd20h: disable stop on fifo threshold.
l3gd20h: check stop on fifo threshold ok.
l3gd20h: l3gd20h_set_fifo_mode/l3gd20h_get_fifo_mode test.
l3gd20h: set fifo mode bypass.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode fifo.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode stream.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode stream to fifo.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode bypass to stream.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode dynamic stream.
l3gd20h: check fifo mode ok.
l3gd20h: set fifo mode bypass to fifo.
l3gd20h: check fifo mode ok.
l3gd20h: l3gd20h_set_fifo_threshold/l3gd20h_get_fifo_threshold test.
l3gd20h: set fifo threshold 17.
l3gd20h: check fifo threshold ok.
l3gd20h: l3gd20h_get_fifo_level test.
l3gd20h: fifo level is 0.
l3gd20h: l3gd20h_get_status test.
l3gd20h: status is 15.
l3gd20h: l3gd20h_get_interrupt_source test.
l3gd20h: interrupt source is 0x42.
l3gd20h: l3gd20h_set_boot/l3gd20h_get_boot test.
l3gd20h: set normal boot.
l3gd20h: check boot ok.
l3gd20h: set reboot.
l3gd20h: check boot ok.
l3gd20h: l3gd20h_soft_reset test.
l3gd20h: soft reset.
l3gd20h: finish register test.
```

```shell
./l3gd20h -t read --interface=spi --times=3

l3gd20h: chip is STMicroelectronic L3GD20H.
l3gd20h: manufacturer is STMicroelectronic.
l3gd20h: interface is IIC SPI.
l3gd20h: driver version is 2.0.
l3gd20h: min supply voltage is 2.2V.
l3gd20h: max supply voltage is 3.6V.
l3gd20h: max current is 5.00mA.
l3gd20h: max temperature is 85.0C.
l3gd20h: min temperature is -40.0C.
l3gd20h: start read test.
l3gd20h: set full scale 245 dps.
l3gd20h: x -0.02 dps.
l3gd20h: y -0.06 dps.
l3gd20h: z -0.08 dps.
l3gd20h: x -0.04 dps.
l3gd20h: y -0.06 dps.
l3gd20h: z -0.04 dps.
l3gd20h: x -0.03 dps.
l3gd20h: y -0.09 dps.
l3gd20h: z -0.11 dps.
l3gd20h: set full scale 500 dps.
l3gd20h: x -0.02 dps.
l3gd20h: y -0.02 dps.
l3gd20h: z -0.04 dps.
l3gd20h: x 0.02 dps.
l3gd20h: y 0.07 dps.
l3gd20h: z 0.05 dps.
l3gd20h: x -0.02 dps.
l3gd20h: y -0.02 dps.
l3gd20h: z -0.02 dps.
l3gd20h: set full scale 2000 dps.
l3gd20h: x -0.07 dps.
l3gd20h: y -0.21 dps.
l3gd20h: z -0.07 dps.
l3gd20h: x -5.25 dps.
l3gd20h: y -7.00 dps.
l3gd20h: z 3.15 dps.
l3gd20h: x 0.00 dps.
l3gd20h: y 0.00 dps.
l3gd20h: z 0.00 dps.
l3gd20h: set big endian.
l3gd20h: x -0.14 dps.
l3gd20h: y -0.14 dps.
l3gd20h: z -0.28 dps.
l3gd20h: x 0.00 dps.
l3gd20h: y -0.07 dps.
l3gd20h: z -0.14 dps.
l3gd20h: x 0.00 dps.
l3gd20h: y 0.00 dps.
l3gd20h: z -0.07 dps.
l3gd20h: read the temperature.
l3gd20h: temperature is 23.00C.
l3gd20h: temperature is 23.00C.
l3gd20h: temperature is 24.00C.
l3gd20h: finish read test.
```

Raspberry INT must be connected to L3GD20H INT2 in FIFO mode.

```shell
./l3gd20h -t fifo --interface=spi

l3gd20h: chip is STMicroelectronic L3GD20H.
l3gd20h: manufacturer is STMicroelectronic.
l3gd20h: interface is IIC SPI.
l3gd20h: driver version is 2.0.
l3gd20h: min supply voltage is 2.2V.
l3gd20h: max supply voltage is 3.6V.
l3gd20h: max current is 5.00mA.
l3gd20h: max temperature is 85.0C.
l3gd20h: min temperature is -40.0C.
l3gd20h: start fifo test.
l3gd20h: irq fifo threshold with 17.
l3gd20h: find interrupt.
l3gd20h: finish fifo test.
```

Raspberry INT must be connected to L3GD20H INT1 in interrupt mode.

```shell
./l3gd20h -t int --interface=spi

l3gd20h: chip is STMicroelectronic L3GD20H.
l3gd20h: manufacturer is STMicroelectronic.
l3gd20h: interface is IIC SPI.
l3gd20h: driver version is 2.0.
l3gd20h: min supply voltage is 2.2V.
l3gd20h: max supply voltage is 3.6V.
l3gd20h: max current is 5.00mA.
l3gd20h: max temperature is 85.0C.
l3gd20h: min temperature is -40.0C.
l3gd20h: start interrupt test.
l3gd20h: threshold is 50.00.
l3gd20h: x -0.01 dps.
l3gd20h: y 0.00 dps.
l3gd20h: z 0.00 dps.
l3gd20h: x -0.02 dps.
l3gd20h: y -0.04 dps.
l3gd20h: z -0.03 dps.
l3gd20h: x -49.60 dps.
l3gd20h: y -4.30 dps.
l3gd20h: z -97.70 dps.
l3gd20h: irq z high threshold.
l3gd20h: x 50.70 dps.
l3gd20h: y 16.06 dps.
l3gd20h: z 76.32 dps.
l3gd20h: irq z high threshold.
l3gd20h: detect the high threshold event.
l3gd20h: finish interrupt test.
```

```shell
./l3gd20h -e read --interface=spi --times=3

l3gd20h: 1/3.
l3gd20h: x 0.00 dps.
l3gd20h: y 0.00 dps.
l3gd20h: z 0.00 dps.
l3gd20h: 2/3.
l3gd20h: x 0.00 dps.
l3gd20h: y 0.05 dps.
l3gd20h: z -0.02 dps.
l3gd20h: 3/3.
l3gd20h: x 0.00 dps.
l3gd20h: y -0.04 dps.
l3gd20h: z -0.08 dps.
```

Raspberry INT must be connected to L3GD20H INT2 in FIFO mode.

```shell
./l3gd20h -e fifo --interface=spi --times=3 --timeout=5000

l3gd20h: fifo irq with 17.
l3gd20h: fifo irq with 17.
l3gd20h: fifo irq with 17.
```

Raspberry INT must be connected to L3GD20H INT1 in interrupt mode.

```shell
./l3gd20h -e int --interface=spi --threshold=50.0 --timeout=5000

l3gd20h: set threshold 50.00.
l3gd20h: irq z high threshold.
l3gd20h: irq x high threshold.
l3gd20h: find interrupt.
```

```shell
./l3gd20h -h

Usage:
  l3gd20h (-i | --information)
  l3gd20h (-h | --help)
  l3gd20h (-p | --port)
  l3gd20h (-t reg | --test=reg) [--addr=<0 | 1>] [--interface=<iic | spi>]
  l3gd20h (-t read | --test=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  l3gd20h (-t fifo | --test=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>]
  l3gd20h (-t int | --test=int) [--addr=<0 | 1>] [--interface=<iic | spi>]
  l3gd20h (-e read | --example=read) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>]
  l3gd20h (-e fifo | --example=fifo) [--addr=<0 | 1>] [--interface=<iic | spi>] [--times=<num>] [--timeout=<ms>]
  l3gd20h (-e int | --example=int) [--addr=<0 | 1>] [--interface=<iic | spi>] [--threshold=<th>] [--timeout=<ms>]

Options:
      --addr=<0 | 1>             Set the addr pin.([default: 0])
  -e <read | fifo | int>, --example=<read | fifo | int>
                                 Run the driver example.
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
      --interface=<iic | spi>    Set the chip interface.([default: iic])
  -p, --port                     Display the pin connections of the current board.
  -t <reg | read | fifo | init>, --test=<reg | read | fifo | int>
                                 Run the driver test.
      --threshold=<th>           Set the interrupt threshold.([default: 50.0f])
      --times=<num>              Set the running times.([default: 3])
      --timeout=<ms>             Set the interrupt timeout in ms.([default: 5000])
```

