
#ifndef SHANG_PAGE_H
#define SHANG_PAGE_H

#include <lvgl.h>
#include <vector>

// using namespace std;

typedef struct
{
    unsigned char PageID; // PageID 每个页面有唯一的ID
    lv_obj_t *Page;       // 界面
    lv_scr_load_anim_t load_anim;
    void (*init)(lv_obj_t *Page); // 页面初始化
} Page;

class PageManager
{
public:
    PageManager();
    ~PageManager();

    void add_Page(Page *Page, unsigned char id, lv_scr_load_anim_t anim, void (*init)(lv_obj_t *Page));
    void switch_Page(unsigned char id);

private:
    Page *curr_Page;
    std::vector<Page *> Page_list;
};

#endif