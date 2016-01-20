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
        if (!s_instance) {
            s_instance = new T;
        }
        return s_instance;
    }

    /*!
      * Destroys the single instance of the singleton type.
      */
    inline void destroy()
    {
        delete s_instance;
        s_instance = 0;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

    static T *s_instance; /*!< the single instance of the class */

private:
    Singleton(const Singleton &);
    Singleton operator =(const Singleton &);
};

#endif // SINGLETON_HPP
