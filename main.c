#include <avr/io.h>
#include <util/delay.h>

/* floorplan

vcc>3				 //can also wait?
	enablecurrent
nonvolmem.full
	disablechargingloop
addtoduty=1

begin mainloop
vcc<3
	disablecurrent
vcc>enableoutputvoltage
	enableout
else
	disableout
memorize totcurrent
vcc>vbat                //hysteresis for when panels can't handle it?
	pwminit				//redundant if already set
	duty+=addtoduty     //duty shouldnt become negative or get stuck on 0
else
	pwmbothoff              //return to beginloop? or conditional for the following? or execute anyway?
wait                         //?
vbatt>batcutoff
	addtoduty=-1
	currentin<currentcutoff
		nonvolmem.full=true
		pwmbothoff
		disablechargingloop
else did setcurrent-totcurrent increase
	invert addtoduty


disablechargingloop              //incorporate device sleep?
	vbat<allowcharingagain
		nonvolmem.full=false
		exitthisloop(to where?)
*/


/*   adc bits
 ADMUX =
            (0 << REFS1) |     // Sets ref. voltage to Vcc, bit 1
            (0 << REFS0) |     // Sets ref. voltage to Vcc, bit 0
            (0 << MUX5)  |     // use ADC1 for input (PA1), MUX bit 5
            (0 << MUX4)  |     // use ADC1 for input (PA1), MUX bit 4
            (0 << MUX3)  |     // use ADC1 for input (PA1), MUX bit 3
            (0 << MUX2)  |     // use ADC1 for input (PA1), MUX bit 2
            (0 << MUX1)  |     // use ADC1 for input (PA1), MUX bit 1
            (1 << MUX0);       // use ADC1 for input (PA1), MUX bit 0
 */



#define charge_cuttoff_voltage 		275//4.1 vvc as ref
#define charge_renable_voltage 		282//4 vvc as ref
#define current_cuttoff 			116//50ma ref as ref //does this not break when higher voltages are tested?
#define enable_output_voltage		352//3.2 vvc as ref
#define enable_mcu_voltage			375//3 vvc as ref
#define charge_setcurrent			1 // or higher?
#define discharge_max_current		2 //output! //or 1?
// ^recalculate these as adc outputs
#define clock_freq
#define adc_clock_freq
#define counter_top
#define counter_freq
#define counter_pwm					counter_freq/2/counter_top
#define pwm_half_deadtime			// add to one substract from the other? or just substract?
#define wait_to_settle				//3 cycles per count+setup
#define n_adc_samples 10

typedef enum { false, true } bool;

struct {
   bool x : 1;
} status;

//which mcu
//can vref be measured with vcc as reference
//use int or uint8_t?
//phase correct pwm or not?
//return type of main?
//use oversampeling?
//calibrate adc,ref?
//clockrate, use crystal?// be mindful of voltage ratings
//external ref?
void main(void) {
  initadc();
  while(1) {

  }
  return;
}

void initpwm(){

}

int getadc(){//rolling average?//muxes have to be set up by now
	int total;
	for (int i=n_adc_samples; i!=0; i--){
		ADCSRA |= (1 << ADSC);         // start ADC measurement
		while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
		total+=ADCH+ADCL;
	}
	return total/n_adc_samples;
}

void initadc()
{
  ADCSRA =
            (1 << ADEN)  |     // Enable ADC
            (1 << ADPS2) |     // set prescaler to 16, bit 2
            (0 << ADPS1) |     // set prescaler to 16, bit 1
            (0 << ADPS0);      // set prescaler to 16, bit 0

  ADCSRB =
   (0 << ADLAR);      // right shift result (for 10-bit values)
}
