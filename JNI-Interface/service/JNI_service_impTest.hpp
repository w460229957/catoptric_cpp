#pragma once
#include "JNI_service.hpp"
#include "../commands/JNI_Command.hpp"
#include "../../CatoptricController.hpp"
#include <unordered_map>
#include <memory>
/**
 * @brief This is a test implementation of @interface JNI_service.
 * @author Zhengyuan Zhang
*/

class JNI_service_impTest : public JNI_service{
private:
    std::shared_ptr<CatoptricController> controller;//No need to have smart pointer here, since the controller's lifetime is the same as the process. 
    std::unordered_map<JNI_Command::Type, std::unique_ptr<JNI_Command>> availCommands;
public:
    JNI_service_impTest();
    JNI_service_impTest& operator=(const JNI_service_impTest&) = delete;
    JNI_service_impTest(const JNI_service_impTest&) = delete;    
    void move(const std::initializer_list<int> & input_list);
    void reset();
    void test();
    void quit();
};

