#include <iostream>
#include <chrono>
#include <cstdlib>
#include <NTL/ZZ.h>
#include "source/setup.h"
#include "source/genkey.h"
#include "server/query.h"
#include "client/verify_intersection.h"

void test(int size, Key *k){
    using namespace std::chrono;
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;

    //generate sets
    DataStructure *dataStructure = new DataStructure;
    dataStructure->setup(k->get_public_key(), k->get_secret_key());
    for(int i = 1; i <= size/10; i++) {
        int j = rand();
        dataStructure->insert(0, j, k->get_public_key(), k->get_secret_key());
        dataStructure->insert(1, j, k->get_public_key(), k->get_secret_key());
    }
    for(int i = 1; i <= 9*size/10; i++) {
        int j = rand();
        dataStructure->insert(0, j, k->get_public_key(), k->get_secret_key());
    }
    for(int i = 1; i < size; i++) {
        int j = rand();
        dataStructure->insert(1, j, k->get_public_key(), k->get_secret_key());
    }

    std::cout<<"Size of first set:\t" << dataStructure->D[0].size()<<"\n";
    std::cout<<"Size of second set:\t" << dataStructure->D[1].size()<<"\n";
//    for(int i = 0; i < dataStructure->m; i++){
//        std::cout<<"AuthD[" << i <<"]:\t" << dataStructure->AuthD[i] << "\n";
//    }

    t1 = high_resolution_clock::now();
    //query intersection
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    Intersection *intersection = new Intersection(v, k->get_public_key(), dataStructure);
    intersection->intersect();
    intersection->subset_witness();
    intersection->completeness_witness();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "query time:\t" << duration << "\n";

    //verify intersection

    t1 = high_resolution_clock::now();
    VerifyIntersection *verifyIntersection = new VerifyIntersection(k->get_public_key(), *intersection->digest_I, intersection->I, intersection->W1, intersection->W2, intersection->Q1, intersection->Q2, dataStructure->AuthD, dataStructure->m);
    bool b = verifyIntersection->verify_intersection();
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "verify time:\t" << duration << "\n";
    std::cout<<"Intersection result is: \t" << b;

}

int main() {
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //generate keys
    Key *k = new Key;
    NTL::ZZ p=NTL::conv<NTL::ZZ>("16798108731015832284940804142231733909759579603404752749028378864165570215949");
    NTL::ZZ_p::init(p);
    k->genkey(p);
//    PUT(k->get_secret_key()->sk);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    std::cout << "Key generation time:\t" << duration << "\n";

    for(int test_size = 10; test_size <= 10 ; test_size *= 10)
        test(test_size, k);

    return 0;
}