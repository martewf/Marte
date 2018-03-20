//inc. til tid og dato forsøk

typedef struct{
	unsigned char  second;
	unsigned char  minute;
	unsigned char  hour;
	unsigned char  date;
	unsigned char  month;
	unsigned short year;
}time;

time t;

void set_time(void)
{
	t.year   =  18;
	t.month  =  3;
	t.date   =  19;
	t.hour   =  12;
	t.minute =  36;
	t.second =  00;  //Bør settes i partallsverdi
}


void counter (void){
		
t.second = t.second + 2;

	if (t.second>=60)        //keep track of time, date, month, and year
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
					if((t.month==2) )
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
