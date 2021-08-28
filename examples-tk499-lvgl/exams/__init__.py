import lv_utils
import lvgl as lv

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 30)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/anim_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[anim_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 90)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/assets/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[assets]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 150)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/event_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[event_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 210)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/layouts_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[layouts_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 270)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/others_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[others_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 330)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/scroll_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[scroll_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 30, 390)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/styles_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[styles_ex]")

btn = lv.btn(lv.scr_act())
btn.set_height(lv.SIZE.CONTENT)
btn.align(lv.ALIGN.TOP_LEFT, 180, 30)
def foo(e):
    if e.get_code() ==  lv.EVENT.CLICKED:
        lv_utils.ex_new_page("exams/widgets_ex/__init__.py")
btn.add_event_cb(foo, lv.EVENT.ALL, None)
label = lv.label(btn)
label.set_text("[widgets_ex]")

