# lcd
import tftlcd

d = tftlcd.LCD43R()  #Build LCD object
d.fill((0, 0, 0))    #fill bkgnd

# touchscreen
import touch

t = touch.FT5436()   #Build a touch screen object
#t.read()            #Get touch status and coordinates 

# LVGL
import lvgl as lv
lv.init()

import lvlcd
lvlcd.init()

# reg lvgl screen buffer
disp_buf1 = lv.disp_draw_buf_t()
buf1_1 = bytes(800*10)
disp_buf1.init(buf1_1, None, len(buf1_1)//4)
disp_drv = lv.disp_drv_t()
disp_drv.init()
disp_drv.draw_buf = disp_buf1
disp_drv.flush_cb = lvlcd.flush
disp_drv.hor_res = 800
disp_drv.ver_res = 480
disp_drv.register()

# reg touchscreen
indev_drv = lv.indev_drv_t()
indev_drv.init() 
indev_drv.type = lv.INDEV_TYPE.POINTER
indev_drv.read_cb = lvlcd.ts_read
indev_drv.register()

# define main loop class

import micropython
import usys

try:
    from machine import Timer
except:
    raise RuntimeError("Missing machine.Timer implementation!")

default_timer_id = 1

import uasyncio

##############################################################################

class event_loop():

    _current_instance = None

    def __init__(self, freq=25, timer_id=default_timer_id, max_scheduled=2, refresh_cb=None, asynchronous=False):
        if self.is_running():
            raise RuntimeError("Event loop is already running!")

        if not lv.is_initialized():
            lv.init()

        event_loop._current_instance = self

        self.delay = 1000 // freq
        self.refresh_cb = refresh_cb

        self.asynchronous = asynchronous
        if self.asynchronous:
            #if not uasyncio_available:
            #    raise RuntimeError("Cannot run asynchronous event loop. uasyncio is not available!")
            self.refresh_event = uasyncio.Event()
            self.refresh_task = uasyncio.create_task(self.async_refresh())
            self.timer_task = uasyncio.create_task(self.async_timer())
        else:
            self.timer = Timer(timer_id)
            self.task_handler_ref = self.task_handler  # Allocation occurs here
            self.timer.init(mode=Timer.PERIODIC, period=self.delay, callback=self.timer_cb)
            self.max_scheduled = max_scheduled
            self.scheduled = 0

    def deinit(self):
        if self.asynchronous:
            self.refresh_task.cancel()
            self.timer_task.cancel()
        else:
            self.timer.deinit()
        event_loop._current_instance = None

    @staticmethod
    def is_running():
        return event_loop._current_instance is not None

    @staticmethod
    def current_instance():
        return event_loop._current_instance

    def task_handler(self, _):
        lv.task_handler()
        if self.refresh_cb: self.refresh_cb()
        self.scheduled -= 1

    def timer_cb(self, t):
        # Can be called in Interrupt context
        # Use task_handler_ref since passing self.task_handler would cause allocation.
        lv.tick_inc(self.delay)
        if self.scheduled < self.max_scheduled:
            try:
                micropython.schedule(self.task_handler_ref, 0)
                self.scheduled += 1
            except:
                pass

    async def async_refresh(self):
        while True:
            await self.refresh_event.wait()
            self.refresh_event.clear()
            lv.task_handler()
            if self.refresh_cb: self.refresh_cb()

    async def async_timer(self):
        while True:
            await uasyncio.sleep_ms(self.delay)
            lv.tick_inc(self.delay)
            self.refresh_event.set()

