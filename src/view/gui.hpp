#pragma once

#include "../model/board/board.hpp"
#include "view.hpp"

#include <SFML/Graphics.hpp>


const int TILE_SIZE = 80;
const int BOARD_SIZE = 8;
const int WINDOW_SIZE = TILE_SIZE * BOARD_SIZE;

class GUI : public View {
  public:
    GUI();

    void show() override;
    Piece ask_about_promotion();
    void update_board(const Board&, int8_t, std::vector<int8_t>) override;
    void notify_checkmate(Colour) override;
    void notify_stalemate(Colour) override;

  private:
    sf::RenderWindow window;
    sf::Texture textures[6][2];
    sf::Sprite pieces[64];
    sf::Font font;
    sf::Color colours[2] = {sf::Color(0xB88762FF), sf::Color(0xEDD6B0FF)};
    sf::Color alt_colours[2] = {sf::Color(0xDCC34BFF), sf::Color(0xF6EB72FF)};


    int show_popup(
        const std::string& message, const std::vector<std::string>& options
    );
    void load_textures();
    void handle_events();
    void handle_click(int, int);
    void draw_board(int, std::vector<int8_t>);
    void draw_pieces(const Board&);
    void draw_annotation(int, int);
};
