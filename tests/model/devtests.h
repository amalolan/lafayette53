#ifndef DEVTESTS_H
#define DEVTESTS_H
#ifdef __APPLE__
#define CODE_BASE_DIRECTORY "../../../lafayette53/"
#elif __linux
#define CODE_BASE_DIRECTORY "../../../lafayette53/"
#endif
#include "gtest/gtest.h"
#include "../../model/modelclass.h"


class DevTests : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    DevTests() : model(nullptr){
    // You can do set-up work for each test here.
    }

    virtual ~DevTests() {
        delete this->model;
        this->model = nullptr;
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        if (this->model == nullptr)
            this->model =  new ModelClass(std::string(CODE_BASE_DIRECTORY) + "database/db.db");
    }

    virtual void TearDown() {
    }
    ModelClass * model;
};

#endif // DEVTESTS_H
