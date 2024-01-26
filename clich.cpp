#include <ftxui/component/captured_mouse.hpp>  // for ftxui
#include <ftxui/component/component.hpp>       // For component
#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>   // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/util/ref.hpp>  // for Ref

class Mode {
 private:
  int mode = 0;

 public:
  explicit Mode() { selectMode(); }

  int getMode() const { return mode; }

  void selectMode() {
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "PULL: 受信",
        "PUSH: 送信",
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

class User {
 private:
  std::string name;

 public:
  explicit User(const std::string key) { fetchName(key); }

  void fetchName(std::string key) {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    InputOption option;
    option.on_enter = screen.ExitLoopClosure();

    // The basic input components:
    Component input_name = Input(&name, key, option);
    Component button = Button(" OK > ", [&] {
      // ここにボタンが押された時の処理を書く
      screen.Exit();
    });

    // The component tree:
    auto component = Container::Vertical({
        input_name,
        button,
    });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
      return vbox({
                 hbox(text(" " + key + " : "), input_name->Render()),
                 separator(), text(" " + key + " : " + name),
                 // separator(),
                 // hbox(button->Render()),
             }) |
             border;
    });

    screen.Loop(renderer);

    return;
  }
};

int main() {
  // モード選択を取得
  Mode m{};

  // ユーザ情報を取得
  User user("user");

  // 送信先を取得
  User to_user("to");

  if (m.getMode() == 0) {
    // PULL モード

  } else {
    // PUSH モード
  }

  return 0;
}
