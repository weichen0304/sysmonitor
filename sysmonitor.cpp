#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>  // 用於讀取真實 RAM 數據
#include "rlutil.h"   // 核心函式庫

using namespace std;

// --- 全域變數 ---
int max_ram = 0;

// --- 取得 Windows 真實記憶體使用率 ---
int getMemoryUsage() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return (int)memInfo.dwMemoryLoad;
}

// --- 根據負載設定顏色 (綠/黃/紅) ---
void setStatusColor(int val) {
    if (val < 50) rlutil::setColor(rlutil::GREEN);      // 正常
    else if (val < 85) rlutil::setColor(rlutil::YELLOW); // 警告
    else rlutil::setColor(rlutil::RED);                   // 危險
}

// --- 繪製監控長條圖 ---
void drawProgressBar(int val, int y, string label) {
    rlutil::locate(5, y); // 定位座標
    rlutil::setColor(rlutil::WHITE);
    cout << label << ": " << val << "% ";

    setStatusColor(val);
    cout << "[";
    for (int i = 0; i < 20; i++) {
        if (i < val / 5) cout << "|";
        else cout << " ";
    }
    cout << "]";
}

// --- 主程式 ---
int main() {
    rlutil::saveDefaultColor(); // 儲存預設顏色
    rlutil::hidecursor();       // 隱藏游標
    rlutil::cls();              // 清空螢幕
    srand(time(NULL));

    bool running = true;
    while (running) {
        // 非阻塞按鍵檢查
        if (kbhit()) { 
            int k = rlutil::getkey(); 
            if (k == rlutil::KEY_ESCAPE) running = false; // 按 ESC 退出
        }

        // --- 1. 數據獲取 ---
        int current_ram = getMemoryUsage();       // 真實 RAM
        if (current_ram > max_ram) max_ram = current_ram;
        
        int cpu_sim = 10 + (rand() % 40);         // 模擬 CPU 負載
        int gpu_sim = 20 + (rand() % 60);         // 模擬 GPU 負載 (補回這行)

        // --- 2. 介面繪製 ---
        rlutil::locate(1, 1);
        rlutil::setColor(rlutil::CYAN); 
        cout << "=== SYSTEM MONITOR DASHBOARD (Sampling: 1Hz) ===";

        drawProgressBar(cpu_sim, 5, "CPU LOAD ");
        drawProgressBar(current_ram, 7, "RAM USAGE");
        drawProgressBar(gpu_sim, 9, "GPU LOAD "); // 補回這行繪圖

        // --- 3. 底部資訊 ---
        rlutil::locate(5, 11);
        rlutil::setColor(rlutil::GREY); 
        cout << "Peak RAM Recorded: " << max_ram << "%  ";

        rlutil::locate(1, 13);
        rlutil::setColor(rlutil::GREY);
        cout << "Press [ESC] to safely terminate the program.";

        rlutil::msleep(1000); // 1秒更新一次
    }

    rlutil::showcursor(); 
    rlutil::resetColor(); 
    rlutil::cls();
    cout << "Monitoring terminated." << endl;

    return 0;
}