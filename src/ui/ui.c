#include <stdbool.h>

#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/button.h"
#include "../../include/ui/button_constants.h"
#include "../../include/ui/container.h"
#include "../../include/ui/picbox.h"
#include "../../include/ui/label.h"
#include "../../include/ui/label_constants.h"
#include "../../include/ui/spritebox.h"
#include "../../include/game/game_constants.h"
#include "../../include/update/animation.h"
#include "../../include/input/input.h"

Button* widget_deal_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *deal_button = button_create(
        462.f, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, 
        true,
        INPUT_EVENT_BUTTON_RELEASE_DEAL,
        BUTTON_STATE_IDLE, 
        TEXTURE_ID_DEAL_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners, 
        (InputListener){(void *)deal_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners, 
        (EventListener){(void *)deal_button, button_notify_deal}
    );
    return deal_button;
}

Button* widget_hit_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *hit_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false, 
        INPUT_EVENT_BUTTON_RELEASE_HIT,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_HIT_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners, 
        (InputListener){(void *)hit_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners, 
        (EventListener){(void *)hit_button, button_notify_hit}
    );
    return hit_button;
}

Button* widget_stand_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *stand_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false, 
        INPUT_EVENT_BUTTON_RELEASE_STAND,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_STAND_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners, 
        (InputListener){(void *)stand_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners, 
        (EventListener){(void *)stand_button, button_notify_stand}
    );
    return stand_button;
}

Button *widget_bet_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *bet_button = button_create(
        640.f, MOVE_BUTTON_ORIGIN_Y,
        MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true,
        INPUT_EVENT_BUTTON_RELEASE_BET,
        BUTTON_STATE_IDLE,
        TEXTURE_ID_BET_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)bet_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)bet_button, button_notify_bet}
    );
    return bet_button;
}

Button *widget_split_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *split_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y,
        MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_SPLIT,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_SPLIT_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners, 
        (InputListener){(void *)split_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)split_button, button_notify_split}
    );
    return split_button;
}

Button *widget_double_down_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *double_down_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y,
        MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_DOUBLE_DOWN,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_DOUBLE_DOWN_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners, 
        (InputListener){(void *)double_down_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)double_down_button, button_notify_double_down}
    );
    return double_down_button;
}

Button *widget_insurance_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *insurance_button = button_create(
        0.f, MOVE_BUTTON_ORIGIN_Y,
        MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        false, 
        INPUT_EVENT_BUTTON_RELEASE_INSURANCE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_INSURANCE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)insurance_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)insurance_button, button_notify_insurance}
    );
    return insurance_button;
}

Container* ui_move_buttons_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Container *move_buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );
    Button* deal_button = widget_deal_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)deal_button);
    Button* hit_button = widget_hit_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)hit_button);
    Button *stand_button = widget_stand_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)stand_button);
    Button *bet_button = widget_bet_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)bet_button);
    Button *split_button = widget_split_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)split_button);
    Button *double_down_button = widget_double_down_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)double_down_button);
    Button *insurance_button = widget_insurance_button_initialize(input_listeners, event_listeners);
    container_add_widget(move_buttons, (Widget *)insurance_button);
    event_listener_register(
        event_listeners, 
        (EventListener){.self=(void*)move_buttons, moveb_container_notify}
    );
    return move_buttons;
}

Button* widget_stack_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *stack_button = button_create(
        STACK_BUTTON_ORIGIN_X, STACK_BUTTON_ORIGIN_Y,
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_STACK,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_NULL
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)stack_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)stack_button, button_notify_stack}
    );
    return stack_button;
}

Button* widget_white_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *white_button = button_create(
        CHIP_BUTTON_X(0), CHIP_BUTTON_Y(0),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_WHITE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_WHITE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)white_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)white_button, button_notify_white}
    );
    return white_button;
}

Button* widget_red_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *red_button = button_create(
        CHIP_BUTTON_X(1), CHIP_BUTTON_Y(1),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_RED,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_RED_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)red_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)red_button, button_notify_red}
    );
    return red_button;
}

Button* widget_blue_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *blue_button = button_create(
        CHIP_BUTTON_X(2), CHIP_BUTTON_Y(2),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_BLUE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_BLUE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)blue_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)blue_button, button_notify_blue}
    );
    return blue_button;
}

Button* widget_green_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *green_button = button_create(
        CHIP_BUTTON_X(3), CHIP_BUTTON_Y(3),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_GREEN,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_GREEN_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)green_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)green_button, button_notify_green}
    );
    return green_button;
}

Button* widget_black_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *black_button = button_create(
        CHIP_BUTTON_X(4), CHIP_BUTTON_Y(4),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_BLACK,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_BLACK_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)black_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)black_button, button_notify_black}
    );
    return black_button;
}

Button* widget_purple_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *purple_button = button_create(
        CHIP_BUTTON_X(5), CHIP_BUTTON_Y(5),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_PURPLE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_PURPLE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)purple_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)purple_button, button_notify_purple}
    );
    return purple_button;
}

Button* widget_yellow_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *yellow_button = button_create(
        CHIP_BUTTON_X(6), CHIP_BUTTON_Y(6),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_YELLOW,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_YELLOW_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)yellow_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)yellow_button, button_notify_yellow}
    );
    return yellow_button;
}

Button* widget_orange_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *orange_button = button_create(
        CHIP_BUTTON_X(7), CHIP_BUTTON_Y(7),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_ORANGE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_ORANGE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)orange_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)orange_button, button_notify_orange}
    );
    return orange_button;
}

Button* widget_redblue_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *redblue_button = button_create(
        CHIP_BUTTON_X(8), CHIP_BUTTON_Y(8),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_REDBLUE,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_REDBLUE_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)redblue_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)redblue_button, button_notify_redblue}
    );
    return redblue_button;
}

Button* widget_gold_button_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Button *gold_button = button_create(
        CHIP_BUTTON_X(9), CHIP_BUTTON_Y(9),
        CHIP_BUTTON_WIDTH, CHIP_BUTTON_HEIGHT,
        false,
        INPUT_EVENT_BUTTON_RELEASE_GOLD,
        BUTTON_STATE_DISABLED,
        TEXTURE_ID_GOLD_BUTTON_SPRITESHEET
    );
    input_listener_register(
        input_listeners,
        (InputListener){(void *)gold_button, input_handle_button_mouse_events}
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)gold_button, button_notify_gold}
    );
    return gold_button;
}

Container* ui_cheque_buttons_initialize(InputListener **input_listeners, EventListener **event_listeners){
    Container *cheque_buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );
    Button *stack_button = widget_stack_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)stack_button);
    Button *white_button = widget_white_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)white_button);
    Button *red_button = widget_red_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)red_button);
    Button *blue_button = widget_blue_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)blue_button);
    Button *green_button = widget_green_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)green_button);
    Button *black_button = widget_black_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)black_button);
    Button *purple_button = widget_purple_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)purple_button);
    Button *yellow_button = widget_yellow_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)yellow_button);
    Button *orange_button = widget_orange_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)orange_button);
    Button *redblue_button = widget_redblue_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)redblue_button);
    Button *gold_button = widget_gold_button_initialize(input_listeners, event_listeners);
    container_add_widget(cheque_buttons, (Widget *)gold_button);
    return cheque_buttons;
}

Label* widget_player_money_label_initialize(font_hash* font_map, EventListener **event_listeners){
    Label *player_money_label = label_create(
        font_map,
        0, 0,
        true,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_PLAYER_MONEY
    );
    label_write(player_money_label, font_map, "MONEY: $%.2f", PLAYER_BEGINNING_MONEY);
    event_listener_register(
        event_listeners, 
        (EventListener){(void *)player_money_label, label_notify_player_money}
    );
    return player_money_label;
}

Label* widget_dealer_hand_label_initialize(font_hash* font_map, EventListener **event_listeners){
    Label *dealer_hand_label = label_create(
        font_map,
        HAND_LABEL_ORIGIN_X, HAND_LABEL_ORIGIN_Y_DEALER,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_DEALER_HAND
    );
    rect_align_y((Rect *)dealer_hand_label, dealer_hand_label->widget.rect.pos.y);
    event_listener_register(
        event_listeners,
        (EventListener){(void *)dealer_hand_label, label_notify_dealer_hand}
    );
    return dealer_hand_label;
}

Label* widget_player_hand_label_initialize(font_hash* font_map, EventListener **event_listeners){
    Label *player_hand_label = label_create(
        font_map,
        HAND_LABEL_ORIGIN_X, HAND_LABEL_ORIGIN_Y_PLAYER,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_PLAYER_HAND
    );
    rect_align_y((Rect *)player_hand_label, player_hand_label->widget.rect.pos.y);
    event_listener_register(
        event_listeners,
        (EventListener){(void *)player_hand_label, label_notify_player_hand}
    );
    return player_hand_label;
}

Label *widget_player_bet_label_initialize(font_hash* font_map, EventListener **event_listeners){
    Label *player_bet_label = label_create(
        font_map,
        BET_LABEL_ORIGIN_X, BET_LABEL_ORIGIN_Y,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_PLAYER_BET
    );
    label_write(player_bet_label, font_map, "BET: $0.00");
    rect_align_x((Rect *)player_bet_label, player_bet_label->widget.rect.pos.x);
    event_listener_register(
        event_listeners,
        (EventListener){(void *)player_bet_label, label_notify_player_bet}
    );
    return player_bet_label;
}

Label *widget_player_split_hand_label_initialize(font_hash* font_map, EventListener **event_listeners){
    Label *player_split_hand_label = label_create(
        font_map,
        HAND_LABEL_ORIGIN_X, SPLIT_HAND_LABEL_SPLITTING_Y_PLAYER,
        false,
        FONT_ID_OPENSANS_32PT,
        TEXTURE_ID_LABEL_SPLIT_PLAYER_HAND
    );
    event_listener_register(
        event_listeners,
        (EventListener){(void *)player_split_hand_label, label_notify_player_split_hand}
    );
    return player_split_hand_label;
}

Container* ui_labels_initialize(font_hash* font_map, EventListener **event_listeners){
    Container *labels = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );

    Label *player_money_label = widget_player_money_label_initialize(font_map, event_listeners);
    container_add_widget(labels, (Widget *)player_money_label);
    Label *dealer_hand_label = widget_dealer_hand_label_initialize(font_map, event_listeners);
    container_add_widget(labels, (Widget *)dealer_hand_label);
    Label *player_hand_label = widget_player_hand_label_initialize(font_map, event_listeners);
    container_add_widget(labels, (Widget *)player_hand_label);
    Label *player_bet_label = widget_player_bet_label_initialize(font_map, event_listeners);
    container_add_widget(labels, (Widget *)player_bet_label);
    Label *player_split_hand_label = widget_player_split_hand_label_initialize(font_map, event_listeners);
    container_add_widget(labels, (Widget *)player_split_hand_label);

    return labels;
}

SpriteBox* widget_arrow_initialize(AnimationPool *anim_pool, EventListener **event_listeners){
    SpriteBox *arrow = spritebox_create(
        0, 0, 
        64, 64,
        false,
        TEXTURE_ID_ARROW_SPRITESHEET,
        0, 0
    );
    anim_add(anim_pool, (Animation){
        (Rect *)arrow,
        ANIMATION_TYPE_SPRITESHEET,
        ANIMATION_STATE_WAITING,
        animation_arrow,
        {.sprite_anim={
            0, (1.f/12), 12, 32
        }}
    });
    event_listener_register(
        event_listeners,
        (EventListener){(void *)arrow, spritebox_notify_arrow}
    );
    return arrow;
}

Container* ui_root_initialize(InputListener **input_listeners, EventListener **event_listeners, font_hash* font_map, AnimationPool *anim_pool){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true);
    
    Container *move_buttons = ui_move_buttons_initialize(input_listeners, event_listeners);
    container_add_widget(root, (Widget *)move_buttons);

    Container *cheque_buttons = ui_cheque_buttons_initialize(input_listeners, event_listeners);
    container_add_widget(root, (Widget *)cheque_buttons);

    Container *labels = ui_labels_initialize(font_map, event_listeners);
    container_add_widget(root, (Widget *)labels);

    SpriteBox *arrow = widget_arrow_initialize(anim_pool, event_listeners);
    container_add_widget(root, (Widget *)arrow);

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