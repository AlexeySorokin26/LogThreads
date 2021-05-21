#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <shared_mutex>

using namespace std;


class Logger {
public:
    Logger(string fn) : fileName(fn) {
        fs.open(fileName, ios::in | ios::out);
    }

    ~Logger() {
        if(fs.is_open())
            fs.close();
    }

    void writer() {
        sm.lock();
        if (!fs.is_open()) {
            fs.open(fileName, ios::out);
            generateRandom();
        }
        else {
            generateRandom();
        }
        fs.close();
        sm.unlock();
    }

    void reader() {
        sm.lock_shared();
        if (!fs.is_open()) {
            fs.open(fileName, ios::in);
            readRandom();
        }
        else {
            readRandom();
        }
        fs.close();
        sm.unlock_shared();
    }

private:
    shared_mutex sm;
    string fileName;
    fstream fs;

    void generateRandom() {
        srand(time(nullptr));
        int n = 10;
        for (int i = 0; i < n; ++i)
            fs << rand() % 10 << endl;
    }

    void readRandom() {
        string tmp;
        while (getline(fs, tmp))
            cout << tmp << ", ";
    }
};


string FileName = "D://Learning//C++//code//SkillFactory//LogThreads//files//t.txt";

Logger l(FileName);

void writer() {
    l.writer();
}

void reader() {
    l.reader();
}


int main()
{
    thread tW(writer);
    thread tR(reader);
    thread tW2(writer);
    thread tR2(reader);

    if(tW.joinable())
        tW.join();
    if (tR.joinable())
        tR.join();
    if (tW2.joinable())
        tW2.join();
    if (tR2.joinable())
        tR2.join();
}

