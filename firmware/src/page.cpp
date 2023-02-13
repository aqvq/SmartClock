#include "page.h"

PageManager::PageManager()
{
    curr_Page = NULL;
}

PageManager::~PageManager()
{
}

void PageManager::add_Page(Page *Page, unsigned char id, lv_scr_load_anim_t anim, void (*init)(lv_obj_t *Page))
{
    Page->PageID = id;
    Page->Page = lv_obj_create(NULL);
    Page->load_anim = anim;
    Page->init = init;
    Page_list.push_back(Page);
}

void PageManager::switch_Page(unsigned char id)
{
    unsigned char i;
    if (!curr_Page || curr_Page->PageID != id)
    {
        for (i = 0; i < Page_list.size(); i++)
        {
            Page *Page = Page_list[i];
            if (Page->PageID == id)
            {
                Page->init(Page->Page);
                lv_scr_load_anim(Page->Page, Page->load_anim, 800, 0, false);
                if (curr_Page != NULL)
                    lv_obj_clean(curr_Page->Page);
                curr_Page = Page;
                break;
            }
        }
    }
}
