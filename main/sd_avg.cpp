#include "sd_avg.h"

double sd(vector<int> v){
    double sum = 0.0, mean, standardDeviation = 0.0;
    
    
    for(auto it = v.begin(); it != v.end(); it++)
    {
        sum += *it;
    }
    
    mean = sum/v.size();
    
    for(auto it = v.begin(); it != v.end(); it++){
        standardDeviation += pow(*it - mean, 2);
    }
    
    return sqrt(standardDeviation / v.size());
}

double avg(vector<int> v){
    double sum = 0.0;
    for(auto it = v.begin(); it != v.end(); it++)
    {
        sum += *it;
    }
    return sum/v.size();
}
