//
// Created by valeriy on 17.05.17.
//

#ifndef FUTUREANDPROMISE_PROMISE_H
#define FUTUREANDPROMISE_PROMISE_H

#include <condition_variable>

#include "State.h"
#include "Future.h"

template<typename T>
class Promise {
public:
    Promise( ) :
            mutex( new std::mutex ), state( new State<T>( )), isValueGot( false ),
            conditionVariable( new std::condition_variable( )) {

    };

    void setValue( const T &value ) {
        std::unique_lock<std::mutex> lock( *this->mutex );
        if ( this->state->getIsValueSet( )) {
            auto ex = std::exception( );
            throw ValueAlreadySet( );
        }
        this->state->setValue( value );
        this->conditionVariable->notify_one( );
    }

    void catchException( const std::exception &ex ) {
        std::unique_lock<std::mutex> lock( *this->mutex );
        if ( this->state->getIsValueSet( )) {
            throw ExceptionAlreadyCatched( );
        }
        this->state->catchException( ex );
        this->conditionVariable->notify_one( );
    }

    std::shared_ptr<Future<T>> getFuture( ) {
        if ( this->isValueGot ) {
            throw FutureAlreadyCalculated( );
        }
        this->isValueGot = true;
        return std::shared_ptr<Future<T>>( new Future<T>( this->state, this->mutex, this->conditionVariable ));
    }

private:
    bool isValueGot;
    std::shared_ptr<State<T>> state;
    std::shared_ptr<std::mutex> mutex;
    std::shared_ptr<std::condition_variable> conditionVariable;
};

#endif //FUTUREANDPROMISE_PROMISE_H
