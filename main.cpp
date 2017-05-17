//
// Created by valeriy on 17.05.17.
//

#include <iostream>
#include <thread>
#include <vector>

#include "State.h"
#include "Future.h"
#include "Promise.h"
#include "Async.h"

long sum( long min, long max ) {
    long sum = 0;
    while ( min <= max ) {
        sum += min++;
    }
    return sum;
}

void TestAsyncTasks( long parameter ) {
    AsyncExecutor executor( 3 );
    std::vector<std::shared_ptr<Future<int>>> results;
    std::cout << "Async tasks: " << std::endl;
    for ( long i = 0; i < 20; i++ ) {
        results.push_back( executor.execute<int>( std::bind( sum, i * parameter, ( i + 1 ) * parameter )));
    }
    for ( int i = 0; i < results.size( ); i++ ) {
        std::cout << results[ i ]->get( ) << std::endl;
    }
    std::cout << std::endl;
}

void TestSyncTasks( long parameter ) {
    AsyncExecutor executor( 3 );
    std::vector<std::shared_ptr<Future<int>>> results;
    std::cout << "Sync tasks: " << std::endl;
    for ( long i = 0; i < 5; i++ ) {
        results.push_back( executor.execute<int>( std::bind( sum, i * parameter, ( i + 1 ) * parameter ), false ));
    }
    for ( int i = 0; i < results.size( ); i++ ) {
        std::cout << results[ i ]->get( ) << std::endl;
    }
    std::cout << std::endl;
}

int main( ) {
    TestAsyncTasks( 10000 );
    std::cout << std::endl;
    TestSyncTasks( 10000 );
    return 0;
}
