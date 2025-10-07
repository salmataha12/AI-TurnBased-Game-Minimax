#pragma comment(lib, "user32.lib")  // Ensure User32.lib is linked

#include <windows.h>
#include <iostream>
#include <cstdio>
#include "Board.h"      // Your native Board class header
#include "MyForm.h"     // Your Windows Forms header

using namespace System;
using namespace System::Windows::Forms;
using namespace std;

[STAThreadAttribute]
int Main(cli::array<System::String^>^ args)
{
    // Allocate a console for this application.
    if (AllocConsole()) {
        FILE* fp;
        // Redirect stdout and stdin to the new console.
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONIN$", "r", stdin);

        // (Optional) Bring the console window to the foreground.
        HWND hConsole = GetConsoleWindow();
        if (hConsole != 0)  // Use 0 instead of nullptr to avoid managed nullptr conflicts.
            SetForegroundWindow(hConsole);
    }

    // Run your pre-GUI code.   
    Board board(5);           // Create a 5x5 board.
    board.SetCell(2, 2, 1);     // Set some example values.
    board.SetCell(1, 3, 2);

    cout << "Printing Board:" << endl;
    board.PrintBoard();

    cout << "\nPress any key to continue and launch the GUI..." << endl;
    system("pause");

    // Launch the Windows Forms GUI.
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew AlgoPrj::MyForm());



    return 0;
}
