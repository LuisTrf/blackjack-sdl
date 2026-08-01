#include "../../include/stb_ds.h"
#include "../../include/ui/widget.h"
#include "../../include/ui/button.h"
#include "../../include/ui/button_constants.h"
#include "../../include/ui/container.h"
#include "../../include/ui/picbox.h"
#include "../../include/ui/label.h"
#include "../../include/ui/spritebox.h"
#include "../../include/input/input.h"
#include "../../include/render/render.h"
#include "../../include/ui/ui.h"

#include <stdio.h>

Button* widget_deal_button_initialize(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *deal_button = button_create(
        368.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT, 
        true,
        BUTTON_EVENT_RELEASE_DEAL,
        BUTTON_STATE_IDLE, 
        hmget(texture_map, TEXTURE_ID_DEAL_BUTTON_SPRITESHEET), 
        button_notify_deal,
        input_handle_button_mouse_events
    );
    input_ctx_widget_listener_register(input_ctx, (Widget *)deal_button);
    event_ctx_widget_listener_register(event_ctx, (Widget *)deal_button);
    button_ctx_register_move_button(button_ctx, deal_button);
    return deal_button;
}

Button* widget_hit_button_initialize(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *hit_button = button_create(
        546.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_HIT,
        BUTTON_STATE_IDLE,
        hmget(texture_map, TEXTURE_ID_HIT_BUTTON_SPRITESHEET), 
        button_notify_hit,
        input_handle_button_mouse_events
    );
    input_ctx_widget_listener_register(input_ctx, (Widget *)hit_button);
    event_ctx_widget_listener_register(event_ctx, (Widget *)hit_button);
    button_ctx_register_move_button(button_ctx, hit_button);
    return hit_button;
}

Button* widget_stand_button_initialize(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Button *stand_button = button_create(
        724.f, 560.f, MOVE_BUTTON_WIDTH, MOVE_BUTTON_HEIGHT,
        true, 
        BUTTON_EVENT_RELEASE_STAND,
        BUTTON_STATE_IDLE,
        hmget(texture_map, TEXTURE_ID_STAND_BUTTON_SPRITESHEET), 
        button_notify_stand,
        input_handle_button_mouse_events
    );
    input_ctx_widget_listener_register(input_ctx, (Widget *)stand_button);
    event_ctx_widget_listener_register(event_ctx, (Widget *)stand_button);
    button_ctx_register_move_button(button_ctx, stand_button);
    return stand_button;
}

Container* ui_buttons_initialize(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Container *buttons = container_create(
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        true,
        NULL,
        NULL
    );
    Button* deal_button = widget_deal_button_initialize(texture_map, input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)deal_button);
    Button* hit_button = widget_hit_button_initialize(texture_map, input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)hit_button);
    Button *stand_button = widget_stand_button_initialize(texture_map, input_ctx, event_ctx, button_ctx);
    container_add_widget(buttons, (Widget *)stand_button);
    return buttons;
}

Container* ui_root_initialize(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx, ButtonContext *button_ctx){
    Container *root = container_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, true, NULL, NULL);
    
    Container *buttons = ui_buttons_initialize(texture_map, input_ctx, event_ctx, button_ctx);
    container_add_widget(root, (Widget *)buttons);

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

UIContext* ui_context_create(texture_hash *texture_map, InputContext *input_ctx, EventContext *event_ctx){
    UIContext ui_ctx;
    ui_ctx.button_ctx = button_context_create();
    ui_ctx.root = ui_root_initialize(texture_map, input_ctx, event_ctx, ui_ctx.button_ctx);
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