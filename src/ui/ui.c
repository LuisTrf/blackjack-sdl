#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/button.h"
#include "../../include/ui/button_constants.h"
#include "../../include/ui/container.h"
#include "../../include/ui/picbox.h"
#include "../../include/ui/label.h"
#include "../../include/ui/spritebox.h"
#include "../../include/input/input.h"
#include "../../include/ui/ui.h"

#include <stdio.h>

Button* widget_deal_button_initialize(InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *deal_button = button_create(
        368.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, 
        true,
        BUTTON_EVENT_RELEASE_DEAL,
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
    button_ctx_register_move_button(button_ctx, deal_button);
    return deal_button;
}

Button* widget_hit_button_initialize(InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *hit_button = button_create(
        546.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_HIT,
        BUTTON_STATE_IDLE,
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
    button_ctx_register_move_button(button_ctx, hit_button);
    return hit_button;
}

Button* widget_stand_button_initialize(InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *stand_button = button_create(
        724.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_STAND,
        BUTTON_STATE_IDLE,
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
    button_ctx_register_move_button(button_ctx, stand_button);
    return stand_button;
}

Container* ui_buttons_initialize(InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Container *buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );
    Button* deal_button = widget_deal_button_initialize(input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)deal_button);
    Button* hit_button = widget_hit_button_initialize(input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)hit_button);
    Button *stand_button = widget_stand_button_initialize(input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)stand_button);
    return buttons;
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

Container* ui_labels_initialize(font_hash* font_map){
    Container *labels = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true
    );

    Label *player_money_label = widget_player_money_label_initialize(font_map);
    container_add_widget(labels, (Widget *)player_money_label);

    return labels;
}

Container* ui_root_initialize(InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx, font_hash* font_map){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true);
    
    Container *buttons = ui_buttons_initialize(input_ctx, event_ctx, button_ctx);
    container_add_widget(root, (Widget *)buttons);

    Container *labels = ui_labels_initialize(font_map);
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
    root = NULL;
}

UIContext* ui_context_create(font_hash* font_map, texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx){
    UIContext ui_ctx;
    ui_ctx.button_ctx = button_context_create();
    ui_ctx.root = ui_root_initialize(input_ctx, event_ctx, ui_ctx.button_ctx, font_map);
    UIContext *p_ui_ctx = malloc(sizeof(UIContext));
    if (p_ui_ctx == NULL){
        abort();
    }
    *p_ui_ctx = ui_ctx;
    return p_ui_ctx;
}

void ui_context_destroy(UIContext *ui_ctx){
    button_context_destroy(ui_ctx->button_ctx);
    ui_ctx->button_ctx = NULL;
    widgets_teardown(ui_ctx->root);
    ui_ctx->root = NULL;
    free(ui_ctx);
}