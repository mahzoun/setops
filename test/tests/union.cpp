//
// Created by sauron on 7/27/18.
//
#include "gtest/gtest.h"
#include "server/query.h"
#include "client/verify_union.h"
#include "client/verify_tree.h"

#define SETS_NUM 2
#define SIZE 10

class UnionTest : public ::testing::Test {
protected:
    Key *k;
    DataStructure *dataStructure;
    std::vector<int> v;
    Union *un;
    VerifyTree *verifyTree;
    VerifyUnion *verifyUnion;

    void SetUp(int sets_no) {
        try {
            NTL::ZZ p = NTL::conv<NTL::ZZ>(
                    "16798108731015832284940804142231733909759579603404752749028378864165570215949");
            NTL::ZZ_p::init(p);
            k = new Key(p);
            dataStructure = new DataStructure(sets_no, k);
            for (int i = 1; i <= SIZE; i++) {
                NTL::ZZ_p j = NTL::random_ZZ_p();
                for (int set_index = 0; set_index < dataStructure->m; set_index++) {
                    dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
                }
            }
            for (int set_index = 0; set_index < dataStructure->m; set_index++)
                for (int i = 1; i <= 9 * SIZE / 10; i++) {
                    NTL::ZZ_p j = NTL::random_ZZ_p();
                    dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
                }

            for (int set_index = 0; set_index < dataStructure->m; set_index++)
                v.push_back(set_index);
        }
        catch (std::exception &e) {
            std::cerr << e.what() << "\n";
        }
    }

    void TearDown() {
        delete (verifyTree);
        delete (verifyUnion);
        delete (un);
        delete (dataStructure);
        delete (k);
    }
};

TEST_F(UnionTest, TwoSets) {
    SetUp(SETS_NUM);
    un = new Union(v, k->get_public_key(), dataStructure);
    un->unionSets();
    verifyTree = new VerifyTree;
    verifyTree->verifyTree(dataStructure, v);
    verifyUnion = new VerifyUnion(k->get_public_key(), un->U, un->tree, dataStructure->m, un->set_indices);
    bool b = verifyUnion->verify_union();
    EXPECT_TRUE(b);
    EXPECT_TRUE(verifyTree->verifiedtree);
}

TEST_F(UnionTest, WrongAccWitnessLeaf) {
    SetUp(SETS_NUM);
    un = new Union(v, k->get_public_key(), dataStructure);
    un->unionSets();
    verifyTree = new VerifyTree;
    verifyTree->verifyTree(dataStructure, v);
    un->tree[0][0].F2 *= 2;
    verifyUnion = new VerifyUnion(k->get_public_key(), un->U, un->tree, dataStructure->m, un->set_indices);
    bool b = verifyUnion->verify_union();
    EXPECT_FALSE(b);
    EXPECT_TRUE(verifyTree->verifiedtree);
}

TEST_F(UnionTest, MultipleSetsEvenindices) {
    SetUp(16);
    delete (dataStructure);
    DataStructure *dataStructure = new DataStructure(16, k);
    for (int i = 1; i <= SIZE; i++) {
        NTL::ZZ_p j = NTL::random_ZZ_p();
        for (int set_index = 0; set_index < dataStructure->m - 1; set_index++) {
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    for (int set_index = 0; set_index < dataStructure->m; set_index++) {
        for (int i = 1; i <= 9 * SIZE / 10; i++) {
            NTL::ZZ_p j = NTL::random_ZZ_p();
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    v.clear();
    for (int set_index = 0; set_index < dataStructure->m; set_index += 2)
        v.push_back(set_index);
    verifyTree = new VerifyTree;
    verifyTree->verifyTree(dataStructure, v);
    un = new Union(v, k->get_public_key(), dataStructure);
    un->unionSets();
    verifyUnion = new VerifyUnion(k->get_public_key(), un->U, un->tree, dataStructure->m, un->set_indices);
    bool b = verifyUnion->verify_union();
    EXPECT_TRUE(b);
    EXPECT_TRUE(verifyTree->verifiedtree);
}


TEST_F(UnionTest, MultipleSetsOddindices) {
    SetUp(16);
    delete (dataStructure);
    DataStructure *dataStructure = new DataStructure(16, k);
    for (int i = 1; i <= SIZE; i++) {
        NTL::ZZ_p j = NTL::random_ZZ_p();
        for (int set_index = 0; set_index < dataStructure->m - 1; set_index++) {
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    for (int set_index = 0; set_index < dataStructure->m; set_index++) {
        for (int i = 1; i <= 9 * SIZE / 10; i++) {
            NTL::ZZ_p j = NTL::random_ZZ_p();
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    v.clear();
    for (int set_index = 1; set_index < dataStructure->m; set_index += 2)
        v.push_back(set_index);
    verifyTree = new VerifyTree;
    verifyTree->verifyTree(dataStructure, v);
    un = new Union(v, k->get_public_key(), dataStructure);
    un->unionSets();
    verifyUnion = new VerifyUnion(k->get_public_key(), un->U, un->tree, dataStructure->m, un->set_indices);
    bool b = verifyUnion->verify_union();
    EXPECT_TRUE(b);
    EXPECT_TRUE(verifyTree->verifiedtree);
}

TEST_F(UnionTest, MultipleSets3kIndices) {
    SetUp(16);
    delete (dataStructure);
    DataStructure *dataStructure = new DataStructure(16, k);
    for (int i = 1; i <= SIZE; i++) {
        NTL::ZZ_p j = NTL::random_ZZ_p();
        for (int set_index = 0; set_index < dataStructure->m - 1; set_index++) {
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    for (int set_index = 0; set_index < dataStructure->m; set_index++) {
        for (int i = 1; i <= 9 * SIZE / 10; i++) {
            NTL::ZZ_p j = NTL::random_ZZ_p();
            dataStructure->insert(set_index, j, k->get_public_key(), k->get_secret_key());
        }
    }
    v.clear();
    for (int set_index = 0; set_index < dataStructure->m; set_index += 3)
        v.push_back(set_index);
    verifyTree = new VerifyTree;
    verifyTree->verifyTree(dataStructure, v);
    un = new Union(v, k->get_public_key(), dataStructure);
    un->unionSets();
    verifyUnion = new VerifyUnion(k->get_public_key(), un->U, un->tree, dataStructure->m, un->set_indices);
    bool b = verifyUnion->verify_union();
    EXPECT_TRUE(b);
    EXPECT_TRUE(verifyTree->verifiedtree);
}
