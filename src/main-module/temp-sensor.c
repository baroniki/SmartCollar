#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <temp-sensor.h>

int main(){
	
	
	ADCSRA=(1<<ADEN) | (1<<ADPS0)|(1<<ADSC);
	int adcValue;
	double volt;
	double temp;
	//the scale of the Temperature sensor is 10mV/C
	max_temp=80; min_temp=5;
	uint16_t ADC_GetAdcValue(){
		ADMUX=0x00;
		DELAY_us(10);
		util_BitSet(ADCSRA,ADSC);
		while(util_IsBitCleared(ADCSRA,ADIF))
			
			return(ADC1);
	}
	
	while(1){
		adcValue=ADC_GetAdcValue();
		//the adc gives 10 bit output
		temp=adcValue/2;
		if(temp>max_temp||temp<min_temp){
			bool temp_warning=1;
		}
		
		
		struct temp_data{double temp,bool temp_warning};
		
		
		
	}

}