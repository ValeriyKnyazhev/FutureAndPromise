//
// Created by valeriy on 17.05.17.
//

#ifndef FUTUREANDPROMISE_ASSYNC_H
#define FUTUREANDPROMISE_ASSYNC_H

#include <list>
#include <iostream>

#include "Promise.h"
#include "Future.h"

template<typename T>
class AsyncTask {
public:
    AsyncTask( std::function<T( )> function ) : function( function ) { }

    void operator()( ) {
        try {
            T value = this->function( );
            this->promise.setValue( value );
        } catch ( std::exception &ex ) {
            this->promise.catchException( ex );
        }
    }

    std::shared_ptr<Future<T>> getFuture( ) {
        if ( !this->isCompleted ) {
            this->future = this->promise.getFuture( );
        }
        this->isCompleted = true;
        return this->future;
    }

private:
    std::function<T( )> function;
    Promise<T> promise;
    std::shared_ptr<Future<T>> future;
    bool isCompleted = false;
};

struct Thread {
    std::thread thread;
    std::shared_ptr<IsCompletedCheckable> future;

    Thread( std::thread thread, std::shared_ptr<IsCompletedCheckable> future ) :
            thread( std::move( thread )), future( future ) {

    }
};

class AsyncExecutor {
public:
    AsyncExecutor( int numThreads ) : numThreads( numThreads ) { }

    ~AsyncExecutor( ) {
        while ( this->threads.size( ) > 0 ) {
            checkCompletedThreads( );
        }
    }

    template<typename T>
    std::shared_ptr<Future<T>> execute( std::function<T( )> function, bool isAsync = true ) {
        std::cout << "Number of threads in progress: " << this->threads.size() << std::endl;
        checkCompletedThreads( );
        AsyncTask<T> task( function );
        std::shared_ptr<Future<T>> future = task.getFuture( );
        if ( isAsync && this->threads.size( ) < this->numThreads ) {
            std::thread thread( task );
            std::cout << "Task in thread with id: " << thread.get_id( ) << " is successfully started\n";
            this->threads.push_back( Thread( std::move( thread ), future ));
        } else {
            task();
            std::cout << "Task in main thread is successfully started\n";
        }
        return future;
    }

    void checkCompletedThreads( ) {
        for ( auto it = this->threads.begin( ); it != this->threads.end( ); ) {
            if ( it->future->isCompleted( )) {
                std::cout << "Task in thread with id: " << it->thread.get_id( ) << " has already been completed\n";
                it->thread.join( );
                this->threads.erase( it );
                it = this->threads.begin( );
            } else {
                ++it;
            }
        }
    }

private:
    int numThreads;
    std::list<Thread> threads;
};

#endif //FUTUREANDPROMISE_ASSYNC_H
