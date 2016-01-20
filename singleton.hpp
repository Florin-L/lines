/*!
  * @file singleton.hpp
  * This file contains the declaration and the implementation of a singleton type.
  */
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/*! This class implements the singleton pattern.
  */
template <typename T> class Singleton
{
public:

    /*!
      * Instantiates if needed and returns the single instance of the singleton type.
      */
    static T* instance()
    {
        static T t;
        return &t;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton &);
    Singleton operator =(const Singleton &);
};

#endif // SINGLETON_HPP
