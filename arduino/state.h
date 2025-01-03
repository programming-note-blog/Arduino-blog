#ifndef STATE_H
#define STATE_H

// 状態を表す列挙型
typedef enum
{
	STATE_STANDBY,
	STATE_LINETRACING,
	STATE_STOPPED,

	STATE_COUNT
} EState;

EState StateGetCurrentstate(void);
void StateOnButtonPress(void);
void StateOnArrive(void);

#endif