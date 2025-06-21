void kernel_main() {
    volatile char* video = (char*)0xb8000;
    const char* msg = "Hello from jakOS!";
    for (int i = 0; msg[i]; i++) {
        video[i * 2] = msg[i];
        video[i * 2 + 1] = 0x07;
    }
    while (1) __asm__("hlt");
}