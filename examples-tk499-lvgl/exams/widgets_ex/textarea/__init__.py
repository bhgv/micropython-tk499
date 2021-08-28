import lv_utils
import lvgl as lv

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 30)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/widgets_ex/textarea/p1.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("p1.py")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 90)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/widgets_ex/textarea/p2.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("p2.py")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 150)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/widgets_ex/textarea/p3.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("p3.py")

