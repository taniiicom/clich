#include <arpa/inet.h>
#include <unistd.h>  // https://linux.die.net/man/2/read

#include <chrono>  // for operator""s, chrono_literals
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <ftxui/component/captured_mouse.hpp>  // for ftxui
#include <ftxui/component/component.hpp>       // For component
#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component.hpp>       // for Slider
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>   // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/dom/elements.hpp>  // for text, bgcolor, color, vbox, hbox, separator, operator|, Elements, Element, Fit, border
#include <ftxui/dom/elements.hpp>  // for Element, operator|, separator, filler, hbox, size, spinner, text, vbox, bold, border, Fit, EQUAL, WIDTH
#include <ftxui/dom/node.hpp>      // for Render
#include <ftxui/screen/color.hpp>  // for Color, Color::Black, Color::Blue, Color::BlueLight, Color::Cyan, Color::CyanLight, Color::Default, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::RedLight, Color::White, Color::Yellow, Color::YellowLight, Color::Palette256, ftxui
#include <ftxui/screen/color.hpp>  // for ftxui
#include <ftxui/screen/color_info.hpp>  // for ColorInfo
#include <ftxui/screen/screen.hpp>      // for Full, Screen
#include <ftxui/screen/terminal.hpp>  // for ColorSupport, Color, Palette16, Palette256, TrueColor
#include <ftxui/util/ref.hpp>  // for Ref
#include <iostream>            // for cout, endl, ostream
#include <iostream>
#include <memory>  // for allocator, shared_ptr
#include <random>
#include <string>  // for to_string, operator<<, string
#include <string>
#include <thread>   // for sleep_for
#include <utility>  // for move
#include <vector>   // for vector

#define BUFF_SIZE 1024

const std::string SIGN = "1iSAUYKj";

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
                 separator(), text(" " + key + " : " + name),
                 // separator(),
                 // hbox(text("   "), button->Render()),
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
                 separator(), text(" " + key + " : " + tx),
                 // separator(),
                 // hbox(text("   "), button->Render()),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    return;
  }
};

class Communicator {
 private:
  std::string server_ip;
  std::string responce;

 public:
  explicit Communicator() { fetchServerIp(); }

  int fetchServerIp() {
    using namespace ftxui;
    const std::string key = "message";

    auto screen = ScreenInteractive::TerminalOutput();

    InputOption option;
    option.on_enter = screen.ExitLoopClosure();
    option.multiline = false;

    // The basic input components:
    Component input_name = Input(&server_ip, key, option);
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
                 hbox(text(" server ip : "), input_name->Render()), separator(),
                 text(" server ip : " + server_ip),
                 // separator(),
                 // hbox(text("   "), button->Render()),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    return 0;
  }

  std::string getRespnce() { return responce; }

  int confirm(std::string query) {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    InputOption option;
    option.on_enter = screen.ExitLoopClosure();
    option.multiline = false;

    // The basic input components:
    Component cancel_button = Button("     CANCEL     ", [&] {
      // ここにボタンが押された時の処理を書く
      std::cout << "canceled" << std::endl;
      exit(1);
    });

    Component button = Button("     SEND >     ", [&] {
      // ここにボタンが押された時の処理を書く
      screen.Exit();
    });

    // The component tree:
    auto component = Container::Vertical({
        cancel_button,
        button,
    });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
      return vbox({
                 hbox(text(" query : " + query)),
                 separator(),
                 hbox(text(" "), cancel_button->Render(), text(" "),
                      button->Render()),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    return 0;
  }

  int sending(std::string query) {
    {  // UI
      using namespace ftxui;
      using namespace std::chrono_literals;

      std::string reset_position;
      for (int index = 0; index < 40; ++index) {
        auto document = hbox({
            hbox({
                text(" sending now "),
                spinner(1, index) | bold,
                text(" "),
            }) | border |
                color(Color::Cyan1),
            filler(),
        });
        auto screen =
            Screen::Create(Dimension::Full(), Dimension::Fit(document));
        Render(screen, document);
        std::cout << reset_position;
        screen.Print();
        reset_position = screen.ResetPosition();

        std::this_thread::sleep_for(0.1s);
      }
      std::cout << std::endl;
    }

    {
      using namespace std;

      cout << "tcp echo client v1.0.0"
           << endl;  // ソースコードへの変更を行ったら数値を変える．

      // サーバのアドレスとポート番号
      // 127.0.0.1は，ループバックアドレス
      // 他のPCと通信する場合は，当該PCのIPアドレスに変更する．
      string serv_ip = this->server_ip;
      in_port_t serv_port = 5000;

      char buff[BUFF_SIZE];  // 受信用バッファ
      int n = 0;             // 戻り値の保存用に使う変数．

      // ソケット作成，入力はIP，ストリーム型，TCPを指定．
      int socketd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if (socketd < 0) {
        cout << "Failed to createa socket\n";
        return -1;
      }

      // サーバのアドレス等を初期化．
      struct sockaddr_in serv_addr;
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = inet_addr(serv_ip.c_str());
      serv_addr.sin_port = htons(serv_port);

      // サーバに接続する．
      n = connect(socketd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
      if (n < 0) {
        cout << "Failed to connect to the server\n";
        return -1;
      }

      // 署名
      string signature = SIGN + "\n";
      n = write(socketd, signature.c_str(), 9);

      if (n <= 0) {
        cout << "Failed to send a signature.\n";
        return -1;
      }

      std::string request = query + "\n";

      // サーバにデータを送信する．
      n = write(
          socketd, request.c_str(),
          query
              .size());  // 文字列の送信．第二引数は記憶域．第３引数は送信するByte数．

      if (n <= 0) {
        cout << "Failed to send a query.\n";
        return -1;
      }

      // サーバからデータを受信する．
      string recv_msg = "";  // 受信メッセージ

      n = read(socketd, buff, sizeof(buff));

      if (n < 0) {
        // 相手の通信が切断されている．
        cout << "failed to read from a socket\n";
        return -1;
      }

      this->responce = buff;
      return 0;
    }
  }

  void sent() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    // The component tree:
    auto component = Container::Vertical({

    });

    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
      return vbox({
                 hbox(text(" successfully sent. ")),
             }) |
             border | color(Color::Cyan1);
    });

    screen.Loop(renderer);

    return;
  }

  int request(std::string query) {
    // 確認画面を表示
    if (confirm(query)) {
      // キャンセルされた
      return 1;
    }

    // 送信中画面を表示
    sending(query);

    // 送信済み
    sent();

    return 0;
  }
};

int main() {
  // 接続先 ip を指定
  // Communicator を作成
  Communicator cmm{};

  // モード選択を取得
  Mode m{};

  // ユーザ情報を取得
  User user("user");

  std::string query = "";

  if (m.getMode() == 0) {
    // PULL モード

    // 受信元を取得
    User from("from");

    // query を作成
    query = "get-from \"" + user.getName() + "\" \"" + from.getName() + "\"";

  } else {
    // PUSH モード

    // 送信先を取得
    User to("to");

    // メッセージを作成
    Message msg{};
    msg.fetchText();

    // query を作成
    query = "send-to \"" + user.getName() + "\" \"" + to.getName() + "\" \"" +
            msg.getTx() + "\"";
  }

  // std::cout << "query = " << query << std::endl;

  // query を送信
  if (cmm.request(query)) {
    // キャンセル or 送信に失敗
    return 1;
  };

  // 受信を表示
  std::cout << cmm.getRespnce() << std::endl;

  return 0;
}
