uv_timer_t timer;
uv_timer_init(uv_default_loop(), &timer);

int timeout = 0;//下个tick(下一帧)即回调tickHandler
int timeout = 1000;//1000毫秒后会掉tickHandler

//timer_once定时器一次，timeout毫秒后回调一次tickHandler并停止定时器
uv_timer_start(&timer_req, tickHandler, timeout, 0);

//timer_once定时器一次，timeout毫秒后回调一次tickHandler并停止定时器
//timer_times，timeout毫秒后开始第一次回调tickHandler，之后每100毫秒回调一次
uv_timer_start(&timer_req, tickHandler, 1000, 100);




**********************************************************************
/* uv_timer_test.c */

#include <stdlib.h>
#include <uv.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

static void timer_cb(uv_timer_t *handle, int status) 
{
  static int count;
  printf("count %d now %d\n", count++, time(NULL));
}

int main(int argc, char *argv[])
{
  int r;
  uv_timer_t timer;
  r = uv_timer_init(uv_default_loop(), &timer);
  assert(r == 0);
  
  assert(!uv_is_active((uv_handle_t *) &timer));
  assert(!uv_is_closing((uv_handle_t *) &timer));
  printf("start %d\n", time(NULL));
  r = uv_timer_start(&timer, timer_cb, atoi(argv[1]), 0);

  r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  assert(r == 0);
  return 0;
}

**********************************************************************
#include <uv.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT assert
static int close_cb_called = 0;


static void close_cb(uv_handle_t* handle) {
  ASSERT(handle != NULL);
  close_cb_called++;
}


static void timer_cb(uv_timer_t* handle, int status) {
  ASSERT(0 && "timer_cb should not have been called");
}


int main()
{
  int r;
  uv_timer_t timer;

  r = uv_timer_init(uv_default_loop(), &timer);
  ASSERT(r == 0);

  ASSERT(!uv_is_active((uv_handle_t*) &timer));
  ASSERT(!uv_is_closing((uv_handle_t*) &timer));

  r = uv_timer_start(&timer, timer_cb, 1000, 0);
  ASSERT(r == 0);

  ASSERT(uv_is_active((uv_handle_t*) &timer));
  ASSERT(!uv_is_closing((uv_handle_t*) &timer));

  r = uv_timer_stop(&timer);
  ASSERT(r == 0);

  ASSERT(!uv_is_active((uv_handle_t*) &timer));
  ASSERT(!uv_is_closing((uv_handle_t*) &timer));

  r = uv_timer_start(&timer, timer_cb, 1000, 0);
  ASSERT(r == 0);

  ASSERT(uv_is_active((uv_handle_t*) &timer));
  ASSERT(!uv_is_closing((uv_handle_t*) &timer));

  uv_close((uv_handle_t*) &timer, close_cb);

  ASSERT(!uv_is_active((uv_handle_t*) &timer));
  ASSERT(uv_is_closing((uv_handle_t*) &timer));

  r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  ASSERT(r == 0);

  ASSERT(close_cb_called == 1);

  return 0;
}