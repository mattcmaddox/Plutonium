#include <pu/ui/elm/elm_Menu.hpp>

namespace pu::ui::elm
{
    MenuItem::MenuItem(const std::string& Name)
    {
        this->clr = { 10, 10, 10, 255 };
        this->name = Name;
        this->hasicon = false;
    }

    std::string MenuItem::GetName()
    {
        return this->name;
    }

    void MenuItem::SetName(const std::string& Name)
    {
        this->name = Name;
    }

    Color MenuItem::GetColor()
    {
        return this->clr;
    }

    void MenuItem::SetColor(Color Color)
    {
        this->clr = Color;
    }

    void MenuItem::AddOnClick(std::function<void()> Callback, u64 Key)
    {
        this->cbs.push_back(Callback);
        this->cbipts.push_back(Key);
    }

    s32 MenuItem::GetCallbackCount()
    {
        return this->cbs.size();
    }

    std::function<void()> MenuItem::GetCallback(s32 Index)
    {
        if(this->cbs.empty()) return [&](){};
        return this->cbs[Index];
    }

    u64 MenuItem::GetCallbackKey(s32 Index)
    {
        return this->cbipts[Index];
    }

    std::string MenuItem::GetIcon()
    {
        return this->icon;
    }

    void MenuItem::SetIcon(std::string Icon)
    {
        std::ifstream ifs(Icon);
        if(ifs.good())
        {
            this->icon = Icon;
            this->hasicon = true;
            this->factor = 0;
        }
        ifs.close();
    }

    bool MenuItem::HasIcon()
    {
        return this->hasicon;
    }

    void MenuItem::SetFactor(float Factor)
    {
        this->factor = Factor;
    }

    float MenuItem::GetFactor()
    {
        return this->factor;
    }

    Menu::Menu(s32 X, s32 Y, s32 Width, Color OptionColor, s32 ItemSize, s32 ItemsToShow, s32 fontSize)
        : Element::Element(), x(X), y(Y), w(Width), clr(OptionColor), isize(ItemSize), ishow(ItemsToShow)
    {
        this->scb = { 110, 110, 110, 255 };
        this->sclr = { 80, 80, 80, 255 };
        this->scrollbarOffsetX = 0;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->onselch = [&](){};
        this->icdown = false;
        this->dtouch = false;
        this->fcs = { 40, 40, 40, 255 };
        this->fct = { 255, 255, 255, 255 };
        this->basestatus = 0;
        this->font = render::LoadDefaultFont(fontSize);
        this->meme = render::LoadSharedFont(render::SharedFont::NintendoExtended, fontSize);
    }

    s32 Menu::GetX()
    {
        return this->x;
    }

    void Menu::SetX(s32 X)
    {
        this->x = X;
    }

    s32 Menu::GetY()
    {
        return this->y;
    }

    void Menu::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 Menu::GetWidth()
    {
        return this->w;
    }

    void Menu::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 Menu::GetHeight()
    {
        return (this->isize * this->ishow);
    }

    s32 Menu::GetItemSize()
    {
        return this->isize;
    }

    void Menu::SetItemSize(s32 ItemSize)
    {
        this->isize = ItemSize;
    }

    s32 Menu::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void Menu::SetNumberOfItemsToShow(s32 ItemsToShow)
    {
        this->ishow = ItemsToShow;
    }

    Color Menu::GetColor()
    {
        return this->clr;
    }

    void Menu::SetColor(Color Color)
    {
        this->clr = Color;
    }
    
    Color Menu::GetOnFocusColor()
    {
        return this->fcs;
    }

    void Menu::SetOnFocusColor(Color Color)
    {
        this->fcs = Color;
    }

    Color Menu::GetOnFocusTextColor()
    {
        return this->fct;
    }

    void Menu::SetOnFocusTextColor(Color Color)
    {
        this->fct = Color;
    }

    Color Menu::GetScrollbarColor()
    {
        return this->scb;
    }

    void Menu::SetScrollbarColor(Color Color)
    {
        this->scb = Color;
    }

    Color Menu::GetScrollbarHandleColor()
    {
        return this->sclr;
    }

    void Menu::SetScrollbarHandleColor(Color Color)
    {
        this->sclr = Color;
    }

    s32 Menu::GetScrollbarOffsetX()
    {
        return this->scrollbarOffsetX;
    }

    void Menu::SetScrollbarOffsetX(s32 Offset)
    {
        this->scrollbarOffsetX = Offset;
    }

    void Menu::SetOnSelectionChanged(std::function<void()> Callback)
    {
        this->onselch = Callback;
    }

    void Menu::RefreshFocusTextRender()
    {
        if(this->itms.empty() || this->loadednames.empty()) return;
        if(this->isel < 0 || this->isel >= (s32)this->itms.size()) return;
        s32 focusIdx = this->isel - this->fisel;
        if(focusIdx < 0 || focusIdx >= (s32)this->loadednames.size()) return;
        render::DeleteTexture(this->loadednames[static_cast<u32>(focusIdx)]);
        auto strname = this->itms[this->isel]->GetName();
        auto tex = render::RenderText(this->font, this->meme, strname, this->fct);
        this->loadednames[static_cast<u32>(focusIdx)] = tex;
    }

    void Menu::SetHideFocusedText(bool Hide)
    {
        this->hideFocusedText = Hide;
    }

    void Menu::SetSuppressFocus(bool Suppress)
    {
        this->suppressFocus = Suppress;
    }

    void Menu::SetSuppressBaseBackground(bool Suppress)
    {
        this->suppressBaseBackground = Suppress;
    }

    void Menu::AddItem(MenuItem::Ref &Item)
    {
        this->itms.push_back(Item);
    }

    void Menu::ClearItems()
    {
        this->itms.clear();
        this->loadednames.clear();
        this->loadedicons.clear();
    }

    void Menu::SetCooldownEnabled(bool Cooldown)
    {
        this->icdown = Cooldown;
    }

    MenuItem::Ref &Menu::GetSelectedItem()
    {
        return this->itms[this->isel];
    }

    std::vector<MenuItem::Ref> &Menu::GetItems()
    {
        return this->itms;
    }

    s32 Menu::GetSelectedIndex()
    {
        return this->isel;
    }

    void Menu::SetSelectedIndex(s32 Index)
    {
        if(this->itms.size() > Index)
        {
            this->isel = Index;
            this->fisel = 0;
            // VIM-zz centered scroll (app-wide convention): place the selection
            // at the viewport midpoint once it passes it, pinned at the top
            // until then and at the bottom for the last rows. Keeps fisel in
            // sync with interactive navigation (OnInput) so the window never
            // disagrees between the two code paths.
            if(this->itms.size() > this->ishow)
            {
                const s32 maxFisel = this->itms.size() - this->ishow;
                const s32 desiredFisel = this->isel - (this->ishow / 2);
                this->fisel = (desiredFisel < 0) ? 0 : ((desiredFisel > maxFisel) ? maxFisel : desiredFisel);
            }

            ReloadItemRenders();
            this->selfact = 255;
            this->pselfact = 0;
        }
    }

    void Menu::OnRender(render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if(!this->itms.empty())
        {
            s32 cx = X;
            s32 cy = Y;
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if(this->loadednames.empty()) ReloadItemRenders();
            for(s32 i = this->fisel; i < (its + this->fisel); i++)
            {
                s32 clrr = this->clr.R;
                s32 clrg = this->clr.G;
                s32 clrb = this->clr.B;
                s32 nr = clrr - 70;
                if(nr < 0) nr = 0;
                s32 ng = clrg - 70;
                if(ng < 0) ng = 0;
                s32 nb = clrb - 70;
                if(nb < 0) nb = 0;
                Color nclr(nr, ng, nb, this->clr.A);
                auto loadedidx = i - this->fisel;
                auto curname = this->loadednames[loadedidx];
                auto curicon = this->loadedicons[loadedidx];
                auto itm = this->itms[i];
                s32 xh = render::GetTextureHeight(curname);
                s32 tx = (cx + 25);
                s32 ty = ((ch - xh) / 2) + cy;
                s32 icx = 0, icw = 0, ich = 0, icy = 0;
                bool hasIcon = itm->HasIcon();
                if(hasIcon)
                {
                    if (itm->GetFactor() == 0) {
                        auto [w,h] = render::GetTextureSize(curicon);
                        itm->SetFactor((float)h/w);
                    }
                    float factor = itm->GetFactor();
                    icw = (this->isize - 10);
                    ich = icw;
                    icx = (cx + 25);
                    icy = (cy + 5);
                    tx = (icx + icw + 25);
                    if(factor < 1)
                    {
                        ich = ich*factor;
                        icy = icy+((this->isize-ich)/2);
                    } else
                    {
                        icw = icw/factor;
                        icx = icx+((this->isize-icw)/2);
                    }
                }
                s32 tw = render::GetTextureWidth(curname);
                const bool isCurrentFocus = (i == this->isel);
                const bool isPreviousFocus = (i == this->previsel && !isCurrentFocus);
                Color bgColor = isCurrentFocus ? this->fcs : (isPreviousFocus ? Color(this->clr.R - 70, this->clr.G - 70, this->clr.B - 70, this->clr.A) : this->clr);
                if(this->selfact < 255 && isCurrentFocus) this->selfact += 48;
                if(!this->suppressBaseBackground && !(this->suppressFocus && isPreviousFocus))
                    Drawer->RenderRectangleFill(bgColor, cx, cy, cw, ch);
                if(hasIcon)
                    Drawer->RenderTexture(curicon, icx, icy, { -1, icw, ich, -1.0f });
                if(!(this->hideFocusedText && (i == this->isel)))
                    Drawer->RenderTexture(curname, tx, ty);
                cy += ch;
            }
            if(this->ishow < this->itms.size())
            {
                s32 scx = X + (this->w - 20) + this->scrollbarOffsetX;
                s32 scy = Y;
                s32 scw = 20;
                s32 sch = (this->ishow * this->isize);
                Drawer->RenderRectangleFill(this->scb, scx, scy, scw, sch);
                s32 fch = ((this->ishow * sch) / this->itms.size());
                s32 fcy = scy + (this->fisel * (sch / this->itms.size()));
                Drawer->RenderRectangleFill(this->sclr, scx, fcy, scw, fch);
            }
            //Drawer->RenderShadowSimple(cx, cy, cw, 5, 160);
        }
    }

    void Menu::OnInput(u64 Down, u64 Up, u64 Held, Touch Pos)
    {
        if(itms.empty()) return;
        if(basestatus == 1)
        {
            auto curtime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - basetime).count();
            if(diff >= 150)
            {
                basestatus = 2;
            }
        }
        if(!Pos.IsEmpty())
        {
            s32 cx = this->GetProcessedX();
            s32 cy = this->GetProcessedY();
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            for(s32 i = this->fisel; i < (this->fisel + its); i++)
            {
                if(((cx + cw) > Pos.X) && (Pos.X > cx) && ((cy + ch) > Pos.Y) && (Pos.Y > cy))
                {
                    this->dtouch = true;
                    this->previsel = this->isel;
                    this->isel = i;
                    (this->onselch)();
                    if(i == this->isel) this->selfact = 255;
                    else if(i == this->previsel) this->pselfact = 0;
                    break;
                }
                cy += this->isize;
            }
        }
        else if(this->dtouch)
        {
            if((this->selfact >= 255) && (this->pselfact <= 0))
            {
                if(this->icdown) this->icdown = false;
                else (this->itms[this->isel]->GetCallback(0))();
                this->dtouch = false;
            }
        }
        else
        {
            if(Down & HidNpadButton_AnyDown)
            {
                bool move = true;
                if(Held & HidNpadButton_StickRDown)
                {
                    move = false;
                    if(basestatus == 0)
                    {
                        basetime = std::chrono::steady_clock::now();
                        basestatus = 1;
                    }
                    else if(basestatus == 2)
                    {
                        basestatus = 0;
                        move = true;
                    }
                }
                if(move)
                {
                    if(this->isel < (this->itms.size() - 1))
                    {
                        this->previsel = this->isel;
                        this->isel++;
                        (this->onselch)();
                        // VIM-zz centered scroll (app-wide convention): keep the
                        // selection at the viewport midpoint once it passes it
                        // (pinned at the top until then, at the bottom for the
                        // last rows) instead of letting it run to the edge.
                        if(this->itms.size() > this->ishow)
                        {
                            const s32 maxFisel = this->itms.size() - this->ishow;
                            const s32 desiredFisel = this->isel - (this->ishow / 2);
                            const s32 newFisel = (desiredFisel < 0) ? 0 : ((desiredFisel > maxFisel) ? maxFisel : desiredFisel);
                            if(newFisel != this->fisel)
                                this->fisel = newFisel;
                        }
                        // Always re-render: even when fisel is unchanged,
                        // the focused/unfocused item text colors differ.
                        ReloadItemRenders();
                        if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                        {
                            if(i == this->isel) this->selfact = 0;
                            else if(i == this->previsel) this->pselfact = 255;
                        }
                    }
                    else
                    {
                        // Wrap-around: the highlight jumps from the last row to
                        // the first, but the selection DID change, so the
                        // callback must fire exactly like a normal move —
                        // otherwise the shop sidebar never selects the wrapped
                        // section and the gallery keeps showing the old one.
                        this->previsel = this->isel;
                        this->isel = 0;
                        this->fisel = 0;
                        (this->onselch)();
                        ReloadItemRenders();
                        if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                        {
                            if(i == this->isel) this->selfact = 0;
                            else if(i == this->previsel) this->pselfact = 255;
                        }
                    }
                }
            }
            else if(Down & HidNpadButton_AnyUp)
            {
                bool move = true;
                if(Held & HidNpadButton_StickRUp)
                {
                    move = false;
                    if(basestatus == 0)
                    {
                        basetime = std::chrono::steady_clock::now();
                        basestatus = 1;
                    }
                    else if(basestatus == 2)
                    {
                        basestatus = 0;
                        move = true;
                    }
                }
                if(move)
                {
                    if(this->isel > 0)
                    {
                        this->previsel = this->isel;
                        this->isel--;
                        (this->onselch)();
                        // VIM-zz centered scroll (app-wide convention) — same
                        // centering as the Down path so the window moves
                        // symmetrically in both directions.
                        if(this->itms.size() > this->ishow)
                        {
                            const s32 maxFisel = this->itms.size() - this->ishow;
                            const s32 desiredFisel = this->isel - (this->ishow / 2);
                            const s32 newFisel = (desiredFisel < 0) ? 0 : ((desiredFisel > maxFisel) ? maxFisel : desiredFisel);
                            if(newFisel != this->fisel)
                                this->fisel = newFisel;
                        }
                        // Always re-render: even when fisel is unchanged,
                        // the focused/unfocused item text colors differ.
                        ReloadItemRenders();
                        if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                        {
                            if(i == this->isel) this->selfact = 0;
                            else if(i == this->previsel) this->pselfact = 255;
                        }
                    }
                    else
                    {
                        // Wrap-around from the top row to the last row: fire the
                        // selection callback too (see the AnyDown wrap above).
                        this->previsel = this->isel;
                        this->isel = this->itms.size() - 1;
                        this->fisel = 0;
                        if(this->itms.size() > this->ishow)
                        {
                            // Wrapped to the last row: center it (zz) instead of
                            // pinning it to the very bottom of the window.
                            const s32 maxFisel = this->itms.size() - this->ishow;
                            const s32 desiredFisel = this->isel - (this->ishow / 2);
                            this->fisel = (desiredFisel < 0) ? 0 : ((desiredFisel > maxFisel) ? maxFisel : desiredFisel);
                        }
                        (this->onselch)();
                        ReloadItemRenders();
                        if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                        {
                            if(i == this->isel) this->selfact = 0;
                            else if(i == this->previsel) this->pselfact = 255;
                        }
                    }
                }
            }
            else
            {
                s32 ipc = this->itms[this->isel]->GetCallbackCount();
                if(ipc > 0) for(s32 i = 0; i < ipc; i++)
                {
                    if(Down & this->itms[this->isel]->GetCallbackKey(i))
                    {
                        if(this->icdown) this->icdown = false;
                        else (this->itms[this->isel]->GetCallback(i))();
                    }
                }
            }
        }
    }
    
    void Menu::ReloadItemRenders()
    {
        for(u32 i = 0; i < this->loadednames.size(); i++) render::DeleteTexture(this->loadednames[i]);
        for(u32 i = 0; i < this->loadedicons.size(); i++) render::DeleteTexture(this->loadedicons[i]);
        this->loadednames.clear();
        this->loadedicons.clear();
        s32 its = this->ishow;
        if(its > this->itms.size()) its = this->itms.size();
        if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
        for(s32 i = this->fisel; i < (its + this->fisel); i++)
        {
            auto strname = this->itms[i]->GetName();
            Color txtclr = (i == this->isel) ? this->fct : this->itms[i]->GetColor();
            auto tex = render::RenderText(this->font, this->meme, strname, txtclr);
            this->loadednames.push_back(tex);
            if(this->itms[i]->HasIcon())
            {
                auto stricon = this->itms[i]->GetIcon();
                auto icontex = render::LoadImage(stricon);
                this->loadedicons.push_back(icontex);
            }
            else this->loadedicons.push_back(NULL);
        }
    }
}
