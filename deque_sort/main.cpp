#include <iostream>
#define arraySize 1000000

#include <deque>

using namespace std;

typedef struct sortDeque SortDeque;


void dequeFormating(deque<int> &dq, deque<deque<int>> &sd, int &p){
    int elem = 0;
    deque<int> asc;
    deque<int> desc;

    asc.clear();
    desc.clear();

    while (!dq.empty()) {
        elem = dq.front();
        dq.pop_front();
        if (asc.empty()) {
            ++p;
            asc.push_back(elem);
            continue;
        } else if (elem >= asc.back()) {
            ++p;
            asc.push_back(elem);
            continue;
        } else if (desc.empty()) {
            ++p;
            desc.push_front(elem);
            continue;
        } else if (elem <= desc.front()) {
            ++p;
            desc.push_front(elem);
            continue;
        } else {
            if (asc.size() > desc.size()) {
                ++p;
                sd.push_back(asc);
                asc.clear();
                asc.push_back(elem);
            } else {
                ++p;
                sd.push_back(desc);
                desc.clear();
                desc.push_back(elem);
            }
        }
    }
    if (!asc.empty()) {
        sd.push_back(asc);
        asc.clear();
    }
    if (!desc.empty()) {
        sd.push_back(desc);
        desc.clear();
    }
}

void mergeDeque(deque<int> &dq, deque<deque<int>> &sd, int &p) {
    deque<int> td1, td2, tdr;

    while (!sd.empty()) {
        td1.clear();
        td2.clear();
        tdr.clear();
        td1 = sd.front();
        sd.pop_front();
        td2 = sd.front();
        sd.pop_front();

        while((!td1.empty()) && (!td2.empty())) {

            if (td1.front() <= td2.front()) {
                ++p;
                tdr.push_back(td1.front());
                td1.pop_front();
            } else {
                ++p;
                tdr.push_back(td1.front());
                td1.pop_front();
            }
        }
        while (!td1.empty()) {
            ++p;
            tdr.push_back(td1.front());
            td1.pop_front();
        }
        while (!td2.empty()) {
            ++p;
            tdr.push_back(td2.front());
            td2.pop_front();
        }
        if (sd.empty()) {
            sd.push_back(tdr);
            break;
        } else {
            sd.push_back(tdr);
        }
    }
    dq = sd.front();
}


int main() {
    int a[arraySize];
    deque<int> dq;
    deque<deque<int>> sd;
    dq.clear();
    sd.clear();
    int tmp = 0;
    for (int i = 0; i < arraySize; ++i) {
        tmp = rand()%100;
        a[i] = tmp;
        dq.push_back(tmp);
    }

    int p = 0;

    //for (int i = 0; i < arraySize; ++i) cout << a[i] << " " ; cout << endl << endl;

    dequeFormating(dq, sd, p);
    deque<deque<int>> sd2;
    sd2 = sd;

    int countLists = sd.size();
    double avgSize = 0;

    while (!sd.empty()) {
        deque<int> &sdt = sd.front();
        avgSize += sdt.size();
        //while (!sdt.empty()) { cout << sdt.front() << " "; sdt.pop_front(); } cout<<endl;
        sd.pop_front();
    }
    avgSize = avgSize/ static_cast<double>(countLists);
    cout << avgSize << endl;


    cout << endl << "p1: " << p << endl;

    mergeDeque(dq,sd2,p);

    cout << endl << "p2: " << p << endl;

    cout << "Length: " << dq.size() << endl ;

    cout<<endl;
  //  while (!dq.empty()) {cout << dq.front() << " " ; dq.pop_front(); } cout << endl;

}