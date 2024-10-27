#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 1, 1)

#include <stack>
#include "0.0_Extentions/DataStructures/temp.h"
#include <queue>
#include <string>
#include <vector>
#include <iostream>

#include <iostream>
#include <vector>

enum class Infotype : unsigned char
{
    None = 0,
    Arrive, Departure, End
};

struct ParkInfo
{
    Infotype type;
    unsigned int number;
    unsigned int time;
};

bool GetInfo(ParkInfo& info);
void HandleInfo(ParkInfo const& info);

Firefly::Stack<std::pair<unsigned int, unsigned int>> parkingLot;
Firefly::Stack<std::pair<unsigned int, unsigned int>> temp;
Firefly::LinkQueue<unsigned int> waiting;
ParkInfo info;
unsigned int size;
float price;
unsigned int nowTime;

int main()
{
    std::cout << "输入停车场大小: \n";
    std::cin >> size;
    std::cout << "输入单位时间收费: \n";
    std::cin >> price;

    while (GetInfo(info))
    {
        if (info.type != Infotype::None)
        {
            HandleInfo(info);
        }
    }
}

bool GetInfo(ParkInfo& info)
{
    std::cout << "输入信息 (A:Arrive, D:Departure, E:End): \n";
    char type;
    unsigned int number;
    unsigned int time;
    std::cin >> type >> number >> time;

    if (std::cin.fail()) {
        std::cout << "无效输入，请重试。\n";

        std::cin.clear();

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        info.type = Infotype::None;
        return true;
    }

    Infotype t =
        type == 'A' ? Infotype::Arrive :
        type == 'D' ? Infotype::Departure :
        type == 'E' ? Infotype::End :
        Infotype::None;

    switch (t)
    {
    case Infotype::Arrive:
    case Infotype::Departure:
        info = { t, number, time };
        return true;
    case Infotype::End:
        std::cout << "输入结束\n";
        info.type = Infotype::None;
        return false;
    default:
        std::cout << "无效类型，请输入 'A', 'D' 或 'E'。\n";
        info.type = Infotype::None;
        return true;
    }
}
void HandleInfo(ParkInfo const& info)
{
    if (info.time < nowTime)
    {
        std::cout << "不能时间倒流，请重试。\n";
        return;
    }

    switch (info.type)
    {
    case Infotype::Departure:
    {
        
        while (parkingLot.top().first != info.number)
        {
            auto pair = parkingLot.top();
            parkingLot.pop();
            temp.push(pair);
            std::cout << "车辆离开，编号为" << pair.first << ", 缴费" << (info.time - pair.second) * price << "。\n";
        }
        auto pair = parkingLot.top();
        parkingLot.pop();
        std::cout << "车辆离开，编号为" << pair.first << ", 缴费" << (info.time - pair.second) * price << "。\n";
        while (!temp.empty())
        {
            auto pair = temp.top();
            temp.pop();
            parkingLot.push(std::make_pair(pair.first, info.time));
            std::cout << "车辆进入, 编号为" << pair.first << ", 位置为" << parkingLot.size() << ", 时间为" << info.time << "。\n";
        }
        if (!waiting.empty())
        {
            auto number = waiting.front();
            waiting.pop();
            parkingLot.push(std::make_pair(number, info.time));
            std::cout << "车辆进入, 编号为" << number << ", 位置为" << parkingLot.size() << ", 时间为" << info.time << "。\n";
        }
        break;
    }
    case Infotype::Arrive:
    {
        if (parkingLot.size() < size)
        {
            parkingLot.push(std::make_pair(info.number, info.time));
            std::cout << "车辆进入, 编号为"<< info.number <<", 位置为" << parkingLot.size() << ", 时间为" << info.time << "。\n";
        }
        else
        {
            waiting.push(info.number);
            std::cout << "车辆进入等待队列，编号为" << info.number << "。\n";
        }
    }
    }
}
#endif