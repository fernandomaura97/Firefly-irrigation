/*
 * Copyright (c) 2016, Zolertia <http://www.zolertia.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
/*---------------------------------------------------------------------------*/
/**
 * \addtogroup zoul-examples
 * @{
 *
 * \defgroup zoul-bme280-test BME280 temperature and humidity sensor test
 *
 * Demonstrates the use of the BME280 digital temperature and humidity sensor
 * @{
 *
 * \file
 *         A quick program for testing the BME280 temperature and humidity sensor
 * \author
 *         Antonio Lignan <alinan@zolertia.com>
 */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "contiki.h"
#include "dev/bme280/bme280.h"
#include "dev/bme280/bme280-sensor.h"
#include "dev/adc-sensors.h"
#include <stdio.h>
#include <stdint.h>
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
#define BME280_USE_STD_API  1
#define ADC_PIN1            5
#define ADC_PIN2            4
#define ADC_PIN3            2

#define LOOP_PERIOD         2
#define LOOP_INTERVAL       (CLOCK_SECOND * LOOP_PERIOD)
#define LEDS_PERIODIC       LEDS_GREEN

/*---------------------------------------------------------------------------*/
PROCESS(zoul_bme280_process, "BME280 test");
AUTOSTART_PROCESSES(&zoul_bme280_process);
/*---------------------------------------------------------------------------*/
static struct etimer et;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(zoul_bme280_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(bme280_sensor);
  uint16_t adc1;
  uint16_t adc2;
  uint16_t adc3;
  adc_sensors.configure(ANALOG_AAC_SENSOR1, ADC_PIN1);
  adc_sensors.configure(ANALOG_AAC_SENSOR2, ADC_PIN2);
  adc_sensors.configure(ANALOG_AAC_SENSOR3, ADC_PIN3);

  /* Let it spin and read sensor data */

  while(1) {
    etimer_set(&et, 5*CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


    
    adc1 = adc_sensors.value(ANALOG_AAC_SENSOR1);
    adc2 = adc_sensors.value(ANALOG_AAC_SENSOR2);
    adc3 = adc_sensors.value(ANALOG_AAC_SENSOR3);

    printf("Humidity sensor 1 = %u\n", adc1);
    printf("Humidity sensor 2 = %u\n", adc2);
    printf("Humidity sensor 3 = %u\n", adc3);
 
  
	#if BME280_USE_STD_API
	    printf("temperature = %-d ", bme280_sensor.value(BME280_SENSOR_TEMP));
	    printf("humidity = %-d ",    bme280_sensor.value(BME280_SENSOR_HUMIDITY));
	    printf("pressure = %-d\n",   bme280_sensor.value(BME280_SENSOR_PRESSURE));
	#else /* BME280_USE_STD_API */
	    /* Trigger burst read */
	    bme280_sensor.value(BME280_SENSOR_TEMP);
	    printf("temperature = %5.2f" , (double)bme280_mea.t_overscale100 / 100.);
	    printf("humidity = %5.2f ",    (double)bme280_mea.h_overscale1024 / 1024.);
	#ifdef BME280_64BIT
	    printf("pressure = %5.2f\n", (double)bme280_mea.p_overscale256 / 256.);
	#else /* BME280_64BIT */
	    printf("pressure = %5.2f\n", (double)bme280_mea.p);
	#endif /* BME280_64BIT */
	#endif /* BME280_USE_STD_API */

  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/**
 * @}
 * @}
 */

