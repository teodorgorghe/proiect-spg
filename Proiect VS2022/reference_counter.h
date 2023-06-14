#pragma once

class reference_counter
{
    int count_;
public:
    explicit reference_counter(const int count)
	    : count_(count)
    {
    }

    explicit reference_counter() : count_(0)
    {
    }

    void add_ref()
    {
        count_++;
    }

    int release()
    {
        return --count_;
    }
};
