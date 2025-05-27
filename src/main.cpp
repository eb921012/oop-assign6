#include <iostream>
#include <string>
#include "view.h"
#include "controller.h"
#include "AnsiPrint.h"
/**
 * Print my id
 * */
void
PrintMyID(std::string studId) {

    std::string str = "ID: " + studId;
    std::cout << AnsiPrint(str.c_str(),YELLOW,RED, true, true) << std::endl << std::endl;
}



int main(){
    // TODO 
    // Integrate the Simple MVC Pattern to run the program.
	// 建立 MVC 架構：Controller 內部會持有 Model 與 View
    View view;
	Controller controller(view);
	//Controller controller;


    // TODO
    // change to your student ID.
	// 執行遊戲主迴圈
    controller.run();
    PrintMyID("112207321");
}
