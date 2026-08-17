#include "auto_drive.h"
#include "move.h"
#include "ultra.h"
#include "pwm.h"
#include "cmsis_os2.h"

#define SAFE_DISTANCE        33.0f
#define EMERGENCY_DISTANCE    2.0f

#define TURN_DIFF             30.0f
#define CORRECT_DIFF          20.0f
#define MIN_TURN_DISTANCE     15.0f
#define Car_Speed             730
#define Car_R_S               870

#define TURN_TIME             700
#define CORRECT_TIME           150

#define BACK_TIME             100

volatile float leftDistance = 0;
volatile float frontDistance = 0;
volatile float rightDistance = 0;

typedef enum
{
    AUTO_FORWARD,
    AUTO_CORRECT_LEFT,
    AUTO_CORRECT_RIGHT,

    AUTO_TURN_LEFT,
    AUTO_TURN_RIGHT,

    AUTO_BACKWARD,
    AUTO_ESCAPE_LEFT,
    AUTO_ESCAPE_RIGHT

} AutoState;
static AutoState state = AUTO_FORWARD;
static uint32_t stateStartTime = 0;
static void SetAutoState(AutoState newState)
{
    state = newState;
    stateStartTime = osKernelGetTickCount();
}
static uint32_t GetStateTime(void)
{
    return osKernelGetTickCount() - stateStartTime;
}
void AutoDrive(void)
{
    float left = leftDistance;
    float front = frontDistance;
    float right = rightDistance;

    float diff = left - right;


    // =========================================
    // 긴급 상황
    // =========================================

    if(left < EMERGENCY_DISTANCE)
    {
        if(state != AUTO_BACKWARD &&
           state != AUTO_ESCAPE_LEFT)
        {
            SetAutoState(AUTO_BACKWARD);
        }
    }
    else if(right < EMERGENCY_DISTANCE)
    {
        if(state != AUTO_BACKWARD &&
           state != AUTO_ESCAPE_RIGHT)
        {
            SetAutoState(AUTO_BACKWARD);
        }
    }
    else if(front < EMERGENCY_DISTANCE)
    {
        if(state != AUTO_BACKWARD)
        {
            SetAutoState(AUTO_BACKWARD);
        }
    }


    // =========================================
    // 상태 머신
    // =========================================

    switch(state)
    {

    // =========================================
    // 직진
    // =========================================

    case AUTO_FORWARD:

        // -------------------------------------
        // 앞에 장애물 있음
        // -------------------------------------

        if(front <= SAFE_DISTANCE)
        {
            // 왼쪽이 충분히 넓고
            // 오른쪽보다 훨씬 넓음
            if(left > MIN_TURN_DISTANCE &&
               left > right &&
               diff > TURN_DIFF)
            {
                SetAutoState(AUTO_TURN_LEFT);
            }

            // 오른쪽이 충분히 넓고
            // 왼쪽보다 훨씬 넓음
            else if(right > MIN_TURN_DISTANCE &&
                    right > left &&
                    diff < -TURN_DIFF)
            {
                SetAutoState(AUTO_TURN_RIGHT);
            }

            // 차이가 크지는 않지만
            // 왼쪽이 더 넓음
            else if(left > MIN_TURN_DISTANCE &&
                    left > right)
            {
                SetAutoState(AUTO_TURN_LEFT);
            }

            // 차이가 크지는 않지만
            // 오른쪽이 더 넓음
            else if(right > MIN_TURN_DISTANCE &&
                    right > left)
            {
                SetAutoState(AUTO_TURN_RIGHT);
            }

            // 양쪽 모두 좁음
            else
            {
                SetAutoState(AUTO_BACKWARD);
            }
        }


        // -------------------------------------
        // 앞은 안전 → 좌우 보정
        // -------------------------------------

        else if(diff >= CORRECT_DIFF &&
                diff < TURN_DIFF)
        {
            SetAutoState(AUTO_CORRECT_LEFT);
        }

        else if(diff <= -CORRECT_DIFF &&
                diff > -TURN_DIFF)
        {
            SetAutoState(AUTO_CORRECT_RIGHT);
        }

        else
        {
            moveForward();
            PWM_SetSpeed(Car_Speed, Car_Speed);
        }

        break;


    // =========================================
    // 왼쪽 미세 보정
    // =========================================

    case AUTO_CORRECT_LEFT:

        moveLeft();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        if(GetStateTime() >= CORRECT_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 오른쪽 미세 보정
    // =========================================

    case AUTO_CORRECT_RIGHT:

        moveRight();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        if(GetStateTime() >= CORRECT_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 왼쪽 크게 회전
    // =========================================

    case AUTO_TURN_LEFT:

        moveLeft();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        // 회전 중에도 계속 초음파 확인

        // 왼쪽이 너무 좁아짐
        if(left < EMERGENCY_DISTANCE)
        {
            SetAutoState(AUTO_BACKWARD);
        }

        // 앞이 너무 가까워짐
        else if(front < EMERGENCY_DISTANCE)
        {
            SetAutoState(AUTO_BACKWARD);
        }

        // 회전 시간 종료
        else if(GetStateTime() >= TURN_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 오른쪽 크게 회전
    // =========================================

    case AUTO_TURN_RIGHT:

        moveRight();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        // 회전 중에도 계속 초음파 확인

        if(right < EMERGENCY_DISTANCE)
        {
            SetAutoState(AUTO_BACKWARD);
        }

        else if(front < EMERGENCY_DISTANCE)
        {
            SetAutoState(AUTO_BACKWARD);
        }

        else if(GetStateTime() >= TURN_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 후진
    // =========================================

    case AUTO_BACKWARD:

        moveBackward();
        PWM_SetSpeed(Car_Speed, Car_Speed);

        if(GetStateTime() >= BACK_TIME)
        {
            // 후진 후 어느 쪽으로 갈지 결정

            if(left > right &&
               left > MIN_TURN_DISTANCE)
            {
                SetAutoState(AUTO_ESCAPE_LEFT);
            }

            else if(right > left &&
                    right > MIN_TURN_DISTANCE)
            {
                SetAutoState(AUTO_ESCAPE_RIGHT);
            }

            else
            {
                // 아직 양쪽 모두 좁으면
                // 다시 짧게 후진
                SetAutoState(AUTO_BACKWARD);
            }
        }

        break;


    // =========================================
    // 후진 후 왼쪽 회피
    // =========================================

    case AUTO_ESCAPE_LEFT:

        moveLeft();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        if(GetStateTime() >= CORRECT_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 후진 후 오른쪽 회피
    // =========================================

    case AUTO_ESCAPE_RIGHT:

        moveRight();
        PWM_SetSpeed(Car_R_S, Car_R_S);

        if(GetStateTime() >= CORRECT_TIME)
        {
            SetAutoState(AUTO_FORWARD);
        }

        break;


    // =========================================
    // 예외
    // =========================================

    default:

        moveStop();
        SetAutoState(AUTO_FORWARD);

        break;
    }
}
