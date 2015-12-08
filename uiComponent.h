/*
(c) Matthew Slocum 2015

uiComponent.h

uiComponent is responsible for drawing the menus and interprating user input
    the menu can:
        change the destination
        update destination list
        change crypto key
        start the messenger
*/

#include <string>

using namespace std;

class regComponent;

class uiComponent {
    private:
        regComponent * reg;
        string select;
        string prompt_helper;
        int cur_menu;
    public:
        enum menu_state {
            MAIN, SET_DEST, SETTINGS, SETTINGS_VIEW_KEY, SETTINGS_SET_KEY, SETTINGS_ADD_IP
        };
        uiComponent();
        void bind_regComponent(regComponent *);
        void run();
        void draw_menu_header(string menu_name);
        void draw_menu_footer();
        void draw_debug();
        void draw_main();
        void draw_settings();
        void draw_set_dest();
        void draw_view_key();
        void draw_set_key();
        void draw_add_ip();
        void getInput();
};

