#pragma once
#include <libds/amt/explicit_hierarchy.h>
#include "Territory.h"
#include "libds/adt/sorts.h"
#include "Algorithm.h"

class Printer {
public:
    static void printHierarchy(ds::amt::MultiWayExplicitHierarchy<Territory*>& hierarchy, ds::amt::MultiWayExplicitHierarchyBlock<Territory*>* actualNode, int number) {
        if (number < 0 || number > 4) {
            return;
        }
        std::string substring;
        int count = 0;
        if (number > 0 && number < 3) {
            std::cout << "Zadaj reazec.\n";
            std::cin >> substring;
            std::transform(substring.begin(), substring.end(), substring.begin(), ::tolower);
        }
        if (number > 2 && number < 5) {
            std::cout << "Zadaj poèet obyvate¾ov.\n";
            std::cin >> count;
        }
        std::cout << "\n";
        ds::amt::ImplicitSequence<Territory*> is;
        Algorithm::search(hierarchy.beginPreAt(actualNode), hierarchy.end(),is, [&](Territory* t) {
                //1 insert all
                if (number == 0) {
                    return true;
                }
                else {
                    std::string officialName = (*t).getOfficial();
                    std::transform(officialName.begin(), officialName.end(), officialName.begin(), ::tolower);
                    std::string mediumName = (*t).getMedium();
                    std::transform(mediumName.begin(), mediumName.end(), mediumName.begin(), ::tolower);
                    int numberOfCitizens = (*t).getNumberOfCitizens();
                    //2 containing substring
                    if (number == 1) {
                        if (officialName.find(substring) != std::string::npos || mediumName.find(substring) != std::string::npos) {
                            return true;
                        }
                        return false;
                    }
                    //3 starting with substring
                    else if (number == 2) {
                        if (officialName._Starts_with(substring) || mediumName._Starts_with(substring)) {
                            return true;
                        }
                        return false;
                    }
                    //4 having number of citizens at least
                    else if (number == 3) {
                        if (count < numberOfCitizens) {
                            return true;
                        }
                        return false;
                    }
                    //5 having number of citizens max
                    else if (number == 4) {
                        if (count > numberOfCitizens) {
                            return true;
                        }
                        return false;
                    }
                }
        });
        if (is.isEmpty()) {
            std::cout << "Nenašli sa územia splòujúce podmienku.\n";
        }
        else {
            for (auto x : is) {
                x->toString();
            }

            std::cout << "_______________________________________________\n";
            std::cout << "Chceš ich aj zotriedi?   \n1 - áno \n2 - nie\n";
            int entry;
            std::cin >> entry;
            if (entry == 1) {
                Algorithm::sort(is);
            }
        }
    }
    static void findInTable(ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*>& table, std::string terr) {
        if (table.contains(terr)) {
            auto sequence = table.find(terr);
            for (int i = 0; i < sequence->size(); i++) {
                sequence->access(i)->data_->toString();
            }
        }
        else {
            std::cout << "Žiadne také územie sa nenašlo.\n";
        }
    }
    static void printTable(ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*>& table) {
        ds::amt::ImplicitSequence<Territory*> is;
        for (auto tabItem : table) {
            auto sequence = tabItem.data_;
            for (int i = 0; i < sequence->size(); i++) {
                is.insertLast().data_ = sequence->access(i)->data_;
            }
        }
        if (is.isEmpty()) {
            std::cout << "Tabu¾ka je prázdna.\n";
        }
        else {
            for (auto x : is) {
                x->toString();
            }

            std::cout << "_______________________________________________\n";
            std::cout << "Chceš ich aj zotriedi?   \n1 - áno \n2 - nie\n";
            int entry;
            std::cin >> entry;
            if (entry == 1) {
                Algorithm::sort(is);
            }
        }
    }

};