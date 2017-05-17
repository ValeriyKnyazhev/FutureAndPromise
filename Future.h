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

class IsCompletedCheckable {
public:
    virtual bool isCompleted( ) = 0;
};

template<typename T>
class Future : public IsCompletedCheckable {
public:
    Future( std::shared_ptr<State<T>> state,
            std::shared_ptr<std::mutex> mutex,
            std::shared_ptr<std::condition_variable> conditionVariable ) :
            state( state ), mutex( mutex ), conditionVariable( conditionVariable ) {

    };

    T get( ) {
        std::unique_lock<std::mutex> lock( *this->mutex );
        this->conditionVariable->wait( lock, [ & ]( ) {
            return this->state->getIsValueSet( );
        } );
        if ( this->state->getIsExceptionCatched( )) {
            throw this->state->getException( );
        }
        return this->state->getValue( );
    }

    bool tryGet( T &value ) {
        std::unique_lock<std::mutex> lock( *this->mutex );
        if ( this->state->getIsValueSet( )) {
            if ( this->state->getIsExceptionCatched( )) {
                throw this->state->getException( );
            }
            value = this->state->getValue( );
            return true;
        }
        return false;
    }

    bool isCompleted( ) {
        std::unique_lock<std::mutex> lock( *this->mutex );
        return this->state->getIsValueSet( );
    }

private:
    std::shared_ptr<State<T>> state;
    std::shared_ptr<std::mutex> mutex;
    std::shared_ptr<std::condition_variable> conditionVariable;
};

#endif //FUTUREANDPROMISE_FUTURE_H
