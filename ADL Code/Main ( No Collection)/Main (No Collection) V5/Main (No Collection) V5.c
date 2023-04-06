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
double I = 1.25;
int D = 1;
int previousError = 0;
int DOT[] = { 0 }; //Turn Sequence
int Order = -1;
int Torder = 0;
int not_done[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };


int ftorder;
int fOrder = -1;
int fDOT[] = { 1,1,1,1,1,1,2,2,3,3,2,2,2,2,1,1 }; //Fastturn

//test variables:
int nav_dist_h = 4;
int nav_dist_l = -1;
int stn_dist_h = 0;
int stn_dist_l = -8;

int nav_dist_p = 34;
int nav_dist_u = 26;

//void run(double Kp, double Ki, double Kd);

//Check if turn has been done before (-1 to indicate turned):
int TP17 = 2;
int TP19 = 1;
int TP32 = 1;
int TP7 = 1;
int TP28 = 1;
int TPS1 = 1;  // NavID = 33
int TP21 = 1;
int TP12 = 1;
int TP13 = 1;
int TP16 = 1;
int TP15 = 1;
int TP3 = 1;
int TP5 = 1;
int TPS2 = 1;   //NavId = 34
int TP9 = 1;


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
        WheelLeft = 100;
        WheelRight = 100;
        if (Duration == 0)
            return 0;
        return 1;
    }

    if (angleDiff <= angularErrorThreshold || angleDiff > 360 - angularErrorThreshold)
        turningSpeed = 0;
    else if (angleDiff <= 180)
        turningSpeed = angleDiff / 2 + 6;
    else
        turningSpeed = (angleDiff - 360) / 2 - 6;
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
        TurnTo(RotationZ, 110);
        printf("turnto");
        break;
    case 5:
        TurnTo(RotationZ, 335);
        printf("turnto");
        break;
    case 6:
        TurnTo(RotationZ, 200);
        printf("turnto");
        break;
    default:
        break;
    }
}

void Fastturn(int ftorder)
{
    switch (ftorder)
    {
    case 1:
        WheelRight = 100;
        WheelLeft = 61;
        break;
    case 2:
        WheelRight = 61;
        WheelLeft = 100;
        break;
    case 3:
        WheelRight = 100;
        WheelLeft = 70;
        break;
    default:
        break;
    }
}

int Turn_Check(int nav_id)
{
    int check = 0;
    switch (nav_id)
    {
    case 3:
        if (TP3 > 0)
        {
            check = 1;
            TP3 -= 1;
        }
        else
        {
            check = 0;
        }
    case 5:
        if (TP5 > 0)
        {
            check = 1;
            TP5 -= 1;
        }
        else
        {
            check = 0;
        }
    case 7:
        if (TP7 > 0)
        {
            check = 1;
            TP7 -= 1;
        }
        else
        {
            check = 0;
        }
    case 9:
        if (TP9 > 0)
        {
            check = 1;
            TP9 -= 1;
        }
        else
        {
            check = 0;
        }
    case 12:
        if (TP12 > 0)
        {
            check = 1;
            TP12 -= 1;
        }
        else
        {
            check = 0;
        }
    case 13:
        if (TP13 > 0)
        {
            check = 1;
            TP13 -= 1;
        }
        else
        {
            check = 0;
        }
    case 15:
        if (TP15 > 0)
        {
            check = 1;
            TP15 -= 1;
        }
        else
        {
            check = 0;
        }
    case 16:
        if (TP16 > 0)
        {
            check = 1;
            TP16 -= 1;
        }
        else
        {
            check = 0;
        }
    case 17:
        if (TP17 > 0)
        {
            check = 1;
            TP17 -= 1;
        }
        else
        {
            check = 0;
        }
    case 19:
        if (TP19 > 0)
        {
            check = 1;
            TP19 -= 1;
        }
        else
        {
            check = 0;
        }
    case 21:
        if (TP21 > 0)
        {
            check = 1;
            TP21 -= 1;
        }
        else
        {
            check = 0;
        }
    case 28:
        if (TP28 > 0)
        {
            check = 1;
            TP28 -= 1;
        }
        else
        {
            check = 0;
        }
    case 32:
        if (TP32 > 0)
        {
            check = 1;
            TP32 -= 1;
        }
        else
        {
            check = 0;
        }
    case 33:
        if (TPS1 > 0)
        {
            check = 1;
            TPS1 -= 1;
        }
        else
        {
            check = 0;
        }
    case 34:
        if (TPS2 > 0)
        {
            check = 1;
            TPS2 -= 1;
        }
        else
        {
            check = 0;
        }
    default:
        check = 0;
    }
    return check;
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
    return "Cyber Potatoes";
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
            for (int i = 0; i < 15; i++)
            {
                printf("%d ", not_done[i]);
            }
        }
    }
    else if (StnID == 1 && StnDist >= stn_dist_l && StnDist <= stn_dist_h)
    {
        Duration = 23;
        CurAction = 4;
        Order++;
    }
    else if (NavID == 13 && NavDist >= 10 && NavDist <= 18 || NavID == 12 && NavDist >= 5 && NavDist <= 17)
    {
        Duration = 8;
        CurAction = 5;
        fOrder++;
    }
    else if (NavID == 29 && NavDist >= 125 && NavDist <= 132 && not_done[9 - 1] == 1 || NavID == 11 && NavDist >= 90 && NavDist <= 95 || NavID == 19 && NavDist >= 10 && NavDist <= 14 || NavID == 32 && NavDist >= 10 && NavDist <= 14 || NavID == 3 && NavDist >= 10 && NavDist <= 14 || NavID == 15 && NavDist >= 10 && NavDist <= 14 || NavID == 17 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 7 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 28 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 9 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 5 && NavDist >= 23 && NavDist <= 31 || NavID == 16 && NavDist >= 23 && NavDist <= 31 || NavID == 21 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 12 && NavDist >= nav_dist_u && NavDist <= nav_dist_p || NavID == 13 && NavDist >= 12 && NavDist <= 18)
    {
        int id;
        if (NavID == 29 && NavDist >= 125 && NavDist <= 132)
        {
            id = 33;
        }
        else if (NavID == 11 && NavDist >= 90 && NavDist <= 95)
        {
            id = 34;
        }
        else
        {
            id = NavID;
        }

        int result = Turn_Check(id);
        if (result == 1)
        {
            if (NavID == 11 && NavDist >= 86 && NavDist <= 91 || NavID == 19 && NavDist >= 10 && NavDist <= 14 || NavID == 32 && NavDist >= 10 && NavDist <= 14 || NavID == 3 && NavDist >= 10 && NavDist <= 14 || NavID == 15 && NavDist >= 10 && NavDist <= 14)
            {
                Duration = 26;
            }
            else
            {
                Duration = 25;
            }
            CurAction = 5;
            fOrder++;
        }    
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
        run(0.43, 0.01, 6.2, 100);
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
        break;
    case 5:
        WheelLeft = 0;
        WheelRight = 0;
        LED = 0;
        ftorder = fDOT[fOrder];
        Fastturn(ftorder);
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