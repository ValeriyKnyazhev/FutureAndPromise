//
// Created by valeriy on 17.05.17.
//

#ifndef FUTUREANDPROMISE_FUTURE_H
#define FUTUREANDPROMISE_FUTURE_H

#include <memory>
#include <condition_variable>
#include <stdexcept>
#include "State.h"

class ValueAlreadySet : public std::exception {
public:
    virtual const char *what( ) const throw( ) {
        return "Value has already been set\n";
    }
};

class ExceptionAlreadyCatched : public std::exception {
public:
    virtual const char *what( ) const throw( ) {
        return "Exception has already been catched\n";
    }
};

class FutureAlreadyCalculated : public std::exception {
public:
    virtual const char *what( ) const throw( ) {
        return "Future has already been calculated\n";
    }
};

template<typename T>
class Future {
public:
    Future( std::shared_ptr<State<T>> state,
            std::shared_ptr<std::mutex> mutex,
            std::shared_ptr<std::condition_variable> conditionVariable ) :
            state( state ), mutex( mutex ), conditionVariable( conditionVariable ) {

    };

    T get( ) {
        std::unique_lock<std::mutex> lock( *mutex );
        conditionVariable->wait( lock, [ & ]( ) {
            return state->isValueSet( );
        } );
        if ( state->isExceptionCatched() ) {
            throw state->exception( );
        }
        return state->value( );
    }

    T tryGet( ) {
        std::unique_lock<std::mutex> lock( *mutex );
        if ( state->isValueSet( )) {
            if ( state->isExceptionCatched( )) {
                throw state->exception( );
            }
            return state->value( );
        }
        return null;
    }

    bool isCompleted( ) {
        std::unique_lock<std::mutex> lock( *mutex );
        return state->isValueSet( );
    }

private:
    std::shared_ptr<State<T>> state;
    std::shared_ptr<std::mutex> mutex;
    std::shared_ptr<std::condition_variable> conditionVariable;
};

#endif //FUTUREANDPROMISE_FUTURE_H
