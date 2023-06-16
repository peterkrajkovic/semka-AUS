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
        std::cout << "Aktuálne sa nachádzate v: ";
        actualNode->data_->toString();
        std::cout << "Èo by ste si želali: \n 1: vypísa všetky podradené územia \n 2: prejs na nadradené územie\n 3: prejs na podradené územie\n 4: vypísa podradené územia zaèínajúce na reazec \n 5: vypísa podradené územia obsahujúce reazec \n 6: vypísa podradené územia s poètom obyvate¾ov aspoò\n 7: vypísa podradené územia s poètom obyvate¾ov maximálne\n 8: vypísa územia daného typu\n 9: vyh¾ada v tabu¾ke\n10: ukonèi\n";
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
                std::cout << "Nadradená územná jednotka neexistuje.\n";
            }
            break;
        case 3:
        {
            size_t sonsCount = actualNode->sons_->size();
            int entry = 0;
            if (sonsCount > 0) {
                std::cout << "Podradené územia: ";
                for (int i = 0; i < sonsCount; i++) {
                    std::cout << i+1 << ": " << actualNode->sons_->access(i)->data_->data_->getOfficial() << " ";
                }
                std::cout << "\nNa ktoré územie chceš prejs?\n";
                std::cin >> entry;
                if (entry > 0 && entry <= sonsCount) {
                    actualNode = hierarchy.accessSon(*actualNode, entry - 1);
                }
                else {
                    std::cout << "Nepodarilo sa prejs na podradené územie\n";
                }
            }
            else {
                std::cout << "Obec nemá podradené územie\n";
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
            std::cout << "Aké územia chceš vypísa? obce/okresy/kraje\n";
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
                std::cout << "Také územie nie je v tabu¾ke.\n";
            }
        }
            break;
        case 9:
        {
            std::string territory, type;
            std::cout << "\nKtoré územie chceš vyh¾ada?\n";
            std::cin >> std::ws;
            std::getline(std::cin, territory);
            std::cout << "Akého typu je dané územie? obec/okres/kraj\n";
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
                std::cout << "Také územie nie je v tabu¾ke.\n";
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