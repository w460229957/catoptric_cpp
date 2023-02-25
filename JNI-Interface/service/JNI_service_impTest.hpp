#pragma once
#include "JNI_service.hpp"
#include "JNI_Command.hpp"
#include "CatoptricController.hpp"
#include <unordered_map>
#include <memory>
#include "../JNI-Interface/commands/JNI_MoveCommand.hpp"
/**
 * @brief This is a test implementation of @interface JNI_service.
 * @details This test implementation is constructed in a sigleton pattern.
 * @author Zhengyuan Zhang
*/

class JNI_service_impTest : public JNI_service{
private:
    CatoptricController * controller;
    std::unordered_map<JNI_Command::Type, std::unique_ptr<JNI_Command>> availCommands;
public:
    JNI_service_impTest();
    JNI_service_impTest& operator=(const JNI_service_impTest&) = delete;
    JNI_service_impTest(const JNI_service_impTest&) = delete;    
    void move(std::vector<int> input_list);
    void reset();
    void test();
    void quit();
};

