#include "Supervisor.hpp"

namespace th08
{
DIFFABLE_STATIC(ControllerMapping, g_ControllerMapping)
DIFFABLE_STATIC(Supervisor, g_Supervisor);

#pragma optimize("s", on)
void Supervisor::ThreadClose()
{
    if (m_Unk284 != NULL)
    {
        utils::GuiDebugPrint("info : Sub Thread Close Request\n");
        m_Unk28c = TRUE;

        while (WaitForSingleObject(m_Unk284, 1000) == WAIT_TIMEOUT)
            Sleep(1);

        CloseHandle(m_Unk284);
        m_Unk284 = NULL;
        m_Unk28c = FALSE;
    }
}
#pragma optimize("s", off)
}; // namespace th08
