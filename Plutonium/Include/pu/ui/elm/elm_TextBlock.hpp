
/*

    Plutonium library

    @file TextBlock.hpp
    @brief A TextBlock is a very useful Element which is used to draw text on the screen.
    @author XorTroll

    @copyright Plutonium project - an easy-to-use UI framework for Nintendo Switch homebrew

*/

#pragma once
#include <pu/ui/elm/elm_Element.hpp>

namespace pu::ui::elm
{
    class TextBlock : public Element
    {
        public:
            TextBlock(s32 X, s32 Y, const std::string& Text, s32 FontSize = 25);
            PU_SMART_CTOR(TextBlock)
            ~TextBlock();

            s32 GetX();
            void SetX(s32 X);
            s32 GetY();
            void SetY(s32 Y);
            s32 GetWidth();
            s32 GetHeight();
            s32 GetTextWidth();
            s32 GetTextHeight();
            std::string GetText();
            void SetText(const std::string& Text);
            void SetFontSize(s32 FontSize);
            // CatHead: lets callers measure with the block's own font size
            // via texture-free metrics (ui/textMetrics.hpp) instead of
            // rasterizing probes through the block.
            s32 GetFontSize();
            Color GetColor();
            void SetColor(Color Color);
            void OnRender(render::Renderer::Ref &Drawer, s32 X, s32 Y);
            // CatHead: pixel-phase smooth scrolling. When BandH >= 1, the
            // render draws ONLY the texture band starting at (possibly
            // NEGATIVE) BandYPx — dst height = BandH, so the drawn result
            // always occupies exactly the element's slot. BandYPx < 0 is
            // legitimate: the blank rows above the texture render as
            // nothing and the rest is shifted down (see RenderTextureBand).
            // SetRenderBand clamps H to >= 1; ClearRenderBand sets H = 0
            // ("band off" = plain full draw).
            void SetRenderBand(s32 BandYPx, s32 BandH);
            void ClearRenderBand();
            void OnInput(u64 Down, u64 Up, u64 Held, Touch Pos);
        private:
            std::string text;
            s32 x;
            s32 y;
            render::NativeFont font;
            render::NativeFont meme;
            s32 fontSize = 25;
            s32 bandYPx = -1;   // <0 = normal full draw
            s32 bandHPx = 0;
            Color clr;
            render::NativeTexture ntex = nullptr;
    };
}