The code included in this repo in this post can be found in the [Chacebot Firmware Github Repo](https://github.com/chacebot/chacebot_firmware) and can be built and used by following the instructions at [Warren Gay's Tutorial Repo for the STM32](https://github.com/ve3wwg/stm32f103c8t6). The only project in my repo is the RC controller and it can be found in this directory:

    chacebot_firmware/
        |- README.md
        |- libopencm3
        |- FreeRTOSv10.0.1
        |- libwwg
        |-rtos
            |- chacebot_rc_reciever/        Project Source Code Directory
            |   |- main.c                   Main file where all the magic happens
