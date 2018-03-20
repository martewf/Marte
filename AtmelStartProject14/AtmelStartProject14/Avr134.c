

/**** A V R  A P P L I C A T I O N  NOTE 1 3 4 ************************** 
 *
 * Title:           Real Time Clock
 * Version:         2.00
 * Last Updated:    24.09.2013
 * Target:          ATmega128 (All AVR Devices with secondary external oscillator)
 *
 * Support E-mail:  avr@atmel.com
 *
 * Description      
 * This application note shows how to implement a Real Time Clock utilizing a secondary 
 * external oscilator. Included a test program that performs this function, which keeps
 * track of time, date, month, and year with auto leap-year configuration. 8 LEDs are used
 * to display the RTC. The 1st LED flashes every second, the next six represents the
 * minute, and the 8th LED represents the hour.
 *
 ******************************************************************************************/ 

typedef struct{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char date;
	unsigned char month;
	unsigned char year;
}time;

time t;
void real_time(void)
{
	if (++t.second==60)        //keep track of time, date, month, and year
	{
		t.second=0;
		if (++t.minute==60)
		{
			t.minute=0;
			if (++t.hour==24)
			{
				t.hour=0;
				if (++t.date==32)
				{
					t.month++;
					t.date=1;
				}
				else if (t.date==31)
				{
					if ((t.month==4) || (t.month==6) || (t.month==9) || (t.month==11))
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==30)
				{
					if(t.month==2)
					{
						t.month++;
						t.date=1;
					}
				}
				else if (t.date==29)
				{
					if((t.month==2) && (not_leap()))
					{
						t.month++;
						t.date=1;
					}
				}
				if (t.month==13)
				{
					t.month=1;
					t.year++;
				}
			}
		}
	}

}


static char not_leap(void)      //check for leap year
{
	if (!(t.year%100))
	{
		return (char)(t.year%400);
	}
	else
	{
		return (char)(t.year%4);
	}
}
