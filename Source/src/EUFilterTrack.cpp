#include "EUFilterTrack.h"
#include "EUTractor.h"


CEUFilterTrack::CEUFilterTrack(CEUTractor& tractor, FilterPriority priority)
    : m_tractor(tractor)
    , m_producer(tractor.producer())
    , m_priority(priority)
{
}

CEUFilterTrack::~CEUFilterTrack ()
{
}

shared_ptr<Mlt::Filter> CEUFilterTrack::filter(int filterIndex)
{
    shared_ptr<Mlt::Filter> filter;

    do
    {
        CHECK_BREAK(!m_producer);

        int nFindIndex = 0;
        int nFilterCount = m_producer->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> filter(m_producer->filter(i));
            CHECK_CONTINUE(!filter);

            int priority = filter->get_int(kFilterPriority);
            CHECK_CONTINUE(priority != m_priority);

            CHECK_CONTINUE(filterIndex != nFindIndex++);
            filter.reset(m_producer->filter(i));
            break;
        }

    } while (false);

    return filter;
}

int CEUFilterTrack::filterCount()
{
    int nCount = 0;

    do
    {
        CHECK_BREAK(!m_producer);

        int nFilterCount = m_producer->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> filter(m_producer->filter(i));
            CHECK_CONTINUE(!filter);

            int priority = filter->get_int(kFilterPriority);
            CHECK_CONTINUE(priority != m_priority);

            nCount += 1;
        }

    } while (false);

    return nCount;
}

int CEUFilterTrack::filterIndex(int position)
{
    int nIndex = -1;

    do
    {
        CHECK_BREAK(!m_producer);

        int nFindIndex = 0;
        int nFilterCount = m_producer->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> filter(m_producer->filter(i));
            CHECK_CONTINUE(!filter);

            int priority = filter->get_int(kFilterPriority);
            CHECK_CONTINUE(priority != m_priority);

            nFindIndex += 1;

            int in = filter->get_in();
            int length = filter->get_length();
            CHECK_CONTINUE(position < in);
            CHECK_CONTINUE(position >= in + length);

            nIndex = nFindIndex - 1;
            break;
        }

    } while (false);

    return nIndex;
}

bool CEUFilterTrack::appendFilter(Mlt::Filter &filter)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);
        CALL_BREAK(filter.is_valid(), bRet);

        int position = 0;
        int nFilterCount = m_producer->filter_count();
        for (int i = 0; i < nFilterCount; ++i)
        {
            shared_ptr<Mlt::Filter> p(m_producer->filter(i));
            CHECK_CONTINUE(!p);

            int priority = p->get_int(kFilterPriority);
            CHECK_CONTINUE(priority != m_priority);

            int in = p->get_in();
            int length = p->get_length();
            position = max(position, in + length);
        }

        filter.set(kFilterPriority, m_priority);
        filter.set_in_and_out(position, position + filter.get_length() - 1);
        CALL_BREAK(!m_producer->attach(filter), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUFilterTrack::overwrite(Mlt::Filter& filter, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);
        CALL_BREAK(filter.is_valid(), bRet);

        for (int i = 0; i < m_producer->filter_count(); ++i)
        {
            shared_ptr<Mlt::Filter> p(m_producer->filter(i));
            CHECK_CONTINUE(!p);

            int priority = p->get_int(kFilterPriority);
            CHECK_CONTINUE(priority != m_priority);

            int in = p->get_in();
            int out = p->get_out();
            int length = p->get_length();

            CHECK_CONTINUE(in >= position + filter.get_length());
            CHECK_CONTINUE(in + length <= position);

            if (in >= position && (in + length) <= position + filter.get_length())
            {
                CALL_BREAK(!m_producer->detach(*p), bRet);
                continue;
            }

            if (in < position && (in + length) <= position + filter.get_length())
            {
                p->set_in_and_out(in, position - 1);
                continue;
            }

            if (in >= position && (in + length) > position + filter.get_length())
            {
                p->set_in_and_out(position + filter.get_length(), out);
                continue;
            }

            p->set_in_and_out(in, position - 1);

            Mlt::Filter newFilter(profile, p->get("mlt_service"));
            CALL_BREAK(newFilter.is_valid(), bRet);
            CALL_BREAK(!newFilter.pass_values(*p, ""), bRet);

            newFilter.set(kFilterPriority, m_priority);
            newFilter.set_in_and_out(position + filter.get_length(), out);
            CALL_BREAK(!m_producer->attach(newFilter), bRet);
        }
        CHECK_BREAK(!bRet);

        filter.set(kFilterPriority, m_priority);
        filter.set_in_and_out(position, position + filter.get_length());
        CALL_BREAK(!m_producer->attach(filter), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUFilterTrack::trimFilterIn(int filterIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter = this->filter(filterIndex);
        FAIL_BREAK(!filter, bRet, false);

        int in = filter->get_in();
        int out = filter->get_out();

        if (in + delta < 0)
        {
            delta = -in;
        }
        else if ((in + delta) >= out)
        {
            delta = out - in - 1;
        }

        int position = in + delta;

        CALL_BREAK(!m_producer->detach(*filter), bRet);
        CALL_BREAK(overwrite(*filter, position), bRet);

    } while (false);

    return bRet;
}

bool CEUFilterTrack::trimFilterOut(int filterIndex, int delta)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter = this->filter(filterIndex);
        FAIL_BREAK(!filter, bRet, false);

        int out = filter->get_out();
        int length = filter->get_length();

        if (delta > length)
        {
            delta = length - 2;
        }

        int position = out - delta;

        CALL_BREAK(!m_producer->detach(*filter), bRet);
        CALL_BREAK(overwrite(*filter, position), bRet);

    } while (false);

    return bRet;
}

bool CEUFilterTrack::moveFilter(int filterIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter = this->filter(filterIndex);
        FAIL_BREAK(!filter, bRet, false);

        CALL_BREAK(!m_producer->detach(*filter), bRet);
        CALL_BREAK(overwrite(*filter, position), bRet);

    } while (false);

    return bRet;
}

bool CEUFilterTrack::splitFilter(int filterIndex, int position)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter = this->filter(filterIndex);
        FAIL_BREAK(!filter, bRet, false);

        int in = filter->get_in();
        int out = filter->get_out();
        int length = filter->get_length();
        FAIL_BREAK(position <= in, bRet, false);
        FAIL_BREAK(position >= in + length, bRet, false);

        filter->set_in_and_out(in, position - 1);

        Mlt::Filter newFilter(profile, filter->get("mlt_service"));
        CALL_BREAK(newFilter.is_valid(), bRet);
        CALL_BREAK(!newFilter.pass_values(*filter, ""), bRet);

        newFilter.set(kFilterPriority, m_priority);
        newFilter.set_in_and_out(position, out);
        CALL_BREAK(!m_producer->attach(newFilter), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}

bool CEUFilterTrack::liftFilter(int filterIndex)
{
    bool bRet = true;

    do
    {
        FAIL_BREAK(!m_producer, bRet, false);

        shared_ptr<Mlt::Filter> filter = this->filter(filterIndex);
        FAIL_BREAK(!filter, bRet, false);

        CALL_BREAK(!m_producer->detach(*filter), bRet);

        m_tractor.onChanged();

    } while (false);

    return bRet;
}


