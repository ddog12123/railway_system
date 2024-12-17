
// view.h

#pragma once

#include "imgui_setup.h"

#include "entity.h"

#include <vector>

// 单例模式
class View
{
public:
    static View& Instance();

public:
    void ViewInit();
    void ViewQuit();

    void ViewShowWindows();

    void ViewConsoleAddLog(const char* fmt, ...) IM_FMTARGS(2); // 添加日志
    void ViewConsoleShowLog(bool* p_open = nullptr);            // 显示日志
    void ViewScrollToBottom();                                  // 滑动到底部

private:
    void show_config_window(bool* p_open = nullptr);      // config window 用于调试
    void show_train_datas_window(bool* p_open = nullptr); // 显示车次信息
    void show_user_input_window(bool* p_open = nullptr);  // 获取用户输入的字符串

private:
    // 布局比例
    float data_window_height  = 0.65f;
    float inuput_window_width = 0.65f;

public:
    bool is_show_demo_window = false;
    bool is_show_train_datas = true;

public:
    int selected_id = -1;

public:
    Color clear_color = { 0x33, 0x33, 0x33, 0xff };

public:
    ImFont* font_default = nullptr;
    ImFont* font_chinese = nullptr;

private:
    std::vector<std::string> logs;

    bool scroll_to_bottom = false;

private:
    View()                       = default;
    ~View()                      = default;
    View(const View&)            = delete;
    View& operator=(const View&) = delete;

    static View* instance;
};
