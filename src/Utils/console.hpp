#pragma once
#include <mutex>
#include <streambuf>




namespace console {
    extern std::mutex __internal_consoleLock;




    class __internal_cout_stream_t : public std::streambuf {
    protected:
        int overflow(int c) override;
    };
    extern std::ostream __internal_cout;


    class __internal_cout_stream_t_wrapper {
    public:
        template<typename T> __internal_cout_stream_t_wrapper &operator<<(const T& val) {
            // std::scoped_lock lock(__internal_consoleLock);
            __internal_cout << val;
            return *this;
        }
        __internal_cout_stream_t_wrapper &operator++(int dummy[[maybe_unused]]) {
            __internal_consoleLock.lock(); // NOSONAR(cpp:S5506)
            return *this;
        }

        __internal_cout_stream_t_wrapper &operator--(int dummy[[maybe_unused]]) {
            __internal_consoleLock.unlock(); // NOSONAR(cpp:S5506)
            return *this;
        }
    };
    extern  __internal_cout_stream_t_wrapper cout;




    class __internal_cerr_stream_t : public std::streambuf {
    protected:
        int overflow(int c) override;
    };
    extern std::ostream __internal_cerr;


    class __internal_cerr_stream_t_wrapper {
    public:
        template<typename T> __internal_cerr_stream_t_wrapper &operator<<(const T& val) {
            // std::scoped_lock lock(__internal_consoleLock);
            __internal_cerr << val;
            return *this;
        }
        __internal_cerr_stream_t_wrapper &operator++(int dummy[[maybe_unused]]) {
            __internal_consoleLock.lock(); // NOSONAR(cpp:S5506)
            return *this;
        }

        __internal_cerr_stream_t_wrapper &operator--(int dummy[[maybe_unused]]) {
            __internal_consoleLock.unlock(); // NOSONAR(cpp:S5506)
            return *this;
        }
    };
    extern  __internal_cerr_stream_t_wrapper cerr;
}