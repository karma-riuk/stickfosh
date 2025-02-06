#include "gui.hpp"

#include "../model/utils/utils.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

GUI::GUI() {
    window.create(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Chess Board");
    load_textures();

    font.loadFromFile("res/arial.ttf");
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
    int file = x / TILE_SIZE;
    int rank = 7 - (y / TILE_SIZE);
    controller->on_tile_selected(file, rank);
}

void GUI::draw_annotation(int file, int rank) {
    if (file == 0) {
        sf::Text annotation(std::to_string(rank + 1), font);
        annotation.setStyle(sf::Text::Bold);
        annotation.setCharacterSize(16);
        annotation.setFillColor(rank % 2 == 0 ? colours[1] : colours[0]);
        annotation.setPosition(
            (file + .05) * TILE_SIZE,
            (7 - rank + .05) * TILE_SIZE
        );
        window.draw(annotation);
    }

    if (rank == 0) {
        sf::Text annotation("abcdefgh"[file], font);
        annotation.setCharacterSize(16);
        annotation.setOrigin(16, 16);
        annotation.setStyle(sf::Text::Bold);
        annotation.setFillColor(file % 2 == 0 ? colours[1] : colours[0]);
        annotation.setPosition(
            (file + 1) * TILE_SIZE,
            (7 - rank + .95) * TILE_SIZE
        );
        window.draw(annotation);
    }
}

void GUI::draw_board(int selected_square, std::vector<Move> moves) {
    sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int8_t index = Coords{file, rank}.to_index();
            square.setPosition(file * TILE_SIZE, (7 - rank) * TILE_SIZE);
            if (index == selected_square)
                square.setFillColor(
                    (file + rank) % 2 == 0 ? alt_colours[0] : alt_colours[1]
                );
            else
                square.setFillColor(
                    (file + rank) % 2 == 0 ? colours[0] : colours[1]
                );
            window.draw(square);
            draw_annotation(file, rank);

            std::vector<int8_t> targets = to_target_square(moves);
            if (std::find(targets.begin(), targets.end(), index)
                != targets.end()) {
                float r = .15 * TILE_SIZE;
                sf::CircleShape circle{r};
                sf::Color c(0x00000055);
                circle.setFillColor(c);
                circle.setOrigin(r, r);
                circle.setPosition(
                    (file + .5) * TILE_SIZE,
                    (7 - rank + .5) * TILE_SIZE
                );
                window.draw(circle);
            }
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
