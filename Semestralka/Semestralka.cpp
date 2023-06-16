#include <libds/heap_monitor.h>
#include <iostream>
#include "Printer.h"
#include "Structures.h"
#include "libds/amt/hierarchy.h"
#include "libds/amt/implicit_sequence.h"
#include "libds/adt/table.h"
#include "libds/adt/sorts.h"
#include <Windows.h>
int main()
{
    initHeapMonitor();
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);
    Structures structures;
    structures.load();
    auto hierarchy = structures.getHierarchy();
    bool program = true;
    auto actualNode = hierarchy.accessRoot();
    int entry = 0;
    while (program) {
        std::cout << "Aktu�lne sa nach�dzate v: ";
        actualNode->data_->toString();
        std::cout << "�o by ste si �elali: \n 1: vyp�sa� v�etky podraden� �zemia \n 2: prejs� na nadraden� �zemie\n 3: prejs� na podraden� �zemie\n 4: vyp�sa� podraden� �zemia za��naj�ce na re�azec \n 5: vyp�sa� podraden� �zemia obsahuj�ce re�azec \n 6: vyp�sa� podraden� �zemia s po�tom obyvate�ov aspo�\n 7: vyp�sa� podraden� �zemia s po�tom obyvate�ov maxim�lne\n 8: vyp�sa� �zemia dan�ho typu\n 9: vyh�ada� v tabu�ke\n10: ukon�i�\n";
        std::cin >> entry;
        switch (entry) {
        case 1:
            Printer::printHierarchy(hierarchy, actualNode, 0);
            break;
        case 2:
            if (actualNode->parent_ != nullptr) {
                actualNode = hierarchy.accessParent(*actualNode);
                
            }
            else {
                std::cout << "Nadraden� �zemn� jednotka neexistuje.\n";
            }
            break;
        case 3:
        {
            size_t sonsCount = actualNode->sons_->size();
            int entry = 0;
            if (sonsCount > 0) {
                std::cout << "Podraden� �zemia: ";
                for (int i = 0; i < sonsCount; i++) {
                    std::cout << i+1 << ": " << actualNode->sons_->access(i)->data_->data_->getOfficial() << " ";
                }
                std::cout << "\nNa ktor� �zemie chce� prejs�?\n";
                std::cin >> entry;
                if (entry > 0 && entry <= sonsCount) {
                    actualNode = hierarchy.accessSon(*actualNode, entry - 1);
                }
                else {
                    std::cout << "Nepodarilo sa prejs� na podraden� �zemie\n";
                }
            }
            else {
                std::cout << "Obec nem� podraden� �zemie\n";
            }
        }
            break;
        case 4:
            Printer::printHierarchy(hierarchy, actualNode, 2);
            break;
        case 5:
            Printer::printHierarchy(hierarchy, actualNode, 1);
            break;
        case 6:
            Printer::printHierarchy(hierarchy, actualNode, 3);
            break;
        case 7:
            Printer::printHierarchy(hierarchy, actualNode, 4);
            break;
        case 8:
        {
            std::string type;
            std::cout << "Ak� �zemia chce� vyp�sa�? obce/okresy/kraje\n";
            std::cin >> type;
            if (type == "obce") {
                auto table = structures.getTowns();
                Printer::printTable(table);
            }
            else if (type == "okresy") {
                auto table = structures.getDistricts();
                Printer::printTable(table);
            }
            else if (type == "kraje") {
                auto table = structures.getRegions();
                Printer::printTable(table);
            }
            else {
                std::cout << "Tak� �zemie nie je v tabu�ke.\n";
            }
        }
            break;
        case 9:
        {
            std::string territory, type;
            std::cout << "\nKtor� �zemie chce� vyh�ada�?\n";
            std::cin >> std::ws;
            std::getline(std::cin, territory);
            std::cout << "Ak�ho typu je dan� �zemie? obec/okres/kraj\n";
            std::cin >> type;
            if (type == "obec") {
                auto table = structures.getTowns();
                Printer::findInTable(table, territory);
            }
            else if (type == "okres") {
                auto table = structures.getDistricts();
                Printer::findInTable(table, territory);
            }
            else if (type == "kraj") {
                auto table = structures.getRegions();
                Printer::findInTable(table, territory);
            }
            else {
                std::cout << "Tak� �zemie nie je v tabu�ke.\n";
            }

        }
            break;
        case 10:
            program = false;
            break;
        default:
            break;
        }
        std::cout << "---------------------------------------------------------\n";
    }
    return 0;
}