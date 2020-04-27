#ifndef DEVTESTS_H
#define DEVTESTS_H
#ifdef __APPLE__
#define CODE_BASE_DIRECTORY "../../../../../lafayette53/"
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
        ModelClass::initdb(std::string(CODE_BASE_DIRECTORY));
    }

    virtual ~DevTests() {
        delete this->model;
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    virtual void SetUp() {
        if (this->model == nullptr)
            this->model =  ModelClass::getInstance(ModelClass::test);
        model->createTables();
    }

    virtual void TearDown() {
        delete this->model;
        this->model = nullptr;
    }
    ModelClass * model;
};

#endif // DEVTESTS_H
