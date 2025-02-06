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
    void update_board(const Board&, int8_t, std::vector<Move>) override;
    void notify_checkmate() override;
    void notify_stalemate() override;

  private:
    sf::RenderWindow window;
    sf::Texture textures[6][2];
    sf::Sprite pieces[64];
    sf::Font font;
    sf::Color colours[2] = {
        sf::Color(0xB88762FF), sf::Color(0xEDD6B0FF)
    }; // Light and dark squares

    void load_textures();
    void handle_events();
    void handle_click(int, int);
    void draw_board(int, std::vector<Move>);
    void draw_pieces(const Board&);
    void draw_annotation(int, int);
};
