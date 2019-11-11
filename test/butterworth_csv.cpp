#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

int main (int,char**)
{
        // create the filter structure for 5th order
        Iir::Butterworth::LowPass<5> f;

	// filter parameters
        const float samplingrate = 100; // Hz
        const float cutoff_frequency = 2; // Hz

	// calc the coefficients
	f.setup (samplingrate, cutoff_frequency);
	
	double b = 0;
	double b2 = 0;
	for(int i=0;i<1000000;i++) 
	{
		float a=0;
		if (i==10) a = 1;
		b2 = b;
		b = f.filter(a);
		assert_print(!isnan(b),"Lowpass output is NAN\n");
		if ((i>20) && (i<100))
			assert_print((b != 0) || (b2 != 0),
				     "Lowpass output is zero\n");
	}
	assert_print(fabs(b) < 1E-25,"Lowpass value for t->inf to high!");

	return 0;
}
