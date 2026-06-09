#include <windows.h>
#include <stdio.h>
#include <string.h>

/*
    ControlWastedBustedScreen
    GTA San Andreas classic PC ASI plugin.

    Tested target: GTA SA classic 1.0 US exe addresses.
    This patches WASTED/BUSTED HUD and transition timing constants.
*/

static const char* INI_PATH = ".\\ControlWastedBustedScreen.ini";
static const char* LOG_PATH = ".\\ControlWastedBustedScreen.log";

void PatchMemory(void* address, const void* data, size_t size)
{
    DWORD oldProtect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(address, data, size);
    VirtualProtect(address, size, oldProtect, &oldProtect);
}

void PatchPushInt(DWORD address, DWORD value)
{
    BYTE patch[5];

    // PUSH imm32
    patch[0] = 0x68;
    memcpy(&patch[1], &value, sizeof(DWORD));

    PatchMemory((void*)address, patch, sizeof(patch));
}

void PatchDword(DWORD address, DWORD value)
{
    PatchMemory((void*)address, &value, sizeof(DWORD));
}

DWORD ReadIniInt(const char* key, DWORD defaultValue)
{
    return GetPrivateProfileIntA("Settings", key, defaultValue, INI_PATH);
}

void LogSettings(
    DWORD bustedTextMs,
    DWORD bustedFadeStartMs,
    DWORD bustedFadeWindowMs,
    DWORD bustedFinishMs,
    DWORD wastedTextMs,
    DWORD wastedFadeStartMs,
    DWORD wastedFadeWindowMs
)
{
    FILE* f = fopen(LOG_PATH, "a");

    if (!f)
        return;

    fprintf(f, "ControlWastedBustedScreen loaded.\n");
    fprintf(f, "BustedTextMs=%lu\n", bustedTextMs);
    fprintf(f, "BustedFadeStartMs=%lu\n", bustedFadeStartMs);
    fprintf(f, "BustedFadeWindowMs=%lu\n", bustedFadeWindowMs);
    fprintf(f, "BustedFinishMs=%lu\n", bustedFinishMs);
    fprintf(f, "WastedTextMs=%lu\n", wastedTextMs);
    fprintf(f, "WastedFadeStartMs=%lu\n", wastedFadeStartMs);
    fprintf(f, "WastedFadeWindowMs=%lu\n", wastedFadeWindowMs);
    fprintf(f, "Patches applied.\n\n");

    fclose(f);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID reserved)
{
    (void)reserved;

    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        /*
            Original values:
            BUSTED text:        5000 ms at 0x00442D7F, PUSH 0x1388
            BUSTED fade start:  3000 ms at 0x004430D7, CMP ESI, 0x0BB8
            BUSTED fade window: 3000 ms at 0x004430E7, CMP ECX, 0x0BB8
            BUSTED finish:      4000 ms at 0x00443154, CMP ESI, 0x0FA0

            WASTED text:        4000 ms at 0x0056E5A7, PUSH 0x0FA0
            WASTED fade start:  3000 ms at 0x00442DC2, CMP EAX, 0x0BB8
            WASTED fade window: 3000 ms at 0x00442DD4, CMP EAX, 0x0BB8
        */

        DWORD bustedTextMs       = ReadIniInt("BustedTextMs", 1000);
        DWORD bustedFadeStartMs  = ReadIniInt("BustedFadeStartMs", 500);
        DWORD bustedFadeWindowMs = ReadIniInt("BustedFadeWindowMs", 500);
        DWORD bustedFinishMs     = ReadIniInt("BustedFinishMs", 1000);

        DWORD wastedTextMs       = ReadIniInt("WastedTextMs", 1000);
        DWORD wastedFadeStartMs  = ReadIniInt("WastedFadeStartMs", 500);
        DWORD wastedFadeWindowMs = ReadIniInt("WastedFadeWindowMs", 500);

        // BUSTED patches
        PatchPushInt(0x00442D7F, bustedTextMs);
        PatchDword(0x004430D7, bustedFadeStartMs);
        PatchDword(0x004430E7, bustedFadeWindowMs);
        PatchDword(0x00443154, bustedFinishMs);

        // WASTED patches
        PatchPushInt(0x0056E5A7, wastedTextMs);
        PatchDword(0x00442DC2, wastedFadeStartMs);
        PatchDword(0x00442DD4, wastedFadeWindowMs);

        LogSettings(
            bustedTextMs,
            bustedFadeStartMs,
            bustedFadeWindowMs,
            bustedFinishMs,
            wastedTextMs,
            wastedFadeStartMs,
            wastedFadeWindowMs
        );
    }

    return TRUE;
}
