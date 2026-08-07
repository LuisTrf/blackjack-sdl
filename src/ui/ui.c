#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/button.h"
#include "../../include/ui/button_constants.h"
#include "../../include/ui/container.h"
#include "../../include/ui/picbox.h"
#include "../../include/ui/label.h"
#include "../../include/ui/label_constants.h"
#include "../../include/ui/spritebox.h"
#include "../../include/input/input.h"

#include <stdbool.h>
#include <stdio.h>

Button* widget_deal_button_initialize(InputContext *input_ctx, EventContext *event_ctx){
    Button *deal_button = button_create(
        MOVE_BUTTON_ORIGIN_X, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, 
        true,
        INPUT_EVENT_BUTTON_RELEASE_DEAL,
        BUTTON_STATE_IDLE, 
        TEXTURE_ID_DEAL_BUTTON_SPRITESHEET
    );
    input_ctx_listener_register(
        input_ctx, 
        input_listener_create((void *)deal_button, input_handle_button_mouse_events)
    );
    event_ctx_listener_register(
        event_ctx, 
        event_listener_create((void *)deal_button, button_notify_deal)
    );
    return deal_button;
}

Button* widget_hit_button_initialize(InputContext *input_ctx, EventContext *event_ctx){
    Button *hit_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false, 
        INPUT_EVENT_BUTTON_RELEASE_HIT,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_HIT_BUTTON_SPRITESHEET
    );
    input_ctx_listener_register(
        input_ctx, 
        input_listener_create((void *)hit_button, input_handle_button_mouse_events)
    );
    event_ctx_listener_register(
        event_ctx, 
        event_listener_create((void *)hit_button, button_notify_hit)
    );
    return hit_button;
}

Button* widget_stand_button_initialize(InputContext *input_ctx, EventContext *event_ctx){
    Button *stand_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false, 
        INPUT_EVENT_BUTTON_RELEASE_STAND,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_STAND_BUTTON_SPRITESHEET
    );
    input_ctx_listener_register(
        input_ctx, 
        input_listener_create((void *)stand_button, input_handle_button_mouse_events)
    );
    event_ctx_listener_register(
        event_ctx, 
        event_listener_create((void *)stand_button, button_notify_stand)
    );
    return stand_button;
}

Container* ui_move_buttons_initialize(InputContext *input_ctx, EventContext *event_ctx){
    Container *move_buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );
    Button* deal_button = widget_deal_button_initialize(input_ctx, event_ctx);
    container_add_widget(move_buttons, (Widget *)deal_button);
    Button* hit_button = widget_hit_button_initialize(input_ctx, event_ctx);
    container_add_widget(move_buttons, (Widget *)hit_button);
    Button *stand_button = widget_stand_button_initialize(input_ctx, event_ctx);
    container_add_widget(move_buttons, (Widget *)stand_button);
    event_ctx_listener_register(
        event_ctx, 
        (EventListener){.self=(void*)move_buttons, moveb_container_notify}
    );
    return move_buttons;
}

Label* widget_player_money_label_initialize(font_hash* font_map){
    Label *player_money_label = label_create(
        font_map,
        0, 0,
        true,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_PLAYER_MONEY
    );
    label_write(player_money_label, font_map, "$%.2f", 10.00);
    return player_money_label;
}

Label* widget_dealer_hand_label_initialize(font_hash* font_map, EventContext *event_ctx){
    Label *dealer_hand_label = label_create(
        font_map,
        HAND_LABEL_ORIGIN_X, HAND_LABEL_ORIGIN_Y_DEALER,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_DEALER_HAND
    );
    label_align_y(dealer_hand_label, dealer_hand_label->widget.rect.pos.y);
    event_ctx_listener_register(
        event_ctx,
        (EventListener){(void *)dealer_hand_label, label_notify_dealer_hand}
    );
    return dealer_hand_label;
}

Label* widget_player_hand_label_initialize(font_hash* font_map, EventContext *event_ctx){
    Label *player_hand_label = label_create(
        font_map,
        HAND_LABEL_ORIGIN_X, HAND_LABEL_ORIGIN_Y_PLAYER,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_PLAYER_HAND
    );
    label_align_y(player_hand_label, player_hand_label->widget.rect.pos.y);
    event_ctx_listener_register(
        event_ctx,
        (EventListener){(void *)player_hand_label, label_notify_player_hand}
    );
    return player_hand_label;
}

Container* ui_labels_initialize(font_hash* font_map, EventContext *event_ctx){
    Container *labels = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );

    Label *player_money_label = widget_player_money_label_initialize(font_map);
    container_add_widget(labels, (Widget *)player_money_label);

    Label *dealer_hand_label = widget_dealer_hand_label_initialize(font_map, event_ctx);
    container_add_widget(labels, (Widget *)dealer_hand_label);

    Label *player_hand_label = widget_player_hand_label_initialize(font_map, event_ctx);
    container_add_widget(labels, (Widget *)player_hand_label);

    return labels;
}

Container* ui_root_initialize(InputContext *input_ctx, EventContext *event_ctx, font_hash* font_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true);
    
    Container *move_buttons = ui_move_buttons_initialize(input_ctx, event_ctx);
    container_add_widget(root, (Widget *)move_buttons);

    Container *labels = ui_labels_initialize(font_map, event_ctx);
    container_add_widget(root, (Widget *)labels);

    return root;
}

void widgets_teardown(Container *root){
    Widget** children = container_get_children(root);
    for (int i = 0; i < arrlen(children); i++){
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
}