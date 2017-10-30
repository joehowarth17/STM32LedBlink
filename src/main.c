/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via ITM).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[]) {

	// Enable clock for GPIOD
	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	//Set pin 15 as output
	GPIOD ->MODER |= GPIO_MODER_MODER15_0;
	GPIOD ->MODER &= ~(GPIO_MODER_MODER15_1);

	//Set GPIO speed to low.
	GPIOD ->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR15;

	//Set output type to push-pull.
	GPIOD ->OTYPER &= ~(GPIO_OTYPER_OT_15);

	//Set no pull-up/pull-down.
	GPIOD -> PUPDR &= ~(GPIO_PUPDR_PUPDR15);



	//Enable timer 2.
	RCC ->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//Set the pre-scaler to 1220 to get around 1Hz
	TIM2 ->PSC  = 1220;

	//Set auto-reload value to 65535, to get proper frequency
	TIM2 ->ARR = 65535;

	//Enable the timer.
	TIM2 ->CR1 |= TIM_CR1_CEN;

	while (1) {

		//If the timer update interrupt flag is set, toggle the LED.
		if(TIM2 ->SR & TIM_SR_UIF){

			TIM2  ->SR &= ~TIM_SR_UIF;
			GPIOD ->ODR ^= (GPIO_ODR_ODR_15);

		}


	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
