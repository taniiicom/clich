#include <ftxui/component/captured_mouse.hpp>  // for ftxui
#include <ftxui/component/component.hpp>       // For component
#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>   // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/dom/elements.hpp>  // for text, bgcolor, color, vbox, hbox, separator, operator|, Elements, Element, Fit, border
#include <ftxui/dom/node.hpp>      // for Render
#include <ftxui/screen/color.hpp>  // for Color, Color::Black, Color::Blue, Color::BlueLight, Color::Cyan, Color::CyanLight, Color::Default, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::RedLight, Color::White, Color::Yellow, Color::YellowLight, Color::Palette256, ftxui
#include <ftxui/screen/color_info.hpp>  // for ColorInfo
#include <ftxui/screen/screen.hpp>      // for Full, Screen
#include <ftxui/screen/terminal.hpp>  // for ColorSupport, Color, Palette16, Palette256, TrueColor
#include <ftxui/util/ref.hpp>  // for Ref
#include <memory>              // for allocator, shared_ptr
#include <utility>             // for move
#include <vector>              // for vector

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
        "PULL: メッセージを受信",
        "PUSH: メッセージを送信",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu = Menu(&entries, &selected, option);

    // The component tree:
    auto component = Container::Vertical({
        menu,
    });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
      return vbox({
                 hbox(text(" menu")),
                 separator(),
                 menu->Render(),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    // std::cout << "mode = " << selected << std::endl;

    mode = selected;

    return;
  }
};

class User {
 private:
  std::string name;

 public:
  explicit User(const std::string key) { fetchName(key); }

  std::string getName() const { return name; }

  void fetchName(std::string key) {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    InputOption option;
    option.on_enter = screen.ExitLoopClosure();
    option.multiline = false;

    // The basic input components:
    Component input_name = Input(&name, key, option);
    Component button = Button("   NEXT >   ", [&] {
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
                 separator(),
                 text(" " + key + " : " + name),
                 separator(),
                 hbox(text("   "), button->Render()),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    return;
  }
};

class Message {
 private:
  std::string tx;

 public:
  explicit Message() {}

  std::string getTx() const { return tx; }

  void fetchText() {
    using namespace ftxui;
    const std::string key = "message";

    auto screen = ScreenInteractive::TerminalOutput();

    InputOption option;
    option.on_enter = screen.ExitLoopClosure();
    option.multiline = false;

    // The basic input components:
    Component input_name = Input(&tx, key, option);
    Component button = Button("   NEXT >   ", [&] {
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
                 separator(),
                 text(" " + key + " : " + tx),
                 separator(),
                 hbox(text("   "), button->Render()),
             }) |
             border | color(Color::Cyan1);
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
  User to("to");

  std::string query = "";

  if (m.getMode() == 0) {
    // PULL モード

    // メッセージを取得
    Message msg{};
    msg.fetchText();

    // query を作成
    query = "send-to" + user.getName() + " " + to.getName() + " " + msg.getTx();

  } else {
    // PUSH モード
  }

  std::cout << "query = " << query << std::endl;

  return 0;
}
