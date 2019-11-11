#include "Iir.h"

#include <stdio.h>

#include "assert_print.h"

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// read CSV
class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index];
        }
        std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream& str)
        {
            std::string         line;
            std::getline(str, line);

            std::stringstream   lineStream(line);
            std::string         cell;

            m_data.clear();
            while(std::getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            // This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                // If there was a trailing comma then add an empty element.
                m_data.push_back("");
            }
        }
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

void print_state (const std::ios& stream) {
std::cout << " good()=" << stream.good();
std::cout << " eof()=" << stream.eof();
std::cout << " fail()=" << stream.fail();
std::cout << " bad()=" << stream.bad();
}

class CSVIterator
{
public:
    typedef std::input_iterator_tag     iterator_category;
    typedef CSVRow                      value_type;
    typedef std::size_t                 difference_type;
    typedef CSVRow*                     pointer;
    typedef CSVRow&                     reference;

    CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
    CSVIterator()                   :m_str(NULL) {}

    // Pre Increment
    CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = NULL;}}return *this;}
    // Post increment
    CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
    CSVRow const& operator*()   const       {return m_row;}
    CSVRow const* operator->()  const       {return &m_row;}

    bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
    bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
private:
    std::istream*       m_str;
    CSVRow              m_row;
};


int main (int,char**)
{

    // Open the csv input file
    std::cout << "Reading CSV:" << std::endl;
    std::ifstream file("./test/wind_ads_meas.csv");
    print_state(file); std::cout << endl;

    // Open the csv output file
    std::ofstream myfile;
    myfile.open ("./test/filtered.csv");
    myfile << "time_ads,angles_x,angles_x_lowpass" << std::endl;


    // create the filter structure for 5th order
    Iir::Butterworth::LowPass<5> f;

    // filter parameters
    const float samplingrate = 100; // Hz
    const float cutoff_frequency = 2; // Hz

    // calc the coefficients
    f.setup (samplingrate, cutoff_frequency);

    float angle_x_lowpass, angle_x;
    CSVIterator it(file);
    ++it;  // skip first row
    long long min_time = stoll((*it)[0]);
    double time_ads;

    while(it != CSVIterator())
    {
        time_ads = (stoll((*it)[0]) - min_time)/1.0e9;
        angle_x = std::stof((*it)[4]);  // (*loop)[4] is field.angle_x
        angle_x_lowpass = f.filter(angle_x);
        assert_print(!isnan(angle_x_lowpass),"Lowpass output is NAN\n");
        myfile << time_ads << "," << angle_x << "," << angle_x_lowpass << std::endl;

        ++it;
    }

    myfile.close();
    return 0;
}
