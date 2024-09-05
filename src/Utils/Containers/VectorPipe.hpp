#pragma once
#include "BasePipe.hpp"




template<class t> struct VectorPipe : public BasePipe<std::vector<t>, t> {
protected:
    ulong __internal_get_len(const std::vector<t> &e) override { return e.size(); }
    void   __internal_append(const std::vector<t> &e) override { BasePipe<std::vector<t>, t>::s.insert(BasePipe<std::vector<t>, t>::s.end(), e.begin(), e.end()); }
    void   __internal_append(const t &e)              override { BasePipe<std::vector<t>, t>::s.push_back(e); }
};
