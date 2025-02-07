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
    const Board& b, int8_t selected_square, std::vector<int8_t> targets
) {
    window.clear();
    draw_board(selected_square, targets);
    draw_pieces(b);
    window.display();
}

void GUI::notify_stalemate(Colour col) {
    std::cout << "Stalemate for " << to_string(col) << std::endl;
}

void GUI::notify_checkmate(Colour col) {
    std::cout << "Checkmate for " << to_string(col) << std::endl;
}

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

void GUI::draw_board(int selected_square, std::vector<int8_t> targets) {
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

int GUI::show_popup(
    const std::string& message, const std::vector<std::string>& options
) {
    sf::RenderWindow popup(sf::VideoMode(300, 200), "Choice");
    sf::Font font;

    if (!font.loadFromFile("res/arial.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    }

    sf::Text text(message, font, 20);
    text.setPosition(20, 20);
    text.setFillColor(sf::Color::Black);

    std::vector<sf::RectangleShape> buttonShapes;
    std::vector<sf::Text> buttonTexts;

    for (size_t i = 0; i < options.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(200, 30));
        button.setPosition(50, 70 + i * 40);
        button.setFillColor(sf::Color(150, 150, 150));
        buttonShapes.push_back(button);

        sf::Text buttonText(options[i], font, 18);
        buttonText.setPosition(60, 75 + i * 40);
        buttonText.setFillColor(sf::Color::Black);
        buttonTexts.push_back(buttonText);
    }

    while (popup.isOpen()) {
        sf::Event event;
        while (popup.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                popup.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                for (size_t i = 0; i < buttonShapes.size(); ++i) {
                    if (buttonShapes[i].getGlobalBounds().contains(
                            event.mouseButton.x,
                            event.mouseButton.y
                        )) {
                        popup.close();
                        return i;
                    }
                }
            }
        }

        popup.clear(sf::Color::White);
        popup.draw(text);
        for (size_t i = 0; i < buttonShapes.size(); ++i) {
            popup.draw(buttonShapes[i]);
            popup.draw(buttonTexts[i]);
        }
        popup.display();
    }
    return -1;
}

Piece GUI::ask_about_promotion() {
    std::vector<std::string> options = {"Queen", "Rook", "Bishop", "Knight"};
    int idx = show_popup("Please choose a promotion for your pawn", options);
    switch (idx) {
    case 0:
        return Queen;
    case 1:
        return Rook;
    case 2:
        return Bishop;
    case 3:
        return Knigt;
    };
    return Piece::None;
}

void GUI::show() {
    while (window.isOpen())
        handle_events();
}
