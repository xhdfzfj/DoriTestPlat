#ifndef ASYNCTIMERCLASS_H
#define ASYNCTIMERCLASS_H

#include <chrono>
#include <functional>
#include <future>

class AsyncTimerClass
{
public:
    AsyncTimerClass( int pWaitTime, std::function< int( void * ) > pCallBack, void * pCallBackParamP )
    {
        std::thread( [pWaitTime, pCallBack, pCallBackParamP ]()
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( pWaitTime ) );
            pCallBack( pCallBackParamP );
        } ).detach();
    }

    ~AsyncTimerClass()
    {
        //printf( "test test test" );
    }
};

#endif // ASYNCTIMERCLASS_H
