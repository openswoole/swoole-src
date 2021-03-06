/*
  +----------------------------------------------------------------------+
  | Open Swoole                                                          |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.0 of the Apache license,    |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.apache.org/licenses/LICENSE-2.0.html                      |
  | If you did not receive a copy of the Apache2.0 license and are unable|
  | to obtain it through the world-wide-web, please send a note to       |
  | hello@swoole.co.uk so we can mail you a copy immediately.            |
  +----------------------------------------------------------------------+
  | @author   Tianfeng Han  <mikan.tenny@gmail.com>                      |
  +----------------------------------------------------------------------+
*/

#include "test_core.h"
#include "swoole_channel.h"

using namespace std;
using namespace swoole;

const int N = 10000000;

TEST(channel, push) {
    auto *c = Channel::make(128 * 1024, 8192, SW_CHAN_LOCK | SW_CHAN_NOTIFY);
    map<int, string> m;

    size_t bytes = 0;
    int index = 0;

    while (bytes < N) {
        char buf[8000];
        int n = swoole_random_bytes(buf, (rand() % (sizeof(buf) / 2)) + (sizeof(buf) / 2));
        if (n <= 0) {
            swoole_trace("no enough data, n=%d, errno=%d\n", n, errno);
            continue;
        }
        m[index++] = string(buf, n);
        bytes += n;
    }

    swoole_trace("size=%lu", m.size());

    thread t1([&]() {
        auto next = m.find(0);
        int index = 1;
        size_t bytes = 0;

        while (bytes < N) {
            if (c->push(next->second.c_str(), next->second.length()) == SW_OK) {
                swoole_trace("[PUSH] index=%d, size=%lu", index, next->second.length());
                bytes += next->second.length();
                next = m.find(index++);
                if (next == m.end()) {
                    break;
                }
            } else {
                usleep(10);
            }
        }
    });

    thread t2([&]() {
        char buf[8000];
        size_t bytes = 0;
        int index = 0;
        while (bytes < N) {
            int retval = c->pop(buf, sizeof(buf));
            if (retval > 0) {
                swoole_trace("[POP] index=%d, size=%d", index, retval);
                string &_data = m[index++];
                bytes += retval;
                ASSERT_EQ(_data, string(buf, retval));
            } else {
                usleep(10);
            }
        }
    });

    t1.join();
    t2.join();

    c->destroy();
}
