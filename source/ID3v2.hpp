#ifndef ID3V2_HPP
#define ID3V2_HPP

#include "ID3v22.hpp"
#include "ID3v23.hpp"
#include "ID3v24.hpp"

/*
 * @brief: ID3 version 2 can't have v22, v23 and v24 simultaneously, only one by time.
 */
union ID3v2{
    public:
		/*
		 * @brief: Default constructor.
		 */
        ID3v2();

        /*
		 * @brief: Default virtual destructor that deallocate 'v22', 'v23' and 'v24'.
		 */
        virtual ~ID3v2();

    public:
        /*
		 * @brief: Check if is the version 2.2.
		 * @return: true if is the version 2.2, false otherwise.
		 */
        bool isV22() const;

        /*
		 * @brief: Check if is the version 2.3.
		 * @return: true if is the version 2.3, false otherwise.
		 */
        bool isV23() const;

        /*
		 * @brief: Check if is the version 2.4.
		 * @return: true if is the version 2.4, false otherwise.
		 */
        bool isV24() const;

        /*
		 * @brief: Remove if has any.
		 */
        void remove();

    private:
        ID3v22* v22;
        ID3v23* v23;
        ID3v24* v24;
};

#endif /* ID3V2_HPP */
