#include <ftxui/component/component.hpp> // For component
#include <ftxui/component/screen_interactive.hpp> // For ScreenInteractive

class Mode {
private:
    int mode = 0;

public:
 explicit Mode() { SelectMode(); }

 int getMode() const { return mode; }

 void SelectMode() {
   using namespace ftxui;
   auto screen = ScreenInteractive::TerminalOutput();

   std::vector<std::string> entries = {
       "CLIENT",
       "SERVER",
   };
   int selected = 0;

   MenuOption option;
   option.on_enter = screen.ExitLoopClosure();
   auto menu = Menu(&entries, &selected, option);

   screen.Loop(menu);

   std::cout << "mode = " << selected << std::endl;

   mode = selected;

   return;
    }
};

int main() {
    // モード選択画面を表示

    Mode m{};

    if (m.getMode() == 0) {
        // CLIENT モード
        
    } else {
        // SERVER モード
    }

    return 0;
}
