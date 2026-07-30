#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/button.h"
#include "../../include/ui/button_constants.h"
#include "../../include/ui/container.h"
#include "../../include/input/input.h"
#include "../../include/render/render.h"

Button* widget_deal_button_initialize(InputContext* p_ic, EventContext *p_ec, Button_Context *p_bc, render_hash* texture_map){
    Button *deal_button = button_create(
        MOVE_BUTTON_ORIGIN_X, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, 
        true,
        BUTTON_EVENT_RELEASE_DEAL,
        BUTTON_STATE_IDLE, 
        hmget(texture_map, TEXTURE_ID_DEAL_BUTTON_SPRITESHEET), 
        button_notify_deal,
        input_handle_button_mouse_events
    );
    ic_widget_listener_register(p_ic, (Widget *)deal_button);
    ec_widget_listener_register(p_ec, (Widget *)deal_button);
    bc_register_move_button(p_bc, deal_button);
    return deal_button;
}

Button* widget_hit_button_initialize(InputContext* p_ic, EventContext *p_ec, Button_Context *p_bc, render_hash* texture_map){
    Button *hit_button = button_create(
        MOVE_BUTTON_ORIGIN_X, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_HIT,
        BUTTON_STATE_IDLE,
        hmget(texture_map, TEXTURE_ID_HIT_BUTTON_SPRITESHEET), 
        button_notify_hit,
        input_handle_button_mouse_events
    );
    ic_widget_listener_register(p_ic, (Widget *)hit_button);
    ec_widget_listener_register(p_ec, (Widget *)hit_button);
    bc_register_move_button(p_bc, hit_button);
    return hit_button;
}

Button* widget_stand_button_initialize_(InputContext* p_ic, EventContext *p_ec, Button_Context *p_bc, render_hash* texture_map){
    Button *stand_button = button_create(
        MOVE_BUTTON_ORIGIN_X, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_STAND,
        BUTTON_STATE_IDLE,
        hmget(texture_map, TEXTURE_ID_STAND_BUTTON_SPRITESHEET), 
        button_notify_stand,
        input_handle_button_mouse_events
    );
    ic_widget_listener_register(p_ic, (Widget *)stand_button);
    ec_widget_listener_register(p_ec, (Widget *)stand_button);
    bc_register_move_button(p_bc, stand_button);
    return stand_button;
}

Container* ui_buttons_initialize(InputContext* p_ic, EventContext *p_ec, Button_Context *p_bc, render_hash* texture_map){
    Container *buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true,
        NULL,
        NULL
    );
    Button* deal_button = widget_deal_button_initialize(p_ic, p_ec, p_bc, texture_map);
    container_add_widget(buttons, (Widget *)deal_button);
    Button* hit_button = widget_hit_button_initialize(p_ic, p_ec, p_bc, texture_map);
    container_add_widget(buttons, (Widget *)hit_button);
    Button *stand_button = widget_stand_button_initialize_(p_ic, p_ec, p_bc, texture_map);
    container_add_widget(buttons, (Widget *)stand_button);
    return buttons;
}

Container* ui_root_initialize(InputContext* p_ic, EventContext *p_ec, Button_Context *p_bc, render_hash* texture_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL, NULL);
    
    Container *buttons = ui_buttons_initialize(p_ic, p_ec, p_bc, texture_map);
    container_add_widget(root, (Widget *)buttons);

    return root;
}

void widgets_teardown(Container *root){
    Widget** children = container_get_children(root);
    for (int i = 0; i < arrlen(root->children); i++){
        switch(children[i]->wtype){
            case WIDGET_CONTAINER:
                widgets_teardown((Container *)children[i]);
                break;
            case WIDGET_PICBOX:
                picturebox_destroy((PictureBox *)children[i]);
                break;
            case WIDGET_BUTTON:
                button_destroy((Button *)children[i]);
                break;
            case WIDGET_LABEL:
                label_destroy((Label *)children[i]);
                break;
            case WIDGET_SPRITEBOX:
                spritebox_destroy((SpriteBox *)children[i]);
                break;
        }
        children[i] = NULL;
    }
    container_destroy(root);
    root = NULL;
}