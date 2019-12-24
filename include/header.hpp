// Copyright 2018 Name <mail@gmail.com>
#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

using std::vector;
using std::fstream;
using std::cout;
using std::endl;

size_t l1, l2, l3;
vector<unsigned> meaningExp;
unsigned char *buffer;

class Experiment {
public:
    Experiment(int L1, int L2, int L3) {
        l1 = L1 * 1024; //kb ->byte
        l2 = L2 * 1024; // kb -> byte
        l3 = L3 * 1024*1024; // mb -> byte
        int x = 1;
        int n = 0;

        while (pow(2, x) < 0.5 * l1) {
            ++x;
        }
        while (pow(2, x + n) < (1.5) * l3) {
            meaningExp.push_back(pow(2, x + n));
            ++n;
        }
        meaningExp.push_back(1.5 * l3);
        for ( unsigned i = 0; i < static_cast<int>(meaningExp.size()); ++i ) {
        cout << i << "exp number" << meaningExp[i] << endl;}
    }


    void createBuffer(unsigned size_exp) {
        buffer = new unsigned char[size_exp];
    }

    void warmUp_cache(unsigned size_exp) {
        for (unsigned j = 0; j < 10; ++j) {
            for (unsigned n = 0; n < size_exp; ++n) {
                unsigned char temp = rand_r(&n) % 128;
                buffer[n] = temp;
            }
        }
    }

    clock_t directPass(unsigned numberExp) {
        clock_t begin = clock();
        unsigned tmp;
        for (int d = 0; d < 1000; ++d) {
            for (unsigned i = 0; i < meaningExp[numberExp]; ++i) {
                tmp = i;
                buffer[i] = static_cast<char>(rand_r(&tmp) % 256 - 128);
            }
        }
        clock_t end = clock();
        return end - begin;
    }

    clock_t returnPass(unsigned numberExp) {
        clock_t begin = clock();
        for (int d = 0; d < 1000; ++d) {
            for (unsigned i = meaningExp[numberExp] - 1; i > 0; --i) {
                buffer[i] = static_cast<char>(rand_r(&i) % 256 - 128);
            }
        }
        clock_t end = clock();
        return end - begin;
    }

    clock_t randomPass(unsigned numberExp) {
        vector<int> current_num;
        for (unsigned i = 0; i < meaningExp[numberExp]; ++i) {
            current_num.push_back(i);
        }

        srand(time(NULL));
        random_shuffle(current_num.begin(), current_num.end());
        clock_t begin = clock();
        for (unsigned i = 0; i < 1000; ++i) {
            for (unsigned n = 0; n < meaningExp[numberExp]; ++n) {
                buffer[current_num[n]] =
                        static_cast<char>(rand_r(&n) % 255 - 128);
            }
        }
        clock_t end = clock();
        return end - begin;
    }

    void start() {
        for (int pass = 0; pass < 3; ++pass) {
            const int dir = 0;
            const int ret = 1;
            const int run = 2;
            printTravel_order(pass);
            for (int count = 0; count < 11; ++count) {
                clock_t time;
                createBuffer(meaningExp[count]);
                warmUp_cache(meaningExp[count]);
                if (pass == dir) {
                    time = directPass(count);
                    printRes(time, count, meaningExp[count]);
                }
                if (pass == ret) {
                    time = returnPass(count);
                    printRes(time, count, meaningExp[count]);
                }
                if (pass == run) {
                    time = randomPass(count);
                    printRes(time, count, meaningExp[count]);
                }
            }
        }
    }

    void printTravel_order(int PASS) {
        std::cout << "investigation:" <<  std::endl;
        if (PASS == 0) {
            cout << "\ttravel_variant: " << "direction" <<  endl;
        }
        if (PASS == 1) {
            cout << "\ttravel_variant: " << "return" <<  endl;
        }
        if (PASS == 2) {
            cout << "\ttravel_variant: " << "random" << endl;
        }
        cout << "\texperiments: " << endl;
    }

    void printRes(clock_t TIME, int COUNT, unsigned MEANING) {
        cout << "\t- experiment:" <<  endl;
        cout << "\t\tnumber:" << COUNT + 1 <<  endl;
        cout << "\t\tinput data: " <<  endl;
        cout << "\t\t buffer_size:";
        cout << MEANING << " byte ";
        cout <<  endl << "\t\tresults:" <<  endl;
        cout << "\t\t duration: ";
        cout << TIME << "ms";
        cout <<  endl;
    }
};
#endif // INCLUDE_HEADER_HPP_
