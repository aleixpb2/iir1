#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

#include <iostream>
#include <fstream>

int main (int,char**)
{

    // Open the csv input file
    //

    // Open the csv output file
    std::ofstream myfile;
    myfile.open ("filtered.csv");
    myfile << "angles_x,angles_x_lowpass" << std::endl;


    // create the filter structure for 5th order
    Iir::Butterworth::LowPass<5> f;

    // filter parameters
    const float samplingrate = 100; // Hz
    const float cutoff_frequency = 2; // Hz

    // calc the coefficients
    f.setup (samplingrate, cutoff_frequency);

    double b = 0;
    for(int i=0;i<..;i++)
    {
        b = f.filter(a);
        assert_print(!isnan(b),"Lowpass output is NAN\n");
        myfile << a << "," << b << std::endl;
    }

    myfile.close();
    return 0;
}
