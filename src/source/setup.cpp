//
// Created by sauron on 7/12/18.
//

#include "source/setup.h"
#define NODEBUG

DataStructure::DataStructure() {
    this->merkleTree = nullptr;
}

DataStructure::DataStructure(int size, Key *key){
    debug("Generating datastructure with %d sets", size);
    this->m = size;
    setup(key->get_public_key(), key->get_secret_key());
}

DataStructure::~DataStructure(){
    if(merkleTree) {
        delete merkleTree;
    }
}

void DataStructure::setup(PublicKey *pk, SecretKey *sk) {
    Utils utils;
    NTL::ZZ_p s = sk->sk;
    this->merkleTree = new MerkleTree(m, this, pk, sk);
    for (int i = 0; i < m; i++) {
        AuthD[i] = utils.compute_digest(D[i], pk->g1, sk);
        DEBUGINDEX("Authenticated value for set ", i , AuthD[i]);
    }
    merkleTree->build(this, pk, sk);
    this->depth = merkleTree->depth;
}

void DataStructure::insert(int index, NTL::ZZ_p element, PublicKey *pk, SecretKey *sk){
    Utils utils;
    try {
        D[index].insert(element);
        NTL::ZZ_p temp1 = sk->sk + element;
        const char* temp1_str = utils.zToString(temp1);
        const mie::Vuint temp(temp1_str);
        free((char*)temp1_str);
        AuthD[index] *= temp;
        DEBUGINDEX("Authenticated value of ", index, AuthD[index]);
        merkleTree->update(this, pk, sk, index);
        this->depth = merkleTree->depth;
    }
    catch (std::exception& e) {
        log_err("Error happened in insert to set function");
        std::cerr<<e.what() << '\n';
    }
}

