#pragma once
#include <vector>
#include "Territory.h"
#include "libds/adt/table.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"
#include <functional>
const std::string vowels = "aeiou·ÈÌÛ˙‰Ù";
const std::string letters = "Aa¡·ƒ‰BbCc»ËDdœÔEe…ÈFfGgHhIiÕÌJjKkLlºæ≈ÂMmNn“ÚOo”Û‘ÙPpQqRr¿‡SsäöTtçùUu⁄˙VvWwXxYy›˝Zzéû";

class Algorithm {
public:
    template<typename Iterator>
    static void search(Iterator start, Iterator end, ds::amt::ImplicitSequence<Territory*>& is, const std::function <bool(Territory*) >& function) {
        while (start != end) {
            if (function(*start)) {
                is.insertLast().data_ = (*start);
            }
            ++start;
        }
    }
    static void sort(ds::amt::ImplicitSequence<Territory*>& is) {
        std::cout << "Podæa Ëoho ich chceö zotriediù ?     \n1 - abecedne\n2 - poËet samohl·sok v n·zve\n3 - poËet obyvateæov\n";
        int num;
        std::cin >> num;
        while (num < 1 || num > 3) {
            std::cout << "Tak· moûnosù nie je.\n";
            std::cout << "Podæa Ëoho ich chceö zotriediù ?     \n1 - abecedne\n2 - poËet samohl·sok v n·zve\n3 - poËet obyvateæov\n";
            std::cin >> num;
        }
        ds::adt::QuickSort<Territory*> sor;
        switch (num) {
        case 1:
            sor.sort(is, [&](Territory* a, Territory* b) {return Algorithm::compare(a->getOfficial(), b->getOfficial());});
            break;
        case 2:
            sor.sort(is, [&](Territory* a, Territory* b) {return Algorithm::countVowels(a->getOfficial()) > Algorithm::countVowels(b->getOfficial());});
            break;
        case 3:
            sor.sort(is, [&](Territory* a, Territory* b) {return a->getNumberOfCitizens() > b->getNumberOfCitizens();});
            break;
        }
        for (auto x : is) {
            x->toString();
        }
    }

    static int countVowels(std::string a) {
        int count = 0;
        for (char i : a) {
            for (char vowel : vowels) {
                if (i == vowel) {
                    count++;
                    break;
                }
            }
        }
        return count;
    }

    static bool compare(std::string a, std::string b) {
        //j represents index of char in string b, j index of char in a 
        int j = 0;
        for (int i = 0; i < a.length(); i++) {
            if (b[j] == ' ') {
                j++;
            }
            if (a[i] == ' ') {
                i++;
            }
            if (j >= b.length()) {
                return false;
            }
            if (i >= a.length()) {
                return true;
            }
            double pos_a = 0;
            double pos_b = 0;
            int pos = 0;
            for (char letter : letters) {
                if (letter == a[i]) {
                    pos_a = pos;
                }
                if (letter == b[j]) {
                    pos_b = pos;
                }
                pos++;
            }
            //check for ch,dz,dû
            if (i + 1 < a.length()) {
                if ((a[i] == 'c' || a[i] == 'C') && a[i + 1] == 'h') {
                    pos_a = letters.find('h') + 0.5;
                    i++;
                }
                else if (a[i] == 'd' || a[i] == 'D') {
                    if (a[i + 1] == 'z') {
                        pos_a = letters.find('Ô') + 0.3;
                        i++;
                    }
                    else if (a[i + 1] == 'û') {
                        pos_a = letters.find('Ô') + 0.6;
                        i++;
                    }
                }
            }
            if (j + 1 < b.length()) {
                if ((b[j] == 'c' || b[j] == 'C') && b[j + 1] == 'h') {
                    j++;
                    pos_b = letters.find('h') + 0.5;
                }
                else if (b[j] == 'd' || b[j] == 'D') {
                    if (b[j + 1] == 'z') {
                        pos_b = letters.find('Ô') + 0.3;
                        j++;
                    }
                    else if (b[j + 1] == 'û') {
                        pos_b = letters.find('Ô') + 0.6;
                        j++;
                    }
                }
            }
            //comparing positions
            if (pos_a > pos_b) {
                return false;
            }
            else if (pos_b > pos_a) {
                return true;
            }

            j++;
        }
        return false;
    }
};