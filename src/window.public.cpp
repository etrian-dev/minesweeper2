// Window class public methods
#include "../include/window.h"

void Window::wnd_loop(Game& curr_game)
{
    // union to hold events generated by the window
    sf::Event event;

    bool close_wnd = false;

    // variables that hold mouse coordinates
    int mouse_x = 0, mouse_y = 0;

#ifdef DEBUG
    std::cout << "Waiting for the first move...\n";
#endif

    // update the window
    wnd_draw(curr_game);

    // wait for an uncover event
    while(
        (main_wnd.waitEvent(event)
         && event.type != sf::Event::MouseButtonPressed)
        || event.mouseButton.button != sf::Mouse::Left
    )
    {
        ;
    }
    // when the mouse left event happens retrieve its coordinates
    mouse_x = (int) (event.mouseButton.x / SIDE);
    mouse_y = (int) (event.mouseButton.y / SIDE);

    // boards are initialized with the first move
    curr_game.init_game(mouse_x, mouse_y);

#ifdef DEBUG
    std::cout << "First move performed. Boards initialized.\n";
#endif

    // stores bound of a Rect, exlusively to center the text in the cells
    sf::FloatRect bounds;

    while(  !close_wnd
            && curr_game.get_continue()
            && !curr_game.test_victory()
        )
    {
        // update the window
        wnd_draw(curr_game);

        // event loop
        while(main_wnd.pollEvent(event))
        {
            // distinguish event type
            switch(event.type)
            {

            // close button has been pressed. Destroys the game class, then the window
            case sf::Event::Closed:
                // exit the window loop
                close_wnd = true;
                break;

            case sf::Event::MouseButtonPressed:
                // store the cell index pressed
                mouse_x = (int) (event.mouseButton.x / SIDE);
                mouse_y = (int) (event.mouseButton.y / SIDE);

                // left or right button pressed
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    // try to uncover cell [y][x]
                    curr_game.move(mouse_x, mouse_y);
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    // toggle flag on cell [y][x]
                    curr_game.flag(mouse_x, mouse_y);
                }
                break;
            }

        }
    }

    // if the game should not continue, then it's lost
    if(!curr_game.get_continue())
    {
        // clear the window to the covered background
        main_wnd.clear(bg_covered);
        txt_obj.setString(std::string("Game lost..."));
    }
    if(curr_game.test_victory())
    {
        // clear to uncovered background
        main_wnd.clear(bg_uncovered);
        txt_obj.setString(std::string("Victory!"));
    }

    // text drawing "black box" that centers it to the window
    bounds = txt_obj.getLocalBounds();
    // the origin of the text obj is changed to the center of the bounding box
    txt_obj.setOrigin   (bounds.left + bounds.width/2.0f,
                        bounds.top + bounds.height/2.0f);
    // then the text position is set
    txt_obj.setPosition (curr_game.get_cols() * SIDE /2.0f,
                        curr_game.get_rows() * SIDE /2.0f);

    // draw text and display it
    main_wnd.draw(txt_obj);
    main_wnd.display();

    // loops if the window is not closed and the game is not finished
    close_wnd = false;
    while(!close_wnd)
    {
        while(main_wnd.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                close_wnd = true;
            }
        }
    }

    // finally closes the window
    main_wnd.close();
}
