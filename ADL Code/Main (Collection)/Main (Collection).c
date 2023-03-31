/////////////////////////////////////////////////////////////////////////
//                                                                     //
//	CoSpace Robot                                                      //
//	Version 2.0.0                                                      //
//	DEC 16 2022                                                        //
//	Copyright (C) 2022 CoSpace Robot. All Rights Reserved              //
//                                                                     //
//  ONLY C Code can be compiled.                                       //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif
int Duration = 0;
int CurAction = -1;
int CurGame = 0;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int PositionX = 0;
int PositionY = 0;
int RotationZ = 0;
int DeltaAngle = 0;
int DeltaDist = 0;
int NavID = 0;
int NavDist = 0;
int CtrID = 0;
int CtrDist = 0;
int StnID = 0;
int StnDist = 0;
int Time = 0;
int MyState = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED = 0;
int AI_TeamID = 1;   //Robot Team ID.    1:Blue Ream;    2:Red Team.
int AI_SensorNum = 15;
int PID = 0;
int P = 1;
double I = 0.3;
int D = 1;
int previousError = 0;
int DOT[] = { 2 , 0 , 1 , 3 , 2 , 3 , 0 , 1 , 3 , 0 , 5 , 3 , 2 , 3 , 0 , 2 , 3 , 2 , 4 , 1 , 0 }; //Turn Sequence
int Order = -1;
int Torder = 0;
int not_done[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//void run(double Kp, double Ki, double Kd);


#define CsBot_AI_C//DO NOT delete this line

////////////CoSpace System Functions, Please DON't modify./////////////////////
///////////////////////////////////////////////////////////////////////////////

char TaskString[256];//For Individual Challenge ONLY
struct Task
{
    int TaskID;
    int StationID;
    int ActionID;
} TaskList[10];//For Individual Challenge ONLY

DLL_EXPORT int  GetGameID()
{
    return CurGame;
}
DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

DLL_EXPORT void SetTaskString(char* gameTask)
{
    strcpy(TaskString, gameTask);
    printf(gameTask);
    printf("\r\n");
}

DLL_EXPORT void AddTaskItem(int TaskID, int StationID, int ActionID)
{
    if (TaskID < 0 || TaskID >= 10) return;
    TaskList[TaskID].TaskID = TaskID;
    TaskList[TaskID].StationID = StationID;
    TaskList[TaskID].ActionID = ActionID;
    printf("Task:(TaskID:%d, StID:%d, ActID:%d)\r\n", TaskID, StationID, ActionID);
}
//May not work well in Swarmland. 
int TurnTo(int curRot, int targetRot)
{
    int angularErrorThreshold = 2;
    int angleDiff = (curRot - targetRot + 360) % 360;
    int turningSpeed;
    if (Duration >= 0 && Duration <= 8)
    {
        WheelLeft = 40;
        WheelRight = 40;
        if (Duration == 0)
            return 0;
        return 1;
    }

    if (angleDiff <= angularErrorThreshold || angleDiff > 360 - angularErrorThreshold)
        turningSpeed = 0;
    else if (angleDiff <= 180)
        turningSpeed = angleDiff / 6 + 6;
    else
        turningSpeed = (angleDiff - 360) / 6 - 6;
    WheelLeft = turningSpeed;
    WheelRight = -WheelLeft;
    if (turningSpeed == 0)
    {
        Duration = 8;
        return 1;
    }
    Duration = 128;
    return 1;
}

void run(double Kp, double Ki, double Kd, int PWR) {
    WheelLeft = 0;
    WheelRight = 0;
    LED = 0;
    P = DeltaDist;
    I = I + DeltaDist;
    D = DeltaDist - previousError;
    PID = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = DeltaDist;
    WheelLeft = PWR + PID;
    WheelRight = PWR - PID;
}

void Turning(int Torder)
{
    switch (Torder)
    {
    case 0: //DOT= Direction of turn
        TurnTo(RotationZ, 0);
        printf("turnto");
        break;
    case 1:
        TurnTo(RotationZ, 90);
        printf("turnto");
        break;
    case 2:
        TurnTo(RotationZ, 180);
        printf("turnto");
        break;
    case 3:
        TurnTo(RotationZ, 270);
        printf("turnto");
        break;
    case 4:
        TurnTo(RotationZ, 120);
        printf("turnto");
        break;
    case 5:
        TurnTo(RotationZ, 305);
        printf("turnto");
        break;
    default:
        break;
    }
}

struct DeliveryItem
{
    int ItemID;
    int CenterID;
    int CollectionPtID;
    int Deadline;
    int ItemScore;
    int CurStatus; //1:at Center; 2:on Car; 3:at Station;
} DeliveryItemList[100]; //All Items

DLL_EXPORT void AddDeliveryItem(int ItemID, int CenterID, int StationID, int ItemScore, int Deadline, int CurStatus)
{
    if (ItemID < 0 || ItemID>99) {
        printf("ItemID Error! ID = %d", ItemID);
        return;
    }
    DeliveryItemList[ItemID].ItemID = ItemID;
    DeliveryItemList[ItemID].CenterID = CenterID;
    DeliveryItemList[ItemID].CollectionPtID = StationID;
    DeliveryItemList[ItemID].ItemScore = ItemScore;
    DeliveryItemList[ItemID].Deadline = Deadline;
    DeliveryItemList[ItemID].CurStatus = CurStatus;
}
DLL_EXPORT void RequestItems(int* Items)
{
    for (int i = 0; i < 6; i++)
    {
        Items[i] = -1;
    }
}


void GameStart()
{
    //Add your code here

}


void AILoopStart()
{
    //Add your code here

}


DLL_EXPORT char* GetTeamName()
{
    return "Test";
}

char info[3000];
DLL_EXPORT char* GetDebugInfo()
{
    sprintf(info, "Duration=%d;CurAction=%d;CurGame=%d;US_Front=%d;US_Left=%d;US_Right=%d;PositionX=%d;PositionY=%d;RotationZ=%d;DeltaAngle=%d;DeltaDist=%d;NavID=%d;NavDist=%d;CtrID=%d;CtrDist=%d;StnID=%d;StnDist=%d;Time=%d;MyState=%d;WheelLeft=%d;WheelRight=%d;LED=%d;", Duration, CurAction, CurGame, US_Front, US_Left, US_Right, PositionX, PositionY, RotationZ, DeltaAngle, DeltaDist, NavID, NavDist, CtrID, CtrDist, StnID, StnDist, Time, MyState, WheelLeft, WheelRight, LED);
    return info;
}
DLL_EXPORT void SetGameID(int GameID)
{
    int i = 0;
    if (CurGame != GameID)
    {
        GameStart();
        for (i = 0; i < 100; i++)
        {
            DeliveryItemList[i].ItemID = i;
            DeliveryItemList[i].CenterID = -1;
            DeliveryItemList[i].CollectionPtID = 0;
            DeliveryItemList[i].Deadline = 0;
            DeliveryItemList[i].ItemScore = 0;
            DeliveryItemList[i].CurStatus = 0;
        }
        for (i = 0; i < 10; i++)
        {
            TaskList[i].TaskID = i;
            TaskList[i].StationID = 0;
            TaskList[i].ActionID = 0;
        }
    }
    CurGame = GameID;
}

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int* AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    PositionX = AI_IN[3]; packet[3] = PositionX; sum += PositionX;
    PositionY = AI_IN[4]; packet[4] = PositionY; sum += PositionY;
    RotationZ = AI_IN[5]; packet[5] = RotationZ; sum += RotationZ;
    DeltaAngle = AI_IN[6]; packet[6] = DeltaAngle; sum += DeltaAngle;
    DeltaDist = AI_IN[7]; packet[7] = DeltaDist; sum += DeltaDist;
    NavID = AI_IN[8]; packet[8] = NavID; sum += NavID;
    NavDist = AI_IN[9]; packet[9] = NavDist; sum += NavDist;
    CtrID = AI_IN[10]; packet[10] = CtrID; sum += CtrID;
    CtrDist = AI_IN[11]; packet[11] = CtrDist; sum += CtrDist;
    StnID = AI_IN[12]; packet[12] = StnID; sum += StnID;
    StnDist = AI_IN[13]; packet[13] = StnDist; sum += StnDist;
    Time = AI_IN[14]; packet[14] = Time; sum += Time;
    MyState = AI_IN[15]; packet[15] = MyState; sum += MyState;
    packet[16] = sum;

}
DLL_EXPORT void GetCommand(int* AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED;
    AI_OUT[3] = MyState;
}
void Game0()
{

    if (Duration > 0)
    {
        Duration--;
    }
    else if (StnID >= 1 && StnID <= 100 && StnDist >= 0 && StnDist <= 1)
    {
        if (not_done[StnID - 1] == 0)
        {
            Duration = 80;
            CurAction = 1;
            not_done[StnID - 1] = 1;
        }
    }
    else if (StnID == 1 && StnDist >= -7 && StnDist <= 0 || StnID == 10 && StnDist >= -7 && StnDist <= 0 || StnID == 8 && StnDist >= -7 && StnDist <= 0)
    {
        Duration = 20;
        CurAction = 4;
        Order++;
    }
    else if (NavID == 17 && NavDist <= 4 || NavID == 22 && NavDist <= 4 || NavID == 23 && NavDist <= 4 || NavID == 30 && NavDist <= 4 || NavID == 7 && NavDist <= 4 || NavID == 28 && NavDist <= 4 || NavID == 27 && NavDist <= 5 || NavID == 26 && NavDist <= 4 || NavID == 9 && NavDist <= 4 || NavID == 6 && NavDist <= 4 || NavID == 5 && NavDist <= 4 || NavID == 16 && NavDist <= 4 || NavID == 13 && NavDist <= 5 || NavID == 12 && NavDist <= 4 || NavID == 21 && NavDist <= 4)
    {
        Duration = 10;
        CurAction = 2;
        Order++;
    }
    else if (true)
    {
        Duration = 0;
        CurAction = 3;
    }
    switch (CurAction)
    {
    case 1:
        WheelLeft = 0;
        WheelRight = 0;
        LED = 1;
        break;
    case 2:
        WheelLeft = 0;
        WheelRight = 0;
        LED = 0;
        Torder = DOT[Order];
        Turning(Torder);
        break;
    case 3:
        WheelLeft = 0;
        WheelRight = 0;
        run(1.6, 0.000000001, 10, 100);
        LED = 0;
        break;
    case 4:
        WheelLeft = 0;
        WheelRight = 0;
        LED = 0;
        Torder = DOT[Order];
        Turning(Torder);
        if (Duration <= 10) // Go Forward to Leave Center.
        {
            WheelLeft = 100;
            WheelRight = 100;
        }
    default:
        break;
    }

}


DLL_EXPORT void AILoop()
{
    AILoopStart();
    switch (CurGame)
    {
    case 0:
        Game0();
        break;
    default:
        WheelLeft = 0;
        WheelRight = 0;
        LED = 0;
        MyState = 0;
        break;
    }
}