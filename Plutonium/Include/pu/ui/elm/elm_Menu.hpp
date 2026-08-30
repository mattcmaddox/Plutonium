
/*

    Plutonium library

    @file Menu.hpp
    @brief A Menu is a very useful Element for option browsing or selecting.
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <pu/ui/elm/elm_Element.hpp>
#include <vector>
#include <chrono>
#include <functional>

namespace pu::ui::elm
{
    class MenuItem
    {
        public:
            MenuItem(const std::string& Name);
            PU_SMART_CTOR(MenuItem)

            std::string GetName();
            void SetName(const std::string& Name);
            Color GetColor();
            void SetColor(Color Color);
            void AddOnClick(std::function<void()> Callback, u64 Key = HidNpadButton_A);
            s32 GetCallbackCount();
            std::function<void()> GetCallback(s32 Index);
            u64 GetCallbackKey(s32 Index);
            std::string GetIcon();
            void SetIcon(std::string Icon);
            bool HasIcon();
            void SetFactor(float Factor);
            float GetFactor();
            // Optional per-item backdrop fill (used e.g. to "fill in" rows whose
            // game was marked with X). Transparent by default = no custom fill.
            void SetBackgroundColor(Color Color);
            bool HasBackgroundColor();
            Color GetBackgroundColor();
        private:
            std::string name;
            Color clr;
            bool hasbg = false;
            Color bgclr;
            bool hasicon = false;
            std::string icon;
            float factor = 0;
            std::vector<std::function<void()>> cbs;
            std::vector<u64> cbipts;
    };

    class Menu : public Element
    {
        public:
            Menu(s32 X, s32 Y, s32 Width, Color OptionColor, s32 ItemSize, s32 ItemsToShow, s32 fontSize = 25);
            PU_SMART_CTOR(Menu)

            s32 GetX();
            void SetX(s32 X);
            s32 GetY();
            void SetY(s32 Y);
            s32 GetWidth();
            void SetWidth(s32 Width);
            s32 GetHeight();
            s32 GetItemSize();
            void SetItemSize(s32 ItemSize);
            s32 GetNumberOfItemsToShow();
            void SetNumberOfItemsToShow(s32 ItemsToShow);
            Color GetColor();
            void SetColor(Color Color);
            Color GetOnFocusColor();
            void SetOnFocusColor(Color Color);
            Color GetOnFocusTextColor();
            void SetOnFocusTextColor(Color Color);
            Color GetScrollbarColor();
            void SetScrollbarColor(Color Color);
            Color GetScrollbarHandleColor();
            void SetScrollbarHandleColor(Color Color);
            s32 GetScrollbarOffsetX();
            void SetScrollbarOffsetX(s32 Offset);
            void SetOnSelectionChanged(std::function<void()> Callback);
            void RefreshFocusTextRender();
            void SetHideFocusedText(bool Hide);
            void SetSuppressFocus(bool Suppress);
            void SetSuppressBaseBackground(bool Suppress);

            void AddItem(MenuItem::Ref &Item);
            void ClearItems();
            void SetCooldownEnabled(bool Cooldown);
            MenuItem::Ref &GetSelectedItem();
            std::vector<MenuItem::Ref> &GetItems();
            s32 GetSelectedIndex();
            void SetSelectedIndex(s32 Index);
            void OnRender(render::Renderer::Ref &Drawer, s32 X, s32 Y);
            void OnInput(u64 Down, u64 Up, u64 Held, Touch Pos);
            void ReloadItemRenders();

            // Per-menu icon styling knobs (CatHead). iconScale scales the icon
            // up/down from its default (row height - 10). When tintIconWithText
            // is true, the focused row's icon is tinted to the on-focus text
            // colour (so a white checkbox outline turns black on the focused
            // row, matching the label), and unfocused icons use their own color.
            void SetIconScale(float Scale);
            float GetIconScale();
            void SetTintIconWithText(bool Tint);
            bool GetTintIconWithText();
            // Per-row backdrop fill overhang (CatHead). Adds this many px to the
            // width of every row's backdrop fill only (not to this->w), so the
            // focused/backdrop bar can extend past the menu's box right edge —
            // used to fill the no-scrollbar gutter flush to the art pane. Rows
            // keep their true state colors because OnRender computes each row's
            // fill color regardless of the overhang.
            void SetRowFillOverhang(s32 Overhang);
            s32 GetRowFillOverhang();
        private:
            bool dtouch;
            s32 x;
            s32 y;
            s32 w;
            s32 isize;
            s32 ishow;
            s32 previsel;
            s32 fisel;
            s32 isel;
            s32 pselfact;
            s32 selfact;
            Color scb;
            Color sclr;
            Color clr;
            Color fcs;
            Color fct;
            bool hideFocusedText = false;
            bool suppressFocus = false;
            bool suppressBaseBackground = false;

            bool tintIconWithText = false;
            float iconScale = 1.0f;
            s32 rowFillOverhang = 0;

            s32 scrollbarOffsetX;
            bool icdown;
            int basestatus;
            std::chrono::time_point<std::chrono::steady_clock> basetime;
            std::function<void()> onselch;
            std::vector<MenuItem::Ref> itms;
            render::NativeFont font;
            render::NativeFont meme;
            std::vector<render::NativeTexture> loadednames;
            std::vector<render::NativeTexture> loadedicons;
    };
}