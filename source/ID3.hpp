#ifndef ID3_HPP
#define ID3_HPP

#include "ID3v1.hpp"
#include "ID3v2.hpp"

/*
 * @brief: ID3 can have ID3 tag version 1 and 2 simultaneously.
 */
class ID3{
    public:
		/*
		 * @brief: Default constructor.
		 */
        ID3();

        /*
		 * @brief: Default virtual destructor.
		 */
        virtual ~ID3();

    public:
        ID3v1* v1;
        ID3v2* v2;
};

#endif /* ID3_HPP */
