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
    std::cout << "����ͣ������С: \n";
    std::cin >> size;
    std::cout << "���뵥λʱ���շ�: \n";
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
    std::cout << "������Ϣ (A:Arrive, D:Departure, E:End): \n";
    char type;
    unsigned int number;
    unsigned int time;
    std::cin >> type >> number >> time;

    if (std::cin.fail()) {
        std::cout << "��Ч���룬�����ԡ�\n";

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
        std::cout << "�������\n";
        info.type = Infotype::None;
        return false;
    default:
        std::cout << "��Ч���ͣ������� 'A', 'D' �� 'E'��\n";
        info.type = Infotype::None;
        return true;
    }
}
void HandleInfo(ParkInfo const& info)
{
    if (info.time < nowTime)
    {
        std::cout << "����ʱ�䵹���������ԡ�\n";
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
            std::cout << "�����뿪�����Ϊ" << pair.first << ", �ɷ�" << (info.time - pair.second) * price << "��\n";
        }
        auto pair = parkingLot.top();
        parkingLot.pop();
        std::cout << "�����뿪�����Ϊ" << pair.first << ", �ɷ�" << (info.time - pair.second) * price << "��\n";
        while (!temp.empty())
        {
            auto pair = temp.top();
            temp.pop();
            parkingLot.push(std::make_pair(pair.first, info.time));
            std::cout << "��������, ���Ϊ" << pair.first << ", λ��Ϊ" << parkingLot.size() << ", ʱ��Ϊ" << info.time << "��\n";
        }
        if (!waiting.empty())
        {
            auto number = waiting.front();
            waiting.pop();
            parkingLot.push(std::make_pair(number, info.time));
            std::cout << "��������, ���Ϊ" << number << ", λ��Ϊ" << parkingLot.size() << ", ʱ��Ϊ" << info.time << "��\n";
        }
        break;
    }
    case Infotype::Arrive:
    {
        if (parkingLot.size() < size)
        {
            parkingLot.push(std::make_pair(info.number, info.time));
            std::cout << "��������, ���Ϊ"<< info.number <<", λ��Ϊ" << parkingLot.size() << ", ʱ��Ϊ" << info.time << "��\n";
        }
        else
        {
            waiting.push(info.number);
            std::cout << "��������ȴ����У����Ϊ" << info.number << "��\n";
        }
    }
    }
}
#endif