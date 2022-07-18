#pragma once

typedef void (__cdecl* _PVFV)(void);

#define CRT_SECTIONS() __pragma(section(".CRT$XCA", long, read))                                 \
                       __pragma(section(".CRT$XCZ", long, read))                                 \
                       extern "C" __declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = { nullptr }; \
                       extern "C" __declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = { nullptr };

#define CRT_INIT() for (_PVFV* ctor = __xc_a; ctor != __xc_z; ctor++) \
                   {                                                  \
                        if (*ctor == nullptr) { continue; }           \
                        (**ctor)();                                   \
                   }