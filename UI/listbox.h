#ifndef LISTBOX_H
#define LISTBOX_H

#include "etl/array.h"

namespace listbox
{
   using namespace etl;

   struct Listbox_t;

   struct ListBoxItem
   {
         ListBoxItem();
         ListBoxItem(const ListBoxItem&);
         ListBoxItem& operator=(const ListBoxItem&);

         char*  caption() { return m_caption; }
         void   set_caption(cchar* cap) { estr::assign<32>(m_caption, cap); }
         //int16  index_in_container() { return m_index_in_container; }
         //void   set_index_in_container(cint16 i) { m_index_in_container = i; }

         int16  index_in_parent() { return m_index_in_parent; }
         void   set_index_in_parent(cint16 i) { m_index_in_parent = i; }

         //ListBoxItem* parent()   { return (m_parent ? m_parent : nullptr); }
         //void   set_parent(ListBoxItem* par) { m_parent = par; }

         int16 parent_index()             { return m_parent_index; }
         void  set_parent_index(int16 i)  { m_parent_index = i; }

         bool is_root()        { return m_is_root; }
         void set_root(bool b) { m_is_root = b; }

         bool  has_child()   { return  m_has_child; }
         void  set_has_child(bool b)   { m_has_child = b; }
         int16 child_count() { return  m_child_count; }
         void  set_child_count(const int16 cnt) { m_child_count = cnt; }

         friend bool operator==(const ListBoxItem& lhs, const ListBoxItem& rhs)
         {
            return ((lhs.m_has_child == rhs.m_has_child) && (lhs.m_is_root == rhs.m_is_root)) /*&&
                 (lhs.m_index_in_container == rhs.m_index_in_container))*/;
         }

         friend bool operator!=(const ListBoxItem& lhs, const ListBoxItem& rhs)
         {
            return !(lhs == rhs);
         }

      public:
         //typedef void(*OnItemClick_t2)(ListBoxItem&);
         //OnItemClick_t2 OnItemClick2;

      private:
         bool  m_has_child, m_is_root;
         char  m_caption[32];
         int16 /*m_index_in_container,*/
         m_index_in_parent,
         m_parent_index, m_child_count;
         //ListBoxItem* m_parent;
   };

   typedef void(*OnItemClick_t)(ListBoxItem&);
   OnItemClick_t OnItemClick;

   ListBoxItem::ListBoxItem()
      : m_has_child(false), m_caption(), m_index_in_parent(-1), m_parent_index(-1), m_child_count(0)
        /*m_parent(nullptr)*/
   {
      estr::set_null(m_caption, 32);
   }

   ListBoxItem::ListBoxItem(const ListBoxItem& rhs) :
      m_has_child(rhs.m_has_child), m_index_in_parent(rhs.m_index_in_parent)
   {
      //m_parent = rhs.m_parent;
      estr::assign<32>(m_caption, rhs.m_caption);
   }

   ListBoxItem& ListBoxItem::operator=(const ListBoxItem& rhs)
   {
      m_has_child = rhs.m_has_child;
      m_index_in_parent  = rhs.m_index_in_parent;
      //m_parent = nullptr;
      //m_parent = rhs.m_parent;
      estr::assign<32>(m_caption, rhs.m_caption);
      return *this;
   }

   struct Listbox_t
   {
         enum ScrollSelPos { Bottom, Down, Top, Up, None};
         Listbox_t();

         void add_item();
         void add_item(cint16, cchar*, bool);
         void add_item_to_root(cint16, cchar*, bool);
         void add_item(ListBoxItem&, ListBoxItem& );
         void add_parent(cint16, cchar*);

         void clear();
         void clear_last_four() {for(int8 i = 0; i < m_last_four.size(); m_last_four[i++] = -1);}
         void create();

         void delete_item(cint16 index);
         void delete_item(ListBoxItem&);

         ListBoxItem& find_item(int16, int16);
         void exec();
         void init_menu();

         void loop_title_n_text();
         void cancel_pressed(ListBoxItem& );
         void ok_pressed(ListBoxItem& );

         void show();
         void show(cint16 par);
         void show(cint16, int16, ScrollSelPos = None);
         void show(ListBoxItem&, ListBoxItem& );
         void show_title();

         bool change_page() { return m_change_page; }
         void set_change_page(bool b) { m_change_page = b; }
         void clear_scroll_selection();
         void selection_bottom(cchar*, bool = false);
         void selection_helper(cchar*, uint8, bool);
         void selection_down(cchar*, bool = false);
         void selection_top(cchar*, bool = false);
         void selection_up(cchar*, bool = false);

         int16 current_parent_child_count()     { return m_lboxes[current_parent_index()].child_count(); }
         int16 current_parent_index()     { return m_current_parent; }
         void  set_current_parent_index(int16 p_id) { m_current_parent = p_id; }
         int16 selected_line() { return m_selected_line; }
         void  set_selected_line(cint16 i, ScrollSelPos);
         int16 selected_element_index() { return m_selected_element_index; }
         void  set_selected_element_index(int16, ScrollSelPos);

         void set_title(cchar* t) { estr::assign<32>(m_title, t); }

         ListBoxItem& item_at(cint16 i) { return m_lboxes[i]; }

         uint16& count() { return m_count; }
         void    update_count() { ++m_count; }

         void    do_sth_local(ListBoxItem& item);

      private:
         void draw_scrollbar();
         void set_cursor_pos(int8&);
         void show_element_title(cchar*, bool = false);
         void show_numbering(cuint8);
         void show_last_four(cchar*);

      private:
         uint16 m_count;
         uint16 m_selected_element_index, m_current_parent, m_prev_parent;
         //uint8  sel_top, sel_bottom;
         int8   m_selected_line;
         bool   m_change_page;
         char   m_title[32];
         //int8   m_last_four[4];
         etl::array<ListBoxItem, 512> m_lboxes;
         etl::array<int8, 4> m_last_four;

   } list_box;

   Listbox_t::Listbox_t() : m_count(0), /*sel_bottom(24), sel_top(12),*/ m_selected_line(1),
      m_selected_element_index(1), m_title()
   {
      for(int8 i = 0; i < m_last_four.size(); ++i)
         m_last_four[i] = -1;
   }

   void do_sth(ListBoxItem& item)
   {
      TP_ShowTextFullScr(&display_info,
                         (uchar*)"%s\npid: %d\nid in parent: %03d\nchildren: %d"
                         "\nis_root: %d",
                         item.caption(),
                         item.parent_index(), item.index_in_parent(),
                         item.child_count(),
                         item.is_root());
   }

   void Listbox_t::do_sth_local(ListBoxItem& item)
   {
      TP_ShowTextFullScr(&display_info, (uchar*)"parent index: %d\nindex: %03d\nChildren: %d\nis_root: %d",
                         item.parent_index(), item.index_in_parent(), item.child_count(), item.is_root());
   }

   /**

   */
   void Listbox_t::add_item(cint16 parent_index, cchar* name, bool is_parent = false)
   {
      m_lboxes[m_count].set_caption(name);
      m_lboxes[m_count].set_has_child(is_parent);
      m_lboxes[m_count].set_parent_index(parent_index);

      int16 i = m_lboxes[parent_index].child_count();

      m_lboxes[m_count].set_index_in_parent(++i);
      m_lboxes[parent_index].set_child_count(i);
      //TP_ShowTextFullScr(&display_info, (uchar*)"%d %d %d", parent_index, m_stop_index, i);

      //++m_stop_index;
      ++m_count;
   }

   void Listbox_t::add_item_to_root(cint16 parent_index,
                                    cchar* name,
                                    bool is_parent = false)
   {
      /** No Item added already */
   }

   void Listbox_t::init_menu()
   {
      ///Prepare Root/base for all other elements
      m_count = 0;
      m_lboxes[m_count].set_caption(m_title);
      m_lboxes[m_count].set_has_child(true);
      m_lboxes[m_count].set_parent_index(-2);
      m_lboxes[m_count].set_root(true);

      //++m_stop_index;
      ++m_count;
   }

   void Listbox_t::add_item(ListBoxItem& parent, ListBoxItem& item)
   {
      /** No Item added already */
      add_item(parent.index_in_parent(), item.caption(), item.has_child());
   }

   void Listbox_t::add_parent(cint16 parent, cchar* name)
   {
   }

   void Listbox_t::clear()
   {
      for(uint16 i = 0; i < m_count; ++i)
         m_lboxes[i] = ListBoxItem();

      for(int8 i = 0; i < m_last_four.size(); ++i)
         m_last_four[i] = -1;
   }

   void Listbox_t::delete_item(cint16 index)
   {
      m_lboxes[index] = ListBoxItem();
   }

   void Listbox_t::delete_item(ListBoxItem& item)
   {
      item = ListBoxItem();
   }

   ListBoxItem& Listbox_t::find_item(int16 par, int16 child)
   {
      for(uint16 i = 0; i < m_lboxes.size(); ++i)
      {
         if(m_lboxes[i].parent_index() == par &&
               m_lboxes[i].index_in_parent() == child)
         {
            return m_lboxes[i];
         }
      }
      return m_lboxes[0];
   }

   void Listbox_t::clear_scroll_selection()
   {
      /*TP_ScrClrRect(11,  sel_top, 121, sel_top);
      TP_ScrClrRect(11,  sel_bottom, 121, sel_bottom);
      TP_ScrClrRect(10,  sel_top, 12, sel_bottom);
      TP_ScrClrRect(121, sel_top, 121, sel_bottom);*/
   }

   void Listbox_t::selection_helper(cchar* text, uint8 pos, bool has_child)
   {
      uint8 sel_pos = 13 * m_selected_line;

      TP_ScrAttrSet(1);
      TP_ScrGotoxyEx(10, sel_pos);
      show_element_title(text, has_child);
      TP_ScrAttrSet(0);

      /*switch (pos)
      {
         case 1:
         {
            sel_pos = 13;
            TP_ScrGotoxyEx(10, sel_pos);
            //TP_ScrAttrSet(1);
            show_element_title(text, has_child);
            set_selected_line(1);
            //TP_ScrAttrSet(0);
         } break;

         case 2:
         {
            sel_pos = 26;
            TP_ScrGotoxyEx(10, sel_pos);
            //TP_ScrAttrSet(1);
            show_element_title(text, has_child);
            //TP_ScrAttrSet(0);
            set_selected_line(2);
         } break;

         case 3:
         {
            sel_pos = 38;
            TP_ScrGotoxyEx(10, sel_pos);
            //TP_ScrAttrSet(1);
            show_element_title(text, has_child);
            set_selected_line(3);
            //TP_ScrAttrSet(0);
         } break;

         case 4:
         {
            sel_pos = 50;
            TP_ScrGotoxyEx(10, sel_pos);
            //TP_ScrAttrSet(1);
            show_element_title(text, has_child);
            set_selected_line(4);
            //TP_ScrAttrSet(0);
         } break;

         default:
            sel_pos = 50;
            TP_ScrGotoxyEx(10, sel_pos);
            //TP_ScrAttrSet(1);
            show_element_title(text, has_child);
            //TP_ScrAttrSet(0);
            break;
      }
      */
   }

   void Listbox_t::selection_down(cchar* text, bool has_child)
   {

   }

   void Listbox_t::selection_up(cchar* text, bool has_child)
   {
   }

   void Listbox_t::selection_bottom(cchar* text, bool has_child)
   {
      selection_helper(text, 4, has_child);
   }

   void Listbox_t::selection_top(cchar* text, bool has_child)
   {
      selection_helper(text, 1, has_child);
   }

   void Listbox_t::set_selected_element_index(int16 i, ScrollSelPos scroll_direction)
   {
      m_selected_element_index = i;
      switch(scroll_direction)
      {
         case Up:
            --m_selected_element_index;
            break;

         case Down:
            ++m_selected_element_index;
            break;

         default:
            break;
      }

      if(m_selected_element_index > current_parent_child_count())
         m_selected_element_index = 1;
      else if (m_selected_element_index < 1)
         m_selected_element_index = current_parent_child_count();
   }

   void Listbox_t::set_selected_line(cint16 line, ScrollSelPos scrl)
   {
      uint8 line_len;
      uint16 child_count = m_lboxes[current_parent_index()].child_count();

      if (child_count >= 4)
         line_len = 4;
      else if (child_count >= 1 && child_count < 4)
         line_len = child_count;

      m_selected_line = line;
      switch(scrl)
      {
         case Up:
            --m_selected_line;
            break;

         case Down:
            ++m_selected_line;
            break;

         default:
            m_selected_line = line;
            break;
      }

      if(m_selected_line >= 1 && m_selected_line <= 4)
         set_change_page(false);
      else if(m_selected_line <= 0 || m_selected_line >= 5)
         set_change_page(true);

      if(m_selected_line > line_len)
         m_selected_line = 1;
      else if (m_selected_line < 1)
         m_selected_line = line_len;
   }

   void Listbox_t::draw_scrollbar()
   {
      uint8 list_count = 7, scrll_separator = 56 / list_count;
      TP_ScrDrawRect(124, 9, 127, scrll_separator*2);
      TP_ScrFillRect(124, 9, 127, scrll_separator*2);
   }

   void Listbox_t::loop_title_n_text()
   {
      using namespace gpos;
      cchar* data = "Looping Strings Along";
      char tmp[32] = {PNULL};
      uint8 i = 0, sz = strlen(data);
      estr::assign<31>(tmp, data);

      TP_SetDisplayArea(4, 0, 121, 10);
      TP_ScrGotoxyEx(4, 0);
      TP_kbLight(1);

      while (1)
      {
         TP_GetKey();
         TP_ScrGotoxyEx(4, 0);
         TP_ScrClrRect(4, 0, 121, 11);
         TP_LcdPrintf((uchar*)tmp);

         TP_ScrGotoxyEx(4, 11);
         TP_ScrClrRect(4, 12, 121, 22);
         TP_LcdPrintf((uchar*)tmp);

         estr::set_null(tmp, 32);
         estr::copy_from_n(data, ++i, tmp, 32);
         timer::start_ms(300);
         if(i == sz)
            i = -1;
      }
   }

   void Listbox_t::cancel_pressed(ListBoxItem& item)
   {
      uint8 pid = item.parent_index();
      //TP_ShowTextFullScr(&display_info, (uchar*)"%d %d", m_lboxes[pid].parent_index(), pid);
      if (pid < 1)
         pid = 1;

      clear_last_four();
      set_change_page(true);
      show(m_lboxes[pid].parent_index(), pid, Up);
      return exec();
   }

   void Listbox_t::ok_pressed(ListBoxItem& item)
   {
      if(item.has_child())
      {
         m_prev_parent = current_parent_index();
         set_change_page(true);
         clear_last_four();
         show(item.index_in_parent(), 1, Listbox_t::Up);
         set_selected_line(1, Listbox_t::Up);
         set_selected_element_index(1, Up);
         return exec();
      }
      else
      {
         ///Perform  Click Action
         OnItemClick = do_sth;
         OnItemClick(item);
         //m_lboxes[0].OnItemClick2 = do_sth;
         //m_lboxes[0].OnItemClick2(find_item(current_parent_index(), selected_element_index()));
      }
   }

   void Listbox_t::show_title()
   {
      uint8 w = TP_ScrDisplayWidthGet((uchar*)m_title, strlen(m_title));
      uint8 offset = ((120 - 1) + (1 - w))/2;

      TP_SetDisplayArea(4, 0, 86, 12);
      TP_ScrGotoxyEx(3, 0);
      TP_LcdPrintf((uchar*)"%s %d ", m_lboxes[current_parent_index()].caption(), m_selected_line);
      TP_SetDisplayArea(4, 0, 127, 12);
      TP_ScrGotoxyEx(96, 0);
      TP_LcdPrintf((uchar*)"%d/%d", selected_element_index(), current_parent_child_count());
   }

   void Listbox_t::set_cursor_pos(int8& cp)
   {
      if(cp < 1)
         cp = 4;
      else if(cp > 4)
         cp = 1;
   }

   void Listbox_t::show_element_title(cchar* title, bool is_parent)
   {
      char tmp[19] = {NULL};
      if (strlen(title) < 17)
      {
         if(is_parent)
            TP_LcdPrintf((uchar*)"%-17s >", title);
         else
            TP_LcdPrintf((uchar*)"%-17s", title);
      }
      else
      {
         if(is_parent)
         {
            estr::assign<17>(tmp, title);
            TP_LcdPrintf((uchar*)"%-17s >", tmp);
         }
         else
            TP_LcdPrintf((uchar*)"%-17s", title);
      }
   }

   void Listbox_t::show_numbering(cuint8 num)
   {
      uint8 y = (num * 12) + 2;
      TP_ScrGotoxyEx(0, y);
      //TP_LcdPrintf((uchar*)"%02d%", num);
      TP_LcdPrintf((uchar*)"-", num);
      TP_ScrClrRect(0, y, 40, 100);
      TP_ScrGotoxyEx(6, y);
      TP_LcdPrintf((uchar*)"%-1s", "");
   }

   void Listbox_t::exec()
   {
      TP_Kbflush();
      while(1)
      {
         TP_NotifyCheck();
         if (TP_Kbhit() == 0xFF)
         {
            TP_kbLight(1);
            switch (TP_GetKey())
            {
               case TP_KEY_DOWN:
               {
                  uint8 tmp = m_selected_line;
                  set_selected_line(tmp, Down);
                  set_selected_element_index(m_selected_element_index, Down);
                  show(current_parent_index(), m_selected_element_index, Down);
                  TP_ScrRestore(0);
               }
               break;
               case TP_KEY_UP:
               {
                  uint8 tmp = m_selected_line;
                  set_selected_line(tmp, Up);
                  set_selected_element_index(m_selected_element_index, Up);
                  show(current_parent_index(), m_selected_element_index, Up);
                  TP_ScrRestore(0);
               }
               break;
               case TP_KEY_OK:
                  TP_ScrRestore(0);
                  ok_pressed(find_item(current_parent_index(), selected_element_index()));
                  TP_ScrRestore(1);
                  break;

               case TP_KEY_CANCEL:
                  TP_ScrRestore(0);
                  cancel_pressed(find_item(current_parent_index(), selected_element_index()));
                  TP_ScrRestore(1);
                  return;
                  break;

               default:
                  TP_ScrRestore(0);
                  break;
            }
            TP_ScrRestore(1);
         }
      }
   }

   int16 start_from(int16 num)
   {
      if(num > 7)
         return num - 4;
      else
         return num % 4;
   }

   void Listbox_t::show(cint16 par,
                        int16  child,
                        ScrollSelPos scroll)
   {
      if (m_lboxes[par].child_count() == 0)
         return;

      TP_ScrCls();
      TP_kbLight(1);

      uint8 sel_pos = 1, last_four_id = 0;
      uint8 top = 0, bottom = 12, margin = 1,
            num = 1, skip;

      if (scroll == Up)
         skip = (child > 4) ? start_from(child) : child;
      else if (scroll == Down)
         skip = (child > 4) ? child - 1 : child;
      else
         skip = (child > 4) ? start_from(child) : child - 1;

      TP_ScrRestore(0);
      //TP_ShowTextFullScr(&display_info, (uchar*)"skip: %d", skip);
      TP_ScrRestore(1);

      for(uint16 i = 1; i < 200; ++i)
      {
         if(m_lboxes[i].parent_index() == par )
         {
            cchar* cap = m_lboxes[i].caption();

            if(change_page())
            {
               if(child > 4 && (num - 1) < skip)
               {
                  ++num;
                  clear_last_four();
                  continue;
               }

               if(estr::length(cap) != 0)
                  show_numbering(margin++);

               if (sel_pos++ == m_selected_line)
               {
                  TP_SetDisplayArea(6, top, 123, bottom);
                  TP_ScrAttrSet(1);
                  show_element_title(cap, m_lboxes[i].has_child());
                  TP_ScrAttrSet(0);
               }
               else
               {
                  TP_SetDisplayArea(6, top, 123, bottom);
                  show_element_title(cap, m_lboxes[i].has_child());
               }

               top    += 13;
               bottom += 12;
               m_last_four[last_four_id++] = i;
            }
            else
            {
               show_last_four(cap);
               break;
            }

            if(margin > 4)
            {
               //set_selected_element_index(child, scroll);
               break;
            }
         }
      }
      set_current_parent_index(par);
      show_title();
   }

   void Listbox_t::show_last_four(cchar* cap)
   {
      uint8 id;
      uint8 sel_pos = 1, top = 0, bottom = 12;

      for (uint8 i = 0; i < 4; ++i)
      {
         id = m_last_four[i];
         if (id == -1)
            continue;

         if(estr::length(m_lboxes[id].caption()) != 0)
            show_numbering(i + 1);

         if (sel_pos++ == m_selected_line)
         {
            TP_ScrAttrSet(1);
            show_element_title(m_lboxes[id].caption(), m_lboxes[id].has_child());
            TP_ScrAttrSet(0);
         }
         else
         {
            TP_SetDisplayArea(6, top, 123, bottom);
            //TP_ScrClsDisplayArea();
            //TP_ScrGotoxyEx(7, (i * 12) + 2);
            show_element_title(m_lboxes[id].caption(), m_lboxes[id].has_child());
         }
         top    += 13;
         bottom += 12;
      }
   }

   void Listbox_t::show()
   {
      //show(0, 1);
   }
}

namespace tests
{
   void listbox_test()
   {
      using namespace listbox;
      //lbox();
      //loop_text();
      //selection_box();
   }

   void listbox_test2()
   {
      using namespace listbox;

      list_box.set_title("Environment");
      list_box.init_menu();

      list_box.add_item(0, "Item 1 On the Menu List", false);
      list_box.add_item(0, "Item 2 with very long tit");
      list_box.add_item(0, "Item 3 with very long tit", true);
      list_box.add_item(0, "Item 4", true);
      list_box.add_item(0, "Item 5", true);
      list_box.add_item(0, "Item 6", true);
      list_box.add_item(0, "Item 7", true);
      list_box.add_item(0, "Item 8", true);
      list_box.add_item(0, "Item 9", true);
      list_box.add_item(0, "Item 10", true);
      list_box.add_item(0, "Item 11", true);
      list_box.add_item(0, "Item 12", true);

      list_box.add_item(4, "Item 4 First Child",  false);
      list_box.add_item(4, "Item 4 Second Child", false);

      list_box.add_item(5, "Item 5 First Child",  true);
      list_box.add_item(5, "Item 5 Second Child", false);
      list_box.add_item(5, "Item 5 Third Child", false);
      list_box.add_item(5, "Item 5 Fourth Child", false);
      list_box.add_item(5, "Item 5 Fifth Child", false);

      ListBoxItem& item1 = list_box.item_at(1);

      ///list_box.delete_item(item1);

      list_box.set_selected_line(4, Listbox_t::None);
      list_box.set_selected_element_index(4, Listbox_t::None);
      list_box.set_change_page(true);
      list_box.show(0, 4, Listbox_t::Up);
      list_box.exec();
   }
}

#endif//LISTBOX_H
