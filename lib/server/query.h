//
// Created by sauron on 7/18/18.
//

#ifndef BILINEAR_QUERY_H
#define BILINEAR_QUERY_H

#include <set>
#include <vector>
#include <algorithm>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZVec.h>
#include <NTL/ZZ_pX.h>
#include <NTL/vector.h>
#include "bn.h"
#include "test_point.hpp"
#include "source/setup.h"
#include "utils/utils.h"
#include "utils/merkletree.h"
#define SETS_MAX_NO 2000
#define SETS_MAX_SIZE 10000
#define SMALL_QUERY_SIZE 2
class Query{
public:
    PublicKey *pk;
    DataStructure *dataStructure;
};

class Intersection: Query{
public:
    std::set<NTL::ZZ_p, ZZ_p_compare> I;
    std::vector<int> indices;
    bn::Ec2 *W[SETS_MAX_NO];
    bn::Ec1 *Q[SETS_MAX_NO], *digest_I;
    NTL::vec_ZZ_p c;
    NTL::ZZ_pX p[SETS_MAX_NO], q[SETS_MAX_NO], polyA, polyB, polyS, polyT, polyD;
    Intersection();
    Intersection(std::vector<int>, PublicKey*, DataStructure*);
    void xgcdTree();
    void intersect();
    void subset_witness();
    void completeness_witness();
};

class Union: Query {
public:
    std::set<NTL::ZZ_p, ZZ_p_compare> U;
    std::vector<int> indices, set_indices;
    NTL::vec_ZZ_p c;
    bn::Ec2 *W1[SETS_MAX_SIZE];
    bn::Ec2 *W2[SETS_MAX_NO];
    NTL::ZZ_pX p;
    Union();
    Union(std::vector<int>, PublicKey*, DataStructure*);
    void unionSets();
    void membership_witness();
    void superset_witness();
};

class Subset: Query {
public:
    bool answer;
    int index[SMALL_QUERY_SIZE];
    bn::Ec2 *W;
    bn::Ec2 *Q[SMALL_QUERY_SIZE];
    NTL::vec_ZZ_p c, tmp_c;
    NTL::ZZ_p y;
    NTL::ZZ_pX p[SMALL_QUERY_SIZE], q[SMALL_QUERY_SIZE], polyD;
    Subset();
    Subset(int, int, PublicKey*, DataStructure*);
    void subset();
    void positiveWitness();
    void negativeWitness();
};

class Difference: Query {
public:
    std::set<NTL::ZZ_p, ZZ_p_compare> D, I;
    int index[2];
    bn::Ec2 *W[SMALL_QUERY_SIZE], *Wd;
    bn::Ec1 *Q[SMALL_QUERY_SIZE], *digest_D;
    NTL::vec_ZZ_p c;
    NTL::ZZ_pX p[SMALL_QUERY_SIZE], q[SMALL_QUERY_SIZE], polyD;
    Difference();
    Difference(int[], PublicKey*, DataStructure*);
    void difference();
    void witness();
};

#endif //BILINEAR_QUERY_H
