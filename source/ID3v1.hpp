#ifndef ID3V1_HPP
#define ID3V1_HPP

#include "ID3v10.hpp"
#include "ID3v11.hpp"

/*
 * @brief: ID3 tag version 1, this can't have 'v10' and 'v11' simultaneously in file, only one by time.
 */
union ID3v1{
    public:
		/*
		 * @brief: Default constructor.
		 */
        ID3v1();

        /*
		 * @brief: Default virtual destructor that deallocate 'v10' and 'v11'.
		 */
        virtual ~ID3v1();

        /*
		 * @brief: Check if is the version 1.0.
		 * @return: true if is the version 1.0, false otherwise.
		 */
        bool isV10() const;

        /*
		 * @brief: Check if is the version 1.1.
		 * @return: true if is the version 1.1, false otherwise.
		 */
        bool isV11() const;

        /*
		 * @brief: Remove if has any.
		 */
        void remove();

    private:
        ID3v10* v10;
        ID3v11* v11;
};

#endif /* ID3V1_HPP */
