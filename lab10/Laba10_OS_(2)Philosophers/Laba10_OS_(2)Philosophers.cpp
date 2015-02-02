#include "stdafx.h"
#include <windows.h>
#include <cstdlib>
#include <cstdio>
 
#define min 100
#define max 500
#define step 4
 
struct ThreadArgs {
   int philosopherNum;
   HANDLE forks[ 2 ];
};
 
CRITICAL_SECTION criticalSection;
 
void output( const char *msg ) {
   EnterCriticalSection( &criticalSection );
   printf( msg );
   LeaveCriticalSection( &criticalSection );
}
 
void output( const char *msg, int arg1 ) {
   EnterCriticalSection( &criticalSection );
   printf( msg, arg1 );
   LeaveCriticalSection( &criticalSection );
}
 
void output( const char *msg, int arg1, int arg2 ) {
   EnterCriticalSection( &criticalSection );
   printf( msg, arg1, arg2 );
   LeaveCriticalSection( &criticalSection );
}

DWORD WINAPI philosopherAction( LPVOID arg ) {
   ThreadArgs *args = static_cast< ThreadArgs * >( arg );
   
   for(int i = 0; i < step; i++) {          //повторяет 4 раза
      Sleep( min + rand() % ( max-min ));  //задержка
      output( "Philosopher[ %i ] want to eat.\n", args->philosopherNum );
      output( "Try to catch forks.\n");
      output( "Left[%i] and right[%i] forks - ", args->philosopherNum, args->philosopherNum + 1 );
      
      if ( WaitForMultipleObjects( 2, args->forks, true, 1 ) != WAIT_OBJECT_0 ) {
         SetEvent( args->forks[ 0 ] );          //объект "событие" в сигнальное состояние
         SetEvent( args->forks[ 1 ] );          //объект "событие" в сигнальное состояние
         output( "Eating." );                   //философ ест
         Sleep( min + rand() % ( max - min ));
         ResetEvent( args->forks[ 0 ] );        //сброс события
         ResetEvent( args->forks[ 1 ] );        //сброс события
      }
      
      Sleep( min + rand() % ( max-min ));
      
      output( "Philosopher[%i] THINKING.\n", args->philosopherNum ); //философ думает
   }
   return 0;
}
 
int main() {
   const unsigned int PHILOSOPHERS_NUM = 5;
   
   HANDLE forks[ PHILOSOPHERS_NUM ] = {};
   HANDLE philosophers[ PHILOSOPHERS_NUM ] = {};
   
   ThreadArgs args[ PHILOSOPHERS_NUM ];
   
   InitializeCriticalSection( &criticalSection );
   
   for ( unsigned int i = 0; i < PHILOSOPHERS_NUM; i++ ) {
      forks[ i ] = CreateEvent( 0, true, false, NULL ); //тип сброса- ручной
   }
   
   for ( unsigned int i = 0; i < PHILOSOPHERS_NUM; i++) {
      args[ i ].philosopherNum = i;
      args[ i ].forks[ 0 ] = forks[ i ];
      
      if ( i < 4 )
         args[ i ].forks[ 1 ] = forks[ i + 1 ];
      else
         args[ i ].forks[ 1 ] = forks[ 0 ];
      
      philosophers[ i ] = CreateThread( NULL, 0, philosopherAction, &args[ i ], 0, NULL );
   }
   
   WaitForMultipleObjects( PHILOSOPHERS_NUM, philosophers, true, INFINITE );
   
   DeleteCriticalSection( &criticalSection );
    system("pause");
   return 0;
}
 
