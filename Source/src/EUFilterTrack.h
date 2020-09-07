#pragma once
#include "EUType.h"


class CEUTractor;

class CEUFilterTrack
{
public:
    CEUFilterTrack(CEUTractor& tractor, FilterPriority priority);
    virtual ~CEUFilterTrack();

    shared_ptr<Mlt::Producer> producer() { return m_producer; }
    shared_ptr<Mlt::Filter> filter(int filterIndex);

    int filterCount();
    int filterIndex(int position);

    bool appendFilter(Mlt::Filter &filter);
    bool overwrite(Mlt::Filter& filter, int position);
    bool trimFilterIn(int filterIndex, int delta);
    bool trimFilterOut(int filterIndex, int delta);
    bool moveFilter(int filterIndex, int position);
    bool splitFilter(int filterIndex, int position);
    bool liftFilter(int filterIndex);

protected:
    CEUTractor& m_tractor;
    shared_ptr<Mlt::Producer> m_producer;
    int m_priority;
};
