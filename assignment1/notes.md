## ISPC
> It compiles a C-based SPMD programming languarge to run on the SIMD units (multiple cores / vector unit) of CPUs and the Intel Xeon Phi architecture. It currently supports the SSE2, SSE4, AVX1, AVX2, AVX512, and Xeon Phi instruction sets.

## Program #1

### 任务划分

对于结果 mandelbrot-thread.ppm，若干连续的行形成一个 tile，每个 thread 负责生成一个 title。结果，4 threads 时，view1 和 view2 分别可获得 3.1x 和 3.35x 的加速比。

### 结果分析

Intel Core i7-6700 CPU with hyper-thread on => 8 logical cores

![view1_speedup](/home/rh/Workspace/cmu15418/assignment1/prog1_mandelbrot_threads/view1_speedup.png)

![view2_speedup](/home/rh/Workspace/cmu15418/assignment1/prog1_mandelbrot_threads/view2_speedup.png)

由图，线程数小于 12 时，speedup 随线程数线性增长。当线程数大于 12 时，speedup 趋于 8，即趋于 CPU 的核心数。

### Intel HT

TODO

## Appendix

- getopt.h