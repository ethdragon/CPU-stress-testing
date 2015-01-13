After make, an executable file "cpu" will appear.
You can use flag to indicate different use mode

./cpu
set cpu usage to default 80%, if multicore detected, will exit immediately.

./cpu -c <cpu usage>
set cpu usage to certain value, if multicore detected, will exit immediately.

./cpu -m
monitor mode, only detect CPU usage, has little effect to CPU.
This mode is able to work in multi-core situation, but only limited up to 4 cores.