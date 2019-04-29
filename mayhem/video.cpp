// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <fmt/format.h>
#include <unordered_map>
#include <SDL_surface.h>
#include <common/defer.h>
#include <SDL_FontCache.h>
#include "game.h"
#include "video.h"
#include "window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mayhem {

    static std::unordered_map<std::string, image_t> s_images{};
    static std::unordered_map<std::string, font_data_t> s_fonts{};

    static std::string make_bank_key(bank_id_t id) {
        return fmt::format("{}:{}", id.bank, id.index);
    }

    static SDL_Color to_fc_color(const color_t& color) {
        return FC_MakeColor(color.r, color.g, color.b, color.a);
    }

    ///////////////////////////////////////////////////////////////////////////

    image_t* image_find(bank_id_t id) {
        auto it = s_images.find(make_bank_key(id));
        if (it == std::end(s_images))
            return nullptr;
        return &it->second;
    }

    bool image_load(
            common::result& r,
            const std::string& path,
            int32_t format,
            bank_id_t id) {
        auto image = image_find(id);
        if (image != nullptr)
            return true;

        auto result = s_images.insert(std::make_pair(make_bank_key(id), image_t {}));
        if (!result.second)
            return false;

        auto& new_image = result.first->second;

        new_image.data = stbi_load(
            path.c_str(),
            &new_image.size.w,
            &new_image.size.h,
            &new_image.format,
            format);
        if (new_image.data == nullptr) {
            r.error("V003", stbi_failure_reason());
            return false;
        }

        int depth, pitch;
        uint32_t pixel_format;
        if (format == STBI_rgb) {
            depth = 24;
            pitch = 3*new_image.size.w;
            pixel_format = SDL_PIXELFORMAT_RGB24;
        } else {
            depth = 32;
            pitch = 4*new_image.size.w;
            pixel_format = SDL_PIXELFORMAT_RGBA32;
        }

        new_image.surface = SDL_CreateRGBSurfaceWithFormatFrom(
            new_image.data,
            new_image.size.w,
            new_image.size.h,
            depth,
            pitch,
            pixel_format);
        if (new_image.surface == nullptr) {
            r.error("V003", "unable to create surface from bitmap.");
            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool font_load(
            common::result& r,
            const std::string& path,
            SDL_Renderer* renderer,
            uint32_t size,
            int32_t style,
            color_t color,
            bank_id_t id) {
        auto font = font_find(id);
        if (font != nullptr)
            return true;

        auto result = s_fonts.insert(std::make_pair(make_bank_key(id), font_data_t {}));
        if (!result.second)
            return false;

        auto& data = result.first->second;
        data.size = size;
        data.style = style;
        data.color = color;
        data.handle = FC_CreateFont();

        return FC_LoadFont(
            data.handle,
            renderer,
            path.c_str(),
            size,
            to_fc_color(data.color),
            style) != 0;
    }

    font_data_t* font_find(bank_id_t id) {
        auto it = s_fonts.find(make_bank_key(id));
        if (it == std::end(s_fonts))
            return nullptr;
        return &it->second;
    }

    ///////////////////////////////////////////////////////////////////////////

    static bool video_draw_tile(game_t& game, uint32_t ty, uint32_t tx, layer_t& layer) {
//        const palette_t* pal = palette(tile.palette);
//        if (pal == NULL)
//            return false;
//
//        const tile_bitmap_t* bitmap = tile_bitmap(tile.id);
//        if (bitmap == NULL)
//            return false;

        auto& video = game.video;
        const auto tile_width = video.tile_size.w;
        const auto tile_height = video.tile_size.h;

        const bool vertical_flip = (layer.flags & (uint8_t)tile_flags_t::vflip) == (uint8_t)tile_flags_t::vflip;
        const bool horizontal_flip = (layer.flags & (uint8_t)tile_flags_t::hflip) == (uint8_t)tile_flags_t::hflip;

        auto sy = (uint8_t) (vertical_flip ? tile_height - 1 : 0);
        auto syd = (int8_t) (vertical_flip ? -1 : 1);
        auto sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < tile_height; y++) {
            auto p = static_cast<uint8_t*>(video.bg->pixels) + ((ty + y) * video.bg->pitch + (tx * 4));
            auto sx = (uint8_t) (horizontal_flip ? tile_width - 1 : 0);
            for (uint32_t x = 0; x < tile_width; x++) {
                const auto pixel_offset = (const uint32_t) (sy * tile_width + sx);
//              const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
//              *p++ = pal_entry->blue;
//              *p++ = pal_entry->green;
//              *p++ = pal_entry->red;
//              *p++ = pal_entry->alpha;
                sx += sxd;
            }
            sy += syd;
        }

        return true;
    }

    static void video_update_bg(game_t& game) {
        const auto tile_map_size = game.video.bg_size.w * game.video.bg_size.h;
        const auto tile_map_max_width = game.video.bg_size.w * game.video.tile_size.w;

        SDL_LockSurface(game.video.bg);

        uint32_t tx = 0;
        uint32_t ty = 0;
        for (uint32_t i = 0; i < tile_map_size; i++) {
            auto& block = game.video.blocks[i];

            for (auto& layer : block.layers) {
                if ((layer.flags & (uint8_t)tile_flags_t::changed) == 0)
                    goto next_tile;

                if ((layer.flags & (uint8_t)tile_flags_t::enabled) == 0)
                    goto next_tile;

                if (video_draw_tile(game, ty, tx, layer))
                    layer.flags &= ~(uint8_t)tile_flags_t::changed;
            }

            next_tile:
            tx += game.video.tile_size.w;
            if (tx == tile_map_max_width) {
                tx = 0;
                ty += game.video.tile_size.h;
            }
        }

        SDL_UnlockSurface(game.video.bg);
    }

    ///////////////////////////////////////////////////////////////////////////

    static bool video_draw_sprite(game_t& game, sprite_t& sprite) {
//        const palette_t* pal = palette(tile.palette);
//        if (pal == NULL)
//            return false;
//
//        const sprite_bitmap_t* bitmap = sprite_bitmap(tile.id);
//        if (bitmap == NULL)
//            return false;

        auto& video = game.video;

        const auto sprite_width = video.sprite_size.w;
        const auto sprite_height =video.sprite_size.h;

        const bool horizontal_flip = (sprite.flags & (uint8_t)sprite_flags_t::hflip) == (uint8_t)sprite_flags_t::hflip;
        const bool vertical_flip = (sprite.flags & (uint8_t)sprite_flags_t::vflip) == (uint8_t)sprite_flags_t::vflip;

        auto sy = (uint8_t) (vertical_flip ? sprite_height - 1 : 0);
        auto syd = (int8_t) (vertical_flip ? -1 : 1);
        auto sxd = (int8_t) (horizontal_flip ? -1 : 1);

        for (uint32_t y = 0; y < sprite_height; y++) {
            uint32_t ty = sprite.pos.y + y;
            if (ty > video.clip.pos.y
            &&  ty < video.clip.pos.y + video.clip.size.h) {
                auto p = static_cast<uint8_t*>(video.fg->pixels) + (ty * video.fg->pitch + (sprite.pos.x * 4));
                auto sx = (uint8_t) (horizontal_flip ? sprite_width - 1 : 0);
                for (uint32_t x = 0; x < sprite_width; x++) {
                    uint32_t tx = sprite.pos.x + x;
                    if (tx < video.clip.pos.x
                    ||  tx > video.clip.pos.x + video.clip.size.w) {
                        p += 4;
                    } else {
                        const auto pixel_offset = (const uint32_t) (sy * sprite_width + sx);
//                        const palette_entry_t* pal_entry = &pal->entries[bitmap->data[pixel_offset]];
//                        if (pal_entry->alpha == 0x00)
//                            p += 4;
//                        else {
//                            *p++ = pal_entry->blue;
//                            *p++ = pal_entry->green;
//                            *p++ = pal_entry->red;
//                            *p++ = pal_entry->alpha;
//                        }
                    }
                    sx += sxd;
                }
            }
            sy += syd;
        }

        return true;
    }

    static void video_update_fg(game_t& game) {
        SDL_LockSurface(game.video.fg);

        for (uint32_t i = 0; i < game.video.max_sprites; i++) {
            auto& sprite = game.video.sprites[i];

            if ((sprite.flags & (uint8_t)sprite_flags_t::enabled) == 0)
                continue;

            if (!video_draw_sprite(game, sprite)) {
                continue;
            }

            sprite.flags &= ~(uint8_t)sprite_flags_t::changed;
        }

        SDL_UnlockSurface(game.video.fg);
    }

    ///////////////////////////////////////////////////////////////////////////

    static void video_draw_vline(game_t& game, vline_t& line) {
        auto p = static_cast<uint8_t*>(game.video.fg->pixels)
            + (line.pos.y * game.video.fg->pitch + (line.pos.x * 4));
        for (uint32_t y = 0; y < line.h; y++) {
            *p++ = line.color.b;
            *p++ = line.color.g;
            *p++ = line.color.r;
            *p++ = line.color.a;
            p += game.video.fg->pitch;
        }
    }

    static void video_draw_hline(game_t& game, hline_t& line) {
        auto p = static_cast<uint8_t*>(game.video.fg->pixels)
            + (line.pos.y * game.video.fg->pitch + (line.pos.x * 4));
        for (uint32_t x = 0; x < line.w; x++) {
            *p++ = line.color.b;
            *p++ = line.color.g;
            *p++ = line.color.r;
            *p++ = line.color.a;
        }
    }

    static void video_draw_rect(game_t& game, box_t& box) {
        if (box.fill) {
            for (uint32_t y = 0; y < box.bounds.size.h; y++) {
                hline_t line{};
                line.color = box.color;
                line.w = box.bounds.size.w;
                line.pos.x = box.bounds.pos.x;
                line.pos.y = box.bounds.pos.y + y;
                video_draw_hline(game, line);
            }
        } else {
            vline_t vline{};
            vline.color = box.color;
            vline.h = box.bounds.size.h;
            vline.pos.x = box.bounds.pos.x;
            vline.pos.y = box.bounds.pos.y;
            video_draw_vline(game, vline);

            vline.pos.x += box.bounds.size.w;
            video_draw_vline(game, vline);

            hline_t hline{};
            hline.color = box.color;
            hline.w = box.bounds.size.w;
            hline.pos.y = box.bounds.pos.y;
            hline.pos.x = box.bounds.pos.x;
            video_draw_hline(game, hline);

            hline.pos.y += box.bounds.size.h;
            video_draw_hline(game, hline);
        }
    }

    ///////////////////////////////////////////////////////////////////////////

    bool video_init(common::result& r, game_t& game) {
        game.video.clip.pos.x = 0;
        game.video.clip.pos.y = 0;
        game.video.clip.size.w = screen_width;
        game.video.clip.size.h = screen_height;

        game.video.sprites.resize(game.video.max_sprites);
        game.video.blocks.resize(game.video.max_bg_size.w * game.video.max_bg_size.h);

        const auto bg_surface_width = game.video.tile_size.w * game.video.bg_size.w;
        const auto bg_surface_height = game.video.tile_size.h * game.video.bg_size.h;
        game.video.bg = SDL_CreateRGBSurfaceWithFormat(
            0,
            bg_surface_width,
            bg_surface_height,
            32,
            SDL_PIXELFORMAT_BGRA32);
        SDL_SetSurfaceBlendMode(game.video.bg, SDL_BLENDMODE_NONE);

        game.video.fg = SDL_CreateRGBSurfaceWithFormat(
            0,
            screen_width,
            screen_height,
            32,
            SDL_PIXELFORMAT_BGRA32);
        SDL_SetSurfaceBlendMode(game.video.fg, SDL_BLENDMODE_NONE);
        //SDL_SetSurfaceRLE(game.video.fg, SDL_TRUE);

        if (!font_load(
                r,
                "../assets/fonts/joystick/joystick.ttf",
                game.window.renderer,
                14,
                TTF_STYLE_NORMAL,
                color_t{0xff, 0xff, 0xff, 0xff},
                bank_id_t{0xff, 0})) {
            r.error("V002", "unable to load font: ../assets/fonts/joystick/joystick.ttf");
            return false;
        }

        if (!image_load(r, "../assets/logos/fmod-logo-white1.png", STBI_rgb_alpha, bank_id_t{0xff, 1})) {
            r.error("V002", "unable to load image: ../assets/logos/fmod-logo-white1.png");
            return false;
        }

        if (!image_load(r, "../assets/logos/nybbles-logo.png", STBI_rgb_alpha, bank_id_t{0xff, 2})) {
            r.error("V002", "unable to load image: ../assets/logos/nybbles-logo.png");
            return false;
        }

        return true;
    }

    bool video_update(common::result& r, game_t& game) {
        video_update_bg(game);

        const auto source_rect = SDL_Rect {
            (int)game.video.x_scroll,
            (int)game.video.y_scroll,
            screen_width,
            screen_height
        };
        SDL_BlitSurface(
            game.video.bg,
            &source_rect,
            game.video.fg,
            nullptr);

        video_update_fg(game);

        auto blit_view = game.registry.view<blit_t>();
        for (auto entity : blit_view) {
            auto& blit = blit_view.get(entity);
            auto image = image_find(blit.image);

            const auto src_rect = SDL_Rect {
                blit.src_rect.pos.x,
                blit.src_rect.pos.y,
                blit.src_rect.size.w,
                blit.src_rect.size.h,
            };
            auto dest_rect = SDL_Rect {
                blit.dest_rect.pos.x,
                blit.dest_rect.pos.y,
                blit.dest_rect.size.w,
                blit.dest_rect.size.h,
            };
            SDL_BlitScaled(image->surface, &src_rect, game.video.fg, &dest_rect);
            game.registry.destroy(entity);
        }

        auto hline_view = game.registry.view<hline_t>();
        for (auto entity : hline_view) {
            auto& hline = hline_view.get(entity);
            video_draw_hline(game, hline);
            game.registry.destroy(entity);
        }

        auto vline_view = game.registry.view<vline_t>();
        for (auto entity : vline_view) {
            auto& vline = vline_view.get(entity);
            video_draw_vline(game, vline);
            game.registry.destroy(entity);
        }

        auto box_view = game.registry.view<box_t>();
        for (auto entity : box_view) {
            auto& box = box_view.get(entity);
            video_draw_rect(game, box);
            game.registry.destroy(entity);
        }

        SDL_UpdateTexture(
            game.window.texture,
            nullptr,
            game.video.fg->pixels,
            game.video.fg->pitch);
        SDL_RenderCopy(
            game.window.renderer,
            game.window.texture,
            nullptr,
            nullptr);

        auto text_view = game.registry.view<text_t>();
        for (auto entity : text_view) {
            auto& text = text_view.get(entity);
            auto font = font_find(text.font.id);
            FC_DrawColor(
                font->handle,
                game.window.renderer,
                text.pos.x,
                text.pos.y,
                to_fc_color(text.color),
                text.data.c_str());
            game.registry.destroy(entity);
        }

        SDL_RenderPresent(game.window.renderer);

        return true;
    }

    bool video_shutdown(common::result& r, game_t& game) {
        SDL_FreeSurface(game.video.bg);
        SDL_FreeSurface(game.video.fg);

        for (const auto& kvp : s_fonts)
            FC_FreeFont(kvp.second.handle);

        for (const auto& kvp : s_images) {
            SDL_FreeSurface(kvp.second.surface);
            stbi_image_free(kvp.second.data);
        }

        return true;
    }

    bool video_queue_text(
            common::result& r,
            game_t& game,
            bank_id_t id,
            color_t color,
            int32_t y,
            int32_t x,
            const std::string& value) {
        auto font = font_find(id);
        if (font == nullptr) {
            r.error("V001", fmt::format("unknown font: {}", make_bank_key(id)));
            return false;
        }

        auto entity = game.registry.create();
        auto& text = game.registry.assign<text_t>(entity);
        text.pos.y = y;
        text.pos.x = x;
        text.data = value;
        text.color = color;
        text.font = font_t{id};

        return true;
    }

    bool video_queue_image(
            common::result& r,
            game_t& game,
            bank_id_t id,
            int32_t y,
            int32_t x,
            size_t size) {
        auto image = image_find(id);
        if (image == nullptr) {
            r.error("V001", fmt::format("unknown image: {}", make_bank_key(id)));
            return false;
        }

        auto entity = game.registry.create();
        auto& blit = game.registry.assign<blit_t>(entity);
        blit.image = id;
        blit.src_rect = rect_t{{0, 0}, {image->size.w, image->size.h}};
        if (size.h != 0 && size.w != 0)
            blit.dest_rect = rect_t{{x, y}, size};
        else
            blit.dest_rect = rect_t{{x, y}, {image->size.w, image->size.h}};

        return true;
    }

    bool video_queue_hline(
            common::result& r,
            game_t& game,
            color_t color,
            int32_t y,
            int32_t x,
            int32_t w) {
        auto entity = game.registry.create();
        auto& hline = game.registry.assign<hline_t>(entity);
        hline.w = w;
        hline.color = color;
        hline.pos = point_t{x, y};

        return true;
    }

    bool video_queue_vline(
            common::result& r,
            game_t& game,
            color_t color,
            int32_t y,
            int32_t x,
            int32_t h) {
        auto entity = game.registry.create();
        auto& vline = game.registry.assign<vline_t>(entity);
        vline.h = h;
        vline.color = color;
        vline.pos = point_t{x, y};

        return true;
    }

    bool video_queue_box(
            common::result& r,
            game_t& game,
            color_t color,
            int32_t y,
            int32_t x,
            int32_t w,
            int32_t h,
            bool fill) {
        auto entity = game.registry.create();
        auto& box = game.registry.assign<box_t>(entity);
        box.fill = fill;
        box.color = color;
        box.bounds = rect_t{x, y, w, h};

        return true;
    }

}