#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "hardware/irq.h"
#include "pico/multicore.h"
#include "/home/arno/pico/pico-sdk/src/rp2_common/hardware_adc/include/hardware/adc.h"
//#include "rp2_common/hardware_adc/include/hardware/adc.h"
void core1_interrupt_handler(){
  while(multicore_fifo_rvalid()){
    uint16_t raw = multicore_fifo_pop_blocking();
    const float conversion_factor = 3.3f/(1<<12);
    float result = raw*conversion_factor;
    float temp = 27-(result -0.706)/0.001721;
    printf("Temp = %f C\n", temp);
  }
  multicore_fifo_clear_irq();
}

void core1_entry(){
  multicore_fifo_clear_irq();
  irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_interrupt_handler);
  irq_set_enabled(SIO_IRQ_PROC1, true);  
}

int main(void){
  stdio_init_all();
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  printf("inside main...");
  sleep_ms(1000);

  multicore_launch_core1(core1_entry);

  adc_init();
  adc_set_temp_sensor_enabled(true);
  adc_select_input(4);

  //Primary Core 0 loop
  while(1){
    uint16_t raw = adc_read();
    multicore_fifo_push_blocking(raw);
    sleep_ms(1000);
    for (int i = 100; i <=1000; i+=100){            
      gpio_put(LED_PIN, 1);
      sleep_ms(i);
      gpio_put(LED_PIN, 0);
      sleep_ms(i);            
    }
    for (int i = 0; i <=10; i++){
      gpio_put(LED_PIN, 1);
      sleep_ms(100);
      gpio_put(LED_PIN, 0);
      sleep_ms(100);
    }
  }  
}
