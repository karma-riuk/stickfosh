#include "gui.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

GUI::GUI() {
    window.create(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Board");
    load_textures();
}

void GUI::update_board(
    const Board& b, int8_t selected_square, std::vector<Move> legal_moves
) {
    window.clear();
    draw_board(selected_square, legal_moves);
    draw_pieces(b);
    window.display();
}

void GUI::notify_stalemate() {}

void GUI::notify_checkmate() {}

void GUI::handle_events() {
    sf::Event event;
    while (window.pollEvent(event))
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseButtonPressed)
            handle_click(event.mouseButton.x, event.mouseButton.y);
}

void GUI::load_textures() {
    const std::string names[6] =
        {"rook", "knight", "bishop", "queen", "king", "pawn"
        }; // don't touch the order, it's reflecting the one in the Piece enum
    for (int i = 0; i < 6; ++i) {
        textures[i][0].loadFromFile("res/pieces/white-" + names[i] + ".png");
        textures[i][1].loadFromFile("res/pieces/black-" + names[i] + ".png");
    }
}

void GUI::handle_click(int x, int y) {
    // int file = x / TILE_SIZE;
    // int rank = 7 - (y / TILE_SIZE);
    // controller.on_tile_selected(file, rank);
}

void GUI::draw_board(int selected_square, std::vector<Move> legal_moves) {
    sf::Color colours[2] = {
        sf::Color(0xEDD6B0FF),
        sf::Color(0xB88762FF)
    }; // Light and dark squares
    // sf::Color alt_colours[2] = {
    //     sf::Color(0xF6EB72),
    //     sf::Color(0xDCC34B)
    // }; // when selected
    // sf::Color circle_colours[2] = {
    //     sf::Color(0xCCB897),
    //     sf::Color(0x9E7454)
    // }; //  legal moves

    sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            square.setPosition(file * TILE_SIZE, (7 - rank) * TILE_SIZE);
            square.setFillColor(
                (file + rank) % 2 == 0 ? colours[0] : colours[1]
            );
            window.draw(square);
        }
    }
}

void GUI::draw_pieces(const Board& board) {
    for (int i = 0; i < 64; ++i) {
        int piece = board.piece_at(i);
        if (piece != Piece::None) {
            int colour = board.colour_at(i) == Colour::White ? 0 : 1;
            pieces[i].setTexture(textures[piece - 1][colour]);

            sf::Vector2 center = textures[piece - 1][colour].getSize() / 2u;
            pieces[i].setOrigin(center.x, center.y);

            pieces[i].setPosition(
                (i % 8 + .5) * TILE_SIZE,
                (7 - (int) (i / 8) + .5) * TILE_SIZE
            );
            window.draw(pieces[i]);
        }
    }
}

void GUI::show() {
    while (window.isOpen())
        handle_events();
}
