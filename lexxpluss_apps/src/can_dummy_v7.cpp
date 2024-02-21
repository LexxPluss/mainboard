/*
 * Copyright (c) 2024, LexxPluss Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/can.h>
#include <shell/shell.h>
#include <algorithm>
#include "can_dummy_v7.hpp"

namespace {

void send_can_data(const device *dev, uint32_t id, const uint8_t *data, uint8_t dlc)
{
    if (dlc > CAN_MAX_DLEN)
        return;
    zcan_frame frame{
        .id{id},
        .rtr{CAN_DATAFRAME},
        .id_type{CAN_STANDARD_IDENTIFIER},
        .dlc{dlc}
    };
    std::copy(data, data + dlc, frame.data);
    can_send(dev, &frame, K_MSEC(100), nullptr, nullptr);
}

void send_bmu(const device *dev)
{
    uint8_t buf[8];
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 81;
    buf[3] = 81;
    buf[4] = 100;
    buf[5] = (2567 >> 8) & 0xff;
    buf[6] = (2567     ) & 0xff;
    buf[7] = 0;
    send_can_data(dev, 0x100, buf, sizeof buf);
    buf[0] = (-1234 >> 8) & 0xff;
    buf[1] = (-1234     ) & 0xff;
    buf[2] = ( 3000 >> 8) & 0xff;
    buf[3] = ( 3000     ) & 0xff;
    buf[4] = (25678 >> 8) & 0xff;
    buf[5] = (25678     ) & 0xff;
    buf[6] = 0;
    buf[7] = 0;
    send_can_data(dev, 0x101, buf, sizeof buf);
    buf[0] = (18789 >> 8) & 0xff;
    buf[1] = (18789     ) & 0xff;
    buf[2] = (18789 >> 8) & 0xff;
    buf[3] = (18789     ) & 0xff;
    buf[4] = (12345 >> 8) & 0xff;
    buf[5] = (12345     ) & 0xff;
    buf[6] = 0;
    buf[7] = 0;
    send_can_data(dev, 0x103, buf, sizeof buf);
    buf[0] = (24567 >> 8) & 0xff;
    buf[1] = (24567     ) & 0xff;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = (24456 >> 8) & 0xff;
    buf[5] = (24456     ) & 0xff;
    buf[6] = 1;
    buf[7] = 0;
    send_can_data(dev, 0x110, buf, sizeof buf);
    buf[0] = (2569 >> 8) & 0xff;
    buf[1] = (2569     ) & 0xff;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = (2568 >> 8) & 0xff;
    buf[5] = (2568     ) & 0xff;
    buf[6] = 1;
    buf[7] = 0;
    send_can_data(dev, 0x111, buf, sizeof buf);
    buf[0] = (-617 >> 8) & 0xff;
    buf[1] = (-617     ) & 0xff;
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = (-617  >> 8) & 0xff;
    buf[5] = (-617      ) & 0xff;
    buf[6] = 1;
    buf[7] = 0;
    send_can_data(dev, 0x112, buf, sizeof buf);
    buf[0] = 0x12;
    buf[1] = 0x34;
    buf[2] = 1;
    buf[3] = 2;
    buf[4] = 0;
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 0;
    send_can_data(dev, 0x113, buf, sizeof buf);
    buf[0] = (4076 >> 8) & 0xff;
    buf[1] = (4076     ) & 0xff;
    buf[2] = 1;
    buf[3] = 0;
    buf[4] = (4094 >> 8) & 0xff;
    buf[5] = (4094     ) & 0xff;
    buf[6] = 0;
    buf[7] = 0;
    send_can_data(dev, 0x120, buf, sizeof buf);
    buf[0] = 0x78;
    buf[1] = 0x9a;
    buf[2] = 0xbc;
    buf[3] = 0xde;
    buf[4] = 0xf0;
    buf[5] = 0x12;
    buf[6] = 0;
    buf[7] = 0;
    send_can_data(dev, 0x130, buf, sizeof buf);
}

void send_uss(const device *dev)
{
    static constexpr uint16_t data[5]{1000, 1001, 1002, 1003, 1004};
    uint64_t work{0};
    for (const auto i: data) {
        work <<= 12;
        work |= i & 0xfff;
    }
    work <<= 4;
    uint8_t buf[8];
    buf[0] = (work >> 56) & 0xff;
    buf[1] = (work >> 48) & 0xff;
    buf[2] = (work >> 40) & 0xff;
    buf[3] = (work >> 32) & 0xff;
    buf[4] = (work >> 24) & 0xff;
    buf[5] = (work >> 16) & 0xff;
    buf[6] = (work >>  8) & 0xff;
    buf[7] = (work      ) & 0xff;
    send_can_data(dev, 0x204, buf, sizeof buf);
}

void send_acc(const device *dev, uint8_t counter)
{
    static constexpr uint16_t data[3]{2000, 2001, 2002};
    uint8_t buf[7];
    buf[0] = (data[0] >> 8) & 0xff;
    buf[1] = (data[0]     ) & 0xff;
    buf[2] = (data[1] >> 8) & 0xff;
    buf[3] = (data[1]     ) & 0xff;
    buf[4] = (data[2] >> 8) & 0xff;
    buf[5] = (data[2]     ) & 0xff;
    buf[6] = counter;
    send_can_data(dev, 0x206, buf, sizeof buf);
}

void send_gyro(const device *dev, uint8_t counter)
{
    static constexpr uint16_t data[3]{3000, 3001, 3002};
    uint8_t buf[7];
    buf[0] = (data[0] >> 8) & 0xff;
    buf[1] = (data[0]     ) & 0xff;
    buf[2] = (data[1] >> 8) & 0xff;
    buf[3] = (data[1]     ) & 0xff;
    buf[4] = (data[2] >> 8) & 0xff;
    buf[5] = (data[2]     ) & 0xff;
    buf[6] = counter;
    send_can_data(dev, 0x207, buf, sizeof buf);
}

void send_encoder(const device *dev)
{
    static constexpr int16_t data[3]{4000, 4001, 4002};
    uint8_t buf[6];
    buf[0] = (data[0] >> 8) & 0xff;
    buf[1] = (data[0]     ) & 0xff;
    buf[2] = (data[1] >> 8) & 0xff;
    buf[3] = (data[1]     ) & 0xff;
    buf[4] = (data[2] >> 8) & 0xff;
    buf[5] = (data[2]     ) & 0xff;
    send_can_data(dev, 0x209, buf, sizeof buf);
}

void send_current(const device *dev)
{
    static constexpr int16_t data[4]{5000, 5001, 5002, 2000};
    uint8_t buf[8];
    buf[0] = (data[0] >> 8) & 0xff;
    buf[1] = (data[0]     ) & 0xff;
    buf[2] = (data[1] >> 8) & 0xff;
    buf[3] = (data[1]     ) & 0xff;
    buf[4] = (data[2] >> 8) & 0xff;
    buf[5] = (data[2]     ) & 0xff;
    buf[6] = (data[3] >> 8) & 0xff;
    buf[7] = (data[3]     ) & 0xff;
    send_can_data(dev, 0x20a, buf, sizeof buf);
}

void receive_pgv(const uint8_t *data)
{
    printk("receive PGV command: %u\n", data[0]);
}

void receive_led(const uint8_t *data)
{
    uint16_t count{static_cast<uint16_t>(data[1] << 8 | data[2])};
    printk("receive LED command: pattern: %u count: %u rgb: %u/%u/%u\n",
           data[0], count, data[3], data[4], data[5]);
}

void receive_actuator(const uint8_t *data)
{
    printk("receive actuator command: L: %d/%u C: %d/%u R: %d/%u\n",
           data[0], data[3], data[1], data[4], data[2], data[5]);
}

void receive_actuator_init(const uint8_t *data)
{
    printk("receive actuator init command: %d\n", data[0]);
}

CAN_DEFINE_MSGQ(msgq_can, 8);

}

namespace lexxhard::can_dummy_v7 {

class can_dummy_v7_impl {
public:
    int init() {
        dev = device_get_binding("CAN_2");
        if (!device_is_ready(dev))
            return -1;
        can_configure(dev, CAN_NORMAL_MODE, 1'000'000);
        static const zcan_filter filter{
            .id{0x200},
            .rtr{CAN_DATAFRAME},
            .id_type{CAN_STANDARD_IDENTIFIER},
            .id_mask{0x7f0},
            .rtr_mask{1}
        };
        can_attach_msgq(dev, &msgq_can, &filter);
        return 0;
    }
    void run(void *p1, void *p2, void *p3) {
        if (!device_is_ready(dev))
            return;
        while (true) {
            uint32_t now_cycle{k_cycle_get_32()};
            uint32_t dt_ms{k_cyc_to_ms_near32(now_cycle - prev_cycle)};
            if (dt_ms > 1'000) {
                prev_cycle = now_cycle;
                send_bmu(dev);
                send_uss(dev);
                send_acc(dev, acc_gyro_counter);
                send_gyro(dev, acc_gyro_counter);
                send_encoder(dev);
                send_current(dev);
                ++acc_gyro_counter;
            }
            zcan_frame frame;
            if (k_msgq_get(&msgq_can, &frame, K_NO_WAIT) == 0) {
                switch (frame.id) {
                case 0x203: receive_pgv(frame.data); break;
                case 0x205: receive_led(frame.data); break;
                case 0x208: receive_actuator(frame.data); break;
                case 0x20b: receive_actuator_init(frame.data); break;
                }
            }
            k_msleep(10);
        }
    }
private:
    const device *dev{nullptr};
    uint32_t prev_cycle{0};
    uint8_t acc_gyro_counter{0};
} impl;

int info(const shell *shell, size_t argc, char **argv)
{
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_can,
    SHELL_CMD(info, NULL, "CAN dummy information", info),
    SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(can, &sub_can, "CAN dummy commands", NULL);

void init()
{
    impl.init();
}

void run(void *p1, void *p2, void *p3)
{
    impl.run(p1, p2, p3);
}

k_thread thread;

}
