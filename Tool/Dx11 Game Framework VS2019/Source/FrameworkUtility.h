#pragma once

#define ReleaseBase(base) if (base) { base->Release(); }
#define ReleaseCOM(com) if (com) { com->Release(); com = nullptr; }