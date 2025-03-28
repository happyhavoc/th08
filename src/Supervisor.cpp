#include "Supervisor.hpp"

namespace th08
{
#pragma optimize("s", on)
void Supervisor::ThreadClose()
{
    if (m_Unk284 != NULL)
    {
        utils::DebugPrint2("info : Sub Thread Close Request\n");
        m_Unk28c = TRUE;

        while (WaitForSingleObject(m_Unk284, 1000) == WAIT_TIMEOUT)
            Sleep(1);
            
        CloseHandle(m_Unk284);
        m_Unk284 = NULL;
        m_Unk28c = FALSE;
    }
}
#pragma optimize("s", off)
DIFFABLE_STATIC(Supervisor, g_Supervisor);
}; // namespace th08
