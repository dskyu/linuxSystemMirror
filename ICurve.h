#ifndef ICURVE_H
#define ICURVE_H

class ICurve
{
public:
    virtual int init(int gridSize, int xGridNum, int yGridNum, int min, int max) = 0;
    virtual void updateData(int value) = 0;
};

#endif // ICURVE_H
