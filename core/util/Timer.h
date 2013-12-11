/**
 * @file Timer.h
 * @brief Containts the Timer class
 * @date 16/06/2010
 * @author Matthias Holländer
 * This is my Timer-Class. I know the implementation part is quite ugly but I had problems under
 * Linux using clock_t so I changed it to clock_gettime. Seems a lot better. No time for refactoring here.
 */

#ifndef INC_TIMER__H_
#define INC_TIMER__H_


#if defined( _MSC_VER )
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif




//class Time
//{
//public:
//	virtual ~Time() {}
//
//	/**
//	 * @return this - pTime in milliseconds
//	 */
//	virtual float		DiffTime( const Time* const pTime ) const = 0;
//};




template< class TimeType >
class TimerT
{
public:
    TimerT() :
        m_bHasStopped( true ),
        m_bHasPaused( false ),
        m_fMatrixTime( 1.f )
    {
    }

    void					Start()
    {
        if ( IsRunnning() ) return;
        m_StartTime = TimeType();
        m_bHasStopped = false;
        m_bHasPaused = false;
    }

    float					GetTime() const
    {
        if ( !m_bHasPaused && !m_bHasStopped ) {
            return ( TimeType().DiffTime( &m_StartTime ) ) * m_fMatrixTime;
        }
        return ( m_PauseTime.DiffTime( &m_StartTime ) ) * m_fMatrixTime;
    }

    float					Pause()
    {
        if ( !m_bHasPaused ) {
            m_PauseTime = TimeType();
            m_bHasPaused = true;
        }
        return GetTime();
    }

    float					Stop()
    {
        m_bHasStopped = true;
        m_bHasPaused = false;

        return GetTime();
    }

    void				Resume()
    {
        if ( !m_bHasPaused ) return;
        m_bHasPaused = false;

        TimeType current();
        m_StartTime += current - m_StartTime;
    }

    void				Reset()
    {
        m_StartTime = TimeType();
    }

    bool				IsRunnning() const
    {
        return ( !m_bHasStopped && !m_bHasPaused );
    }

    bool				IsPaused() const
    {
        return m_bHasPaused;
    }

    bool				IsStopped() const
    {
        return m_bHasStopped;
    }

    void				SetTimerSpeed( float fSpeed )
    {
        m_fMatrixTime = fSpeed;
    }

    float				GetTimerSpeed() const
    {
        return m_fMatrixTime;
    }

private:
    bool				m_bHasStopped;
    bool				m_bHasPaused;

    float				m_fMatrixTime;							///< By which factor the time is sped up or slowed down
    TimeType			m_StartTime;
    TimeType			m_PauseTime;
};




#if defined( _MSC_VER )

class TimeWin32
{
public:
    TimeWin32()
    {
        if ( !ms_bInitialized ) {
            InitFrequency();
        }
        QueryPerformanceCounter( ( LARGE_INTEGER* ) &m_Current );
    }

    float					DiffTime( const TimeWin32* const pTime ) const
    {
        //const TimeWin32* const pTimeWin32 = static_cast< const TimeWin32* >( pTime );
        return ( float ) ( ( ( double )( this->m_Current - pTime->m_Current ) ) / ( double )ms_Frequency ) * 1000.f;
    }

    double getCurrentTime()
    {
        return (double)this->m_Current;
    }


    float					DiffTime( double pTime ) const
    {
        //const TimeWin32* const pTimeWin32 = static_cast< const TimeWin32* >( pTime );
        return ( float ) ( ( ( double )( this->m_Current - pTime) ) / ( double )ms_Frequency ) * 1000.f;
    }

private:
    static void				InitFrequency()
    {
        // frequency
        QueryPerformanceFrequency( ( LARGE_INTEGER* ) &ms_Frequency );
        ms_bInitialized = true;
    }

    __int64					m_Current;

    static __int64			ms_Frequency;
    static bool				ms_bInitialized;
};



#else

class TimeUnix
{
public:
    TimeUnix()
    {
        //const clockid_t g_cCLOCKID = CLOCK_REALTIME;				// Didn't work with my Ubuntu
        const clockid_t c_cCLOCKID = CLOCK_MONOTONIC;				// Worked for me
        //const clockid_t g_cCLOCKID = CLOCK_PROCESS_CPUTIME_ID;	// Worked for me
        //const clockid_t g_cCLOCKID = CLOCK_THREAD_CPUTIME_ID;		// Worked for me


        clock_gettime( c_cCLOCKID, &m_Current );
    }

    float					DiffTime(double pTime)
    {

        double diff = getCurrentTime() - pTime;
        return (float)diff;
    }

    double getCurrentTime()
    {
        return m_Current.tv_nsec / 1000000000.0 * 1000.0;
    }
private:
    timespec				m_Current;
};
#endif // _MSC_VER





#if defined( _MSC_VER )
typedef TimeWin32					Time;
typedef TimerT< TimeWin32 >				Timer;
#else
typedef TimeUnix					Time;
typedef TimerT< TimeUnix >				Timer;
#endif // _MSC_VER





#endif // INC_TIMER__H_
